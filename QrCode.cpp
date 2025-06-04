#include <QrCode.h>

namespace Afc
{
QrCode::QrCode(const QByteArray &data):
  _type(BarcodeType::Qr), _data(data)
{
}

QByteArray  QrCode::getData() const
{
  return _data;
}

BarcodeType  QrCode::getBarcodeType() const
{
  return _type;
}
}
