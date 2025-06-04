#ifndef SCU_H
#define SCU_H

#include <QSharedPointer>
#include <QTimer>
#include <Utilities.h>


class QTcpSocket;
class QTcpServer;

namespace Afc
{
class Scu: public QObject
{
  Q_OBJECT

public:
  enum ConfigurationMask
  {
    Online                     = 0b10000000,
    StationClosed              = 0b01000000,
    UnknownConfiguration       = 0b00100000,
    Reserved1                  = 0b00010000,
    GateConfigMask             = 0b00001111,
    ConfigControlledBlocked    = 0b00000001,
    ConfigBlockedControlled    = 0b00000010,
    ConfigControlledControlled = 0b00000011,
    ConfigBlockedFree          = 0b00000100,
    ConfigBlockedBlocked       = 0b00000101,
    ConfigFreeFree             = 0b00000110,
    ConfigControlledFree       = 0b00000111,
    ConfigFreeBlocked          = 0b00001000,
    ConfigFreeControlled       = 0b00001001,
    Reserved2                  = 0b00001010,
  };

public:
  static QSharedPointer<Scu>  getInstance()
  {
    static const auto &ins = QSharedPointer<Scu>(new Scu());

    return ins;
  }

  void               start();

  ConfigurationMask  getConfiguration() const;

  bool               isConnected() const;

  ~Scu();

  void               setConfiguration(const ConfigurationMask &configuration);

private:
  Scu();

  enum class Command : quint8
  {
    SetEquipment = 0x04,
    Ping         = 0x05,
    SetDateTime  = 0x07,
    GetDateTime  = 0x08,
    LogIn        = 0x1C,
    LogOut       = 0x1D,
    OpenStation  = 0x1E,
    CloseStation = 0x1F,
    OneFreePassA = 0xA0,
    OneFreePassB = 0xB0,
    GetInfo      = 0xF0,
  };

  enum PingMode
  {
    FILE_TRANSFER         = 0b10000000,
    FILE_RECEIVE          = 0b01000000,
    SINGLE_RECORD_REQUEST = 0b00100000,
    COUNTER_REQUEST       = 0b00010000,
    FILE_CLOSE_REQUEST    = 0b00001000,
    RESERVED              = 0b00000100,
    DPTC_RESERVED         = 0b00000011,
  };
  struct FileInfo
  {
    QString     name;
    quint32     size;
    quint32     offset;
    QByteArray  data;

    QByteArray         toByteArray(bool skipData = true) const;
  };
  struct NetworkPacket
  {
    static qint32      fromByteArray(const QByteArray &buffer, NetworkPacket *packet);

    ConfigurationMask  getConfiguration() const;

    quint8      getPingMode() const;

    QByteArray  toByteArray() const;

    quint8      header, address, command;
    QByteArray  data;
  };

private slots:
  void        onNewConnection();

  void        onReadyRead();

  void        onDisconnected();

  void        onTimerTimeout();

private:
  void        sendPacket(const NetworkPacket &packet);

  QByteArray  handleFileReceive(const QByteArray &data);

  FileInfo    getFileInfo(const QByteArray &data) const;

  QByteArray  prepareFileForSend(FileInfo info, bool isAct);

  QByteArray  handleFileTransfer(const QByteArray &data);

private:
  bool                        _connected;
  QTimer                      _timer;
  QSharedPointer<QTcpServer>  _server;
  QSharedPointer<QTcpSocket>  _client;
  QByteArray                  _dataBuffer;
  QString                     _tempFileName;
  QByteArray                  _tempFileBuff;
  ConfigurationMask           _configuration;
  qint64                      _lastDataTime;
  quint32                     _currentPacketNumber;
  quint32                     _closeStationCounter;
  bool                        _closeStationCommited;
};
}

#endif // SCU_H
