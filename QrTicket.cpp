#include <QrTicket.h>
#include <QrKeyStore.h>
#include <Cryptography.h>
#include <QVector>
#include <QDebug>


namespace Afc
{
QrTicket::QrTicket(const QByteArray &data): QrCode(QByteArray())
{
  try
  {
    QString const keyId(data[data.size() - 1]);
    auto const encData = data.mid(ENC_DATA_SIZE, data.size() - ENC_DATA_SIZE - 1);
    auto const decData = Cryptography::decryptAes(QrKeyStore::getInstance().getKey(keyId.toInt()), QByteArray::fromBase64(encData));


    _data.append(data.left(ENC_DATA_SIZE));
    _data.append(decData);
    _data.append(keyId);

#ifdef VERBOSE_LOG
    qDebug() <<"ticket key" << keyId << data << data.size();
    qDebug() << "encData" << encData << encData.size() << QByteArray::fromBase64(encData).size();
    prettyPrint();
#endif
  }
  catch (const std::exception &e)
  {
#ifdef VERBOSE_LOG
    qDebug() << "QrTicketException:" << e.what();
#endif
  }
  catch(...)
  {
#ifdef VERBOSE_LOG
    qDebug() << "WTF";
#endif
  }
}

QString QrTicket::getEncryptedSection() const
{
  return _data.left(ENC_DATA_SIZE);
}

QString QrTicket::getDecryptedSection() const
{
  return _data.mid(ENC_DATA_SIZE, _data.size() - ENC_DATA_SIZE - 1);
}

QString QrTicket::getSerialNumber() const
{
  return _data.mid(ENC_DATA_SIZE, 12);
}

QString QrTicket::getSpare() const
{
  return _data.mid(ENC_DATA_SIZE + 12, 1);
}

QDate QrTicket::getIssuanceDate() const
{
  return QDate::fromString(getIssuanceDateString(), "yyMMdd").addYears(100);
}

QString QrTicket::getIssuanceDateString() const
{
  return _data.mid(ENC_DATA_SIZE + 13, 6);
}

QString QrTicket::getValidityDays() const
{
  return _data.mid(ENC_DATA_SIZE + 19, 3);
}

QString QrTicket::getPrice() const
{
  return _data.mid(ENC_DATA_SIZE + 22, 8);
}

QString QrTicket::getQrType() const
{
  return _data.mid(ENC_DATA_SIZE + 30, 1);
}

quint16 QrTicket::getTicketType() const
{
  return getQrType().toUShort() + 244;
}

QString QrTicket::getTicketTypeHex() const
{
  return QString::number(getTicketType(), 16);
}

QString QrTicket::getIssuerType() const
{
  return _data.mid(ENC_DATA_SIZE + 31, 3);
}

QString QrTicket::getIssuerId() const
{
  return _data.mid(ENC_DATA_SIZE + 34, 12);
}

QString QrTicket::getIssuerIpv4() const
{
  QString res = getIssuerId();

  res = QString::number(res.mid(0, 3).toUInt())
        + "." + QString::number(res.mid(3, 3).toUInt())
        + "." + QString::number(res.mid(6, 3).toUInt())
        + "." + QString::number(res.mid(9, 3).toUInt());

  return res;
}

QString QrTicket::getIssuerCounter() const
{
  return _data.mid(ENC_DATA_SIZE + 46, 6);
}

QString QrTicket::getMac() const
{
  return _data.mid(ENC_DATA_SIZE + 52, 12);
}

QString QrTicket::getKeyId() const
{
  return _data.mid(ENC_DATA_SIZE + 64, 1);
}

quint32 QrTicket::getTotalTripCount() const
{
  quint32 res = 1;

  auto const type = getQrType().toUInt();

  switch (type)
  {
  case 1:
  case 2:
  {
    res = type;
  }
  break;
  case 7:
  case 8:
  {
    res = type - 6;
  }
  break;
  }

  return res;
}

void QrTicket::prettyPrint() const
{
  qDebug() << "Serial:" << getSerialNumber();
  qDebug() << "Issued Date:" << getIssuanceDateString();
  qDebug() << "Validity Days:" << getValidityDays();
  qDebug() << "Price:" << getPrice();
  qDebug() << "Type:" << getQrType() << "->" << getTicketType();
  qDebug() << "Issuer:" << getIssuerType() + getIssuerId() << getIssuerCounter();
}

bool QrTicket::isValid() const
{
  return _isValid;
}
}
