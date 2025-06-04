#ifndef METROBUSINESS_H
#define METROBUSINESS_H

#include <Business.h>
#include <CardReader.h>
#include <QrServerManager.h>
#include <ParameterManager.h>
#include <QObject>
#include <QFile>


class QTimer;


namespace Afc
{
class WindowManager;
class BarcodeReader;
class MifareCard;
class CityPayCard;
class CityWayCard;
class CityPaySam;
class Barcode;
class TransactionLog;

class MetroBusiness: public QObject, public Business
{
  Q_OBJECT

public:

  MetroBusiness(WindowManager *windowMgr, const QSharedPointer<CardReader>    &cardReader,
                const QSharedPointer<BarcodeReader> &barcodeReader, const QSharedPointer<CityPaySam> &citypaySam);

  ~MetroBusiness() final;


  // Business interface

public:

  void    start() final;

  void    stop() final;

private slots:

  void    onTimerTimeout();

  void    onTimerSessionTimeout();

  void    onQrValidationReady(const QSharedPointer<QrTicket> &ticket, const QrServerManager::ValidationResult &result);

private:

  void    saveSession();

  void    loadSession();

  Result  getValidationActions(const ParameterManager::ValidationProperty  &validationProp,
                               QVector<ParameterManager::ValidationAction> *actions);

  // Result  transactWithCard(const QSharedPointer<MifareCard> &card);

  Result  transactWithCard(const QSharedPointer<CityPayCard> &card);

  Result  transactWithCard(const QSharedPointer<CityWayCard> &card);

  Result  transactWithQr(const QSharedPointer<QrTicket> &barcode);

  Result  execCityPayApdu(const CardReader::ApduCommand &apdu, CardReader::ApduResponse *response);

  Result  purchaseCityWay(const QByteArray &cardId, quint32 amount, quint32 minBalance, quint32 *newBalance, QByteArray *cwCode);

  Result  purchaseCityPay(const QByteArray &cardId, quint32 amount, QByteArray *cardSerial, QByteArray *cardPan,
                          quint32 *cardVersion, quint32 *newBalance, QByteArray *cwCode);

private:
  WindowManager                 *_windowManager;
  QSharedPointer<CardReader>     _cardReader;
  QSharedPointer<BarcodeReader>  _barcodeReader;
  QSharedPointer<CityPaySam>     _citypaySam;
  QSharedPointer<QTimer>         _timerSearchCard;
  QSharedPointer<QTimer>         _timerSession;
  QSharedPointer<CreditCard>     _lastDetectedCard;
  Result                         _lastTransResult;
  qint64                         _lastCardDetectionTime;
  qint64                         _lastQrDetectionTime;
  quint32                        _transactionId;
  QFile                          _fileSession;
  QDataStream                    _streamSession;
};
}

#endif // METROBUSINESS_H
