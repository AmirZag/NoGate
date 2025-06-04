#ifndef QRSERVERMANAGER_H
#define QRSERVERMANAGER_H

#include <QSharedPointer>
#include <QTcpServer>
#include <QTcpSocket>
#include <QFile>
#include <QTimer>


namespace Afc
{
class QrTicket;

class QrServerManager: public QObject
{
  Q_OBJECT
public:
  enum class ServerResponseCode
  {
    NotResponding                 = 0x00,
    Valid                         = 0x02,
    UsedBefore                    = 0x03,
    DisconnectedFromCentralServer = 0x04,
    DirectConnectionToBank        = 0x05,
    MultiTrip                     = 0x06,
    OfflineMode                   = 0x07,
    InvalidCrc                    = 0x08,
    NotValid                      = 0xFF,
  };

  enum class ValidationStatus
  {
    Valid,
    NotValid,
    OfflineQuotaReached,
  };

  struct ValidationResult
  {
    ValidationStatus    status;
    ServerResponseCode  responseCode;
    quint8              remainingTrips;
  };

public:
  static QrServerManager& getInstance()
  {
    static QrServerManager instance;

    return instance;
  }

  void validate(const QSharedPointer<QrTicket> &ticket, quint16 stationId);

  void incrementTicketCounter(bool offline);

  quint32 getTicketCounter() const;

  void setServerIp(const QString &ip);

signals:
  void validationReady(const QSharedPointer<QrTicket> &ticket, const QrServerManager::ValidationResult &result);

private slots:
  void onSocketConnected();
  void onSocketReadyRead();
  void onSocketError(QAbstractSocket::SocketError err);
  void connectToServer();
  void onTimerNetwork();

private:
  QrServerManager();

  void reconnect();
  void saveSession();
  void loadSession();
  void sendPacket(const QString &data);
  void processPacket(const QByteArray &packet);

private:
  QByteArray                _dataBuffer;
  QTimer                    _timerNetwork;
  QTcpSocket                _socket;
  QFile                     _fileSession;
  QDataStream               _streamSession;
  QString                   _systemIp, _serverIp;
  quint32                   _ticketCounter, _offlineTicketCounter;
  QSharedPointer<QrTicket>  _pendingTicket;
  bool                      _connected;
  qint64                    _connectionLostTime;
  qint64                    _lastRequestTime, _lastResponseTime;
};
}

#endif // QRSERVERMANAGER_H
