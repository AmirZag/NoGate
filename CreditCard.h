#ifndef CREDITCARD_H
#define CREDITCARD_H

#include <Result.h>
#include <CreditCardType.h>
#include <QByteArray>
#include <QSharedPointer>


namespace Afc
{
class CardReader;

class CreditCard
{
public:
  virtual ~CreditCard()
  {
  }

  virtual QByteArray      getId() const = 0;

  virtual CreditCardType  getType() const = 0;
};
}

#endif // CREDITCARD_H
