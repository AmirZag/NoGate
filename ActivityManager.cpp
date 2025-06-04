#include <ActivityManager.h>
#include <GateController.h>
#include <Settings.h>
#include <Utilities.h>
#include <QDir>
#include <QDebug>


#define SESSION_FILE_NAME "~am"
#define ACT_FILE_NAME     "~act"
#define SET_BIT(N) (1 << N)

static QDataStream & operator<<(QDataStream &ds, const Afc::ActivityManager::ShiftValidationLog &data)
{
  ds << data.date;
  ds.writeBytes(reinterpret_cast<const char *>(data.equipmentSerial), sizeof(data.equipmentSerial));
  ds << quint8(data.equipmentType) << data.equipmentSubType << data.equipmentSoftwareRelease << data.equipmentSoftwareAmendment
     << quint8(data.shiftMode) << data.stationId << data.gateId << data.shiftId << quint8(data.configuration)
     << quint8(data.operatingMode) << data.operatorId;

  return ds;
}

static QDataStream & operator>>(QDataStream &ds, Afc::ActivityManager::ShiftValidationLog &data)
{
  quint32  len;

  ds >> data.date;
  ds.readBytes(reinterpret_cast<char * &>(data.equipmentSerial), len);
  ds >> reinterpret_cast<quint8 &>(data.equipmentType) >> data.equipmentSubType
                                                       >> data.equipmentSoftwareRelease >> data.equipmentSoftwareAmendment
                                                       >> reinterpret_cast<quint8 &>(data.shiftMode) >> data.stationId >> data.gateId
                                                       >> data.shiftId >> reinterpret_cast<quint8 &>(data.configuration)
                                                       >> reinterpret_cast<quint8 &>(data.operatingMode) >> data.operatorId;

  return ds;
}

static QDataStream & operator<<(QDataStream &ds, const Afc::ActivityManager::ActivityValidationLog &data)
{
  ds << quint8(data.activityMode) << data.date << data.companyId << data.stationId << quint8(data.equipmentType) << data.equipmentSubType;
  ds.writeBytes(reinterpret_cast<const char *>(data.equipmentSerial), sizeof(data.equipmentSerial));
  ds << data.equipmentSoftwareRelease << data.equipmentSoftwareAmendment;

  return ds;
}

static QDataStream & operator>>(QDataStream &ds, Afc::ActivityManager::ActivityValidationLog &data)
{
  quint32  len;

  ds >> reinterpret_cast<quint8 &>(data.activityMode) >> data.date >> data.companyId >> data.stationId
                                                      >> reinterpret_cast<quint8 &>(data.equipmentType) >> data.equipmentSubType;
  ds.readBytes(reinterpret_cast<char * &>(data.equipmentSerial), len);
  ds >> data.equipmentSoftwareRelease >> data.equipmentSoftwareAmendment;

  return ds;
}

