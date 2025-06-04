#ifndef CITYPAYCARD_H
#define CITYPAYCARD_H

#include <CreditCard.h>
#include <CardReader.h>


class QDateTime;

namespace Afc
{
class CityPayCard: public CreditCard
{
public:
  CityPayCard(const QByteArray &cardId);

  ~CityPayCard() final;

  static CardReader::ApduCommand  authorizePin(const QByteArray &pin);

  static CardReader::ApduCommand  getRandom();

  static CardReader::ApduCommand  getSerialNumber();

  static CardReader::ApduCommand  purchase(const QByteArray &mediaInfo);

  static CardReader::ApduCommand  selectApplet();

  static CardReader::ApduCommand  selectIsd();

  static CardReader::ApduCommand  secureChannel(const QByteArray &samToken);

  static Result                   checkApduResponse(const CardReader::ApduResponse &response);

  // CreditCard interface

public:
  QByteArray      getId() const;

  CreditCardType  getType() const;

private:
  const CreditCardType  _type;
  const QByteArray      _id;
};
}

#endif // CITYPAYCARD_H
