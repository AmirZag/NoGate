#ifndef SAMCITYPAY
#define SAMCITYPAY

#include <CardReader.h>


class QDateTime;

namespace Afc
{
class CityPaySam
{
public:
  CityPaySam(const QSharedPointer<CardReader> &reader, quint8 slot, const QByteArray &key);

  ~CityPaySam();

  Result      initialize();


  Result      citywayInitForOperation(const QByteArray &cardSerial, CardReader::ApduResponse *response);

  Result      citywayGetBalance(const QByteArray &blkOneWithCounter, const QByteArray &counter, CardReader::ApduResponse *response);

  Result      citywayGetSectorKey(CardReader::BlockAuthMode mode, quint8 sector, CardReader::ApduResponse *response);

  Result      citywayPurchase(const QByteArray &blkOne, const QByteArray &counter, quint32 amount, const QDateTime &date,
                              CardReader::ApduResponse *response);

  Result      citypayEncryptForTransmission(const QByteArray &data, CardReader::ApduResponse *response);

  Result      citypaySignatureForPayment(QByteArray data, CardReader::ApduResponse *response);

  QByteArray  citypayGetSamTerminal() const;

  Result      citypaySetKey(const QByteArray &key, CardReader::ApduResponse *response);

  QByteArray  getSamSerial() const;

  bool        getCityPayInitialized() const;

  bool        getCityWayInitialized() const;

  QString     getSamModuleErrors() const;

  QString     getCityPayErrors() const;

  QString     getCityWayErrors() const;

private:
  Result      initializeCityPay();

  Result      initializeCityWay();

  Result      sendApdu(const CardReader::ApduCommand &apdu, CardReader::ApduResponse *response);

  QSharedPointer<CardReader>  _cardReader;
  quint8                      _samSlot;
  QByteArray                  _samSerial;
  QByteArray                  _samKey;
  QByteArray                  _samTerminal;
  QString                     _samModuleErrors, _cityPayErrors, _cityWayErrors;
  bool                        _cityPayInitialized, _cityWayInitialized;
};
}

#endif // SAMCITYPAY
