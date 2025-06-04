#ifndef QRCODE_H
#define QRCODE_H

#include <Barcode.h>


namespace Afc
{
class QrCode: public Barcode
{
public:
  QrCode(const QByteArray &data);

  // Barcode interface

public:
  QByteArray   getData() const override;

  BarcodeType  getBarcodeType() const override;

protected:
  const BarcodeType  _type;
  QByteArray   _data;
};
}


#endif // QRCODE_H
