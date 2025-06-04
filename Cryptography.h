#ifndef CRYPTOGRAPHY
#define CRYPTOGRAPHY

#include <QByteArray>

namespace Afc
{
class Cryptography
{
public:
  static QByteArray  encrypt2Des(const QByteArray &key, const QByteArray &data);

  static QByteArray  decrypt2Des(const QByteArray &key, const QByteArray &data);

  static QByteArray  encrypt3Des(const QByteArray &key, const QByteArray &data);

  static QByteArray  decryptAes(const QByteArray &key, const QByteArray &data);
};
}

#endif // CRYPTOGRAPHY
