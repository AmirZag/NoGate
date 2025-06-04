#ifndef PARAMETERMANAGER
#define PARAMETERMANAGER

#include <QSharedPointer>
#include <QDateTime>
#include <QPair>
#include <QVector>
#include <QSet>
#include <QMap>
#include <Result.h>


namespace Afc
{
class ParameterManager: public QObject
{
  Q_OBJECT

public:
  enum class EquipmentType : quint8
  {
    Cms    = 1,
    Gcu    = 24,
    Reader = 76,
    Bom    = 122,
    Pr     = 185,
    Sc     = 190
  };

  enum class EquipmentOperationStatus : quint8
  {
    InOrder    = 0,
    OutOfOrder = 1,
  };

  enum class ActionType : quint8
  {
    CheckAndEncodeTicketEvd                                        = 100,
    CheckAndDecreaseTheRemainingJourneys                           = 109,
    CheckAndDecreaseTheAmountOfMoney                               = 112,
    CheckIfTicketIsOnBlackList                                     = 115,
    CheckIfBlackListFlagIsActivated                                = 119,
    CheckTheAntiPassBackTime                                       = 182,
    CheckTimeAndRemainingJourneysDecreaseRemainingJourneysOnExit   = 183,
    CheckTheSectionsAccordingToTheSectionsAllowed                  = 184,
    CheckIfValidationStationIsValid                                = 185,
    CheckTimeAndRemainingValueDecreaseRemainingValueOnExit         = 186,
    CheckAndDecreaseTheAmountOnExit                                = 187,
    CheckLastValidationDecreaseTheRegularisationAmount             = 188,
    CalculateDiscount                                              = 189,
    CheckTheReturnedCardFlag                                       = 190,
    CheckLastValidationDecreaseTheRegularisationAmountPlusFlatFare = 198,
    CheckIfRegularDay                                              = 200,
    CheckIfPublicHoliday                                           = 201,
    CheckIfSchoolHoliday                                           = 202,
  };

  enum class Status
  {
    Normal,
    LoadingParameters,
    DownloadingParameters,
  };

  struct Station
  {
    quint16  id;
    quint16  order;
    QString  name;
  };
  struct ValidationAction
  {
    ActionType  id;
    quint8      failMessageId;
    quint8      successMessageId;
    quint8      notUsed;
    quint8      refusalTableId;
  };
  struct ValidationProperty
  {
    quint16  id;
    quint8   kindOfTicket;
    quint8   parameterTableId;
    quint8   kindOfValidity;
    QDate    startOfValidationAuthorization;
    QDate    endOfValidationAuthorization;
    quint32  fare1;
    quint32  fare2;
    qint16   maxTrips;
    quint8   transferAllowed;
    quint8   transferTableId;
    quint8   actionTableIdforEntry;
    quint8   encodingMaskId;
    quint8   periodTableId;
    quint8   validityTableIdForRegularDays;
    quint8   validityTableIdForSchoolDays;
    quint8   validityTableIdForPublicHolidays;
    quint8   actionTableIdForExit;
    quint8   tableIdForAmountOrTokenUseForRegularDays;
    quint8   tableIdForAmountOrTokenUseForSchoolDays;
    quint8   tableIdForAmountOrTokenUseForPublicHolidays;
    quint8   writeActionIndex;
    quint8   fareIndex;
  };
  struct DiscountSlot
  {
    quint32  start;
    quint32  end;
    quint8   rate;
  };
  struct TicketParameter
  {
    quint16  id;
    quint16  discountRateId;
    quint16  costCategoryId;
    quint32  fineValue;
    quint32  depositValue;
  };
  struct TransferTable
  {
    quint8  id;
    quint8  maxTripDuration;
    quint8  maxMultiUserValidationDuration;
    quint8  passBackDuration;
  };
  struct ValidityDaysTimeSlot
  {
    quint8           dayId;
    quint8           timeSlotTableId;
    quint8           validityBits;
    QVector<quint8>  discountRates;
  };
  struct Section
  {
    quint16  id;
    quint32  fare1;
    quint32  fare2;
  };
  struct FlatFare
  {
    quint16  fare1;
    quint16  fare2;
    quint16  jeton1;
    quint16  jeton2;
  };

public:
  static QSharedPointer<ParameterManager>  getInstance()
  {
    static const auto &ins = QSharedPointer<ParameterManager>(new ParameterManager());

    return ins;
  }

  QString  getCardTypeName(quint16 cardTypeId);

  QString  getWord(quint8 id);


  // Result defineBlacklist();

  Result                                       getStationById(quint16 id, Station *s);

  void                                         load();

  void                                         initiate();

  quint16                                      getParamsVersion() const;

  QMap<quint16, ValidationProperty>            getValidationProperties() const;

  QMap<quint16, QVector<FlatFare>>             getFlatFares() const;

  QMap<quint8, QVector<ValidityDaysTimeSlot>>  getValidityDaysTimes() const;

  Result                                       getTimeSlotAndDiscountRate(quint8 tableId, quint8 *timeSlot,
                                                                          quint8 *discountRate) const;

