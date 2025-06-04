#ifndef QRTICKET_H
#define QRTICKET_H

#include <QrCode.h>
#include <QDateTime>


namespace Afc
{
class QrTicket: public QrCode
{
public:
  QrTicket(const QByteArray &data);

  QString getEncryptedSection() const;

  QString getDecryptedSection() const;

  QString getSerialNumber() const;

  QString getSpare() const;

  QDate   getIssuanceDate() const;

  QString getIssuanceDateString() const;

  QString getValidityDays() const;

  QString getPrice() const;

  QString getQrType() const;

  quint16 getTicketType() const;

  QString getTicketTypeHex() const;

  QString getIssuerType() const;

  QString getIssuerId() const;

  QString getIssuerIpv4() const;

  QString getIssuerCounter() const;

  QString getMac() const;

  QString getKeyId() const;

  quint32 getTotalTripCount() const;

  void  prettyPrint() const;

  bool isValid() const;

  static constexpr auto ENC_DATA_SIZE = 44;

private:
  bool _isValid;
};
}

#endif // QRTICKET_H
