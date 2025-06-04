#include <HoneywellBarcodeReader.h>
#include <QrTicket.h>
#include <QTimer>
#include <QProcess>
#include <QKeyEvent>
#include <QDateTime>
#include <QDebug>
#include <thread>


static constexpr auto  TIMER_STATUS_INTERVAL = 5000;
static constexpr auto  SERIAL_TIMEOUT_MSEC   = 50;
static const QString   PORT_NAME("ttyACM0");
//static const QString   PID_VID("0c2e:0e81");


namespace Afc
{
HoneywellBarcodeReader::HoneywellBarcodeReader():
  _connected(false)
{
  _timerStatus.reset(new QTimer());
  connect(_timerStatus.data(), SIGNAL(timeout()), this, SLOT(onTimerStatus()), Qt::QueuedConnection);
  _timerStatus->setInterval(TIMER_STATUS_INTERVAL);
  _timerStatus->start();

}

Result  HoneywellBarcodeReader::initializeDevice()
{
  Result  res;

  _serialPort = QSharedPointer<QSerialPort>(new QSerialPort(PORT_NAME));

  if (!_serialPort->open(QIODevice::ReadWrite))
  {
    res.message = _serialPort->errorString() + " " + PORT_NAME;
  }
  else
  {
    const bool  baudSet = _serialPort->setBaudRate(QSerialPort::Baud115200);
    const bool  dataSet = _serialPort->setDataBits(QSerialPort::Data8);
    const bool  flowSet = _serialPort->setFlowControl(QSerialPort::NoFlowControl);
    const bool  stopSet = _serialPort->setStopBits(QSerialPort::OneStop);
    const bool  pairSet = _serialPort->setParity(QSerialPort::NoParity);

    if (baudSet && dataSet && flowSet && stopSet && pairSet)
    {
      connect(_serialPort.data(), SIGNAL(readyRead()), this, SLOT(onSerialDataReady()));
      res.succeed = true;
    }
    else
    {
      _serialPort->close();
      res.message = "Failed to initialize QR reader.";
    }
  }

  return res;
}

Result  HoneywellBarcodeReader::restartDevice()
{
  Result  res(true);

  return res;
}

Result  HoneywellBarcodeReader::getVersion(QByteArray *)
{
  Result  res;

  res.succeed = _connected;

  return res;
}

Result  HoneywellBarcodeReader::search(QSharedPointer<Barcode> *barcode)
{
  Result        res;
  QMutexLocker  lock(&_mutexQr);

  if (!_qr.isNull())
  {
    *barcode    = _qr;
    _qr.clear();
    res.succeed = true;
  }

  return res;
}

void  HoneywellBarcodeReader::onTimerStatus()
{
  QProcess  proc;

  proc.start("ls /dev");
  proc.waitForFinished();

  const auto &output          = QString(proc.readAllStandardOutput());
  const auto &splittedOutput  = output.split("\n");
  bool        c               = false;

  for (qint32 i = 0; i < splittedOutput.size(); ++i)
  {
    if (splittedOutput[i].contains(PORT_NAME))
    {
      c = true;
      break;
    }
  }

  _connected = c;
}

void HoneywellBarcodeReader::onSerialDataReady()
{
  QMutexLocker  lock(&_mutexQr);
  _serialPort->waitForReadyRead(SERIAL_TIMEOUT_MSEC);
  auto const    buffer = _serialPort->readAll().trimmed();
  _qr = QSharedPointer<QrTicket>(new QrTicket(buffer));
}
}
