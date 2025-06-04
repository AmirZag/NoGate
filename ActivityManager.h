#ifndef ACTIVITYMANAGER_H
#define ACTIVITYMANAGER_H

#include <QSharedPointer>
#include <QFile>
#include <QDataStream>
#include <QDateTime>
#include <ParameterManager.h>


namespace Afc
{
class ActivityManager
{
public:
  enum class ActivityMode : quint8
  {
    OperatingMode   = 1,
    MaintenanceMode = 2
  };

  enum class ShiftMode : quint8
  {
    OperatingShift   = 1,
    MaintenanceShift = 2
  };

  enum class ConfigurationMode : quint8
  {
    Online = 1,
    Local  = 2
  };

  enum class GateOperatingMode : quint8
  {
    Entry         = 1,
    Exit          = 2,
    Locked        = 3,
    FreeEntryExit = 4,
    FreeExit      = 5,
  };

  enum class MediumType : quint8
  {
    Mag = 2,
    Csc = 3,
    Tag = 4,
    Mbl = 5,
    Des = 6,
    Qr1 = 7,
    Ja3 = 8,
    Ja4 = 9
  };

  enum class ValidationMode : quint8
  {
    DontValidate    = 0,
    ValidateOnEntry = 1,
    ValidateOnExit  = 2
  };

  enum class QrValidationMode: quint8
  {
    Valid                         = 0x02,
    DisconnectedFromCentralServer = 0x04,
    DirectConnectionToBank        = 0x05,
    MultiTrip                     = 0x06,
    OfflineMode                   = 0x07,
  };

  enum class ValidationLogType : quint8
  {
    Entry,
    Exit
  };

  enum class AccessControlGateType : quint8
  {
    Entry     = 1,
    Exit      = 2,
    EntryExit = 3,
  };

  enum class AlarmGroup : quint8
  {
    Kitcas      = 0b00000001,
    EntryReader = 0b00000010,
    ExitReader  = 0b00000100,
    Logic       = 0b00001000,
    Sc          = 0b00010000,
  };

  enum class AlarmBitNumber : quint16
  {
    Bit0  = 0b0000000000000001,
    Bit1  = 0b0000000000000010,
    Bit2  = 0b0000000000000100,
    Bit3  = 0b0000000000001000,
    Bit4  = 0b0000000000010000,
    Bit5  = 0b0000000000100000,
    Bit6  = 0b0000000001000000,
    Bit7  = 0b0000000010000000,
    Bit8  = 0b0000000100000000,
    Bit9  = 0b0000001000000000,
    Bit10 = 0b0000010000000000,
    Bit11 = 0b0000100000000000,
    Bit12 = 0b0001000000000000,
    Bit13 = 0b0010000000000000,
    Bit14 = 0b0100000000000000,
    Bit15 = 0b1000000000000000,
  };

  enum class AlarmStatus : quint8
  {
    Start = 0,
    End   = 1
  };

  struct CardValidationLog
  {
    QTime           time;
    quint16         transactionId;
    MediumType      mediumType;
    quint8          mediumSerialId[8];
    quint8          intrinsicMediumSerialId[8];
    quint16         ticketType;
    quint8          contractId;
    ValidationMode  prevValidationMode;
    QDateTime       prevValidationDate;
    quint8          prevCompanyId;
    quint16         prevStationId;
    QDate           ticketEvd;
    quint8          discountRate;
    quint16         numValidSections;
    quint32         amountDecreased;
    quint8          tripDecreased;
    quint32         remainingAmount;
    quint8          remainingTrips;
    quint32         prevRemainingAmount;
    quint8          samSerialId[10];
    quint8          cwCode[45];
  };

  struct QrValidationLog
  {
    QDate               issuanceDate;
    QByteArray          issuerSerial;
    QDateTime           validationTime;
    quint64             qrCounter;
    QByteArray          qrSerial;
    quint64             readerCounter;
    QByteArray          readerSerial;
    quint8              type;
    quint16             validationStationId;
    quint16             ticketType;
    QDate               ticketEvd;
    quint32             validityDays;
    quint32             amount;
    quint8              remainingTrips;
    quint8              keyNumber;
    QByteArray          mac;
    QByteArray          encryptedData;
  };

