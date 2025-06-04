#include <CityWayCard.h>
#include <Utilities.h>
#include <QDateTime>


namespace Afc
{
CityWayCard::CityWayCard(const QByteArray &cardId):
  _type(CreditCardType::CityWay), _id(cardId)
{
}

CityWayCard::~CityWayCard()
{
}

CardReader::ApduCommand  CityWayCard::authorizePin(const QByteArray &pin)
{
  return CardReader::ApduCommand(0x80, 0xC1, 0x01, 0x00, pin);
}

CardReader::ApduCommand  CityWayCard::purchase(quint8 samBin, const QByteArray &samTerminalId, const QDateTime &date,
                                               quint32 amount, quint16 transactionId, quint8 walletId,
                                               quint8 merchantId)
{
  QByteArray  data;

  data.append(samBin);
  data.append(samTerminalId);
  data.append(Utilities::convertDateToBcd(date));
  data.append((amount & 0xFF000000) >> 24);
  data.append((amount & 0x00FF0000) >> 16);
  data.append((amount & 0x0000FF00) >> 8);
  data.append((amount & 0x000000FF));

  // TODO: transactionId.
  data.append((char)0);
  data.append((transactionId & 0xFF00) >> 8);
  data.append((transactionId & 0x00FF));

  data.append(walletId);
  data.append(merchantId);

  return CardReader::ApduCommand(0x80, 0xC3, 0x00, 0x00, data, 0x18);
}

Result  CityWayCard::checkApduResponse(const CardReader::ApduResponse &response)
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
      res.message = "پایان تعداد تراکنش مجاز";
    }
    break;
    case 0xA2:
    {
      res.message = "سقف مبلغ تراکنش پر شده است";
    }
    break;
    case 0xA3:
    {
      res.message = "خارج از بازه زمانی سرویس دهی";
    }
    break;
    case 0xA4:
    {
      res.message = "مبلغ تراکنش غیر مجاز";
    }
    break;
    case 0xA5:
    {
      res.message = "مبلغ تراکنش غیر مجاز";
    }
    break;
    case 0xA6:
    {
      res.message = "پایان تاریخ اعتبار";
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
      res.message = "خطای امنیتی";
    }
    break;
    case 0xF4:
    {
      res.message = "خطای تراکنش";
    }
    break;
    case 0xF5:
    {
      res.message = "خطای تراکنش";
    }
    break;
    case 0xF8:
    {
      res.message = "خطای تراکنش";
    }
    break;
    case 0xFA:
    {
      res.message = "کارت مسدود شده است";
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

QByteArray  CityWayCard::getId() const
{
  return _id;
}

CreditCardType  CityWayCard::getType() const
{
  return _type;
}
}
