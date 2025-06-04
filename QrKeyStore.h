#ifndef QRKEYSTORE_H
#define QRKEYSTORE_H

#include <QByteArray>
#include <QVector>
#include <QSettings>


namespace Afc
{
class QrKeyStore
{
public:
  static bool initialize(const QByteArray &storeKey);

  static QrKeyStore& getInstance()
  {
    static QrKeyStore _instance;

    return _instance;
  }

  QByteArray getKey(qint32 id) const;

  quint32    getKeyCount() const;

private:
  QrKeyStore();

  bool loadKeys(const QByteArray &storeKey);

private:
  bool                _initialized;
  QByteArray          _storeKey;
  QSettings           _settings;
  QVector<QByteArray> _keys;
};
}

#endif // QRKEYSTORE_H
