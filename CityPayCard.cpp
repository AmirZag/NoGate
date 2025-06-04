#include <CityPayCard.h>
#include <Utilities.h>
#include <QDateTime>
#include <QtEndian>

namespace Afc
{
CityPayCard::CityPayCard(const QByteArray &cardId):
  _type(CreditCardType::CityPay), _id(cardId)
{
}

CityPayCard::~CityPayCard()
{
}

CardReader::ApduCommand  CityPayCard::authorizePin(const QByteArray &pin)
{
  return CardReader::ApduCommand(0x80, 0xC1, 0x01, 0x00, pin);
}

CardReader::ApduCommand  CityPayCard::getRandom()
{
  return CardReader::ApduCommand(0x80, 0xA0, 0x00, 0x00, 0x08);
}

CardReader::ApduCommand  CityPayCard::getSerialNumber()
{
  return CardReader::ApduCommand(0x00, 0xCA, 0x9F, 0x7F, 0x2A);
}

CardReader::ApduCommand  CityPayCard::purchase(const QByteArray &mediaInfo)
{
  return CardReader::ApduCommand(0x80, 0xC3, 0x00, 0x00, mediaInfo, 0x18);
}

CardReader::ApduCommand  CityPayCard::selectApplet()
{
  return CardReader::ApduCommand(0x00, 0xA4, 0x04, 0x00, QByteArray("\xD3\x64\x24\x89\x22\x65\xA0\x00\x01", 9));
}

CardReader::ApduCommand  CityPayCard::selectIsd()
{
  return CardReader::ApduCommand(0x00, 0xA4, 0x04, 0x00, QByteArray("\xA0\x00\x00\x00\x03\x00\x00\0x00", 8));
}

CardReader::ApduCommand  CityPayCard::secureChannel(const QByteArray &samToken)
{
  return CardReader::ApduCommand(0x80, 0xCF, 0x00, 0x00, samToken);
}

Result  CityPayCard::checkApduResponse(const CardReader::ApduResponse &response)
{
  Result  res;

  switch (response.sw1)
  {
  case 0x63:
  {
    switch (response.sw2)
    {
    case 0x00:
    case 0x01:
    case 0x02:
    case 0x03:
    case 0x04:
    {
      res.message = "رمز وارد شده اشتباه است";
    }
    break;
    case 0xA0:
    {
      res.message = "موجودی کافی نیست";
    }
    break;
    case 0xA1:
    {
      res.message = "رسانه نیازمند تسویه است";
    }
    break;
    case 0xA2:
    {
      res.message = "رسانه نیازمند تسویه است";
    }
    break;
    case 0xA3:
    {
      res.message = "پایان ساعت استفاده";
    }
    break;
    case 0xA4:
    {
      res.message = "مبلغ بیش از حداکثر\nمبلغ مجاز برای هر تراکنش است";
    }
    break;
    case 0xA5:
    {
      res.message = "مبلغ کمتر از حداقل\nمبلغ مجاز برای هر تراکنش است";
    }
    break;
    case 0xA6:
    {
      res.message = "رسانه نیازمند تسویه است";
    }
    break;
    case 0xA7:
    {
      res.message = "کیف پول انتخاب شده اشتباه است";
    }
    break;
    case 0xF0:
    {
      res.message = "رمز وارد نشده است";
    }
    break;
    case 0xF1:
    {
      res.message = "تراکنش ناموفق";
    }
    break;
    case 0xF4:
    {
      res.message = "تراکنش ناموفق";
    }
    break;
    case 0xF5:
    {
      res.message = "تراکنش ناموفق";
    }
    break;
    case 0xF8:
    {
      res.message = "تراکنش ناموفق";
    }
    break;
    case 0xFA:
    {
      res.message = "کارت و یا رسانه مسدود است";
    }
    break;
    default:
      break;
    }
  }
  break;
  case 0x67:
  {
    res.message = "خطای پروتکل";
  }
  break;
  case 0x69:
  {
    switch (response.sw2)
    {
    case 0x82:
    {
      res.message = "خطای امنیتی";
    }
    break;
    case 0x86:
    {
      res.message = "رسانه راه اندازی نشده است";
    }
    break;
    default:
      break;
    }
  }
  break;
  case 0x6A:
  {
    switch (response.sw2)
    {
    case 0x80:
    {
      res.message = "خطای داده دریافتی";
    }
    break;
    case 0x82:
    {
      res.message = "داده مورد نظر یافت نشد";
    }
    break;
    case 0x86:
    {
      res.message = "خطای پروتکل";
    }
    break;
    default:
      break;
    }
  }
  break;
  case 0x90:
  {
    res.succeed = true;
  }
  break;
  default:
    break;
  }

  return res;
}

QByteArray  CityPayCard::getId() const
{
  return _id;
}

CreditCardType  CityPayCard::getType() const
{
  return _type;
}
}
