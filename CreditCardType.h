#ifndef CREDITCARDTYPE_H
#define CREDITCARDTYPE_H

#include <QString>


namespace Afc
{
enum class CreditCardType
{
  Mifare,
  Desfire,
  CityWay,
  CityPay,
};

inline QString  cardTypeToString(CreditCardType type)
{
  QString  res;

  switch (type)
  {
  case CreditCardType::Mifare:
    res = "Mifare";
    break;
  case CreditCardType::Desfire:
    res = "Desfire";
    break;
  case CreditCardType::CityWay:
    res = "CityWay";
    break;
  case CreditCardType::CityPay:
    res = "CityPay";
    break;
  default:
    res = "Unknown";
    break;
  }

  return res;
}
}

#endif // CREDITCARDTYPE_H
