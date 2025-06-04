#include <CityPaySam.h>
#include <Cryptography.h>
#include <Utilities.h>
#include <QDateTime>
#include <QtEndian>
#include <QDebug>


static const QByteArray  AFC_MASTER_KEY_MAPRA("\x9E\x91\x84\x8B\xC2\x52\xE5\x1B\x48\x6C\x29\xC0\xAC\xD4\x2D\x32", 16);
static const QByteArray  AFC_MASTER_KEY_ELIN("\xFA\xAD\x40\x11\x03\xB8\x73\xC1\xE1\x82\x13\xC9\x06\x2A\x8C\x41", 16);
static const QByteArray  AFC_APP_CODE("\x1E\x2B\x41\xC4\x13\xE2\x01\xF6", 8);
static const QByteArray  SAM_ISD("\xA0\x00\x00\x00\x03\x00\x00\0x00", 8);
static const QByteArray  CITYPAY_SELECT_APPLET("\xD3\x64\x24\x89\x22\x65\xB0\x00\x01", 9);
static const QByteArray  CITYWAY_SELECT_APPLET("\xD3\x64\x24\x89\x22\x65\xB1\x00\x01", 9);
static const QByteArray  SAM_AUTHORIZE_PIN("\x00\x00\x00\x00", 4);

