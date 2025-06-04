#include <Cryptography.h>
#include <QString>
#include "cryptlib.h"
#include "filters.h"
#include "modes.h"
#include "des.h"
#include "aes.h"
#include <QDebug>


using namespace CryptoPP;

namespace Afc
{
QByteArray  Cryptography::encrypt2Des(const QByteArray &key, const QByteArray &data)
{
  quint8  result[1024];
  quint8  iv[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  CBC_Mode<DES_EDE2>::Encryption  e;

  e.SetKeyWithIV(reinterpret_cast<const quint8 *>(key.data()), key.size(), iv, sizeof(iv));
  e.ProcessData(result, reinterpret_cast<const quint8 *>(data.data()), data.size());

  return QByteArray(reinterpret_cast<char *>(result), data.size());
}

QByteArray  Cryptography::decrypt2Des(const QByteArray &key, const QByteArray &data)
{
  QByteArray res;

  if (0 < key.size())
  {
    quint8  result[1024];
    quint8  iv[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };

    CBC_Mode<DES_EDE2>::Decryption  d;

    d.SetKeyWithIV(reinterpret_cast<const quint8 *>(key.data()), key.size(), iv, sizeof(iv));
    d.ProcessData(result, reinterpret_cast<const quint8 *>(data.data()), data.size());
    res =  QByteArray((char *)result, data.size());
  }
  else
  {
    throw std::invalid_argument("Invalid key");
  }

  return res;
}

QByteArray  Cryptography::encrypt3Des(const QByteArray &key, const QByteArray &data)
{
  quint8  result[1024];
  quint8  iv[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  CBC_Mode<DES_EDE3>::Encryption  e;

  e.SetKeyWithIV(reinterpret_cast<const quint8 *>(key.data()), key.size(), iv, sizeof(iv));
  e.ProcessData(result, reinterpret_cast<const quint8 *>(data.data()), data.size());

  return QByteArray(reinterpret_cast<char *>(result), data.size());
}

QByteArray Cryptography::decryptAes(const QByteArray &key, const QByteArray &data)
{
  QByteArray res;
  std::string result;
  ECB_Mode<AES>::Decryption e;

  if (0 < key.size())
  {
    e.SetKey(reinterpret_cast<const quint8 *>(key.data()), key.size());
    std::string input(data.data(), data.size());
    StringSource(input, true, new StreamTransformationFilter(e, new StringSink(result), StreamTransformationFilter::PKCS_PADDING));

    res = QString::fromStdString(result).toLatin1();
  }
  else
  {
    throw std::invalid_argument("Invalid key");
  }

  return res;
}
}
