#include <QrServerManager.h>
#include <ActivityManager.h>
#include <QrDbManager.h>
#include <QrTicket.h>
#include <QDateTime>
#include <Utilities.h>
#include <QStringList>
#include <QTimer>


namespace Afc
{
static constexpr auto SESSION_FILE_NAME     = "~qrServerSession";
static constexpr auto SOCKET_BASE_PORT      = 4000;
static constexpr auto RECONNECT_DELAY_MSEC  = 1000;
static constexpr auto CXN_TIMEOUT_MSEC      = 2000;
static constexpr auto MAX_OFFLINE_TIME_MSEC = (2 * 3600) * 1000;
static constexpr auto MAX_OFFLINE_TICKETS   = 5000;

static const QByteArray PACKET_HEADER("MAPRA");
static constexpr auto   PACKET_HEADER_SIZE    = 5;
static constexpr auto   PACKET_CHECKSUM_SIZE  = 2;
static constexpr auto   PACKET_BASE_SIZE      = PACKET_HEADER_SIZE + PACKET_CHECKSUM_SIZE + 1;
static constexpr auto   PACKET_DATA_OFFSET    = PACKET_BASE_SIZE - PACKET_CHECKSUM_SIZE;


QrServerManager::QrServerManager(): _ticketCounter(0), _offlineTicketCounter(0),
  _lastRequestTime(0), _lastResponseTime(0)
{
  loadSession();

  _systemIp = Utilities::getIpString();

  if (12 != _systemIp.size())
  {
    _systemIp = QString(12, '0');
  }

  connect(&_socket, SIGNAL(connected()), this, SLOT(onSocketConnected()));
  connect(&_socket, SIGNAL(readyRead()), this, SLOT(onSocketReadyRead()));
  connect(&_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(onSocketError(QAbstractSocket::SocketError)));
  connect(&_timerNetwork, SIGNAL(timeout()), this, SLOT(onTimerNetwork()));
  _timerNetwork.setInterval(CXN_TIMEOUT_MSEC);
  _timerNetwork.start();

  reconnect();
}

void QrServerManager::reconnect()
{
  QTimer::singleShot(RECONNECT_DELAY_MSEC, this, SLOT(connectToServer()));
}

void QrServerManager::validate(const QSharedPointer<QrTicket> &ticket, quint16 stationId)
{
  if (nullptr != ticket)
  {
    auto const now = QDateTime::currentMSecsSinceEpoch();

    _pendingTicket = ticket;

#ifdef VERBOSE_LOG
    qDebug() << "qr ticket:" << ticket->getIssuerIpv4() << ticket->getIssuerType() << ticket->getTicketType() << ticket->getQrType();
#endif

    if (QTcpSocket::ConnectedState != _socket.state())
    {
      ValidationResult res;

      if (now < _connectionLostTime + MAX_OFFLINE_TIME_MSEC)
      {
        res.remainingTrips = 0;
        res.responseCode = ServerResponseCode::OfflineMode;

        if (_offlineTicketCounter < MAX_OFFLINE_TICKETS && 1 == ticket->getQrType().toInt())
        {
          qDebug() << "_offlineTicketCounter" <<  _offlineTicketCounter;
          res.status = QrDbManager::getInstance().checkTicket(ticket, stationId) ? ValidationStatus::Valid : ValidationStatus::NotValid;
          incrementTicketCounter(true);
        }
        else
        {
          qDebug() << "MAX_OFFLINE_TICKETS" << now << _offlineTicketCounter;
          res.status = ValidationStatus::OfflineQuotaReached;
        }
      }
      else
      {
        qDebug() << "cxn lost" << now << _connectionLostTime;
        res.status = ValidationStatus::NotValid;
      }

      emit validationReady(_pendingTicket, res);
    }
    else
    {
      QString data;

      data += QString::number(ActivityManager::getInstance()->getStationId(), 16).rightJustified(2, '0');
      data += QString::number(_systemIp.right(3).toUInt(), 16); // Last IP section.
      data += ticket->getIssuerType();
      data += ticket->getIssuerId();
      data += ticket->getTicketTypeHex();
      data += ticket->getEncryptedSection();
      data += ticket->getEncryptedSection(); // Duplicated data.
      data += ticket->getDecryptedSection();
      data += QDateTime::currentDateTimeUtc().toString("yyMMddHHmmss");
      data += _systemIp; // System IP;
      data += ticket->getKeyId();
      data += QString::number(_ticketCounter).rightJustified(6, '0'); // Daily ticket counter that is valid and passed from gate (not related to gate counter).
      data += QString::number(ActivityManager::getInstance()->getStationId()).rightJustified(2, '0');

      sendPacket(data);
    }
  }
  else
  {
    emit validationReady(QSharedPointer<QrTicket>(), ValidationResult());
  }
}

void QrServerManager::incrementTicketCounter(bool offline)
{
  ++_ticketCounter;

  if (offline)
  {
    ++_offlineTicketCounter;
  }

  saveSession();
}

void QrServerManager::saveSession()
{
  _fileSession.seek(0);

  _streamSession.setDevice(&_fileSession);
  _streamSession << QDateTime::currentDateTimeUtc() << _connectionLostTime << _ticketCounter << _offlineTicketCounter;

  _fileSession.flush();
}

void QrServerManager::loadSession()
{
  _fileSession.setFileName(SESSION_FILE_NAME);
  bool  hasSession = _fileSession.exists();

  if (_fileSession.open(QIODevice::ReadWrite))
  {
    _streamSession.setDevice(&_fileSession);

    if (hasSession)
    {
      auto const now = QDateTime::currentDateTimeUtc().date();
      QDateTime sessionDate;

      _streamSession >> sessionDate >> _connectionLostTime >> _ticketCounter >> _offlineTicketCounter;

      if (now != sessionDate.date())
      {
        _ticketCounter        = 0;
        _offlineTicketCounter = 0;
      }

      if (0 == _connectionLostTime)
      {
        _connectionLostTime = QDateTime::currentMSecsSinceEpoch();
      }
    }
  }
}

void QrServerManager::sendPacket(const QString &data)
{
  QByteArray buffer;
  QDataStream stream(&buffer, QIODevice::Append | QIODevice::ReadWrite);
  const quint16 dataSize = data.size();
  const auto dsHex = QString::number(dataSize, 16).rightJustified(2, '0').toUpper();

  stream.writeRawData(PACKET_HEADER, PACKET_HEADER_SIZE);
  stream.writeRawData(dsHex.toLatin1().data(), dsHex.size());
  stream.writeRawData(data.toLatin1().data(), data.size());

  quint16 c = Utilities::crc16ModBus(data.toLatin1());
  c = (c >> 8) | (c << 8);
  auto const crc = QString::number(c, 16).rightJustified(4, '0').toUpper();

  stream.writeRawData(crc.toLatin1().data(), crc.size());

#ifdef VERBOSE_LOG
  qDebug() << "sending packet:" << buffer;
#endif

  _socket.write(buffer);
  _lastRequestTime = QDateTime::currentMSecsSinceEpoch();
}

quint32 QrServerManager::getTicketCounter() const
{
  return _ticketCounter;
}

void QrServerManager::setServerIp(const QString &ip)
{
  _serverIp = ip;
}

void QrServerManager::onSocketConnected()
{
#ifdef VERBOSE_LOG
  qDebug() << "qr socket connected";
#endif

  QrDbManager::getInstance().rebuildOfflineDb();

  {
    QByteArray packet("1");
    auto const offlineFiles = QrDbManager::getInstance().getOfflineDbs();

    for (auto name : offlineFiles)
    {
      qDebug() << name << QFileInfo(name).baseName();

      sendPacket(packet + QFileInfo(name).baseName().toLatin1());
      break;
    }
  }

  _connected = true;
}

void QrServerManager::onSocketReadyRead()
{
  _lastResponseTime = QDateTime::currentMSecsSinceEpoch();
  _dataBuffer.append(_socket.readAll());

#ifdef VERBOSE_LOG
  qDebug() << "onSocketReadyRead" << _dataBuffer.size() << _dataBuffer << _dataBuffer.toHex();
#endif

  while (PACKET_BASE_SIZE <= _dataBuffer.size())
  {
    if (_dataBuffer.startsWith(PACKET_HEADER))
    {
      const quint16 dataSize    = *reinterpret_cast<quint8 *>(_dataBuffer.data() + PACKET_HEADER_SIZE);
      const qint32  packetSize  = PACKET_BASE_SIZE + dataSize;

#ifdef VERBOSE_LOG
      qDebug() << "datasize:" << dataSize << packetSize;
#endif

      if (49 == dataSize && _dataBuffer.size()) // offline qr response.
      {
        auto const realSize = 27;
        qDebug() << _dataBuffer.mid(PACKET_DATA_OFFSET, realSize).mid(2, 15);

        QrDbManager::getInstance().removeDb(_dataBuffer.mid(PACKET_DATA_OFFSET, realSize).mid(2, 15));
        _dataBuffer = _dataBuffer.mid(realSize);
      }
      else if (packetSize <= _dataBuffer.size())
      {
        // TODO: check crc;
        //const auto offset = packetSize - PACKET_CHECKSUM_SIZE;

        //quint16 crc = _dataBuffer.mid(offset, PACKET_CHECKSUM_SIZE).toUShort();

        //crc = (crc >> 8) | (crc << 8);

        //if (crc == Utilities::crc16ModBus(_dataBuffer.left(PACKET_DATA_OFFSET)))
        //{
          processPacket(_dataBuffer.mid(PACKET_DATA_OFFSET, dataSize));
        //}
        //else
        //{
          //qDebug() << "invalid crc" << _dataBuffer.mid(offset, PACKET_CHECKSUM_SIZE) << Utilities::crc16ModBus(_dataBuffer.left(offset));
        //}

        _dataBuffer = _dataBuffer.mid(packetSize);
      }
      else
      {
#ifdef VERBOSE_LOG
        qDebug() << "invalid packet size";
#endif
        break;
      }
    }
    else
    {
      _dataBuffer = _dataBuffer.mid(1);
    }
  }
}

void QrServerManager::processPacket(const QByteArray &packet)
{
#ifdef VERBOSE_LOG
  qDebug() << "QrServerManager::processPacket" << packet.toHex();
#endif
  ValidationResult result;

  result.status = ValidationStatus::NotValid;
  result.responseCode = ServerResponseCode::NotResponding;
  result.remainingTrips = 0;

  qDebug() << "packet size & header" << packet.size() << packet[0];

  if (0x10 == packet.size())
  {
    result.responseCode = static_cast<ServerResponseCode>(packet[0]);

    switch (result.responseCode)
    {
    case ServerResponseCode::Valid:
    case ServerResponseCode::DisconnectedFromCentralServer:
    case ServerResponseCode::DirectConnectionToBank:
    {
      incrementTicketCounter(false);
      result.status = ValidationStatus::Valid;
      result.remainingTrips = 0;
    }
    break;
    case ServerResponseCode::MultiTrip:
    {
      incrementTicketCounter(false);
      result.status = ValidationStatus::Valid;
      result.remainingTrips = packet[1];
    }
    default:
    break;
    }
  }
  else
  {
    //reconnect();
  }

  emit validationReady(_pendingTicket, result);

  _pendingTicket.clear();
}

void QrServerManager::onSocketError(QAbstractSocket::SocketError err)
{
#ifdef VERBOSE_LOG
   qDebug() << "socket error" << err << _serverIp;
#endif

  if (_connected)
  {
    _connectionLostTime = QDateTime::currentMSecsSinceEpoch();
    saveSession();
  }

  _connected = false;

  reconnect();
}


void QrServerManager::connectToServer()
{
  if (QTcpSocket::ConnectedState == _socket.state())
  {
    _socket.disconnectFromHost();
  }

  _socket.connectToHost(QHostAddress(_serverIp), SOCKET_BASE_PORT + _systemIp.right(3).toUInt());
}

void QrServerManager::onTimerNetwork()
{
  auto const now = QDateTime::currentMSecsSinceEpoch();

  if (_lastResponseTime < _lastRequestTime)
  {
    if (CXN_TIMEOUT_MSEC < now - _lastRequestTime)
    {
      _socket.disconnectFromHost();
      _lastResponseTime = _lastRequestTime;
      reconnect();
    }
  }
}
}
