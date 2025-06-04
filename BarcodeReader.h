#ifndef BARCODEREADER_H
#define BARCODEREADER_H

#include <Device.h>
#include <QSharedPointer>


namespace Afc
{
class Barcode;

class BarcodeReader: public Device
{
public:
  virtual Result  search(QSharedPointer<Barcode> *barcode) = 0;
};
}

#endif // BARCODEREADER_H
