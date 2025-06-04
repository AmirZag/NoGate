#include <QrKeyStore.h>
#include <Cryptography.h>
#include <Settings.h>
#include <QDebug>


namespace Afc
{
// TODO: delete these keys.
static auto const KEY_QR_TEST_0 = QByteArray("bbb6d7f3e6473040");
static auto const KEY_QR_TEST_1 = QByteArray("241284dc4294ae1c");
static auto const KEY_QR_TEST_2 = QByteArray("acb6a8f3e21880bb");
static auto const KEY_QR_TEST_3 = QByteArray("b3b6d7f3f64730cc");
static auto const KEY_QR_TEST_4 = QByteArray("cbd6d7e3e44730aa");
static auto const KEY_QR_TEST_5 = QByteArray("dbc6d7f3e64220dd");


static auto constexpr QR_KEY_SIZE       = 16;
static auto constexpr QR_MAX_KEY_COUNT  = 16;


QrKeyStore::QrKeyStore(): _initialized(false), _settings(QString(Settings::QR_DATA_DIR) + "/keys.ini", QSettings::IniFormat)
{

}

QByteArray QrKeyStore::getKey(qint32 id) const
{
  QByteArray res;

  if (id < _keys.size())
  {
    res = _keys[id];
  }

  return res;
}

quint32 QrKeyStore::getKeyCount() const
{
  return _keys.size();
}

bool QrKeyStore::loadKeys(const QByteArray &storeKey)
{
  bool res = true;
  _storeKey = storeKey;


  // TODO: delete this section after test.
  {
    _settings.setValue("0", Cryptography::encrypt2Des(_storeKey, KEY_QR_TEST_0).toBase64().data());
    _settings.setValue("1", Cryptography::encrypt2Des(_storeKey, KEY_QR_TEST_1).toBase64().data());
    _settings.setValue("2", Cryptography::encrypt2Des(_storeKey, KEY_QR_TEST_2).toBase64().data());
    _settings.setValue("3", Cryptography::encrypt2Des(_storeKey, KEY_QR_TEST_3).toBase64().data());
    _settings.setValue("4", Cryptography::encrypt2Des(_storeKey, KEY_QR_TEST_4).toBase64().data());
    _settings.setValue("5", Cryptography::encrypt2Des(_storeKey, KEY_QR_TEST_5).toBase64().data());
    _settings.sync();
  }

  for (qint32 i = 0; i < QR_MAX_KEY_COUNT; ++i)
  {
    auto const k = QString::number(i);

    if (_settings.contains(k))
    {
      auto const encryptedKey = QByteArray::fromBase64(_settings.value(k).toByteArray());

      if (QR_KEY_SIZE == encryptedKey.size())
      {
        try
        {
          auto const key = Cryptography::decrypt2Des(_storeKey, encryptedKey);

#ifdef VERBOSE_LOG
          qDebug() << "loading qr key" << k << "=" << key;
#endif

          if (QR_KEY_SIZE == key.size())
          {
            _keys.append(key);
          }
          else
          {
#ifdef VERBOSE_LOG
            qDebug() << "Invalid QR key!";
#endif
            res = false;

            break;
          }
        }
        catch(...)
        {
#ifdef VERBOSE_LOG
          qDebug() << "Invalid QR key!";
#endif
          res = false;

          break;
        }
      }
    }
    else
    {
      res = false;
      break;
    }
  }

  return res;
}

bool QrKeyStore::initialize(const QByteArray &storeKey)
{
  if (!getInstance()._initialized)
  {
    if (getInstance().loadKeys(storeKey))
    {
      getInstance()._initialized = true;
    }
  }

  return getInstance()._initialized;
}

}