namespace Afc
{
struct AlarmMap
{
  AlarmMap()
  {
    map[ActivityManager::AlarmGroup::Kitcas].insert(SET_BIT(0), 100);
    map[ActivityManager::AlarmGroup::Kitcas].insert(SET_BIT(1), 101);
    map[ActivityManager::AlarmGroup::Kitcas].insert(SET_BIT(2), 192);
    map[ActivityManager::AlarmGroup::Kitcas].insert(SET_BIT(3), 185);
    map[ActivityManager::AlarmGroup::Kitcas].insert(SET_BIT(4), 184);
    map[ActivityManager::AlarmGroup::Kitcas].insert(SET_BIT(5), 7);
    map[ActivityManager::AlarmGroup::Kitcas].insert(SET_BIT(6), 40);
    map[ActivityManager::AlarmGroup::Kitcas].insert(SET_BIT(7), 6);
    map[ActivityManager::AlarmGroup::Kitcas].insert(SET_BIT(8), 5);
    map[ActivityManager::AlarmGroup::Kitcas].insert(SET_BIT(9), 44);
    map[ActivityManager::AlarmGroup::Kitcas].insert(SET_BIT(10), 45);
    map[ActivityManager::AlarmGroup::Kitcas].insert(SET_BIT(11), 183);
    map[ActivityManager::AlarmGroup::Kitcas].insert(SET_BIT(12), 183);
    map[ActivityManager::AlarmGroup::Kitcas].insert(SET_BIT(13), 183);
    map[ActivityManager::AlarmGroup::Kitcas].insert(SET_BIT(14), 183);
    map[ActivityManager::AlarmGroup::Kitcas].insert(SET_BIT(15), 183);

    map[ActivityManager::AlarmGroup::EntryReader].insert(SET_BIT(0), 10185);
    map[ActivityManager::AlarmGroup::EntryReader].insert(SET_BIT(1), 10004);
    map[ActivityManager::AlarmGroup::EntryReader].insert(SET_BIT(2), 10005);
    map[ActivityManager::AlarmGroup::EntryReader].insert(SET_BIT(3), 10002);
    map[ActivityManager::AlarmGroup::EntryReader].insert(SET_BIT(4), 10102);
    map[ActivityManager::AlarmGroup::EntryReader].insert(SET_BIT(5), 10007);
    map[ActivityManager::AlarmGroup::EntryReader].insert(SET_BIT(6), 10027);
    map[ActivityManager::AlarmGroup::EntryReader].insert(SET_BIT(7), 10120);
    map[ActivityManager::AlarmGroup::EntryReader].insert(SET_BIT(8), 10103);
    map[ActivityManager::AlarmGroup::EntryReader].insert(SET_BIT(9), 10190);
    map[ActivityManager::AlarmGroup::EntryReader].insert(SET_BIT(10),10030);
    map[ActivityManager::AlarmGroup::EntryReader].insert(SET_BIT(11),10031);
    map[ActivityManager::AlarmGroup::EntryReader].insert(SET_BIT(12),10032);
    map[ActivityManager::AlarmGroup::EntryReader].insert(SET_BIT(13),10033);
    map[ActivityManager::AlarmGroup::EntryReader].insert(SET_BIT(14),10034);
    map[ActivityManager::AlarmGroup::EntryReader].insert(SET_BIT(15),10035);

    map[ActivityManager::AlarmGroup::ExitReader].insert(SET_BIT(0), 20185);
    map[ActivityManager::AlarmGroup::ExitReader].insert(SET_BIT(1), 20004);
    map[ActivityManager::AlarmGroup::ExitReader].insert(SET_BIT(2), 20005);
    map[ActivityManager::AlarmGroup::ExitReader].insert(SET_BIT(3), 20002);
    map[ActivityManager::AlarmGroup::ExitReader].insert(SET_BIT(4), 20102);
    map[ActivityManager::AlarmGroup::ExitReader].insert(SET_BIT(5), 20007);
    map[ActivityManager::AlarmGroup::ExitReader].insert(SET_BIT(6), 20027);
    map[ActivityManager::AlarmGroup::ExitReader].insert(SET_BIT(7), 20120);
    map[ActivityManager::AlarmGroup::ExitReader].insert(SET_BIT(8), 20103);
    map[ActivityManager::AlarmGroup::ExitReader].insert(SET_BIT(9), 20190);
    map[ActivityManager::AlarmGroup::ExitReader].insert(SET_BIT(10),20030);
    map[ActivityManager::AlarmGroup::ExitReader].insert(SET_BIT(11),20031);
    map[ActivityManager::AlarmGroup::ExitReader].insert(SET_BIT(12),20032);
    map[ActivityManager::AlarmGroup::ExitReader].insert(SET_BIT(13),20033);
    map[ActivityManager::AlarmGroup::ExitReader].insert(SET_BIT(14),20034);
    map[ActivityManager::AlarmGroup::ExitReader].insert(SET_BIT(15),20035);

    map[ActivityManager::AlarmGroup::Logic].insert(SET_BIT(0), 171);
    map[ActivityManager::AlarmGroup::Logic].insert(SET_BIT(1), 172);
    map[ActivityManager::AlarmGroup::Logic].insert(SET_BIT(2), 173);
    map[ActivityManager::AlarmGroup::Logic].insert(SET_BIT(3), 174);
    map[ActivityManager::AlarmGroup::Logic].insert(SET_BIT(4), 175);
    map[ActivityManager::AlarmGroup::Logic].insert(SET_BIT(5), 160);
    map[ActivityManager::AlarmGroup::Logic].insert(SET_BIT(6), 161);
    map[ActivityManager::AlarmGroup::Logic].insert(SET_BIT(7), 163);
    map[ActivityManager::AlarmGroup::Logic].insert(SET_BIT(8), 169);
    map[ActivityManager::AlarmGroup::Logic].insert(SET_BIT(9), 170);
    map[ActivityManager::AlarmGroup::Logic].insert(SET_BIT(10), 176);
    map[ActivityManager::AlarmGroup::Logic].insert(SET_BIT(11), 177);
    map[ActivityManager::AlarmGroup::Logic].insert(SET_BIT(12), 178);
    map[ActivityManager::AlarmGroup::Logic].insert(SET_BIT(13), 179);
    map[ActivityManager::AlarmGroup::Logic].insert(SET_BIT(14), 180);
    map[ActivityManager::AlarmGroup::Logic].insert(SET_BIT(15), 181);

    map[ActivityManager::AlarmGroup::Sc].insert(SET_BIT(0),  40100);
    map[ActivityManager::AlarmGroup::Sc].insert(SET_BIT(1),  40101);
    map[ActivityManager::AlarmGroup::Sc].insert(SET_BIT(2),  40102);
    map[ActivityManager::AlarmGroup::Sc].insert(SET_BIT(3),  40103);
    map[ActivityManager::AlarmGroup::Sc].insert(SET_BIT(4),  40104);
    map[ActivityManager::AlarmGroup::Sc].insert(SET_BIT(5),  40105);
    map[ActivityManager::AlarmGroup::Sc].insert(SET_BIT(6),  40106);
    map[ActivityManager::AlarmGroup::Sc].insert(SET_BIT(7),  40107);
    map[ActivityManager::AlarmGroup::Sc].insert(SET_BIT(8),  40108);
    map[ActivityManager::AlarmGroup::Sc].insert(SET_BIT(9),  40109);
    map[ActivityManager::AlarmGroup::Sc].insert(SET_BIT(10), 40110);
    map[ActivityManager::AlarmGroup::Sc].insert(SET_BIT(11), 40111);
    map[ActivityManager::AlarmGroup::Sc].insert(SET_BIT(12), 40112);
    map[ActivityManager::AlarmGroup::Sc].insert(SET_BIT(13), 40113);
    map[ActivityManager::AlarmGroup::Sc].insert(SET_BIT(14), 40114);
    map[ActivityManager::AlarmGroup::Sc].insert(SET_BIT(15), 40115);
  }

