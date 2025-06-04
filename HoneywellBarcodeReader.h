#ifndef HONEYWELLBARCODEREADER_H
#define HONEYWELLBARCODEREADER_H

#include <BarcodeReader.h>
#include <QrCode.h>
#include <QTimer>
#include <QMutex>
#include <qserialport.h>


class QKeyEvent;

namespace Afc
{
class HoneywellBarcodeReader: public QObject, public BarcodeReader
{
  Q_OBJECT

public:
  HoneywellBarcodeReader();

  // Device interface

public:
  Result  initializeDevice() final;

  Result  restartDevice() final;

  Result  getVersion(QByteArray *version) final;

  // BarcodeReader interface

public:
  Result  search(QSharedPointer<Barcode> *barcode) final;

private slots:
  void    onTimerStatus();
  void    onSerialDataReady();

private:
  bool                          _connected;
  QMutex                        _mutexQr;
  QSharedPointer<QSerialPort>   _serialPort;
  QScopedPointer<QTimer>        _timerStatus;
  QSharedPointer<QrCode>        _qr;
};
}

#endif // HONEYWELLBARCODEREADER_H
