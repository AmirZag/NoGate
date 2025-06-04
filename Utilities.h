#ifndef UTILITIES
#define UTILITIES

#include <QString>
#include <QHostAddress>


class QDateTime;

namespace Afc
{
class Utilities final
{
public:
  static quint16       crc16ModBus(const QByteArray &data);

  static QByteArray    convertDateToBcd(const QDateTime &dateTime);

  static QString       convertToQString(char *str);

  static QByteArray    generateRandom(quint32 size);

  static void          initUsers();

  static QString       prettyPrice(double price);

  static void          setSystemDateTime(const QDateTime &date);

  static QByteArray    uint32ToByteArray(quint32 n);

  static QByteArray    uint16ToByteArray(quint16 n);

  static quint16       readUint16(const QByteArray &data);

  static quint32       readUint32(const QByteArray &data);

  static QHostAddress  getIpAddress();

  static QString       getIpString();

  static QString       getMacAddress();

  static bool          setIpAddress(const QString &ipv4);

  static quint16       generateSerialId(quint8 seed);

  static QByteArray    generateEquipmentSerialId();

  static QDate         miladiToShamsi(QDate miladi);

  static bool          miladiIsLeap(int year);

  static QByteArray    macStringToByteArray(const QString &mac);

  static void          systemReboot();

  static bool          decompressXz(const QString &path);

  static void          setOwner(const QString &path, const QString &owner);

  static bool          exec(const QString &proc, const QStringList &params);
};
}

#endif // UTILITIES
