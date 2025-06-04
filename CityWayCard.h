#ifndef CITYWAYCARD_H
#define CITYWAYCARD_H


#include <CreditCard.h>
#include <CardReader.h>


class QDateTime;

namespace Afc
{
class CityWayCard: public CreditCard
{
public:
  struct MemoryMapping
  {
    quint8   endingValidityYear;
    quint8   endingValidityMonth;
    quint8   endingValidityDay;
    bool     blacklisted;
    quint8   companyId;
    quint8   sequenceID;
    quint32  totalAmount;
    quint8   remainingTravels;
    quint8   validationIndicator;
    quint16  validationStation;
    quint8   validationYear;
    quint8   validationMonth;
    quint8   validationDay;
    quint8   validationHour;
    quint8   validationMinute;
    quint8   contractId;
    quint8   rejectCode;
  };

public:
  CityWayCard(const QByteArray &cardId);

  ~CityWayCard() final;

  static CardReader::ApduCommand  authorizePin(const QByteArray &pin);

  static CardReader::ApduCommand  purchase(quint8 samBin, const QByteArray &samTerminalId, const QDateTime &date,
                                           quint32 amount, quint16 transactionId, quint8 walletId, quint8 merchantId);

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


#endif // CITYWAYCARD_H
