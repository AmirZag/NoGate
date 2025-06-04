#include <Scu.h>
#include <QDateTime>
#include <QFile>
#include <QDir>
#include <QDataStream>
#include <QTcpSocket>
#include <QTcpServer>
#include <QDebug>
#include <Settings.h>
#include <ActivityManager.h>
#include <GateController.h>
#include <QrDbManager.h>


#define CONNECTION_TIMEOUT 5000


namespace Afc
{
Scu::Scu():
  _connected(false), _lastDataTime(0)
{
  _server.clear();
  _server = QSharedPointer<QTcpServer>(new QTcpServer());
  connect(_server.data(), SIGNAL(newConnection()), this, SLOT(onNewConnection()));

  _timer.setInterval(2000);
  connect(&_timer, SIGNAL(timeout()), this, SLOT(onTimerTimeout()));
  _timer.start();
}

void  Scu::start()
{
  _server->listen(QHostAddress::Any, 1470);
}

Scu::ConfigurationMask  Scu::getConfiguration() const
{
  return _configuration;
}

bool  Scu::isConnected() const
{
  return _connected;
}

Scu::~Scu()
{
  _client.clear();
}

void  Scu::onNewConnection()
{
  while (_server->hasPendingConnections())
  {
    _currentPacketNumber  = 0;
    _closeStationCounter  = 0;
    _closeStationCommited = 0;

    _client.clear();
    _client = QSharedPointer<QTcpSocket>(_server->nextPendingConnection());
    connect(_client.data(), SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    connect(_client.data(), SIGNAL(disconnected()), this, SLOT(onDisconnected()));
    _connected = true;

    qDebug() << "SC connected.";
  }
}

void  Scu::onReadyRead()
{
  const auto    &AM = ActivityManager::getInstance();
  NetworkPacket  packet;
  qint32         bytesRead;

  _dataBuffer.append(_client->readAll());

  while (0 < (bytesRead = NetworkPacket::fromByteArray(_dataBuffer, &packet)))
  {
    ++_currentPacketNumber;
    _lastDataTime = QDateTime::currentMSecsSinceEpoch();
    _dataBuffer   = _dataBuffer.mid(bytesRead);

    // qDebug() << "Packet:" << QString::number(packet.command, 16).rightJustified(2, '0');
    // qDebug() << packet.header << packet.address << packet.command << packet.data.toHex();

    QByteArray   response;
    QDataStream  dsResponse(&response, QIODevice::WriteOnly);

    switch (static_cast<Command>(packet.command))
    {
    case Command::SetEquipment:
    {
      qDebug() << "SetEquipment";
    }
    break;
    case Command::Ping:
    {
      {
        const auto &now = QDateTime::currentDateTimeUtc();

        if (5 == now.time().hour() && 0 == now.time().minute())
        {
          if (now.time().second() < 30)
          {
            GateController::getInstance()->resetCounters();
            QrDbManager::getInstance().cleanOldEntries();
          }
          else if (58 < now.time().second())
          {
            Utilities::systemReboot();
          }
        }
      }

      // qDebug() << "ping mode:"
      // << QString::number(packet.getPingMode(), 2).rightJustified(8, '0')
      // << QString::number(packet.getConfiguration(), 2).rightJustified(8, '0');

      quint8                  pingMode = packet.getPingMode();
      quint8                  config   = quint8(packet.getConfiguration());
      GateController::Status  gateStatus;

      if (Scu::Reserved2 == config)
      {
        ++_closeStationCounter;
      }
      else
      {
        _closeStationCounter = 0;
        _closeStationCommited = false;
      }

      if (AM->getStationStatus())
      {
        if (ActivityManager::ConfigurationMode::Online == AM->getConfigurationMode())
        {
          if (ActivityManager::AccessControlGateType::Exit != AM->getAcgType())
          {
            GateController::getInstance()->setConfiguration(config & GateConfigMask);
          }
          else
          {
            if (Scu::ConfigBlockedControlled != (config & GateConfigMask))
            {
              GateController::getInstance()->setConfiguration(config & GateConfigMask);
            }
            else
            {
              GateController::getInstance()->setConfiguration(Scu::ConfigBlockedFree);
            }
          }
        }
      }
      else
      {
        const auto &now = QDateTime::currentDateTimeUtc();

        if (4 < _closeStationCounter && !_closeStationCommited)
        {
          GateController::getInstance()->closeGate();
          _closeStationCommited = true;
        }
      }

      if (GateController::getInstance()->getStatus(&gateStatus))
      {
        AM->setAlarm(ActivityManager::AlarmGroup::Logic, gateStatus.alarm);

        if (ActivityManager::AccessControlGateType::Exit != AM->getAcgType())
        {
          config &= ~GateConfigMask;
          config |= gateStatus.configuration;
        }
        else
        {
          if (Scu::ConfigBlockedControlled != (config & GateConfigMask))
          {
            config &= ~GateConfigMask;
            config |= gateStatus.configuration;
          }
        }
      }
      else
      {
        AM->setAlarm(ActivityManager::AlarmGroup::Logic, ActivityManager::AlarmBitNumber::Bit5, true);
        config &= ~GateConfigMask;
      }

      if (ActivityManager::ConfigurationMode::Online == AM->getConfigurationMode())
      {
        config |= Online;
      }
      else
      {
        config &= ~Online;
        config &= ~GateConfigMask;
        config |= (_configuration & GateConfigMask);
      }

      setConfiguration(static_cast<ConfigurationMask>(config));

      dsResponse << pingMode << config << static_cast<quint16>(AM->getActFileCount());

      {
        quint8  alarmMask = AM->getAlarmsMask();
        dsResponse << alarmMask;

        if (alarmMask & quint8(ActivityManager::AlarmGroup::Kitcas))
        {
          dsResponse << AM->getAlarm(ActivityManager::AlarmGroup::Kitcas);
        }

        if (alarmMask & quint8(ActivityManager::AlarmGroup::EntryReader))
        {
          dsResponse << AM->getAlarm(ActivityManager::AlarmGroup::EntryReader);
        }

        if (alarmMask & quint8(ActivityManager::AlarmGroup::ExitReader))
        {
          dsResponse << AM->getAlarm(ActivityManager::AlarmGroup::ExitReader);
        }

        if (alarmMask & quint8(ActivityManager::AlarmGroup::Logic))
        {
          dsResponse << AM->getAlarm(ActivityManager::AlarmGroup::Logic);
        }

        if (alarmMask & quint8(ActivityManager::AlarmGroup::Sc))
        {
          dsResponse << AM->getAlarm(ActivityManager::AlarmGroup::Sc);
        }
      }

      if (0 != (pingMode & COUNTER_REQUEST))
      {
        dsResponse << gateStatus.counterA << gateStatus.counterB;
      }

      if (0 != (pingMode & FILE_RECEIVE))
      {
        // qDebug() << "PM: FILE_RECEIVE";
        response.append(handleFileReceive(packet.data.mid(2)));
      }

      if (0 != (pingMode & FILE_TRANSFER))
      {
        qDebug() << "PM: FILE_TRANSFER";
        ParameterManager::getInstance()->setStatus(ParameterManager::Status::DownloadingParameters);
        response.append(handleFileTransfer(packet.data.mid(2)));
      }
      else
      {
        ParameterManager::getInstance()->setStatus(ParameterManager::Status::Normal);
      }

      if (0 != (pingMode & FILE_CLOSE_REQUEST))
      {
        qDebug() << "FILE_CLOSE_REQUEST";
        AM->closeActFile(QDateTime::currentDateTimeUtc());
      }
    }
    break;
    case Command::SetDateTime:
    {
      qDebug() << "SetDateTime";

      QByteArray   tmp = packet.data;
      QDataStream  dsTmp(&tmp, QIODevice::ReadOnly);
      quint16      year;
      quint8       month, day, dayOfWeek, hour, minute, second;

      dsTmp >> year >> month >> day >> dayOfWeek >> hour >> minute >> second;
      Utilities::setSystemDateTime(QDateTime(QDate(year, month, day), QTime(hour, minute, second), Qt::UTC));
    }
    break;
    case Command::GetDateTime:
    {
      const auto &now = QDateTime::currentDateTimeUtc();

      qDebug() << "GetDateTime";
      quint16     year;
      quint8      month, day, dayOfWeek, hour, minute, second;

      year      = now.date().year();
      month     = now.date().month();
      day       = now.date().day();
      dayOfWeek = now.date().dayOfWeek();
      hour      = now.time().hour();
      minute    = now.time().minute();
      second    = now.time().second();
      dsResponse << year << month << day << dayOfWeek << hour << minute << second;
    }
    break;
    case Command::LogIn:
    {
      qDebug() << "LogIn" << Utilities::readUint16(packet.data.mid(1, 2));

      QrDbManager::getInstance().rebuildOfflineDb(false);

      ActivityManager::ShiftValidationLog  shift;
      shift.configuration = AM->getConfigurationMode();
      shift.date          = QDateTime::currentDateTimeUtc();
      shift.shiftMode     = static_cast<ActivityManager::ShiftMode>(packet.data.at(0));
      shift.shiftId       = Utilities::readUint16(packet.data.mid(1, 2));
      shift.operatorId    = Utilities::readUint16(packet.data.mid(3, 2));
      shift.gateId        = 0;
      shift.stationId     = AM->getStationId();

      const auto &acg = AM->getAcgType();

      if (ActivityManager::AccessControlGateType::Exit == acg)
      {
        shift.operatingMode = ActivityManager::GateOperatingMode::Exit;
      }
      else
      {
        shift.operatingMode = ActivityManager::GateOperatingMode::Entry;
      }

      if (AM->isShiftOpen())
      {
        GateController::Status  gateStatus;
        GateController::getInstance()->getStatus(&gateStatus);

        AM->closeShift(QDateTime::currentDateTimeUtc().time(), gateStatus.counterA, gateStatus.counterB);
        QrDbManager::getInstance().rebuildOfflineDb(false);
      }

      if (AM->isActOpen())
      {
        AM->closeActFile(QDateTime::currentDateTimeUtc());
      }

      {
        ActivityManager::ActivityValidationLog  log;
        const auto                             &eqid = Utilities::generateEquipmentSerialId();

        log.activityMode     = ActivityManager::ActivityMode::OperatingMode;
        log.date             = shift.date;
        log.companyId        = Settings::COMPANY_ID;
        log.stationId        = AM->getStationId();
        log.equipmentType    = ParameterManager::EquipmentType::Reader;
        log.equipmentSubType = Settings::EQUIPMENT_SUB_TYPE;
        memcpy(log.equipmentSerial, eqid.constData(), eqid.size());
        log.equipmentSoftwareRelease   = ((Settings::MAIN_SOFTWARE_VERSION & 0xFF00) >> 8);
        log.equipmentSoftwareAmendment = (Settings::MAIN_SOFTWARE_VERSION & 0x00FF);

        AM->openActFile(log);
      }

      AM->openShift(shift);
    }
    break;
    case Command::LogOut:
    {
      qDebug() << "LogOut";

      QrDbManager::getInstance().rebuildOfflineDb(false);

      if (AM->isShiftOpen())
      {
        GateController::Status  gateStatus;
        GateController::getInstance()->getStatus(&gateStatus);

        AM->closeShift(QDateTime::currentDateTimeUtc().time(), gateStatus.counterA, gateStatus.counterB);
      }

      if (AM->isActOpen())
      {
        AM->closeActFile(QDateTime::currentDateTimeUtc());
      }

      {
        ActivityManager::ShiftValidationLog  shift;

        shift.configuration = AM->getConfigurationMode();
        shift.date          = QDateTime::currentDateTimeUtc();
        shift.shiftMode     = static_cast<ActivityManager::ShiftMode>(packet.data.at(0));
        shift.shiftId       = 254;
        shift.operatorId    = 1;
        shift.gateId        = 0;
        shift.stationId     = AM->getStationId();

        ActivityManager::ActivityValidationLog  log;
        const auto                             &eqid = Utilities::generateEquipmentSerialId();

        log.activityMode     = ActivityManager::ActivityMode::OperatingMode;
        log.date             = shift.date;
        log.companyId        = Settings::COMPANY_ID;
        log.stationId        = shift.stationId;
        log.equipmentType    = ParameterManager::EquipmentType::Reader;
        log.equipmentSubType = Settings::EQUIPMENT_SUB_TYPE;
        memcpy(log.equipmentSerial, eqid.constData(), eqid.size());
        log.equipmentSoftwareRelease   = ((Settings::MAIN_SOFTWARE_VERSION & 0xFF00) >> 8);
        log.equipmentSoftwareAmendment = (Settings::MAIN_SOFTWARE_VERSION & 0x00FF);

        AM->openActFile(log);
        AM->openShift(shift);
      }

    }
    break;
    case Command::OpenStation:
    {
      qDebug() << "OpenStation";

      QByteArray   tmp = packet.data.mid(1);
      QDataStream  dsTmp(&tmp, QIODevice::ReadOnly);
      quint16      year;
      quint8       month, day, dayOfWeek, hour, minute, second;

      dsTmp >> year >> month >> day >> dayOfWeek >> hour >> minute >> second;
      auto  newDate = QDateTime(QDate(year, month, day), QTime(hour, minute, second), Qt::UTC);
      Utilities::setSystemDateTime(newDate);
qDebug() << "QQ 11";
      AM->setStationStatus(true);
qDebug() << "QQ 12";
      if (AM->isShiftOpen())
      {
        GateController::Status  gateStatus;
        GateController::getInstance()->getStatus(&gateStatus);
        AM->closeShift(newDate.time(), gateStatus.counterA, gateStatus.counterB);
      }
      qDebug() << "QQ 13";

      if (AM->isActOpen())
      {
        AM->closeActFile(newDate);
      }

      qDebug() << "QQ 14";

      //if (4 < _closeStationCounter)

//      {
//        const auto &now = QDateTime::currentDateTimeUtc();

//        if (0 == now.time().hour() && 0 == now.time().minute())
//        {
//          GateController::getInstance()->resetCounters();
//          GateController::getInstance()->closeGate();
//        }
//      }

      AM->reopenActFile();
      AM->reopenShift();

      qDebug() << "QQ 15";

      response = QString::number(ParameterManager::getInstance()->getParamsVersion()).rightJustified(2, '0').toLatin1();
    }
    break;
    case Command::CloseStation:
    {
      qDebug() << "CloseStation";

      QrDbManager::getInstance().rebuildOfflineDb(false);

      const auto &now = QDateTime::currentDateTimeUtc();
      AM->setStationStatus(false);

      if (AM->isShiftOpen())
      {
        GateController::Status  gateStatus;
        GateController::getInstance()->getStatus(&gateStatus);

        AM->closeShift(now.time(), gateStatus.counterA, gateStatus.counterB);
      }

      if (AM->isActOpen())
      {
        AM->closeActFile(now);
      }

      response = QString::number(ParameterManager::getInstance()->getParamsVersion()).rightJustified(2, '0').toLatin1();
    }
    break;
    case Command::OneFreePassA:
    {
      qDebug() << "OneFreePassA";

      if (ActivityManager::AccessControlGateType::Exit != AM->getAcgType())
      {
        GateController::getInstance()->authorize();
      }
    }
    case Command::OneFreePassB:
    {
      qDebug() << "OneFreePassB";

      if (ActivityManager::AccessControlGateType::Exit == AM->getAcgType())
      {
        GateController::getInstance()->authorize();
      }
    }
    break;
    case Command::GetInfo:
    {
      qDebug() << "GetInfo";

      quint16  numDevices  = 1;
      quint8   type        = static_cast<quint8>(ParameterManager::EquipmentType::Gcu);
      quint8   subType     = Settings::EQUIPMENT_SUB_TYPE;
      quint16  serialId    = Utilities::generateSerialId(AM->getStationId());
      quint8   equipmentId = Settings::EQUIPMENT_ID;

      dsResponse << numDevices << type << subType << serialId << equipmentId
                 << Settings::MAIN_SOFTWARE_VERSION << Settings::AUX_SOFTWARE_VERSION;
    }
    default:
    break;
    }

    packet.data = response;
    sendPacket(packet);
  }
}

void  Scu::onDisconnected()
{
  qDebug() << "SC disconnected";
  _connected = false;
}

void  Scu::onTimerTimeout()
{
  if (CONNECTION_TIMEOUT < QDateTime::currentMSecsSinceEpoch() - _lastDataTime)
  {
    _connected = false;
  }
  else
  {
    _connected = true;
  }
}

QByteArray  Scu::handleFileTransfer(const QByteArray &data)
{
  const auto &info = getFileInfo(data);

  if (info.name != _tempFileName)
  {
    _tempFileBuff.clear();
  }

  _tempFileName = info.name;
  _tempFileBuff.append(info.data);

  if (!_tempFileName.isEmpty() && (info.offset + info.data.size() == info.size))
  {
    QFile  file(QString(Settings::SCU_PARAMS_DIR) + "/" + _tempFileName);

    if (file.exists())
    {
      file.remove();
    }

    if (file.open(QIODevice::WriteOnly))
    {
      file.write(_tempFileBuff);
      file.flush();
      file.close();

      if (0 == _tempFileName.compare(QString(Settings::SCU_INITIATE_FILE_NAME) + Settings::SCU_PARAM_EXT, Qt::CaseInsensitive))
      {
        if (ActivityManager::ConfigurationMode::Online == ActivityManager::getInstance()->getConfigurationMode())
        {
          qDebug() << "WWWWWWWWW";
          ParameterManager::getInstance()->initiate();
        }
        else
        {
          qDebug() << "WWWWWWWWW 22222";
        }
      }
      else if (0 == _tempFileName.compare(QString(Settings::SCU_BASE_PARAM_FILE_NAME) + Settings::SCU_PARAM_EXT, Qt::CaseInsensitive))
      {
        qDebug() << "WWWWWWWWW 33333";
        ParameterManager::getInstance()->load();
      }
    }

    _tempFileBuff.clear();
    _tempFileName.clear();
  }

  // Next chunk offset.
  return data.mid(0, 16) + Utilities::uint32ToByteArray(info.offset + info.data.size());
}

void  Scu::setConfiguration(const ConfigurationMask &configuration)
{
  _configuration = configuration;
}

QByteArray  Scu::handleFileReceive(const QByteArray &data)
{
  const auto &info = getFileInfo(data);

  return prepareFileForSend(info, info.name.isEmpty() || info.name.contains(Settings::SCU_ACTS_EXT, Qt::CaseInsensitive));
}

Scu::FileInfo  Scu::getFileInfo(const QByteArray &data) const
{
  FileInfo  res;

  res.size = Utilities::readUint32(data.mid(0, sizeof(quint32)));
  res.name = QString(data.mid(4, 12));

  if (!res.name.isEmpty())
  {
    if (!res.name.simplified().contains("."))
    {
      res.name = res.name.simplified().append(Settings::SCU_PARAM_EXT);
    }
  }

  res.offset = Utilities::readUint32(data.mid(16, sizeof(quint32)));
  res.data   = data.mid(22);

  qDebug() << "Receiving/Sending File:" << res.name;
  qDebug() << "File Size:" << res.size;
  qDebug() << "File Offset:" << res.offset;
  qDebug() << "File datalength:" << res.data.size();

  return res;
}

QByteArray  Scu::prepareFileForSend(FileInfo info, bool isAct)
{
  QByteArray  result;

  if (isAct)
  {
    // Get next act file name.
    if (info.name.isEmpty())
    {
      QDir  d(Settings::SCU_ACTS_DIR);
      d.setFilter(QDir::Files | QDir::NoSymLinks | QDir::NoDotAndDotDot);
      info.name   = d.entryList().takeFirst();
      info.offset = 0;
    }
  }

  QString  path = (isAct) ? (QString(Settings::SCU_ACTS_DIR) + "/" + info.name)
                          : (QString(Settings::SCU_PARAMS_DIR) + "/" + info.name);
  QFile  file(path);

  if (file.open(QIODevice::ReadOnly))
  {
    if (file.seek(info.offset))
    {
      const qint64  BUFFER_MAX_SIZE = 1024;
      QByteArray    fileData        = file.read(BUFFER_MAX_SIZE);

      result.append(Utilities::uint32ToByteArray(file.size()));
      QString  tempFileName;

      if (!isAct)
      {
        tempFileName = info.name.remove(Settings::SCU_PARAM_EXT, Qt::CaseInsensitive);
        tempFileName = tempFileName.leftJustified(8, '\0', true);
        tempFileName.append(Settings::SCU_PARAM_EXT);
      }
      else
      {
        tempFileName = info.name.remove(Settings::SCU_ACTS_EXT, Qt::CaseInsensitive);
        tempFileName = tempFileName.leftJustified(8, '\0', true);
        tempFileName.append(Settings::SCU_ACTS_EXT);
      }

      result.append(tempFileName);
      result.append(Utilities::uint32ToByteArray(info.offset));
      result.append(Utilities::uint16ToByteArray(fileData.size()));

      if (info.offset + fileData.size() == info.size)
      {
        file.remove();
      }

      result.append(fileData);
    }
  }
  else
  {
    result.append(info.toByteArray());
    result.append(QByteArray::fromHex("0000"));
  }

  return result;
}

void  Scu::sendPacket(const NetworkPacket &packet)
{
  _client->write(packet.toByteArray());
}

QByteArray  Scu::FileInfo::toByteArray(bool skipData) const
{
  QByteArray   res;
  QDataStream  ds(&res, QIODevice::WriteOnly);
  QByteArray   nameBuffer(name.leftJustified(12, '\0').toLatin1());

  ds << size;
  ds.writeRawData(nameBuffer.data(), nameBuffer.size());
  ds << offset;

  if (!skipData)
  {
    // TODO:
  }

  return res;
}

qint32  Scu::NetworkPacket::fromByteArray(const QByteArray &buffer, Scu::NetworkPacket *packet)
{
  qint32  res = 0;

  if (7 <= buffer.size())
  {
    qint32  packetSize = 7 + static_cast<quint8>(buffer[3]) * 256 + static_cast<quint8>(buffer[4]);

    if (packetSize <= buffer.size())
    {
      QDataStream  ds(buffer);
      quint16      dataLength, crc;

      ds >> packet->header >> packet->address >> packet->command >> dataLength;
      packet->data.resize(dataLength);
      ds.readRawData(packet->data.data(), dataLength);
      ds >> crc;

      res = packetSize;
    }
  }

  return res;
}

Scu::ConfigurationMask  Scu::NetworkPacket::getConfiguration() const
{
  return static_cast<Scu::ConfigurationMask>(data.at(1));
}

quint8  Scu::NetworkPacket::getPingMode() const
{
  return static_cast<quint8>(data.at(0));
}

QByteArray  Scu::NetworkPacket::toByteArray() const
{
  QByteArray   buffer;
  QDataStream  ds(&buffer, QIODevice::WriteOnly);
  quint16      dataLength = data.length();

  ds << header << address << command << dataLength;
  ds.writeRawData(data.data(), dataLength);
  ds << Utilities::crc16ModBus(buffer.mid(1));

  return buffer;
}
}
