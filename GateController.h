#ifndef GATECONTROLLER_H
#define GATECONTROLLER_H

#include <Device.h>
#include <QSharedPointer>
#include <QTimer>
#include <QFile>
#include <QDataStream>
#include <QDateTime>


class QSerialPort;

namespace Afc
{
class GateController: public QObject
{
  Q_OBJECT

  enum CommandId
  {
    CMD_GET_DATA      = 0x14,
    CMD_CLOSE_GATE    = 0x16,
    CMD_CLEAR_COUNTER = 0x17,
    CMD_SET_CONFIG    = 0x18,
    CMD_SET_MODE      = 0x19,
    CMD_AUTHORIZE     = 0x1A,
  };
  struct CommandResponse
  {
    QByteArray  data;
  };

public:
  struct Status
  {
    quint8   mode;
    quint8   configuration;
    quint16  alarm;
    quint32  counterA;
    quint32  counterB;
  };

public:
  static QSharedPointer<GateController>  getInstance()
  {
    static const auto &ins = QSharedPointer<GateController>(new GateController());

    return ins;
  }

  ~GateController();

  // Device interface

public:
  Result  authorize();

  Result  closeGate();

  Result  getStatus(Status *status);

  Result  resetCounters(const QString &commander = "SCU");

  Result  setConfiguration(quint8 mode);

  // Result  setMode(bool online);

  Result  initializeDevice(const QString &serialPortName);

  QString getLastClearedBy() const;

  QDateTime getLastClearedDate() const;

private slots:
  void    onTimerGetStatus();

  void    onTimerSession();

private:
  GateController();

  Result  sendCommand(const QByteArray &cmd, CommandResponse *response = nullptr);

  Result  getResponse(CommandResponse *response);

  void    saveSession();

  void    loadSession();

  Result  localResetCounters();

private:
  QString                      _portName;
  QSharedPointer<QSerialPort>  _serialPort;
  QByteArray                   _dataBuffer;
  Status                       _status;
  QTimer                       _timerStatus;
  QTimer                       _timerSession;
  QFile                        _fileSession;
  QDataStream                  _streamSession;
  QString                      _lastClearedBy;
  QDateTime                    _lastClearedDate;
  quint8                       _mode;
  int                          _queryCount;
  quint32                      _lastCounterA, _lastCounterB;
  quint32                      _sessionCounterA, _sessionCounterB;
  bool                         _counterFlushed, _pendingReset;
  quint8                       _timeoutCounter;
};
}

#endif // GATECONTROLLER_H
