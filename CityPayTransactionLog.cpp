#include <CityPayTransactionLog.h>


namespace Afc
{
CityPayTransactionLog::CityPayTransactionLog(Version version, quint32 lineId, quint32 paymentInfo, const QByteArray &mediaInfo,
                                             const CityPayTransactionLog::MediaIdCityPay &mediaId, const QString &merchantIndex)
{
  _data.append((Version4 == version) ? "P4;" : "P;");

  // TODO: is 6 ok?
  _data.append(QString::number(lineId).rightJustified(6, '0') + ";");
  _data.append(QString::number(paymentInfo) + ";");
  _data.append(mediaInfo.toHex() + ";");
  _data.append(mediaId.cardSn.toHex() + ";");
  _data.append(mediaId.pan.toHex() + ";");
  _data.append(merchantIndex);
}

CityPayTransactionLog::CityPayTransactionLog(quint32 lineId, quint32 paymentInfo, const QByteArray &mediaInfo,
                                             const CityPayTransactionLog::MediaIdCityWay &mediaId, const QString &merchantIndex)
{
  _data.append("W;");

  // TODO: is 6 ok?
  _data.append(QString::number(lineId).rightJustified(6, '0') + ";");
  _data.append(QString::number(paymentInfo) + ";");
  _data.append(mediaInfo.toHex() + ";");
  _data.append(mediaId.cardSn.toHex() + ";");
  _data.append(merchantIndex);
}

QByteArray  CityPayTransactionLog::serialize() const
{
  return _data;
}
}
