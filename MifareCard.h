#ifndef MIFARECARD_H
#define MIFARECARD_H

#include <CreditCard.h>
#include <MemoryBlock.h>


namespace Afc
{
class MifareCard: public CreditCard
{
public:
  MifareCard(const QByteArray &cardId);

  ~MifareCard() final;

  // CreditCard interface.

public:
  QByteArray      getId() const final;

  CreditCardType  getType() const final;

private:
  const CreditCardType  _type;
  const QByteArray      _id;
};
}

#endif // MIFARECARD_H
