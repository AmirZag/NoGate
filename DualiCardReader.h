#ifndef DUALICARDREADER_H
#define DUALICARDREADER_H

#include <CardReader.h>

class QSerialPort;

namespace Afc
{
class DualiCardReader: public CardReader
{
  enum CommandId
  {
    CMD_RF_ON               = 0x10,
    CMD_RF_OFF              = 0x11,
    CMD_RESET               = 0x12,
    CMD_GET_VERSION         = 0x16,
    CMD_RF_RESET            = 0x20,
    CMD_IDLE_REQUEST        = 0x21,
    CMD_WAKE_UP             = 0x22,
    CMD_ANTI_COLLISION      = 0x23,
    CMD_SELECT_CARD         = 0x24,
    CMD_HALT                = 0x26,
    CMD_READ_BLK            = 0x27,
    CMD_WRITE_BLK           = 0x28,
    CMD_AUTH_KEY            = 0x30,
    CMD_DEC_TRANSFER_VERIFY = 0x34,
    CMD_REQ_ANTI_SEL        = 0x39,
    CMD_ANTI_SEL            = 0x3D,
    CMD_RF_FIND_CARD        = 0x4C,
    CMD_RF_APDU             = 0x61,
    CMD_IC_POWER_ON         = 0xC0,
    CMD_IC_STATUS           = 0xCA,
    CMD_IC_CASE1            = 0xC1,
    CMD_IC_CASE2            = 0xC2,
    CMD_IC_CASE3            = 0xC3,
    CMD_IC_CASE4            = 0xC4,
  };
  enum ResponseErrorCode
  {
    RES_OK                      = 0x00,
    RES_NO_RESPONSE             = 0x02,
    RES_CRC_ERR                 = 0x03,
    RES_SMART_CARD_NOT_INSERTED = 0x04,
    RES_MIFARE_AUTH_ERR         = 0x05,
    RES_SMART_CARD_OFF          = 0x05,
    RES_WRONG_COMMAND           = 0x07,
    RES_PROTOCOL_MISS_MATCH     = 0x0C,
    RES_MIFARE_WRITE_ERR        = 0x0F,
    RES_COMMAND_NOT_SUPPORTED   = 0x17,
    RES_COLLISION_DETECTED      = 0x8,
    RES_TEMPERATURE_ERR         = 0x4C,
    RES_NOT_IMPLEMENTED         = 0x64,
    RES_DATA_OVERFLOW           = 0x7B,
  };
  struct CommandResponse
  {
    QByteArray  data;
    quint8      errorCode;
  };

public:
  DualiCardReader(const QString &serialPortName);

  ~DualiCardReader() final;

  // Device interface.

public:
  Result  initializeDevice() final;

  Result  restartDevice() final;

  Result  getVersion(QByteArray *version) final;

  // CardReader interface.

public:
  bool  isResponding() final;

  Result  resetRf() final;

  Result  searchCard(QSharedPointer<CreditCard> *card) final;

  Result  cardApdu(const ApduCommand &apdu, ApduResponse *response) final;

  Result  samGetStatus(quint8 slot) final;

  Result  samPowerOn(quint8 slot, QByteArray *atr) final;

  Result  samApdu(quint8 slot, const ApduCommand &apdu, ApduResponse *response) final;

  Result  mifareAuthBlock(BlockAuthMode mode, const QByteArray &sectorKey, quint8 blockNumber) final;

  Result  mifareReadBlock(quint8 blockNumber, MifareMemoryBlock *data) final;

  Result  mifareWriteBlock(quint8 blockNumber, const MifareMemoryBlock &data) final;

  Result  mifareDecrementTransfer(quint8 blockNumber, quint32 number, quint8 targetBlock) final;

private:
  Result  sendCommand(const QByteArray &cmd, CommandResponse *response = nullptr);

  Result  getResponse(CommandResponse *response);

  void    setRfState(bool turnOn);

private:
  QString                      _portName;
  QSharedPointer<QSerialPort>  _serialPort;
  QByteArray                   _dataBuffer;
  CommandId                    _lastCmdId;
  bool                         _notResponding;
};
}

#endif // DUALICARDREADER