  struct ShiftValidationLog
  {
    QDateTime                        date;
    quint8                           equipmentSerial[20];
    ParameterManager::EquipmentType  equipmentType;
    quint8                           equipmentSubType;
    quint8                           equipmentSoftwareRelease;
    quint8                           equipmentSoftwareAmendment;
    ShiftMode                        shiftMode;
    quint16                          stationId;
    quint8                           gateId;
    quint16                          shiftId;
    ConfigurationMode                configuration;
    GateOperatingMode                operatingMode;
    quint16                          operatorId;
  };
  struct ActivityValidationLog
  {
    ActivityMode                     activityMode;
    QDateTime                        date;
    quint8                           companyId;
    quint16                          stationId;
    ParameterManager::EquipmentType  equipmentType;
    quint8                           equipmentSubType;
    quint8                           equipmentSerial[20];
    quint16                          equipmentSoftwareRelease;
    quint16                          equipmentSoftwareAmendment;
  };
  struct AlarmLog
  {
    QTime                                       time;
    ParameterManager::EquipmentType             equipmentType;
    quint8                                      equipmentSubType;
    quint8                                      equipmentSerial[20];
    ParameterManager::EquipmentOperationStatus  equipmentOperationStatus;
    AlarmStatus                                 alarmStatus;
    quint16                                     alarmCode;
  };
  struct RejectedValidationLog
  {
    QTime       time;
    MediumType  mediumType;
    quint16     ticketType;
    quint16     rejectCode;
    quint8      mediumSerialId[8];
    quint8      intrinsicMediumSerialId[8];
    quint8      samSerialId[10];
  };
  struct BlackListLog
  {
    QTime       time;
    MediumType  mediumType;
    quint16     ticketType;
    quint8      mediumSerialId[8];
    quint8      intrinsicMediumSerialId[8];
    quint8      samSerialId[10];
  };

public:
  static QSharedPointer<ActivityManager>  getInstance()
  {
    static const auto &ins = QSharedPointer<ActivityManager>(new ActivityManager());

    return ins;
  }

  void                   openActFile(const ActivityValidationLog &log);

  void                   reopenActFile();

  void                   closeActFile(const QDateTime &date);

  void                   checkFileSize();

  void                   openShift(const ShiftValidationLog &log);

  void                   reopenShift();

  void                   closeShift(const QTime &time, quint32 numPassengerEntries, quint32 numPassengerExit);

  void                   appendBlackList(const BlackListLog &log);

  void                   appendRejectedValidation(const RejectedValidationLog &log);

  void                   appendValidation(ValidationLogType type, const CardValidationLog &log);

  void                   appendValidation(quint8 validationStatusCode, const QrValidationLog &log);

  bool                   isActOpen() const;

  bool                   isShiftOpen() const;

  void                   setStationId(quint16 id);

  void                   setMaxFileSize(quint16 size);

  void                   setValidationType(quint8 type);

  void                   setCompanyService(quint8 cs);

  void                   setAcgType(AccessControlGateType type);

  void                   setAlarm(AlarmGroup group, AlarmBitNumber bit, bool enable);

  void                   setAlarm(AlarmGroup group, quint16 alarm);

  quint16                getAlarm(AlarmGroup group) const;

  quint8                 getAlarmsMask() const;

  void                   setLogicType(quint8 type);

  ConfigurationMode      getConfigurationMode() const;

  quint16                getStationId() const;

  AccessControlGateType  getAcgType() const;

  bool                   getStationStatus() const;

  void                   setStationStatus(bool stationStatus);

  quint32                getActFileCount() const;

  // quint32                getCounterEntry() const;

  // quint32                getCounterExit() const;

  void     setConfigurationMode(const ConfigurationMode &configurationMode);

  quint16  getMaxFileSize() const;

  quint8   getValidationType() const;

  quint8   getCompanyService() const;

  quint8   getLogicType() const;

  bool     getShiftStatus() const;

  // void                   setCounterEntry(const quint32 &counterEntry);

  // void                   setCounterExit(const quint32 &counterExit);

  void          setShiftStatus(bool shiftStatus);

  ActivityMode  getActivityMode() const;

  void          setActivityMode(const ActivityMode &activityMode);

private:
  ActivityManager();

  void     appendAlarm(const AlarmLog &log);

  void     countActFiles() const;

  QString  generateActFileName();

  void     loadSession();

  void     saveSession();

private:
  QFile                      _fileSession;
  QDataStream                _streamSession;
  QFile                      _fileAct;
  QDataStream                _streamAct;
  QDate                      _lastGeneratedIdDate;
  quint16                    _actId;
  quint16                    _stationId;
  bool                       _stationStatus;
  quint16                    _maxFileSize;
  quint8                     _validationType;
  quint8                     _companyService;
  AccessControlGateType      _acgType;
  quint8                     _logicType;
  ConfigurationMode          _configurationMode;
  ActivityMode               _activityMode;
  QMap<AlarmGroup, quint16>  _alarms;
  ActivityValidationLog      _lastAct;
  ShiftValidationLog         _lastShift;
  bool                       _shiftStatus;
  // quint32                    _counterEntry, _counterExit;
  mutable quint32  _actFileCount;
};
}
#endif // ACTIVITYMANAGER_H
