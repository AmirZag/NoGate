#ifndef DESFIRECARD_H
#define DESFIRECARD_H

#include <CreditCard.h>


namespace Afc
{
class DesfireCard: public CreditCard
{
public:
  DesfireCard(const QByteArray &cardId);

  ~DesfireCard() final;

  // CreditCard interface

public:
  QByteArray      getId() const;

  CreditCardType  getType() const;

private:
  const CreditCardType  _type;
  const QByteArray      _id;
};
}

#endif // DESFIRECARD_H
