#include <DesfireCard.h>


namespace Afc
{
DesfireCard::DesfireCard(const QByteArray &cardId):
  _type(CreditCardType::Desfire), _id(cardId)
{
}

DesfireCard::~DesfireCard()
{
}

QByteArray  DesfireCard::getId() const
{
  return _id;
}

CreditCardType  DesfireCard::getType() const
{
  return _type;
}
}
