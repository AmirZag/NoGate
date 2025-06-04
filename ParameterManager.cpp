#include <ParameterManager.h>
#include <ActivityManager.h>
#include <BuzzerController.h>
#include <QCoreApplication>
#include <QFile>
#include <QDataStream>
#include <Utilities.h>
#include <Settings.h>
#include <QDir>
#include <QTimer>
#include <QDebug>
#include <bitset>


namespace Afc
{
static constexpr auto  PARAM_FILE_NAME_LENGTH      = 8;
static constexpr auto  PARAM_FILE_SIGNATURE_LENGTH = 8;
static constexpr auto  PARAM_FILE_VERSION_LENGTH   = 2;

ParameterManager::ParameterManager():
  _status(Status::Normal), _familleVersion(0)
{
  if (!QDir(Settings::SCU_PARAMS_DIR).exists())
  {
    QDir().mkdir(Settings::SCU_PARAMS_DIR);
    Utilities::setOwner(Settings::SCU_PARAMS_DIR, Settings::USER_NAME);
  }

  // TODO: is it ok?
  loadParams();
  loadINITIATE();
}

void  ParameterManager::load()
{
  QTimer::singleShot(3000, this, SLOT(loadParams()));
}

void  ParameterManager::initiate()
{
  QTimer::singleShot(3000, this, SLOT(loadINITIATE()));
}

void  ParameterManager::loadParams()
{
  Result  res(true);

  _status         = Status::LoadingParameters;
  _businessNumber = 0;
  _familleVersion  = 0;
  _paramVersion.clear();
  _stations.clear();
  _words.clear();
  _cardTypeNames.clear();
  _messages.clear();
  _statusMessages.clear();
  _validationActions.clear();
  _validationProperties.clear();
  _discounts.clear();
  _periods.clear();
  _refusalCodesEncoding.clear();
  _ticketParams.clear();
  _timeSlots.clear();
  _transferTables.clear();
  _validityDaysTimes.clear();
  _costCategories.clear();
  _sectionIds.clear();
  _flatFares.clear();
  _publicHolidays.clear();
  _schoolHolidays.clear();
  _blacklists.clear();
  //_paragenTime; // TODO:
  //_fare1Date;
  //_fare2Date;

  res.succeed &= loadFAMILLE();
  res.succeed &= loadLNTSC();
  res.succeed &= loadDEFSTA();
  res.succeed &= loadPARAGEN();
  res.succeed &= loadTISVTE();
  res.succeed &= loadMESVIE();
  res.succeed &= loadETAMES();
  res.succeed &= loadTABACT();
  res.succeed &= loadTISVAL();
  res.succeed &= loadDISCOUNT();
  res.succeed &= loadPERREF();
  res.succeed &= loadCODREF();
  res.succeed &= loadTIVARPAR();
  res.succeed &= loadTHTIMS();
  res.succeed &= loadTABCOR();
  res.succeed &= loadVALTIT();
  res.succeed &= loadMATPRIX();
  res.succeed &= loadMATCCV();
  res.succeed &= loadMADTICV();
  res.succeed &= loadTABJF();
  res.succeed &= loadTABVAC();

  // res.succeed &= loadMAPSSC();
  // res.succeed &= loadALARMES();
  res.succeed &= loadBINRAY();
  // res.succeed &= loadDATASCC();
  // res.succeed &= loadDEFSSL();
  // res.succeed &= loadILNTMA();
  // res.succeed &= loadILNTSC();
  // res.succeed &= loadLNAGT();
  // res.succeed &= loadLNTITM();
  // res.succeed &= loadMASQCOD();
  // res.succeed &= loadTABTYP();
  // res.succeed &= loadTIMVAL();
  // res.succeed &= loadVALPAR();
  _status = Status::Normal;

  ActivityManager::getInstance()->setAlarm(ActivityManager::AlarmGroup::Kitcas, ActivityManager::AlarmBitNumber::Bit4, !res.succeed);
}

void  ParameterManager::loadINITIATE()
{
  Result      res;
  QFile       file((QString(Settings::SCU_PARAMS_DIR) + "/") + Settings::SCU_INITIATE_FILE_NAME + QString(Settings::SCU_PARAM_EXT));
  const auto &AM = ActivityManager::getInstance();

  if ((res.succeed = file.open(QIODevice::ReadOnly)))
  {
    QDataStream  ds(&file);
    quint16      stationId, maxFileSize;
    quint8       validationType, companyService, acgType, logicType;

    ds >> stationId >> maxFileSize >> validationType >> companyService >> acgType >> logicType;
    AM->setStationId(stationId);
    AM->setMaxFileSize(maxFileSize);
    AM->setValidationType(validationType);
    AM->setCompanyService(companyService);
    qDebug() << "SC loadINITIATE" << acgType;
    AM->setAcgType(static_cast<ActivityManager::AccessControlGateType>(acgType));
    AM->setLogicType(logicType);
    AM->setAlarm(ActivityManager::AlarmGroup::Kitcas, ActivityManager::AlarmBitNumber::Bit11, false);
    qDebug() << "QQ 10";
  }
  else
  {
    AM->setAlarm(ActivityManager::AlarmGroup::Kitcas, ActivityManager::AlarmBitNumber::Bit11, true);
  }

  _status = Status::Normal;
}

QMap<quint8, ParameterManager::TransferTable>  ParameterManager::getTransferTables() const
{
  return _transferTables;
}

void  ParameterManager::setStatus(const Status &status)
{
  _status = status;
}

ParameterManager::Status  ParameterManager::getStatus() const
{
  return _status;
}

QDate  ParameterManager::getFare2Date() const
{
  return _fare2Date;
}

QDate  ParameterManager::getFare1Date() const
{
  return _fare1Date;
}

QMap<quint8, quint16>  ParameterManager::getPeriods() const
{
  return _periods;
}

QMap<quint8, bool>  ParameterManager::getRefusalCodesEncoding() const
{
  return _refusalCodesEncoding;
}

bool  ParameterManager::isEncodeEnabled(quint8 tableId) const
{
  return _refusalCodesEncoding.contains(tableId) && _refusalCodesEncoding[tableId];
}

QSet<quint32>  ParameterManager::getBlacklists() const
{
  return _blacklists;
}

QMap<quint16, QVector<ParameterManager::ValidationAction>>  ParameterManager::getValidationActions() const
{
  return _validationActions;
}

QMap<quint16, QVector<ParameterManager::Section>>  ParameterManager::getCostCategories() const
{
  return _costCategories;
}

QVector<quint8>  ParameterManager::getSectionIds() const
{
  return _sectionIds;
}

QMap<quint16, ParameterManager::Station>  ParameterManager::getStations() const
{
  return _stations;
}

QMap<quint16, ParameterManager::TicketParameter>  ParameterManager::getTicketParams() const
{
  return _ticketParams;
}

bool  ParameterManager::isSchoolHoliday(const QDate &date) const
{
  bool  res = false;

  for (const auto &p : getSchoolHolidays())
  {
    if ((p.first <= date) && (date <= p.second))
    {
      res = true;
      break;
    }
  }

  return res;
}

QVector<QPair<QDate, QDate>>  ParameterManager::getSchoolHolidays() const
{
  return _schoolHolidays;
}

QVector<QDate>  ParameterManager::getPublicHolidays() const
{
  return _publicHolidays;
}

bool  ParameterManager::isPublicHolidays(const QDate &date) const
{
  return getPublicHolidays().contains(date);
}

QMap<quint8, QVector<ParameterManager::ValidityDaysTimeSlot>>  ParameterManager::getValidityDaysTimes() const
{
  return _validityDaysTimes;
}

Result  ParameterManager::getTimeSlotAndDiscountRate(quint8 tableId, quint8 *timeSlot, quint8 *discountRate) const
{
  Result  res;

  *discountRate = 0;
  *timeSlot     = 0;

  const auto &itValidity = _validityDaysTimes.find(tableId);

  for (auto it = _validityDaysTimes.begin(); it != _validityDaysTimes.end(); ++it)
  {
    qDebug() << "h" << it.key();
  }

  if (itValidity != _validityDaysTimes.constEnd())
  {
    const auto &now = QDateTime::currentDateTimeUtc();

    for (qint32 i = 0; i < itValidity.value().size(); ++i)
    {
      if ((now.date().dayOfWeek() == itValidity.value()[i].dayId) || (0 == itValidity.value()[i].dayId))
      {
        if (0 == itValidity.value()[i].timeSlotTableId)
        {
          if (0 < itValidity.value()[i].discountRates.size())
          {
            *discountRate = itValidity.value()[i].discountRates[0];
          }

          res.succeed = true;
          break;
        }

        std::bitset<8>  mbits(itValidity.value()[i].validityBits);
        const auto     &itTimeSlots = _timeSlots.find(itValidity.value().at(i).timeSlotTableId);

        if (_timeSlots.constEnd() == itTimeSlots)
        {
          break;
        }

        if (8 < itTimeSlots.value().size())
        {
          break;
        }

        for (qint32 j = 0; j < itTimeSlots.value().size(); ++j)
        {
          if ((itTimeSlots.value()[j].first <= now.time()) && (now.time() <= itTimeSlots.value()[j].second))
          {
            if (mbits.test((mbits.size() - 1) - j))
            {
              if (j < itValidity.value()[i].discountRates.size())
              {
                *discountRate = itValidity.value()[i].discountRates[j];
              }

              *timeSlot   = j;
              res.succeed = true;

              return res;
            }
          }
        }
      }
    }
  }

  return res;
}

QMap<quint16, QVector<ParameterManager::FlatFare>>  ParameterManager::getFlatFares() const
{
  return _flatFares;
}

QMap<quint16, ParameterManager::ValidationProperty>  ParameterManager::getValidationProperties() const
{
  return _validationProperties;
}

Result  ParameterManager::loadALARMES()
{
  Result  res;

  return res;
}

Result  ParameterManager::loadBINRAY()
{
  //TODO: remove return and add crc check.
  return Result(true);
  Result  res;
  QFile   file(QString(Settings::SCU_PARAMS_DIR) + "/BINARY" + QString(Settings::SCU_PARAM_EXT));

  if ((res.succeed = file.open(QIODevice::ReadOnly)))
  {
    QDataStream   ds(&file);
    const qint32  fileSize = file.size() - PARAM_FILE_NAME_LENGTH - PARAM_FILE_VERSION_LENGTH
                             - PARAM_FILE_SIGNATURE_LENGTH;
    QByteArray  buffer(fileSize, Qt::Uninitialized);
    quint16     newVersion;
    readParamNameAndVersion(ds, nullptr, &newVersion);

    qDebug() << "Version PARAM:" << newVersion * 0x100 << Settings::MAIN_SOFTWARE_VERSION;

    if (newVersion * 0x100 != Settings::MAIN_SOFTWARE_VERSION)
    {
      ds.readRawData(buffer.data(), buffer.size());

      const QString  fileNameUpdate("update.xz");
      QFile          fileNewBin(fileNameUpdate);

      qDebug() << "BIN SIZE:" << buffer.size();

      if (fileNewBin.open(QIODevice::ReadWrite | QIODevice::Truncate))
      {
        fileNewBin.write(buffer);
        fileNewBin.close();

        if (Utilities::decompressXz(fileNameUpdate))
        {
          if (Utilities::exec("chmod", QStringList() << "+x" << "update"))
          {
            if (Utilities::exec("mv", QStringList() << "update" << QFileInfo(QCoreApplication::applicationFilePath()).fileName()))
            {
              QTimer::singleShot(1000, this, SLOT(exitApp()));
            }
          }
        }
      }
    }
  }
  else
  {
    qDebug() << "PARAMETER MISSING:" << file.fileName();
    // ActivityManager::getInstance()->setAlarm(ActivityManager::AlarmGroup::Kitcas, ActivityManager::AlarmBitNumber::Bit4, true);
  }

  return res;
}

Result  ParameterManager::loadCODREF()
{
  Result  res;
  QFile   file(QString(Settings::SCU_PARAMS_DIR) + "/CODREF" + QString(Settings::SCU_PARAM_EXT));

  if ((res.succeed = file.open(QIODevice::ReadOnly)))
  {
    QDataStream  ds(&file);
    quint16      refusalCount;

    if (res = readParamNameAndVersion(ds))
    {
      ds >> refusalCount;

      for (quint32 i = 0; i < refusalCount; ++i)
      {
        quint8  refusalCodeId, encodeOnTicket;
        ds >> refusalCodeId;
        ds.skipRawData(1);
        ds >> encodeOnTicket;
        ds.skipRawData(3);
        _refusalCodesEncoding[refusalCodeId] = (0 != encodeOnTicket) ? true : false;
        qDebug() << "ENCODE ON TICKET" << refusalCodeId << encodeOnTicket;
      }
    }
  }
  else
  {
    qDebug() << "PARAMETER MISSING:" << file.fileName();
    ActivityManager::getInstance()->setAlarm(ActivityManager::AlarmGroup::Kitcas, ActivityManager::AlarmBitNumber::Bit4, true);
  }

  return res;
}

Result  ParameterManager::loadDATASCC()
{
  Result  res;

  return res;
}

Result  ParameterManager::loadDEFSSL()
{
  Result  res;

  return res;
}

Result  ParameterManager::loadDEFSTA()
{
  Result       res;
  QFile        file(QString(Settings::SCU_PARAMS_DIR) + "/DEFSTA" + QString(Settings::SCU_PARAM_EXT));
  QDataStream  ds(&file);

  if ((res.succeed = file.open(QIODevice::ReadOnly)))
  {
    if (res = readParamNameAndVersion(ds))
    {
      quint16  stationCount;
      ds >> stationCount;

      for (quint16 i = 0; i < stationCount; ++i)
      {
        Station     s;
        QByteArray  stationName(30, '\0');
        ds.readRawData(stationName.data(), stationName.size());
        ds >> s.id;
        ds.skipRawData(10);

        s.name          = Utilities::convertToQString(stationName.data());
        s.order         = i;
        _stations[s.id] = s;
      }
    }
  }
  else
  {
    qDebug() << "PARAMETER MISSING:" << file.fileName();
    ActivityManager::getInstance()->setAlarm(ActivityManager::AlarmGroup::Kitcas, ActivityManager::AlarmBitNumber::Bit4, true);
  }

  return res;
}

Result  ParameterManager::loadDISCOUNT()
{
  Result       res;
  QFile        file(QString(Settings::SCU_PARAMS_DIR) + "/DISCOUNT" + QString(Settings::SCU_PARAM_EXT));
  QDataStream  ds(&file);

  if ((res.succeed = file.open(QIODevice::ReadOnly)))
  {
    quint16  discountsCount;

    // TODO: undo this.
    readParamNameAndVersion(ds);
    //if (res = readParamNameAndVersion(ds))
    {
      ds >> discountsCount;

      for (quint32 i = 0; i < discountsCount; ++i)
      {
        quint16  id, slotsCount;
        ds >> id >> slotsCount;

        QVector<DiscountSlot>  discountSlots;

        for (quint32 j = 0; j < slotsCount; ++j)
        {
          DiscountSlot  discountSlot;
          ds >> discountSlot.start >> discountSlot.end >> discountSlot.rate;
          discountSlots.append(discountSlot);
        }

        _discounts[id] = discountSlots;
      }
    }
  }
  else
  {
    qDebug() << "PARAMETER MISSING:" << file.fileName();
    res.succeed = true; // TODO
    // TODO:
    // ActivityManager::getInstance()->setAlarm(ActivityManager::AlarmGroup::Kitcas, ActivityManager::AlarmBitNumber::Bit4, true);
  }

  return res;
}

Result  ParameterManager::loadETAMES()
{
  Result       res;
  QFile        file(QString(Settings::SCU_PARAMS_DIR) + "/ETAMES" + QString(Settings::SCU_PARAM_EXT));
  QDataStream  ds(&file);

  if ((res.succeed = file.open(QIODevice::ReadOnly)))
  {
    quint16  equipmentCount;

    if (res = readParamNameAndVersion(ds))
    {
      ds >> equipmentCount;

      for (quint32 i = 0; i < equipmentCount; ++i)
      {
        quint16  equipmentTypeId, equipmentStatusCount;
        ds >> equipmentTypeId >> equipmentStatusCount;

        for (quint32 j = 0; j < equipmentStatusCount; ++j)
        {
          quint8  statusId, messageId;

          ds >> statusId >> messageId;

          if (EquipmentType::Reader == static_cast<EquipmentType>(equipmentTypeId))
          {
            _statusMessages[statusId] = messageId;
          }
        }
      }
    }
  }
  else
  {
    qDebug() << "PARAMETER MISSING:" << file.fileName();
    qDebug() << "PARAMETER MISSING:" << file.fileName();
    ActivityManager::getInstance()->setAlarm(ActivityManager::AlarmGroup::Kitcas, ActivityManager::AlarmBitNumber::Bit4, true);
  }

  return res;
}

Result  ParameterManager::readParamNameAndVersion(QDataStream &ds, QString *name, quint16 *version)
{
  Result res;
  QByteArray  buffer(PARAM_FILE_NAME_LENGTH, '\0');
  char        v[PARAM_FILE_VERSION_LENGTH];

  ds.readRawData(buffer.data(), PARAM_FILE_NAME_LENGTH);
  ds.readRawData(v, sizeof(v));

  const QString paramName(buffer);
  const quint16 paramVersion = QString::fromLatin1(v, sizeof(v)).toUShort();

  if (nullptr != name)
  {
    *name = paramName;
  }

  if (nullptr != version)
  {
    *version = paramVersion;
  }

  if (Settings::SCU_BASE_PARAM_FILE_NAME != paramName)
  {
    res.succeed = (_paramVersion[EquipmentType::Reader].contains(paramName)
                  && paramVersion == _paramVersion[EquipmentType::Reader][paramName]);
  }
  else
  {
    res.succeed = true;
  }

  qDebug() << "Reading parameter:" << paramName << paramVersion << _paramVersion[EquipmentType::Reader][paramName];

  return res;
}

void  ParameterManager::exitApp()
{
  QCoreApplication::exit();
}

quint16  ParameterManager::getParamsVersion() const
{
  return _familleVersion;
}

Result  ParameterManager::loadFAMILLE()
{
  Result  res;
  QFile   file((QString(Settings::SCU_PARAMS_DIR) + "/") + Settings::SCU_BASE_PARAM_FILE_NAME + QString(Settings::SCU_PARAM_EXT));

  if ((res.succeed = file.open(QIODevice::ReadOnly)))
  {
    QDataStream  ds(&file);
    quint16      numEquipments;

    readParamNameAndVersion(ds, nullptr, &_familleVersion);

    ds >> _businessNumber;
    ds >> numEquipments;

    for (quint16 eq = 0; eq < numEquipments; ++eq)
    {
      quint16  equipmentType, equipmentSubType, equipmentFileCount;

      ds >> equipmentType >> equipmentSubType >> equipmentFileCount;
      qDebug() << "Equipment" << equipmentType << equipmentSubType << "params:";

      for (quint16 j = 0; j < equipmentFileCount; ++j)
      {
        QByteArray  buffer(PARAM_FILE_NAME_LENGTH, '\0');
        QString     paramName;
        quint16     paramVersion;

        ds.readRawData(buffer.data(), PARAM_FILE_NAME_LENGTH);
        paramName = QString(buffer);
        ds >> paramVersion;

        _paramVersion[static_cast<EquipmentType>(equipmentType)][paramName] = paramVersion;
      }
    }

    qDebug() << "End of FAMILLE";
  }
  else
  {
    qDebug() << "PARAMETER MISSING:" << file.fileName();
    ActivityManager::getInstance()->setAlarm(ActivityManager::AlarmGroup::Kitcas, ActivityManager::AlarmBitNumber::Bit4, true);
  }

  return res;
}

Result  ParameterManager::loadILNTMA()
{
  Result  res;

  return res;
}

Result  ParameterManager::loadILNTSC()
{
  Result  res;

  return res;
}

Result  ParameterManager::loadLNAGT()
{
  Result  res;

  return res;
}

Result  ParameterManager::loadLNTITM()
{
  Result  res;

  return res;
}

Result  ParameterManager::loadLNTSC()
{
  Result  res;
  QFile   file(QString(Settings::SCU_PARAMS_DIR) + "/LNTSC" + QString(Settings::SCU_PARAM_EXT));

  if ((res.succeed = file.open(QIODevice::ReadOnly)))
  {
    QDataStream  ds(&file);
    quint16      count;

    if (res = readParamNameAndVersion(ds))
    {
      ds >> count;

      for (quint16 i = 0; i < count; ++i)
      {
        // TODO: what is type?
        quint16  type;
        quint32  serial;
        ds >> type >> serial;
        _blacklists.insert(serial);
      }
    }
  }
  else
  {
    qDebug() << "PARAMETER MISSING:" << file.fileName();
    ActivityManager::getInstance()->setAlarm(ActivityManager::AlarmGroup::Kitcas, ActivityManager::AlarmBitNumber::Bit4, true);
  }

  return res;
}

Result  ParameterManager::loadMADTICV()
{
  Result  res;
  QFile   file(QString(Settings::SCU_PARAMS_DIR) + "/MADTICV" + QString(Settings::SCU_PARAM_EXT));

  if ((res.succeed = file.open(QIODevice::ReadOnly)))
  {
    QDataStream  ds(&file);
    quint16      flatFaresCount;

    if (res = readParamNameAndVersion(ds))
    {
      ds >> flatFaresCount;

      for (quint16 i = 0; i < flatFaresCount; ++i)
      {
        // TODO: what is this?
        quint16  id, whatIsThis;
        ds >> id >> whatIsThis;
        ds.skipRawData(2);

        QVector<FlatFare>  flatFaresList;

        for (quint16 i = 0; i < 8; ++i)
        {
          FlatFare  ff;
          ds >> ff.fare1 >> ff.fare2 >> ff.jeton1 >> ff.jeton2;
          flatFaresList.append(ff);

          qDebug() << "flatFares" << id << "=" << ff.fare1 << ff.fare2;
        }

        _flatFares[id] = flatFaresList;
      }
    }
  }
  else
  {
    qDebug() << "PARAMETER MISSING:" << file.fileName();
    ActivityManager::getInstance()->setAlarm(ActivityManager::AlarmGroup::Kitcas, ActivityManager::AlarmBitNumber::Bit4, true);
  }

  return res;
}

Result  ParameterManager::loadMAPSSC()
{
  Result  res;

  return res;
}

Result  ParameterManager::loadMASQCOD()
{
  Result  res;

  return res;
}

Result  ParameterManager::loadMATCCV()
{
  Result  res;
  QFile   file(QString(Settings::SCU_PARAMS_DIR) + "/MATCCV" + QString(Settings::SCU_PARAM_EXT));

  if ((res.succeed = file.open(QIODevice::ReadOnly)))
  {
    QDataStream  ds(&file);
    quint32      itemsCount;

    if (res = readParamNameAndVersion(ds))
    {
      ds >> itemsCount;

      for (quint32 i = 0; i < itemsCount; ++i)
      {
        quint8  id;
        ds >> id;
        _sectionIds.append(id);
      }
    }
  }
  else
  {
    qDebug() << "PARAMETER MISSING:" << file.fileName();
    ActivityManager::getInstance()->setAlarm(ActivityManager::AlarmGroup::Kitcas, ActivityManager::AlarmBitNumber::Bit4, true);
  }

  return res;
}

Result  ParameterManager::loadMATPRIX()
{
  Result  res;
  QFile   file(QString(Settings::SCU_PARAMS_DIR) + "/MATPRIX" + QString(Settings::SCU_PARAM_EXT));

  if ((res.succeed = file.open(QIODevice::ReadOnly)))
  {
    QDataStream  ds(&file);
    quint16      costCategoriesCount;

    if (res = readParamNameAndVersion(ds))
    {
      ds >> costCategoriesCount;

      for (quint32 i = 0; i < costCategoriesCount; ++i)
      {
        quint16  id, sectionsCount;
        ds >> id >> sectionsCount;

        QVector<Section>  sections;

        for (quint32 j = 0; j < sectionsCount; ++j)
        {
          Section  s;

          ds >> s.id >> s.fare1 >> s.fare2;
          sections.append(s);
        }

        _costCategories[id] = sections;
      }
    }
  }
  else
  {
    qDebug() << "PARAMETER MISSING:" << file.fileName();
    ActivityManager::getInstance()->setAlarm(ActivityManager::AlarmGroup::Kitcas, ActivityManager::AlarmBitNumber::Bit4, true);
  }

  return res;
}

Result  ParameterManager::loadMESVIE()
{
  Result       res;
  QFile        file(QString(Settings::SCU_PARAMS_DIR) + "/MESVIE" + QString(Settings::SCU_PARAM_EXT));
  QDataStream  ds(&file);

  if ((res.succeed = file.open(QIODevice::ReadOnly)))
  {
    quint16  numWords, numMessages, numData;

    if (res = readParamNameAndVersion(ds))
    {
      ds >> numWords;

      for (quint16 i = 0; i < numWords; ++i)
      {
        quint8      wordId, wordSize;
        QByteArray  word(32, '\0');
        ds >> wordId >> wordSize;
        ds.readRawData(word.data(), wordSize);
        _words[wordId] = Utilities::convertToQString(word.data());
        ds.skipRawData(32 - wordSize);
      }

      ds >> numMessages;

      for (quint16 i = 0; i < numMessages; ++i)
      {
        QVector<QString>  wordsInMessage(256), dataInMessage(256);
        QString           message("");
        quint8            messageId, numWordsInMessage;
        quint16           dataId;
        quint8            dataPosition;
        quint8            spare;

        ds >> messageId >> numWordsInMessage;

        for (quint8 j = 0; j < numWordsInMessage; ++j)
        {
          quint8  wordId, wordPosition;
          ds >> wordId >> wordPosition;
          wordsInMessage[wordPosition] = getWord(wordId);
        }

        for (qint32 j = 0; j < wordsInMessage.size(); ++j)
        {
          message += wordsInMessage[i];
        }

        _messages[messageId] = message;
        ds.skipRawData(1);
        ds >> numData;

        for (int j = 0; j < numData; ++j)
        {
          ds >> dataId >> dataPosition >> spare;
          dataInMessage[dataPosition] = dataId;
        }

        // TODO: how to use data in message?
      }
    }
  }
  else
  {
    qDebug() << "PARAMETER MISSING:" << file.fileName();
    ActivityManager::getInstance()->setAlarm(ActivityManager::AlarmGroup::Kitcas, ActivityManager::AlarmBitNumber::Bit4, true);
  }

  return res;
}

Result  ParameterManager::loadPARAGEN()
{
  Result       res;
  QFile        file(QString(Settings::SCU_PARAMS_DIR) + "/PARAGEN" + QString(Settings::SCU_PARAM_EXT));
  QDataStream  ds(&file);

  if ((res.succeed = file.open(QIODevice::ReadOnly)))
  {
    quint8  yy, MM, dd, hh, mm;

    if (res = readParamNameAndVersion(ds))
    {
      ds.skipRawData(14);
      ds >> hh >> mm;
      _paragenTime.setHMS(hh, mm, 0);
      ds.skipRawData(6);
      ds >> yy >> MM >> dd;
      _fare1Date.setDate(2000 + yy, MM, dd);
      ds >> yy >> MM >> dd;
      _fare2Date.setDate(2000 + yy, MM, dd);
    }
  }
  else
  {
    qDebug() << "PARAMETER MISSING:" << file.fileName();
    ActivityManager::getInstance()->setAlarm(ActivityManager::AlarmGroup::Kitcas, ActivityManager::AlarmBitNumber::Bit4, true);
  }

  return res;
}

Result  ParameterManager::loadPERREF()
{
  Result       res;
  QFile        file(QString(Settings::SCU_PARAMS_DIR) + "/PERREF" + QString(Settings::SCU_PARAM_EXT));
  QDataStream  ds(&file);

  if ((res.succeed = file.open(QIODevice::ReadOnly)))
  {
    quint16  periodsCount;

    if (res = readParamNameAndVersion(ds))
    {
      ds >> periodsCount;

      qDebug() <<"PERIOD COUNT" << periodsCount;
      for (quint32 i = 0; i < periodsCount; ++i)
      {
        quint8  tableId, periodId, unitsCount;
        ds >> tableId >> periodId;
        ds.skipRawData(1);
        ds >> unitsCount;
        ds.skipRawData(4);
        quint16  totalDays = 0;

        // TODO: check it.
        if ((0xFF == unitsCount) || (0x6D == unitsCount))
        {
          totalDays = 365;
        }
        else
        {
          totalDays = unitsCount;
        }

        _periods[tableId] = totalDays;
        qDebug() << "PERIOD" << tableId << periodId << unitsCount << totalDays;
      }
    }
  }
  else
  {
    qDebug() << "PARAMETER MISSING:" << file.fileName();
    ActivityManager::getInstance()->setAlarm(ActivityManager::AlarmGroup::Kitcas, ActivityManager::AlarmBitNumber::Bit4, true);
  }

  return res;
}

Result  ParameterManager::loadTABACT()
{
  Result       res;
  QFile        file(QString(Settings::SCU_PARAMS_DIR) + "/TABACT" + QString(Settings::SCU_PARAM_EXT));
  QDataStream  ds(&file);

  if ((res.succeed = file.open(QIODevice::ReadOnly)))
  {
    quint16  actionsCount;

    if (res = readParamNameAndVersion(ds))
    {
      ds >> actionsCount;

      for (quint32 i = 0; i < actionsCount; ++i)
      {
        quint16  actionId;
        quint8   successMessageId;
        quint16  validationCount;

        ds >> actionId;
        ds >> successMessageId;
        ds.skipRawData(5);
        ds >> validationCount;

        QVector<ValidationAction>  validationActionList;

        for (quint32 j = 0; j < validationCount; ++j)
        {
          ValidationAction  validationAction;
          ds >> reinterpret_cast<quint8 &>(validationAction.id);
          ds >> validationAction.failMessageId;
          validationAction.successMessageId = successMessageId;
          ds >> validationAction.notUsed;
          ds >> validationAction.refusalTableId;
          validationActionList.append(validationAction);
        }

        _validationActions[actionId] = validationActionList;

        for (quint32 j = 0; j < 3; ++j)
        {
          quint16  sellingCount;
          ds >> sellingCount;

          for (quint32 k = 0; k < sellingCount; ++k)
          {
            char  skip[4];
            ds.readRawData(skip, 4);
          }
        }
      }
    }
  }
  else
  {
    qDebug() << "PARAMETER MISSING:" << file.fileName();
    ActivityManager::getInstance()->setAlarm(ActivityManager::AlarmGroup::Kitcas, ActivityManager::AlarmBitNumber::Bit4, true);
  }

  return res;
}

Result  ParameterManager::loadTABCOR()
{
  Result  res;
  QFile   file(QString(Settings::SCU_PARAMS_DIR) + "/TABCOR" + QString(Settings::SCU_PARAM_EXT));

  if ((res.succeed = file.open(QIODevice::ReadOnly)))
  {
    QDataStream  ds(&file);

    if (res = readParamNameAndVersion(ds))
    {
      quint16  transfersCount;
      ds >> transfersCount;

      for (quint32 i = 0; i < transfersCount; ++i)
      {
        TransferTable  tt;
        ds >> tt.id;
        char  uselessPart[2];
        ds.readRawData(uselessPart, 2);
        ds >> tt.maxTripDuration >> tt.maxMultiUserValidationDuration >> tt.passBackDuration;

        _transferTables[tt.id] = tt;
      }
    }
  }
  else
  {
    qDebug() << "PARAMETER MISSING:" << file.fileName();
    ActivityManager::getInstance()->setAlarm(ActivityManager::AlarmGroup::Kitcas, ActivityManager::AlarmBitNumber::Bit4, true);
  }

  return res;
}

Result  ParameterManager::loadTABJF()
{
  Result  res;
  QFile   file(QString(Settings::SCU_PARAMS_DIR) + "/TABJF" + QString(Settings::SCU_PARAM_EXT));

  if ((res.succeed = file.open(QIODevice::ReadOnly)))
  {
    QDataStream  ds(&file);
    quint16      count;

    if (res = readParamNameAndVersion(ds))
    {
      ds >> count;

      for (quint16 i = 0; i < count; i++)
      {
        quint8  yy, MM, dd;
        ds >> yy >> MM >> dd;
        _publicHolidays.append(QDate(yy, MM, dd));
      }
    }
  }
  else
  {
    qDebug() << "PARAMETER MISSING:" << file.fileName();
    ActivityManager::getInstance()->setAlarm(ActivityManager::AlarmGroup::Kitcas, ActivityManager::AlarmBitNumber::Bit4, true);
  }

  return res;
}

Result  ParameterManager::loadTABTYP()
{
  Result  res;

  return res;
}

Result  ParameterManager::loadTABVAC()
{
  Result  res;
  QFile   file(QString(Settings::SCU_PARAMS_DIR) + "/TABVAC" + QString(Settings::SCU_PARAM_EXT));

  if ((res.succeed = file.open(QIODevice::ReadOnly)))
  {
    QDataStream  ds(&file);
    quint16      count;

    if (res = readParamNameAndVersion(ds))
    {
      ds >> count;

      for (quint16 i = 0; i < count; i++)
      {
        quint8  yy, MM, dd;
        ds >> yy >> MM >> dd;
        QDate  start(yy, MM, dd);
        ds >> yy >> MM >> dd;
        QDate  end(yy, MM, dd);
        _schoolHolidays.append(QPair<QDate, QDate>(start, end));
      }
    }
  }
  else
  {
    qDebug() << "PARAMETER MISSING:" << file.fileName();
    ActivityManager::getInstance()->setAlarm(ActivityManager::AlarmGroup::Kitcas, ActivityManager::AlarmBitNumber::Bit4, true);
  }

  return res;
}

Result  ParameterManager::loadTHTIMS()
{
  Result  res;
  QFile   file(QString(Settings::SCU_PARAMS_DIR) + "/THTIMS" + QString(Settings::SCU_PARAM_EXT));

  if ((res.succeed = file.open(QIODevice::ReadOnly)))
  {
    QDataStream  ds(&file);
    quint16      tableCount;

    if (res = readParamNameAndVersion(ds))
    {
      ds >> tableCount;

      for (quint32 i = 0; i < tableCount; ++i)
      {
        QTime   netOpenTime(_paragenTime);
        quint8  tableId, timeSlotsCount;
        ds >> tableId >> timeSlotsCount;

        QVector<QPair<QTime, QTime>>  timeSlots;

        for (quint32 i = 0; i < timeSlotsCount; ++i)
        {
          QPair<QTime, QTime>  timeRange;
          quint8               hh, mm;
          ds >> hh >> mm;
          const auto &time = QTime(hh, mm);
          timeRange.first  = netOpenTime;
          timeRange.second = time;
          timeSlots.append(timeRange);
          netOpenTime = time;
        }

        _timeSlots[tableId] = timeSlots;
      }
    }
  }
  else
  {
    qDebug() << "PARAMETER MISSING:" << file.fileName();
    ActivityManager::getInstance()->setAlarm(ActivityManager::AlarmGroup::Kitcas, ActivityManager::AlarmBitNumber::Bit4, true);
  }

  return res;
}

Result  ParameterManager::loadTIMVAL()
{
  Result  res;

  return res;
}

Result  ParameterManager::loadTISVAL()
{
  Result       res;
  QFile        file(QString(Settings::SCU_PARAMS_DIR) + "/TISVAL" + QString(Settings::SCU_PARAM_EXT));
  QDataStream  ds(&file);

  if ((res.succeed = file.open(QIODevice::ReadOnly)))
  {
    quint16  ticketsCount;

    if (res = readParamNameAndVersion(ds))
    {
      ds >> ticketsCount;

      for (quint32 i = 0; i < ticketsCount; ++i)
      {
        ValidationProperty  vp;
        quint8              yy, mm, dd;

        ds >> vp.id >> vp.kindOfTicket;
        ds.skipRawData(1);
        ds >> vp.parameterTableId >> vp.kindOfValidity;
        ds >> yy >> mm >> dd;
        vp.startOfValidationAuthorization.setDate(2000 + yy, mm, dd);
        ds >> yy >> mm >> dd;
        vp.endOfValidationAuthorization.setDate(2000 + yy, mm, dd);
        ds >> vp.fare1 >> vp.fare2;
        ds.skipRawData(2);
        ds >> vp.maxTrips >> vp.transferAllowed >> vp.transferTableId >> vp.actionTableIdforEntry
            >> vp.encodingMaskId >> vp.periodTableId >> vp.validityTableIdForRegularDays >> vp.validityTableIdForSchoolDays;
        ds.skipRawData(1);
        ds >> vp.validityTableIdForPublicHolidays >> vp.actionTableIdForExit
            >> vp.tableIdForAmountOrTokenUseForRegularDays >> vp.tableIdForAmountOrTokenUseForSchoolDays;
        ds.skipRawData(1);
        ds >> vp.tableIdForAmountOrTokenUseForPublicHolidays >> vp.writeActionIndex >> vp.fareIndex;
        ds.skipRawData(12);
        _validationProperties[vp.id] = vp;
      }
    }
  }
  else
  {
    qDebug() << "PARAMETER MISSING:" << file.fileName();
    ActivityManager::getInstance()->setAlarm(ActivityManager::AlarmGroup::Kitcas, ActivityManager::AlarmBitNumber::Bit4, true);
  }

  return res;
}

Result  ParameterManager::loadTISVTE()
{
  Result       res;
  QFile        file(QString(Settings::SCU_PARAMS_DIR) + "/TISVTE" + QString(Settings::SCU_PARAM_EXT));
  QDataStream  ds(&file);

  if ((res.succeed = file.open(QIODevice::ReadOnly)))
  {
    quint16  count;

    if (res = readParamNameAndVersion(ds))
    {
      ds >> count;

      for (quint16 i = 0; i < count; ++count)
      {
        quint16     cardTypeId;
        QByteArray  cardName(30, '\0');
        ds >> cardTypeId;
        ds.skipRawData(36);
        ds.readRawData(cardName.data(), cardName.size());
        ds.skipRawData(50);
        _cardTypeNames[cardTypeId] = Utilities::convertToQString(cardName.data());
      }
    }
  }
  else
  {
    qDebug() << "PARAMETER MISSING:" << file.fileName();
    ActivityManager::getInstance()->setAlarm(ActivityManager::AlarmGroup::Kitcas, ActivityManager::AlarmBitNumber::Bit4, true);
  }

  return res;
}

Result  ParameterManager::loadTIVARPAR()
{
  Result  res;
  QFile   file(QString(Settings::SCU_PARAMS_DIR) + "/TIVARPAR" + QString(Settings::SCU_PARAM_EXT));

  if ((res.succeed = file.open(QIODevice::ReadOnly)))
  {
    QDataStream  ds(&file);
    quint16      paramsCount;

    if (res = readParamNameAndVersion(ds))
    {
      ds >> paramsCount;

      for (quint32 i = 0; i < paramsCount; ++i)
      {
        TicketParameter  tp;
        ds >> tp.id >> tp.discountRateId >> tp.costCategoryId >> tp.fineValue >> tp.depositValue;
        ds.skipRawData(2);
        _ticketParams[tp.id] = tp;
      }
    }
  }
  else
  {
    qDebug() << "PARAMETER MISSING:" << file.fileName();
    ActivityManager::getInstance()->setAlarm(ActivityManager::AlarmGroup::Kitcas, ActivityManager::AlarmBitNumber::Bit4, true);
  }

  return res;
}

Result  ParameterManager::loadVALPAR()
{
  Result  res;

  return res;
}

Result  ParameterManager::loadVALTIT()
{
  Result  res;
  QFile   file(QString(Settings::SCU_PARAMS_DIR) + "/VALTIT" + QString(Settings::SCU_PARAM_EXT));

  if ((res.succeed = file.open(QIODevice::ReadOnly)))
  {
    QDataStream  ds(&file);
    quint16      tableCount;

    if (res = readParamNameAndVersion(ds))
    {
      ds >> tableCount;

      for (quint32 i = 0; i < tableCount; i++)
      {
        QVector<ValidityDaysTimeSlot>  validityDaysTimeSlots;
        quint8                         tableId, daysCount;
        ds >> tableId >> daysCount;

        qDebug() << "table" << i << "tableId" << tableId << "dayCount" << daysCount;
        for (quint32 j = 0; j < daysCount; ++j)
        {
          ValidityDaysTimeSlot  vdts;
          ds >> vdts.dayId >> vdts.timeSlotTableId >> vdts.validityBits;

          qDebug() << "day" << j;
          for (quint32 k = 0; k < 8; ++k)
          {
            quint8  discount;
            ds >> discount;
            vdts.discountRates.push_back(discount);

            qDebug() << "discount" << discount;
          }

          validityDaysTimeSlots.append(vdts);
        }

        _validityDaysTimes[tableId] = validityDaysTimeSlots;
      }
    }
  }
  else
  {
    qDebug() << "PARAMETER MISSING:" << file.fileName();
    ActivityManager::getInstance()->setAlarm(ActivityManager::AlarmGroup::Kitcas, ActivityManager::AlarmBitNumber::Bit4, true);
  }

  return res;
}

QString  ParameterManager::getCardTypeName(quint16 cardTypeId)
{
  return _cardTypeNames[cardTypeId];
}

QString  ParameterManager::getWord(quint8 id)
{
  return _words[id];
}

Result  ParameterManager::getStationById(quint16 id, Station *s)
{
  Result  res;

  if (_stations.contains(id))
  {
    *s          = _stations[id];
    res.succeed = true;
  }

  return res;
}
}
