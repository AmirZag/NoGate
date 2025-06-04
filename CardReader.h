#ifndef CARDREADER_H
#define CARDREADER_H

#include <Device.h>
#include <MemoryBlock.h>
#include <QSharedPointer>


namespace Afc
{
class CreditCard;

class CardReader: public Device
{
public:
  using MifareMemoryBlock = MemoryBlock<16>;
  enum class BlockAuthMode
  {
    A,
    B
  };

  struct ApduCommand
  {
    ApduCommand(quint8 _cla, quint8 _ins, quint8 _p1, quint8 _p2)
    {
      cla = _cla;
      ins = _ins;
      p1  = _p1;
      p2  = _p2;
      le  = 0x00;
    }

    ApduCommand(quint8 _cla, quint8 _ins, quint8 _p1, quint8 _p2, quint8 _le)
    {
      cla = _cla;
      ins = _ins;
      p1  = _p1;
      p2  = _p2;
      le  = _le;
    }

    ApduCommand(quint8 _cla, quint8 _ins, quint8 _p1, quint8 _p2, const QByteArray &_lc)
    {
      cla = _cla;
      ins = _ins;
      p1  = _p1;
      p2  = _p2;
      lc  = _lc;
      le  = 0x00;
    }

    ApduCommand(quint8 _cla, quint8 _ins, quint8 _p1, quint8 _p2, const QByteArray &_lc, quint8 _le)
    {
      cla = _cla;
      ins = _ins;
      p1  = _p1;
      p2  = _p2;
      lc  = _lc;
      le  = _le;
    }

    quint8      cla;
    quint8      ins;
    quint8      p1;
    quint8      p2;
    quint8      le;
    QByteArray  lc;
  };
  struct ApduResponse
  {
    QByteArray  data;
    char        sw1;
    char        sw2;
  };

  // Common.
  virtual Result  resetRf() = 0;

  virtual Result  searchCard(QSharedPointer<CreditCard> *card) = 0;

  virtual Result  cardApdu(const ApduCommand &apdu, ApduResponse *response) = 0;

  virtual bool    isResponding() = 0;

  // Sam.
  virtual Result  samGetStatus(quint8 slot) = 0;

  virtual Result  samPowerOn(quint8 slot, QByteArray *atr) = 0;

  virtual Result  samApdu(quint8 slot, const ApduCommand &apdu, ApduResponse *response) = 0;

  // Mifare.
  virtual Result  mifareAuthBlock(BlockAuthMode mode, const QByteArray &sectorKey, quint8 blockNumber) = 0;

  virtual Result  mifareReadBlock(quint8 blockNumber, MifareMemoryBlock *data) = 0;

  virtual Result  mifareWriteBlock(quint8 blockNumber, const MifareMemoryBlock &data) = 0;

  virtual Result  mifareDecrementTransfer(quint8 blockNumber, quint32 number, quint8 targetBlock) = 0;
};
}

#endif // CARDREADER_H