  QVector<QDate>                               getPublicHolidays() const;

  QVector<QPair<QDate, QDate>>                 getSchoolHolidays() const;

  QMap<quint16, TicketParameter>               getTicketParams() const;

  QMap<quint16, Station>                       getStations() const;

  QVector<quint8>                              getSectionIds() const;

  QMap<quint16, QVector<Section>>              getCostCategories() const;

  QMap<quint16, QVector<ValidationAction>>     getValidationActions() const;

  bool                                         isSchoolHoliday(const QDate &date) const;

  bool                                         isPublicHolidays(const QDate &date) const;

  QSet<quint32>                                getBlacklists() const;

  QMap<quint8, bool>                           getRefusalCodesEncoding() const;

  QMap<quint8, quint16>                        getPeriods() const;

  QDate                                        getFare1Date() const;

  QDate                                        getFare2Date() const;

  Status                                       getStatus() const;

  void                                         setStatus(const Status &status);

  QMap<quint8, TransferTable>                  getTransferTables() const;

  bool                                         isEncodeEnabled(quint8 tableId) const;
private:
  ParameterManager();

  Result  loadALARMES();  // Table of alarm definitions.

  Result  loadBINRAY();

  Result  loadCODREF();  // Table of validation refusal IDs.

  Result  loadDATASCC();  // Table of definition of the Mifare contactless card encoding mask.

  Result  loadDEFSSL();  // Itinerary definition.

  Result  loadDEFSTA();  // Station identification.

  Result  loadDISCOUNT();  // Validation discount rate when selling.

  Result  loadETAMES();  // Reader status messages.

  Result  loadFAMILLE();  // Family file.

  Result  loadILNTMA();  // Magnetic tickets interval black list.

  Result  loadILNTSC();  // Contactless card interval black list.

  Result  loadLNAGT();  // Employee black list.

  Result  loadLNTITM();  // Magnetic tickets black list.

  Result  loadLNTSC();  // Contactless card black list.

  Result  loadMADTICV();

  Result  loadMAPSSC();  // Mifare mapping.

  Result  loadMASQCOD();  // Table of definition of the magnetic encoding mask.

  Result  loadMATCCV();  // Origin/destination section number table (half matrix).

  Result  loadMATPRIX();  // Cost category price matrix.

  Result  loadMESVIE();  // Messages.

  Result  loadPARAGEN();  // General operating parameters.

  Result  loadPERREF();  // Table of period of validity.

  Result  loadTABACT();  // Table of actions.

  Result  loadTABCOR();  // Table of transfer definitions.

  Result  loadTABJF();  // Table of public holidays.

  Result  loadTABTYP();  // Table of tickets allowed on an equipment.

  Result  loadTABVAC();  // Table of school holiday.

  Result  loadTHTIMS();  // Table of time slots.

  Result  loadTIMVAL();  // Table of validation properties for magnetic tickets.

  Result  loadTISVAL();  // Table of validation properties for contactless card.

  Result  loadTISVTE();  // Table of selling properties for contactless card.

  Result  loadTIVARPAR();  // Various ticket parameters.

  Result  loadVALPAR();

  Result  loadVALTIT();  // Table of validity according to time slots.

  Result readParamNameAndVersion(QDataStream &ds, QString *name = nullptr, quint16 *version = nullptr);

private slots:
  void    exitApp();
  void    loadParams();

  void    loadINITIATE();

private:
  Status                                       _status;
  quint16                                      _businessNumber;
  quint16                                      _familleVersion;
  QMap<EquipmentType, QMap<QString, quint16>>  _paramVersion;
  QMap<quint16, Station>                       _stations;
  QMap<quint8, QString>                        _words;
  QMap<quint16, QString>                       _cardTypeNames;
  QMap<quint8, QString>                        _messages;
  QMap<quint8, quint8>                         _statusMessages;
  QMap<quint16, QVector<ValidationAction>>     _validationActions;
  QMap<quint16, ValidationProperty>            _validationProperties;
  QMap<quint16, QVector<DiscountSlot>>         _discounts;
  QMap<quint8, quint16>                        _periods;
  QMap<quint8, bool>                           _refusalCodesEncoding;
  QMap<quint16, TicketParameter>               _ticketParams;
  QMap<quint8, QVector<QPair<QTime, QTime>>>   _timeSlots;
  QMap<quint8, TransferTable>                  _transferTables;
  QMap<quint8, QVector<ValidityDaysTimeSlot>>  _validityDaysTimes;
  QMap<quint16, QVector<Section>>              _costCategories;
  QVector<quint8>                              _sectionIds;
  QMap<quint16, QVector<FlatFare>>             _flatFares;
  QVector<QDate>                               _publicHolidays;
  QVector<QPair<QDate, QDate>>                 _schoolHolidays;
  QSet<quint32>                                _blacklists;
  QTime                                        _paragenTime; // TODO: I don't know what is it!
  QDate                                        _fare1Date, _fare2Date;
};
}

#endif // PARAMETERMANAGER
