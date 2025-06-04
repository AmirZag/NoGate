#include <DualiCardReader.h>
#include <qserialport.h>
#include <MifareCard.h>
#include <DesfireCard.h>
#include <CityPayCard.h>
#include <QTimer>
#include <QDebug>

#ifdef VERBOSE_LOG
#include <QDebug>
#endif


namespace Afc
{
constexpr int  TIMEOUT_MSEC = 3000;

quint8  calculateLrc(const QByteArray &cmd)
{
  quint8  crc = 0x00;

  for (qint32 i = 0; i < cmd.size(); ++i)
  {
    crc ^= cmd[i];
  }

  return crc;
}

DualiCardReader::DualiCardReader(const QString &serialPortName):
  _portName(serialPortName), _notResponding(false)
{
  _serialPort = QSharedPointer<QSerialPort>(new QSerialPort(_portName));
}

DualiCardReader::~DualiCardReader()
{
  if (_serialPort && _serialPort->isOpen())
  {
    _serialPort->close();
  }
}

Result  DualiCardReader::initializeDevice()
{
  Result  res;

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
      setRfState(true);

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

Result  DualiCardReader::restartDevice()
{
  Result  res;

  return res;
}

Result  DualiCardReader::getVersion(QByteArray *version)
{
  Result           res;
  QByteArray       cmd;
  CommandResponse  response;

  cmd.append(CMD_GET_VERSION);

  if (res = sendCommand(cmd, &response))
  {
    if (nullptr != version)
    {
      *version = response.data;
    }
  }

  return res;
}

bool DualiCardReader::isResponding()
{
  return !_notResponding;
}

Result  DualiCardReader::resetRf()
{
  // resetTerminal();

  // return Result(true);
  QByteArray       cmd;
  CommandResponse  response;

  cmd.append(CMD_RF_RESET);

  // Delay in msec. (LSB first)
  cmd.append((char)0x00);
  cmd.append((char)0x00);

  return sendCommand(cmd, &response);
}

Result  DualiCardReader::searchCard(QSharedPointer<CreditCard> *card)
{
  Result           res;
  QByteArray       cmd;
  CommandResponse  response;

  cmd.append(CMD_RF_FIND_CARD);
  cmd.append((char)0x00);
  cmd.append((char)0x00);
  cmd.append(0x01);
  cmd.append((char)0x00);

  res = sendCommand(cmd, &response);

  if (res)
  {
    const char  cardType = response.data.at(0);
    const auto &id       = response.data.mid(2);

    switch (cardType)
    {
    case 'M':
    {
      *card = QSharedPointer<MifareCard>(new MifareCard(id));
    }
    break;
    case 'B':
    {
      *card = QSharedPointer<DesfireCard>(new DesfireCard(id));
    }
    break;
    case 'A':
    {
      *card = QSharedPointer<CityPayCard>(new CityPayCard(id));
    }
    break;
    default:
#ifdef VERBOSE_LOG
      qDebug() << "Unknown SAK:" << cardType;
#endif
    break;
    }
  }

  // Result           res;
  // QByteArray       cmd;
  // CommandResponse  response;

  // cmd.append(CMD_REQ_ANTI_SEL);
  // cmd.append((char)0x00);

  // res = sendCommand(cmd, &response);

  // if (res)
  // {
  // const quint8  sak = response.data.at(0);
  // const auto   &id  = response.data.mid(1);

  // switch (sak)
  // {
  // case 0x08:
  // case 0x28:
  // {
  // *card = QSharedPointer<MifareCard>(new MifareCard(id));
  // }
  // break;
  // case 0x20:
  // {
  // *card = QSharedPointer<DesfireCard>(new DesfireCard(id));
  // }
  // break;
  // case 0x38:
  // {
  // *card = QSharedPointer<CityPayCard>(new CityPayCard(id));
  // }
  // break;
  // default:
  // #ifdef VERBOSE_LOG
  // qDebug() << "Unknown SAK:" << sak;
  // #endif
  // break;
  // }
  // }

  return res;
}

Result  DualiCardReader::samGetStatus(quint8 slot)
{
  QByteArray  cmd;

  cmd.append(CMD_IC_STATUS);
  cmd.append(slot);

  return sendCommand(cmd);
}

Result  DualiCardReader::samPowerOn(quint8 slot, QByteArray *atr)
{
  Result           res;
  QByteArray       cmd;
  CommandResponse  response;

  cmd.append(CMD_IC_POWER_ON);
  cmd.append(slot);
  cmd.append(0xF0); // Auto PPS.

  res = sendCommand(cmd, &response);

  if (res && (atr != nullptr))
  {
    *atr = response.data;
  }

  return res;
}

Result  DualiCardReader::samApdu(quint8 slot, const ApduCommand &apdu, CardReader::ApduResponse *response)
{
  Result           res;
  QByteArray       cmd;
  CommandResponse  cmdResponse;
  quint8           mode = (0 == apdu.lc.size() ? 0 : 2) + (0 == apdu.le ? 0 : 1);

  cmd.append(CMD_IC_CASE1 + mode);
  cmd.append(slot);
  cmd.append(apdu.cla);
  cmd.append(apdu.ins);
  cmd.append(apdu.p1);
  cmd.append(apdu.p2);

  if ((2 == mode) || (3 == mode))
  {
    cmd.append(apdu.lc.size() & 0xFF);
    cmd.append(apdu.lc);
  }

  if (2 != mode)
  {
    cmd.append(apdu.le);
  }

  res = sendCommand(cmd, &cmdResponse);

  if (res)
  {
    if ((ResponseErrorCode::RES_OK == cmdResponse.errorCode) && (2 <= cmdResponse.data.size()))
    {
      if (nullptr != response)
      {
        if (2 < cmdResponse.data.size())
        {
          response->data = cmdResponse.data.left(cmdResponse.data.size() - 2);
        }

        response->sw1 = cmdResponse.data.at(cmdResponse.data.size() - 2);
        response->sw2 = cmdResponse.data.at(cmdResponse.data.size() - 1);

        if (0x61 == response->sw1)
        {
          res = samApdu(slot, ApduCommand(apdu.cla, 0xC0, 0x00, 0x00, response->sw2), response);
        }
      }
    }
    else
    {
      res.succeed = false;
    }
  }

  return res;
}

Result  DualiCardReader::mifareAuthBlock(BlockAuthMode mode, const QByteArray &sectorKey,
                                         quint8 blockNumber)
{
  QByteArray       cmd;
  CommandResponse  response;

  cmd.append(CMD_AUTH_KEY);

  if (BlockAuthMode::A == mode)
  {
    cmd.append((char)0x00);
  }
  else
  {
    cmd.append(0x04);
  }

  cmd.append(sectorKey);
  cmd.append(blockNumber);

  return sendCommand(cmd, &response);
}

Result  DualiCardReader::mifareReadBlock(quint8 blockNumber, MifareMemoryBlock *data)
{
  Result           res;
  QByteArray       cmd;
  CommandResponse  response;

  cmd.append(CMD_READ_BLK);
  cmd.append(blockNumber);

  res = sendCommand(cmd, &response);

  if (res && (nullptr != data))
  {
    *data = MifareMemoryBlock(response.data);
  }

  return res;
}

Result  DualiCardReader::mifareWriteBlock(quint8 blockNumber, const MifareMemoryBlock &data)
{
  QByteArray       cmd;
  CommandResponse  response;

  cmd.append(CMD_WRITE_BLK);
  cmd.append(blockNumber);
  cmd.append(data.getData());

  return sendCommand(cmd, &response);
}

Result DualiCardReader::mifareDecrementTransfer(quint8 blockNumber, quint32 number, quint8 targetBlock)
{
  QByteArray       cmd;
  CommandResponse  response;

  cmd.append(CMD_DEC_TRANSFER_VERIFY);
  cmd.append(blockNumber);
  cmd.append(QByteArray(reinterpret_cast<const char *>(&number), 4));
  cmd.append(targetBlock);

  return sendCommand(cmd, &response);
}

Result  DualiCardReader::cardApdu(const CardReader::ApduCommand &apdu, CardReader::ApduResponse *response)
{
  Result           res;
  QByteArray       cmd;
  CommandResponse  cmdResponse;
  quint8           mode = (0 == apdu.lc.size() ? 0 : 2) + (0 == apdu.le ? 0 : 1);

  cmd.append(CMD_RF_APDU);
  cmd.append(apdu.cla);
  cmd.append(apdu.ins);
  cmd.append(apdu.p1);
  cmd.append(apdu.p2);

  if ((2 == mode) || (3 == mode))
  {
    cmd.append(apdu.lc.size() & 0xFF);
    cmd.append(apdu.lc);
  }

  if (2 != mode)
  {
    cmd.append(apdu.le);
  }

  res = sendCommand(cmd, &cmdResponse);

  if (res)
  {
    if ((ResponseErrorCode::RES_OK == cmdResponse.errorCode) && (2 <= cmdResponse.data.size()))
    {
      if (nullptr != response)
      {
        if (2 < cmdResponse.data.size())
        {
          response->data = cmdResponse.data.left(cmdResponse.data.size() - 2);
        }

        response->sw1 = cmdResponse.data.at(cmdResponse.data.size() - 2);
        response->sw2 = cmdResponse.data.at(cmdResponse.data.size() - 1);

        if (0x61 == response->sw1)
        {
          res = cardApdu(ApduCommand(0x00, 0xC0, 0x00, 0x00, response->sw2), response);
        }
      }
    }
    else
    {
      res.succeed = false;
    }
  }

  return res;
}

Result  DualiCardReader::sendCommand(const QByteArray &cmd, CommandResponse *response)
{
  // DUALi Protocol Specification. Part 2.1.1, Command frame format.
  Result        res;
  QByteArray    dualiCmd;

  if (!_notResponding)
  {
    _lastCmdId = static_cast<CommandId>(cmd.at(0));

    dualiCmd.append(0x02); // STX.
    dualiCmd.append(cmd.size() / 0x100); // LEN-H.
    dualiCmd.append(cmd.size() % 0x100); // LEN-L.
    dualiCmd.append(cmd); // CMD & DATA.

    // Calculating LRC, skipping STX.
    const quint8  lrc = calculateLrc(dualiCmd.mid(1));

    dualiCmd.append(lrc);

#ifdef VERBOSE_LOG
    qDebug() << "[Duali] sending command:" << dualiCmd.toHex();
#endif

    const auto &bytesWritten = _serialPort->write(dualiCmd);
    const auto &waitResult   = _serialPort->waitForBytesWritten(TIMEOUT_MSEC);

    if ((bytesWritten < 0) || !waitResult)
    {
      _notResponding = true;
      res.message = "Error in sending data to device.";
    }
    else
    {
      res = getResponse(response);

      if (res)
      {
        if (response != nullptr)
        {
          if (ResponseErrorCode::RES_OK == response->errorCode)
          {
            res.succeed = true;
          }
          else
          {
            res.succeed = false;
          }
        }
      }
    }
  }

  return res;
}

Result  DualiCardReader::getResponse(CommandResponse *response)
{
  // DUALi Protocol Specification. Part 2.1.2, Response frame format.

  Result  res(false, "No response from device.");
  qint32  packetSize = 0;

  while (_dataBuffer.size() < 3 || _dataBuffer.size() < 4 + 0x100 * quint8(_dataBuffer[1]) + quint8(_dataBuffer[2]))
  {
    if (_serialPort->waitForReadyRead(TIMEOUT_MSEC))
    {
      _dataBuffer.append(_serialPort->readAll());

      if (0x02 != _dataBuffer[0])
      {
        break;
      }

      if (3 <= _dataBuffer.size())
      {
        packetSize = 4 + 0x100 * quint8(_dataBuffer[1]) + quint8(_dataBuffer[2]);

        if (packetSize <= _dataBuffer.size())
        {
          res.succeed = true;
          res.message = "";

          if (response != nullptr)
          {
            response->data      = _dataBuffer.mid(4, packetSize - 5);
            response->errorCode = _dataBuffer.at(3);
          }

#ifdef VERBOSE_LOG
          qDebug() << "[Duali] response:" << _dataBuffer.left(packetSize).toHex()
                   << "|||" << _dataBuffer.mid(4, packetSize - 5);
#endif

          _dataBuffer = _dataBuffer.mid(packetSize);

          break;
        }
      }
    }
    else
    {
      if (CMD_RF_RESET != _lastCmdId)
      {
        resetRf();
        break;
      }
      else
      {
        _notResponding = true;
        break;
      }
    }
  }

  return res;
}

void  DualiCardReader::setRfState(bool turnOn)
{
  QByteArray  cmd;

  if (turnOn)
  {
    cmd.append(CMD_RF_ON);
  }
  else
  {
    cmd.append(CMD_RF_OFF);
  }

  sendCommand(cmd);
}
}
