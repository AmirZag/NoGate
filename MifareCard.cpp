#include <MifareCard.h>


namespace Afc
{
MifareCard::MifareCard(const QByteArray &cardId):
  _type(CreditCardType::Mifare), _id(cardId)
{
}

MifareCard::~MifareCard()
{
}

QByteArray  MifareCard::getId() const
{
  return _id;
}

CreditCardType  MifareCard::getType() const
{
  return _type;
}
}
