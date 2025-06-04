#ifndef BARCODE_H
#define BARCODE_H

#include <BarcodeType.h>
#include <QByteArray>


namespace Afc
{
class Barcode
{
public:
  virtual ~Barcode()
  {
  }

  virtual QByteArray   getData() const = 0;

  virtual BarcodeType  getBarcodeType() const = 0;
};
}

#endif // BARCODE_H
