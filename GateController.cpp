#include <GateController.h>
#include <Scu.h>
#include <Utilities.h>
#include <QtEndian>
#include <qserialport.h>


#define SESSION_FILE_NAME  "~gate"

#define SET_BIT(N) (1 << N)
#define HAS_BIT(X, N) (0 != (X & SET_BIT(N)))

namespace Afc
{
constexpr int  TIMEOUT_MSEC = 600;

GateController::GateController():
  _mode(0), _queryCount(0), _lastCounterA(0), _lastCounterB(0), _sessionCounterA(0), _sessionCounterB(0),
  _counterFlushed(false), _pendingReset(false), _timeoutCounter(0)
{
  _status.counterA = 0;
  _status.counterB = 0;
}

GateController::~GateController()
{
  if (_serialPort && _serialPort->isOpen())
  {
    _serialPort->close();
  }
}

Result  GateController::authorize()
{
  QByteArray  cmd;

  cmd.append(GateController::CMD_AUTHORIZE);

  Result res = sendCommand(cmd);

  for (int i = 0; i < 2 && false == res.succeed; ++i)
  {
    res = sendCommand(cmd);
  }

  return res;
}

Result  GateController::closeGate()
{
  QByteArray  cmd;

  cmd.append(GateController::CMD_CLOSE_GATE);

  return sendCommand(cmd);
}

Result  GateController::getStatus(Status *status)
{
// QMutexLocker  lockStatus(&_mutexStatus);

  *status = _status;

  return Result(true);
}

Result  GateController::resetCounters(const QString &commander)
{
  _lastCounterA    = 0;
  _lastCounterB    = 0;
  _sessionCounterA = 0;
  _sessionCounterB = 0;
  _status.counterA = 0;
  _status.counterB = 0;
  _lastClearedBy   = commander;
  _lastClearedDate = QDateTime::currentDateTimeUtc();
  _pendingReset = true;

  saveSession();

  return closeGate();
}

Result  GateController::setConfiguration(quint8 mode)
{
  _mode = mode;

  QByteArray  cmd;

  cmd.append(GateController::CMD_SET_CONFIG);
  cmd.append(0x01);
  cmd.append(mode);

  return sendCommand(cmd);
}

// Result  GateController::setMode(bool online)
// {
// QByteArray  cmd;

// cmd.append(GateController::CMD_SET_MODE);
// cmd.append(0x01);
// cmd.append(online ? 0x01 : char(0x00));

// return sendCommand(cmd);
// }

Result  GateController::initializeDevice(const QString &serialPortName)
{
  Result  res;

  _portName   = serialPortName;
  _serialPort = QSharedPointer<QSerialPort>(new QSerialPort(_portName));

  if (!_serialPort->open(QIODevice::ReadWrite))
  {
    res.message = _serialPort->errorString() + " " + _portName;
  }
  else
  {
    const bool  baudSet = _serialPort->setBaudRate(QSerialPort::Baud115200);
    const bool  dataSet = _serialPort->setDataBits(QSerialPort::Data8);
    const bool  flowSet = _serialPort->setFlowControl(QSerialPort::NoFlowControl);
    const bool  stopSet = _serialPort->setStopBits(QSerialPort::OneStop);
    const bool  pairSet = _serialPort->setParity(QSerialPort::NoParity);

    if (baudSet && dataSet && flowSet && stopSet && pairSet)
    {
      loadSession();
      saveSession();

      connect(&_timerStatus, SIGNAL(timeout()), this, SLOT(onTimerGetStatus()));
      _timerStatus.setInterval(1000);
      _timerStatus.start();

      connect(&_timerSession, SIGNAL(timeout()), this, SLOT(onTimerSession()));
      _timerSession.setInterval(3000);
      _timerSession.start();

      res.succeed = true;
    }
    else
    {
      _serialPort->close();
      res.message = "Failed to initialize port.";
    }
  }

  return res;
}

Result  GateController::sendCommand(const QByteArray &cmd, CommandResponse *response)
{
  Result      res;
  QByteArray  gateCmd;

  gateCmd.append(0x05);
  gateCmd.append(cmd);
  const auto &crc = Utilities::crc16ModBus(gateCmd);
  gateCmd.append(crc & 0xFF);
  gateCmd.append((crc & 0xFF00) >> 8);
  // gateCmd.append(CRC(gateCmd));

#ifdef VERBOSE_LOG_GATE
  qDebug() << "[Gate] sending command:" << gateCmd.toHex();
#endif

  const auto &bytesWritten = _serialPort->write(gateCmd);
  const auto &waitResult   = _serialPort->waitForBytesWritten(TIMEOUT_MSEC);

  if ((bytesWritten < 0) || !waitResult)
  {
    res.message = "Error in sending data to device.";
  }
  else
  {
    res = getResponse(response);
  }

  return res;
}

Result  GateController::getResponse(CommandResponse *response)
{
  Result  res(false, "No response from device.");
  qint32  packetSize = 0;

  {
    if (_serialPort->waitForReadyRead(TIMEOUT_MSEC))
    {
      _dataBuffer.append(_serialPort->readAll());

      if (5 <= _dataBuffer.size())
      {
        packetSize = 5 + static_cast<quint8>(_dataBuffer[2]);

        if (packetSize <= _dataBuffer.size())
        {
          res.succeed = true;
          res.message = "";

          if (response != nullptr)
          {
            response->data = _dataBuffer.mid(3, packetSize - 5);
          }

#ifdef VERBOSE_LOG_GATE
          qDebug() << "[Gate] response:" << _dataBuffer.left(packetSize).toHex();
#endif
        }
        else
        {
        }
      }
      else
      {
      }

      res.succeed = !_dataBuffer.isEmpty();
      _dataBuffer.clear();
    }
    else
    {
      // break;
    }
  }

  return res;
}

void  GateController::onTimerGetStatus()
{
  QByteArray       cmd;
  CommandResponse  response;

  cmd.append(GateController::CMD_GET_DATA);

  if (sendCommand(cmd, &response))
  {
    if (13 <= response.data.size())
    {
      _status.mode          = response.data[1];
      _status.configuration = response.data[2];
      _status.alarm         = qFromLittleEndian<quint16>(reinterpret_cast<uchar *>(response.data.mid(3, 2).data()));

      // Mapping gate alarm code to SCU alarm code.
      {
        quint16 mappedAlarm = 0;
        if (HAS_BIT(_status.alarm, 3))
        {
          mappedAlarm |= SET_BIT(8);
        }

        if (HAS_BIT(_status.alarm, 4))
        {
          mappedAlarm |= SET_BIT(6);
        }

        if (HAS_BIT(_status.alarm, 6))
        {
          mappedAlarm |= SET_BIT(1);
        }

        if (HAS_BIT(_status.alarm, 9))
        {
          mappedAlarm |= SET_BIT(3);
        }

        if (HAS_BIT(_status.alarm, 10))
        {
          mappedAlarm |= SET_BIT(4);
        }

        if (HAS_BIT(_status.alarm, 11))
        {
          mappedAlarm |= SET_BIT(2);
        }

        _status.alarm = mappedAlarm;
      }

      const quint32  counterA = qFromLittleEndian<quint32>(reinterpret_cast<uchar *>(response.data.mid(5, 4).data()));
      const quint32  counterB = qFromLittleEndian<quint32>(reinterpret_cast<uchar *>(response.data.mid(9, 4).data()));

      if (!_counterFlushed)
      {
        if ((0 == counterA) && (0 == counterB))
        {
          _counterFlushed = true;
        }
        else if (counterA <= 1 && counterB <= 1)
        {
          localResetCounters();
        }
        else
        {
          if (_sessionCounterA < counterA)
          {
            _sessionCounterA = counterA;
          }

          if (_sessionCounterB < counterB)
          {
            _sessionCounterB = counterB;
          }

          localResetCounters();
        }
      }

      if (((counterA < _lastCounterA) || (counterB < _lastCounterB)) && !_pendingReset)
      {
        _sessionCounterA += _lastCounterA;
        _sessionCounterB += _lastCounterB;
      }

      if (_counterFlushed)
      {
        _status.counterA = _sessionCounterA + counterA;
        _status.counterB = _sessionCounterB + counterB;
      }

      _lastCounterA = counterA;
      _lastCounterB = counterB;

      if (0 == counterA && 0 == counterB)
      {
        _pendingReset = false;
      }
    }
    else
    {
      _status.alarm = SET_BIT(5);
    }

    _timeoutCounter = 0;
  }
  else
  {
    if (_timeoutCounter < 4)
    {
      ++_timeoutCounter;
    }

    if (3 < _timeoutCounter)
    {
      _status.alarm = SET_BIT(5);
    }
  }
}

void  GateController::onTimerSession()
{
  saveSession();
}

void  GateController::saveSession()
{
  _fileSession.seek(0);
  _streamSession.setDevice(&_fileSession);

  {
    _streamSession << _status.counterA << _status.counterB << _mode << _lastClearedBy << _lastClearedDate;
  }

  _fileSession.flush();
}

void  GateController::loadSession()
{
  _fileSession.setFileName(SESSION_FILE_NAME);
  bool  hasSession = _fileSession.exists();

  if (_fileSession.open(QIODevice::ReadWrite))
  {
    _streamSession.setDevice(&_fileSession);

    if (hasSession)
    {
      _streamSession >> _sessionCounterA >> _sessionCounterB >> _mode >> _lastClearedBy >> _lastClearedDate;
      _status.counterA = _sessionCounterA;
      _status.counterB = _sessionCounterB;
    }
  }
}

Result  GateController::localResetCounters()
{
  QByteArray  cmd;

  cmd.append(GateController::CMD_CLOSE_GATE);

  return sendCommand(cmd);
}

QDateTime GateController::getLastClearedDate() const
{
  return _lastClearedDate;
}

QString GateController::getLastClearedBy() const
{
  return _lastClearedBy;
}
}
