#ifndef CITYPAYTRANSACTIONLOG_H
#define CITYPAYTRANSACTIONLOG_H

#include <TransactionLog.h>
#include <QDateTime>


namespace Afc
{
class CityPayTransactionLog: public TransactionLog
{
public:
  enum Version
  {
    Version3 = 3,
    Version4 = 4
  };

  ///
  /// \brief The MediaIdCityPay struct
  ///
  struct MediaIdCityPay
  {
    QByteArray  cardSn;
    QByteArray  pan;
  };

  ///
  /// \brief The MediaIdCityWay struct
  ///
  struct MediaIdCityWay
  {
    QByteArray  cardSn;
  };

  CityPayTransactionLog(Version version, quint32 lineId, quint32 paymentInfo, const QByteArray &mediaInfo,
                        const MediaIdCityPay &mediaId, const QString &merchantIndex);

  CityPayTransactionLog(quint32 lineId, quint32 paymentInfo, const QByteArray &mediaInfo,
                        const MediaIdCityWay &mediaId, const QString &merchantIndex);


  // TransactionLog interface

public:
  QByteArray  serialize() const final;

private:
  QByteArray  _data;
};
}

#endif // CITYPAYTRANSACTIONLOG_H