  QMap<ActivityManager::AlarmGroup, QMap<quint16, quint16>> map;
};

static const AlarmMap ALARM_MAP;


ActivityManager::ActivityManager():
  _actId(1), _stationId(0), _stationStatus(false), _maxFileSize(0),
  _configurationMode(ActivityManager::ConfigurationMode::Online), _shiftStatus(false)
{
  // _counterEntry = 0;
  // _counterExit  = 0;

  _alarms[AlarmGroup::Kitcas]      = 0;
  _alarms[AlarmGroup::EntryReader] = 0;
  _alarms[AlarmGroup::ExitReader]  = 0;
  _alarms[AlarmGroup::Logic]       = 0;
  _alarms[AlarmGroup::Sc]          = 0;

  if (!QDir(Settings::SCU_ACTS_DIR).exists())
  {
    QDir().mkdir(Settings::SCU_ACTS_DIR);
    Utilities::setOwner(Settings::SCU_ACTS_DIR , Settings::USER_NAME);
  }

  countActFiles();
  loadSession();
}

void  ActivityManager::countActFiles() const
{
  QDir  dirActs(Settings::SCU_ACTS_DIR);

  dirActs.setFilter(QDir::Files | QDir::NoSymLinks | QDir::NoDotAndDotDot);
  _actFileCount = dirActs.count();
}

QString  ActivityManager::generateActFileName()
{
  const auto &date = QDateTime::currentDateTimeUtc().date();
  const auto &now  = QString::number(date.dayOfYear()).rightJustified(3, '0');

  if (date != _lastGeneratedIdDate)
  {
    _actId = 1;
  }

  auto  fileName = QString(Settings::SCU_ACTS_DIR) + QString("/")
                   + "Y" + now + QString::number(_actId).rightJustified(4, '0') + Settings::SCU_ACTS_EXT;

  while (QFile::exists(fileName))
  {
    ++_actId;
    fileName = Settings::SCU_ACTS_DIR + QString("/")
               + "Y" + now + QString::number(_actId).rightJustified(4, '0') + Settings::SCU_ACTS_EXT;
  }

  return fileName;
}

ActivityManager::ActivityMode  ActivityManager::getActivityMode() const
{
  return _activityMode;
}

void  ActivityManager::setActivityMode(const ActivityMode &activityMode)
{
  _activityMode = activityMode;
}

void  ActivityManager::setShiftStatus(bool)
{
  // _shiftStatus = shiftStatus;
}

// void  ActivityManager::setCounterExit(const quint32 &counterExit)
// {
// _counterExit = counterExit;
// }

// void  ActivityManager::setCounterEntry(const quint32 &counterEntry)
// {
// _counterEntry = counterEntry;
// }

bool  ActivityManager::getShiftStatus() const
{
  return _shiftStatus;
}

quint8  ActivityManager::getLogicType() const
{
  return _logicType;
}

quint8  ActivityManager::getCompanyService() const
{
  return _companyService;
}

quint8  ActivityManager::getValidationType() const
{
  return _validationType;
}

quint16  ActivityManager::getMaxFileSize() const
{
  return _maxFileSize;
}

void  ActivityManager::setConfigurationMode(const ConfigurationMode &configurationMode)
{
  _configurationMode = configurationMode;
}

// quint32  ActivityManager::getCounterExit() const
// {
// return _counterExit;
// }

// quint32  ActivityManager::getCounterEntry() const
// {
// return _counterEntry;
// }

quint32  ActivityManager::getActFileCount() const
{
  countActFiles();

  return _actFileCount;
}

bool  ActivityManager::getStationStatus() const
{
  return _stationStatus;
}

void  ActivityManager::setStationStatus(bool stationStatus)
{
  _stationStatus = stationStatus;
}

ActivityManager::AccessControlGateType  ActivityManager::getAcgType() const
{
  return _acgType;
}

quint16  ActivityManager::getStationId() const
{
  return _stationId;
}

ActivityManager::ConfigurationMode  ActivityManager::getConfigurationMode() const
{
  return _configurationMode;
}

void  ActivityManager::openActFile(const ActivityValidationLog &log)
{
  if (!_fileAct.isOpen())
  {
    _fileAct.setFileName(ACT_FILE_NAME);
    _fileAct.open(QIODevice::ReadWrite);
    _fileAct.seek(_fileAct.size());
  }

  if (_fileAct.isOpen())
  {
    _lastAct = log;
    _streamAct.setDevice(&_fileAct);

    switch (log.activityMode)
    {
    case ActivityMode::OperatingMode:
    {
      _streamAct.writeRawData("ME", 2);
    }
    break;
    case ActivityMode::MaintenanceMode:
    {
      _streamAct.writeRawData("MM", 2);
    }
    break;
    default:
    {
      _streamAct.writeRawData("MM", 2);
    }
    break;
    }

    _streamAct << quint8(log.date.date().day()) << quint8(log.date.date().month()) << quint8(log.date.date().year() % 100)
               << quint8(log.date.time().hour()) << quint8(log.date.time().minute()) << quint8(log.date.time().second())
               << log.companyId << log.stationId << quint8(log.equipmentType) << log.equipmentSubType;
    _streamAct.writeRawData(reinterpret_cast<const char *>(log.equipmentSerial), sizeof(log.equipmentSerial));
    _streamAct << _actId;

    switch (log.activityMode)
    {
    case ActivityMode::OperatingMode:
    {
      _streamAct << log.equipmentSoftwareRelease << log.equipmentSoftwareAmendment;
    }
    break;
    case ActivityMode::MaintenanceMode:
    {
      _streamAct << quint8(log.equipmentSoftwareRelease) << quint8(log.equipmentSoftwareAmendment);
    }
    break;
    }

    saveSession();
  }
  else
  {
    qDebug() << "T1 File not found:" << _fileAct.fileName();
  }
}

void  ActivityManager::reopenActFile()
{
  openActFile(_lastAct);
}

void  ActivityManager::closeActFile(const QDateTime &date)
{
  quint8  signature[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  _streamAct.writeRawData("SF", 2);
  _streamAct << quint8(date.date().day()) << quint8(date.date().month()) << quint8(date.date().year() % 100)
             << quint8(date.time().hour()) << quint8(date.time().minute()) << quint8(date.time().second()) << quint8(0);
  _streamAct.writeRawData(reinterpret_cast<const char *>(signature), sizeof(signature));

  _fileAct.flush();

  // Removing unnecessary act files. TODO: CAUTION!
  if (130 < _fileAct.size())
  {
    _fileAct.rename(generateActFileName());
  }
  else
  {
    _fileAct.remove();
  }

  ++_actId;

  countActFiles();
  saveSession();
}

void  ActivityManager::checkFileSize()
{
  if (_fileAct.isOpen())
  {
    _fileAct.flush();

    // TODO: check all blocks, line by line.
    if (_maxFileSize <= _fileAct.size() + 200)
    {
      const auto &now = QDateTime::currentDateTimeUtc();
      closeShift(now.time(), 1, 1);
      closeActFile(now);

      ActivityValidationLog  act = _lastAct;
      act.date = QDateTime::currentDateTimeUtc();
      openActFile(act);

      ShiftValidationLog  shift = _lastShift;
      shift.date = now;
      openShift(shift);
    }
  }
  else
  {
    qDebug() << "T2 File not found:" << _fileAct.fileName();
  }
}

void  ActivityManager::openShift(const ShiftValidationLog &log)
{
  _shiftStatus = true;
  _lastShift   = log;

  _streamAct.writeRawData("AE", 2);
  _streamAct << quint8(log.date.date().day()) << quint8(log.date.date().month()) << quint8(log.date.date().year() % 100)
             << quint8(log.date.time().hour()) << quint8(log.date.time().minute()) << quint8(log.date.time().second()) << quint8(1);
  _streamAct.writeRawData(reinterpret_cast<const char *>(log.equipmentSerial), sizeof(log.equipmentSerial));
  _streamAct << quint8(log.equipmentType) << log.equipmentSubType << log.equipmentSoftwareRelease << log.equipmentSoftwareAmendment
             << quint8(log.shiftMode) << log.stationId << log.gateId << log.shiftId << quint8(log.configuration)
             << quint8(log.operatingMode) << log.operatorId;

  saveSession();
}

void  ActivityManager::reopenShift()
{
  openShift(_lastShift);
}

void  ActivityManager::closeShift(const QTime &time, quint32 numPassengerEntries, quint32 numPassengerExit)
{
  _shiftStatus = false;
  _streamAct.writeRawData("AF", 2);
  _streamAct << numPassengerEntries << numPassengerExit << quint8(time.hour()) << quint8(time.minute()) << quint8(time.second());

  saveSession();
}

void  ActivityManager::appendValidation(ValidationLogType type, const CardValidationLog &log)
{
  if (_fileAct.isOpen())
  {
    switch (type)
    {
    case ValidationLogType::Entry:
    {
      _streamAct.writeRawData("VC", 2);
    }
    break;
    case ValidationLogType::Exit:
    {
      _streamAct.writeRawData("VD", 2);
    }
    break;
    }

    _streamAct << quint16(114) << quint8(log.time.hour()) << quint8(log.time.minute()) << quint8(log.time.second())
               << log.transactionId << quint8(log.mediumType);
    _streamAct.writeRawData(reinterpret_cast<const char *>(log.mediumSerialId), sizeof(log.mediumSerialId));
    _streamAct.writeRawData(reinterpret_cast<const char *>(log.intrinsicMediumSerialId), sizeof(log.intrinsicMediumSerialId));
    _streamAct << log.ticketType << log.contractId << quint8(log.prevValidationMode)
               << quint8(log.prevValidationDate.date().day())
               << quint8(log.prevValidationDate.date().month())
               << quint8(log.prevValidationDate.date().year() % 100)
               << quint8(log.prevValidationDate.time().hour())
               << quint8(log.prevValidationDate.time().minute())
               << quint8(log.prevValidationDate.time().second())
               << log.prevCompanyId << log.prevStationId
               << quint8(log.ticketEvd.day())
               << quint8(log.ticketEvd.month())
               << quint8(log.ticketEvd.year() % 100)
               << log.discountRate << log.numValidSections << log.amountDecreased << log.tripDecreased
               << log.remainingAmount << log.remainingTrips << log.prevRemainingAmount;
    _streamAct.writeRawData(reinterpret_cast<const char *>(log.samSerialId), sizeof(log.samSerialId));
    _streamAct.writeRawData(reinterpret_cast<const char *>(log.cwCode), sizeof(log.cwCode));

    checkFileSize();
  }
  else
  {
    qDebug() << "T3 File not found:" << _fileAct.fileName();
    reopenActFile();
  }
}

void ActivityManager::appendValidation(quint8 validationStatusCode, const ActivityManager::QrValidationLog &log)
{
  if (_fileAct.isOpen())
  {
    _streamAct.writeRawData("VQ", 2);
    //_streamAct.writeRawData(log.issuanceDate.toString("yyMMdd").toLatin1().data(), 6);
    _streamAct << quint8(log.issuanceDate.day());
    _streamAct << quint8(log.issuanceDate.month());
    _streamAct << quint8(log.issuanceDate.year() % 100);
    _streamAct.writeRawData(log.issuerSerial.data(), log.issuerSerial.size());
    //_streamAct.writeRawData(log.validationTime.toString("yyMMddhhmmss").toLatin1().data(), 12);
    _streamAct << quint8(log.validationTime.date().day());
    _streamAct << quint8(log.validationTime.date().month());
    _streamAct << quint8(log.validationTime.date().year() % 100);
    _streamAct << quint8(log.validationTime.time().hour());
    _streamAct << quint8(log.validationTime.time().minute());
    _streamAct << quint8(log.validationTime.time().second());

    _streamAct << log.qrCounter;
    _streamAct.writeRawData(log.qrSerial.data(), log.qrSerial.size());
    _streamAct << log.readerCounter;
    _streamAct.writeRawData(log.readerSerial.data(), 12);
    _streamAct << log.type << log.validationStationId << log.ticketType << validationStatusCode;
    //_streamAct.writeRawData(log.ticketEvd.toString("yyMMdd").toLatin1().data(), 6);
    _streamAct << quint8(log.ticketEvd.day());
    _streamAct << quint8(log.ticketEvd.month());
    _streamAct << quint8(log.ticketEvd.year() % 100);
    _streamAct << log.validityDays;
    _streamAct << log.amount << log.remainingTrips << log.keyNumber;
    _streamAct.writeRawData(log.mac.data(), log.mac.size());
    _streamAct.writeRawData(log.encryptedData.data(), log.encryptedData.size());

    checkFileSize();
  }
  else
  {
    qDebug() << "T1 File not found:" << _fileAct.fileName();
    reopenActFile();
  }
}

bool  ActivityManager::isActOpen() const
{
  return _fileAct.isOpen() && 0 < _fileAct.size();
}

bool  ActivityManager::isShiftOpen() const
{
  return _shiftStatus;
}

void  ActivityManager::setStationId(quint16 id)
{
  _stationId = id;
}

void  ActivityManager::setMaxFileSize(quint16 size)
{
  _maxFileSize = size;
}

void  ActivityManager::setValidationType(quint8 type)
{
  _validationType = type;
}

void  ActivityManager::setCompanyService(quint8 cs)
{
  _companyService = cs;
}

void  ActivityManager::setAcgType(AccessControlGateType type)
{
  _acgType = type;
}

void  ActivityManager::setAlarm(AlarmGroup group, AlarmBitNumber bit, bool enable)
{
  const auto  oldState = _alarms[group];

  if (enable)
  {
    _alarms[group] |= quint16(bit);
  }
  else
  {
    _alarms[group] &= ~quint16(bit);
  }

  if (oldState != _alarms[group])
  {
    AlarmLog    log;
    const auto &eqid = Utilities::generateEquipmentSerialId();

    log.alarmCode                = enable ? ALARM_MAP.map[group][_alarms[group]] : ALARM_MAP.map[group][oldState];
    log.alarmStatus              = enable ? AlarmStatus::Start : AlarmStatus::End;
    log.equipmentOperationStatus = ParameterManager::EquipmentOperationStatus::OutOfOrder;
    log.equipmentType            = ParameterManager::EquipmentType::Reader;
    log.equipmentSubType         = Settings::EQUIPMENT_SUB_TYPE;
    memcpy(log.equipmentSerial, eqid.constData(), eqid.size());
    log.time = QDateTime::currentDateTimeUtc().time();

    appendAlarm(log);
  }
}

void  ActivityManager::setAlarm(AlarmGroup group, quint16 alarm)
{
  const auto  oldState = _alarms[group];

  _alarms[group] = alarm;

  if (oldState != _alarms[group])
  {
    AlarmLog    log;
    const auto &eqid = Utilities::generateEquipmentSerialId();

    if (0 < alarm)
    {
      log.alarmCode = ALARM_MAP.map[group][alarm];
    }
    else
    {
      log.alarmCode = ALARM_MAP.map[group][oldState];
    }

    log.alarmStatus              = 0 < alarm ? AlarmStatus::Start : AlarmStatus::End;
    log.equipmentOperationStatus = ParameterManager::EquipmentOperationStatus::OutOfOrder;
    log.equipmentType            = ParameterManager::EquipmentType::Reader;
    log.equipmentSubType         = Settings::EQUIPMENT_SUB_TYPE;
    memcpy(log.equipmentSerial, eqid.constData(), eqid.size());
    log.time = QDateTime::currentDateTimeUtc().time();

    appendAlarm(log);
  }
}

quint16  ActivityManager::getAlarm(ActivityManager::AlarmGroup group) const
{
  return _alarms[group];
}

quint8  ActivityManager::getAlarmsMask() const
{
  quint8  mask = 0;

  mask |= _alarms[AlarmGroup::Kitcas] ? quint8(AlarmGroup::Kitcas) : 0;
  mask |= _alarms[AlarmGroup::EntryReader] ? quint8(AlarmGroup::EntryReader) : 0;
  mask |= _alarms[AlarmGroup::ExitReader] ? quint8(AlarmGroup::ExitReader) : 0;
  mask |= _alarms[AlarmGroup::Logic] ? quint8(AlarmGroup::Logic) : 0;
  mask |= _alarms[AlarmGroup::Sc] ? quint8(AlarmGroup::Sc) : 0;

  return mask;
}

void  ActivityManager::setLogicType(quint8 type)
{
  _logicType = type;
}

void  ActivityManager::appendAlarm(const AlarmLog &log)
{
  if (_fileAct.isOpen())
  {
    _streamAct.writeRawData("ZZ", 2);
    _streamAct << quint8(log.time.hour()) << quint8(log.time.minute()) << quint8(log.time.second())
               << quint8(log.equipmentType) << log.equipmentSubType;
    _streamAct.writeRawData(reinterpret_cast<const char *>(log.equipmentSerial), sizeof(log.equipmentSerial));
    _streamAct << quint8(log.equipmentOperationStatus) << quint8(log.alarmStatus) << quint16(0) << quint8(0) << log.alarmCode;

    checkFileSize();
  }
  else
  {
    qDebug() << "T4 File not found:" << _fileAct.fileName();
  }
}

void  ActivityManager::appendBlackList(const ActivityManager::BlackListLog &log)
{
  if (_fileAct.isOpen())
  {
    _streamAct.writeRawData("BZ", 2);
    _streamAct << quint16(37) << quint8(log.time.hour()) << quint8(log.time.minute()) << quint8(log.time.second())
               << quint8(log.mediumType) << quint8(0) << log.ticketType;
    _streamAct.writeRawData(reinterpret_cast<const char *>(log.mediumSerialId), sizeof(log.mediumSerialId));
    _streamAct.writeRawData(reinterpret_cast<const char *>(log.intrinsicMediumSerialId), sizeof(log.intrinsicMediumSerialId));
    _streamAct.writeRawData(reinterpret_cast<const char *>(log.samSerialId), sizeof(log.samSerialId));

    checkFileSize();
  }
  else
  {
    qDebug() << "T5 File not found:" << _fileAct.fileName();
  }
}

void  ActivityManager::appendRejectedValidation(const ActivityManager::RejectedValidationLog &log)
{
  if (_fileAct.isOpen())
  {
    _streamAct.writeRawData("BY", 2);
    _streamAct << quint16(40) << quint8(log.time.hour()) << quint8(log.time.minute()) << quint8(log.time.second())
               << quint8(log.mediumType) << quint8(0) << log.ticketType << log.rejectCode << quint8(0);
    _streamAct.writeRawData(reinterpret_cast<const char *>(log.mediumSerialId), sizeof(log.mediumSerialId));
    _streamAct.writeRawData(reinterpret_cast<const char *>(log.intrinsicMediumSerialId), sizeof(log.intrinsicMediumSerialId));
    _streamAct.writeRawData(reinterpret_cast<const char *>(log.samSerialId), sizeof(log.samSerialId));

    checkFileSize();
  }
  else
  {
    qDebug() << "T6 File not found:" << _fileAct.fileName();
  }
}

void  ActivityManager::loadSession()
{
  const auto &now = QDateTime::currentDateTimeUtc().date();

  _fileSession.setFileName(SESSION_FILE_NAME);
  bool  hasSession = _fileSession.exists();

  if (_fileSession.open(QIODevice::ReadWrite))
  {
    _streamSession.setDevice(&_fileSession);

    if (hasSession)
    {
      quint16  id;
      _streamSession >> _lastGeneratedIdDate >> id >> _lastAct >> _lastShift >> _shiftStatus;

      if (now == _lastGeneratedIdDate)
      {
        _actId = id;
      }
      else
      {
        _actId = 1;
      }

      _fileAct.setFileName(ACT_FILE_NAME);
      _fileAct.open(QIODevice::ReadWrite);
      _fileAct.seek(_fileAct.size());
      _streamAct.setDevice(&_fileAct);
    }
    else
    {
      _lastGeneratedIdDate = now;
      saveSession();
    }
  }
}

void  ActivityManager::saveSession()
{
  _fileSession.seek(0);
  _streamSession.setDevice(&_fileSession);
  _streamSession << QDateTime::currentDateTimeUtc().date() << _actId << _lastAct << _lastShift << _shiftStatus;
  _fileSession.flush();
}
}