namespace Afc
{
CityPaySam::CityPaySam(const QSharedPointer<CardReader> &reader, quint8 slot, const QByteArray &key):
  _cardReader(reader), _samSlot(slot), _samKey(key),
  _cityPayInitialized(false), _cityWayInitialized(false)
{
}

CityPaySam::~CityPaySam()
{
}

Result  CityPaySam::initialize()
{
  bool        powerOn = false;
  Result      res;
  QByteArray  samAtr;

  if (_cardReader->samPowerOn(_samSlot, &samAtr))
  {
    if (0 < samAtr.size())
    {
      CardReader::ApduResponse  response;

      // Get sam serial.
      if (_cardReader->samApdu(_samSlot, CardReader::ApduCommand(0x00, 0xCA, 0x9F, 0x7F, 0x2A), &response))
      {
        if (16 <= response.data.size())
        {
          _samSerial = response.data.mid(0, 2) + response.data.mid(16, 2) + response.data.mid(12, 4);

          // Select ISD.
          if (_cardReader->samApdu(_samSlot, CardReader::ApduCommand(0x00, 0xA4, 0x04, 0x00, SAM_ISD), &response))
          {
            powerOn = true;

            Result initRes;

            if (initRes = initializeCityPay())
            {
              _cityPayInitialized = true;
            }
            else
            {
              _cityPayErrors = initRes.message;
            }

            if (initRes = initializeCityWay())
            {
              _cityWayInitialized = true;
            }
            else
            {
              _cityWayErrors = initRes.message;
            }

            res.succeed = _cityPayInitialized || _cityWayInitialized;
          }
          else
          {
            _samModuleErrors = "SAM ISD error.";
          }
        }
        else
        {
          _samModuleErrors = "SAM serial error.";
        }
      }
      else
      {
        _samModuleErrors = "SAM serial error.";
      }
    }
    else
    {
      _samModuleErrors = "SAM atr error.";
    }
  }
  else
  {
    _samModuleErrors = "Hardware error.";
  }

  res.message = _samModuleErrors;

  if (!powerOn)
  {
    _cityPayErrors = _samModuleErrors;
    _cityWayErrors = _samModuleErrors;
  }

  return res;
}

Result  CityPaySam::initializeCityPay()
{
  Result  res;

  if (16 != _samKey.size())
  {
    res.message = "Invalid SAM key settings. (check config file)";
  }
  else
  {
    CardReader::ApduResponse  response;

    // Select applet. (CityPay)
    if (_cardReader->samApdu(_samSlot, CardReader::ApduCommand(0x00, 0xA4, 0x04, 0x00, CITYPAY_SELECT_APPLET),
                             &response))
    {
      // Set sam key.
      {
        _cardReader->samApdu(_samSlot, CardReader::ApduCommand(0x80, 0xAB, 0x00, 0x00, _samKey), &response);
      }

      // Get terminal.
      if (_cardReader->samApdu(_samSlot, CardReader::ApduCommand(0x80, 0xC2, 0x00, 0x00, 0x05), &response))
      {
        _samTerminal = response.data;

        // Authorize pin.
        if (_cardReader->samApdu(_samSlot, CardReader::ApduCommand(0x80, 0xAC, 0x01, 0x00, SAM_AUTHORIZE_PIN),
                                 &response))
        {

          // Get random.
          if (_cardReader->samApdu(_samSlot, CardReader::ApduCommand(0x80, 0xA0, 0x00, 0x00, 0x08), &response))
          {
            const auto &encRnd = Cryptography::encrypt2Des(_samKey, response.data);

            // External authenticate.
            if ((_cardReader->samApdu(_samSlot, CardReader::ApduCommand(0x80, 0xA1, 0x03, 0x01, encRnd),
                                      &response)
                 && (0x90 == (response.sw1 & 0xFF))))
            {
              qDebug() << "SAM CityPay successfully initialized.";
              res.succeed = true;
            }
            else
            {
              res.message = "SAM external authenticate error.";
            }
          }
          else
          {
            res.message = "SAM get random error.";
          }
        }
        else
        {
          res.message = "SAM authorize pin error.";
        }
      }
      else
      {
        res.message = "SAM get terminal error.";
      }
    }
    else
    {
      res.message = "SAM select applet error.";
    }
  }

  return res;
}

Result  CityPaySam::initializeCityWay()
{
  Result  res;

  if (8 != _samSerial.size())
  {
    res.message = "Invalid SAM serial.";
  }
  else
  {
    CardReader::ApduResponse  response;

    // Select applet. (CityWay)
    if (_cardReader->samApdu(_samSlot, CardReader::ApduCommand(0x01, 0xA4, 0x04, 0x00, CITYWAY_SELECT_APPLET), &response))
    {
      // Get sam version.
      if (_cardReader->samApdu(_samSlot, CardReader::ApduCommand(0x81, 0xFF, 0x00, 0x00, 0x01), &response))
      {
        auto samMasterKey = _samKey;

        if (16 == _samKey.size())
        {
          // Set master key.
          if (_cardReader->samApdu(_samSlot, CardReader::ApduCommand(0x81, 0x01, 0x00, 0x00, samMasterKey), &response))
          {
            // Get random.
            if (_cardReader->samApdu(_samSlot, CardReader::ApduCommand(0x81, 0x10, 0x00, 0x00, 0x08), &response))
            {
              const auto &terminalRnd = Utilities::generateRandom(8);
              const auto &encRnd      = Cryptography::encrypt2Des(samMasterKey, response.data + terminalRnd);

              // Mutual authenticate.
              if ((_cardReader->samApdu(_samSlot, CardReader::ApduCommand(0x81, 0x20, 0x00, 0x00, encRnd),
                                        &response) && (0x90 == (response.sw1 & 0xFF))))
              {
                // Authorize pin.
                if (_cardReader->samApdu(_samSlot, CardReader::ApduCommand(0x81, 0x52, 0x00, 0x00, SAM_AUTHORIZE_PIN), &response))
                {
                  qDebug() << "SAM CityWay successfully initialized.";
                  res.succeed = true;
                }
                else
                {
                  res.message = "SAM authorize pin error.";
                }
              }
              else // Try with old key.
              {
                samMasterKey = Cryptography::encrypt2Des(AFC_MASTER_KEY_ELIN, _samSerial + AFC_APP_CODE);

                const auto &terminalRnd2 = Utilities::generateRandom(8);
                const auto &encRnd2      = Cryptography::encrypt2Des(samMasterKey, response.data + terminalRnd2);

                if ((_cardReader->samApdu(_samSlot, CardReader::ApduCommand(0x81, 0x20, 0x00, 0x00, encRnd2),
                                                            &response) && (0x90 == (response.sw1 & 0xFF))))
                {
                  // Authorize pin.
                  if (_cardReader->samApdu(_samSlot, CardReader::ApduCommand(0x81, 0x52, 0x00, 0x00, SAM_AUTHORIZE_PIN), &response))
                  {
                    qDebug() << "SAM CityWay successfully initialized.";
                    res.succeed = true;
                  }
                  else
                  {
                    res.message = "SAM authorize pin error.";
                  }
                }
                else
                {
                  res.message = "SAM mutual authenticate error.";
                }
              }
            }
            else
            {
              res.message = "SAM get random error.";
            }
          }
          else
          {
            res.message = "Set master key error.";
          }
        }
        else
        {
          res.message = "Invalid SAM key settings. (check config file)";
        }
      }
    }
    else
    {
      res.message = "SAM select applet error.";
    }
  }

  return res;
}

Result  CityPaySam::citywayInitForOperation(const QByteArray &cardSerial, CardReader::ApduResponse *response)
{
  return sendApdu(CardReader::ApduCommand(0x81, 0x30, 0x00, 0x00, cardSerial), response);
}

Result  CityPaySam::citywayGetBalance(const QByteArray &blkOne, const QByteArray &counter, CardReader::ApduResponse *response)
{
  return sendApdu(CardReader::ApduCommand(0x81, 0x31, 0x00, 0x00, blkOne + counter), response);
}

Result  CityPaySam::citywayGetSectorKey(CardReader::BlockAuthMode mode, quint8 sector, CardReader::ApduResponse *response)
{
  return sendApdu(CardReader::ApduCommand(0x81, 0x55, CardReader::BlockAuthMode::A == mode ? 0x00 : 0x01, sector, 0x06), response);
}

Result  CityPaySam::citywayPurchase(const QByteArray &blkOne, const QByteArray &counter, quint32 amount, const QDateTime &date,
                                    CardReader::ApduResponse *response)
{
  amount = qToBigEndian(amount);
  const auto &beAmount = QByteArray(reinterpret_cast<const char *>(&amount), 4);
  const auto &bcdDate  = Utilities::convertDateToBcd(date).mid(1, 6); // Converting to 6-bytes BCD date.

  return sendApdu(CardReader::ApduCommand(0x81, 0x31, 0x01, 0x00, blkOne + counter + beAmount + bcdDate), response);
}

Result  CityPaySam::citypayEncryptForTransmission(const QByteArray &data, CardReader::ApduResponse *response)
{
  return sendApdu(CardReader::ApduCommand(0x80, 0xA6, 0x00, 0x00, data), response);
}

Result  CityPaySam::citypaySignatureForPayment(QByteArray data, CardReader::ApduResponse *response)
{
  if (data.size() % 8)
  {
    data.append(0x80);

    while (data.size() % 8)
    {
      data.append('\x00');
    }
  }

  return sendApdu(CardReader::ApduCommand(0x80, 0xC1, 0x00, 0x00, data), response);
}

QByteArray  CityPaySam::citypayGetSamTerminal() const
{
  return _samTerminal;
}

Result  CityPaySam::citypaySetKey(const QByteArray &key, CardReader::ApduResponse *response)
{
  Result  res;

  if (16 == key.size())
  {
    res = sendApdu(CardReader::ApduCommand(0x80, 0xAB, 0x00, 0x00, key), response);
  }
  else
  {
    res.message = "Invalid key size.";
  }

  return res;
}

QByteArray  CityPaySam::getSamSerial() const
{
  return _samSerial;
}

Result  CityPaySam::sendApdu(const CardReader::ApduCommand &apdu, CardReader::ApduResponse *response)
{
  Result  res;

  //if (_cityPayInitialized && _cityWayInitialized)
  {
    res = _cardReader->samApdu(_samSlot, apdu, response);
  }
  //  else
  {
    //  res.message = "SAM is not initialized.";
  }

  return res;
}

QString CityPaySam::getCityWayErrors() const
{
  return _cityWayErrors;
}

QString CityPaySam::getCityPayErrors() const
{
  return _cityPayErrors;
}

QString CityPaySam::getSamModuleErrors() const
{
  return _samModuleErrors;
}

bool CityPaySam::getCityWayInitialized() const
{
  return _cityWayInitialized;
}

bool CityPaySam::getCityPayInitialized() const
{
  return _cityPayInitialized;
}
}
