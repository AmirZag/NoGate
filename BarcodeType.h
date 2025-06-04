#ifndef BARCODETYPE_H
#define BARCODETYPE_H

#include <QString>


namespace Afc
{
enum class BarcodeType
{
  Qr,
};

inline QString  barcodeTypeToString(BarcodeType type)
{
  QString  res;

  switch (type)
  {
  case BarcodeType::Qr:
    res = "QR";
    break;
  default:
    res = "Unknown";
    break;
  }

  return res;
}
}

#endif // BARCODETYPE_H
