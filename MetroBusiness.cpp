#include <MetroBusiness.h>
#include <WindowManager.h>
#include <BarcodeReader.h>
#include <CityPaySam.h>
#include <CityPayCard.h>
#include <CityWayCard.h>
#include <QrTicket.h>
#include <QrDbManager.h>
#include <MifareCard.h>
#include <ActivityManager.h>
#include <BuzzerController.h>
#include <GateController.h>
#include <Scu.h>
#include <Utilities.h>
#include <Settings.h>
#include <QTimer>
#include <QDateTime>
#include <QtEndian>
#include <QDebug>


// #define CLEAR_CITYWAY_EVD
#define SESSION_FILE_NAME  "~session"


namespace Afc
{
constexpr qint32  SEARCH_INTERVAL_MSEC      = 800;
constexpr qint32  SESSION_INTERVAL_MSEC     = 3000;
constexpr qint32  TRANSACTION_INTERVAL_MSEC = 2000;
constexpr qint32  MIFARE_KEY_SIZE           = 6;
constexpr auto    ERR_TRANSACTION           = "لطفا دوباره سعی کنید";
constexpr auto    ERR_INVALID_TICKET        = "بلیت نامعتبر است";
// const QByteArray  KEY_3_B("\xC5\x52\xC1\xB9\x23\x95", MIFARE_KEY_SIZE);
// const QByteArray  KEY_4_B("\xF4\xA4\xAA\x2F\x63\xA4", MIFARE_KEY_SIZE);
// const QByteArray  KEY_5_B("\x25\xEC\xB7\xB2\xBA\xB1", MIFARE_KEY_SIZE);

MetroBusiness::MetroBusiness(WindowManager                       *windowMgr,
                             const QSharedPointer<CardReader>    &cardReader,
                             const QSharedPointer<BarcodeReader> &barcodeReader,
                             const QSharedPointer<CityPaySam>    &citypaySam):
  _windowManager(windowMgr), _cardReader(cardReader), _barcodeReader(barcodeReader), _citypaySam(citypaySam),
  _lastQrDetectionTime(0), _transactionId(1)
{
  _timerSearchCard = QSharedPointer<QTimer>(new QTimer());
  _timerSearchCard->setInterval(SEARCH_INTERVAL_MSEC);
  connect(_timerSearchCard.data(), SIGNAL(timeout()), this, SLOT(onTimerTimeout()));

  _timerSession = QSharedPointer<QTimer>(new QTimer());
  _timerSession->setInterval(SESSION_INTERVAL_MSEC);
  connect(_timerSession.data(), SIGNAL(timeout()), this, SLOT(onTimerSessionTimeout()));
}

MetroBusiness::~MetroBusiness()
{
  _timerSearchCard->stop();
  _timerSession->stop();
}

Result  MetroBusiness::transactWithCard(const QSharedPointer<CityPayCard> &card)
{
  Result      res(false, ERR_INVALID_TICKET);
  const auto &PM = ParameterManager::getInstance();

  if (PM->getValidationProperties().contains(Settings::TICKET_TYPE_CITYPAY))
  {
    auto                                         validationProps = PM->getValidationProperties()[3];
    QVector<ParameterManager::ValidationAction>  actionsList;

    getValidationActions(validationProps, &actionsList);

    {
      const auto &PM = ParameterManager::getInstance();

      auto     tableIdForToken    = validationProps.tableIdForAmountOrTokenUseForRegularDays;
      //auto     tableIdForValidity = validationProps.validityTableIdForRegularDays;
      quint32  amountToDecrease   = 0;

      if (PM->getFlatFares().contains(tableIdForToken))
      {
        const auto  flatFares = PM->getFlatFares()[tableIdForToken];

        //quint8  timeSlotNumber, discountRate;

        // if (PM->getTimeSlotAndDiscountRate(tableIdForValidity, &timeSlotNumber, &discountRate))
        {
          const auto &now = QDateTime::currentDateTimeUtc();

          // if (timeSlotNumber < flatFares.size())
          {
            amountToDecrease = flatFares[0].fare1;

            // amountToDecrease = PM->getFare2Date() < now.date()
            // ? flatFares[timeSlotNumber].fare2 : flatFares[timeSlotNumber].fare1;

            // amountToDecrease -= (amountToDecrease * discountRate) / 100;

            QByteArray  cardSerial(8, 0);
            QByteArray  cardPan(8, 0);
            quint32     cardVersion;
            quint32     newBalance = 0;
            QByteArray  cwCode;

            if (res = purchaseCityPay(card->getId(), amountToDecrease, &cardSerial, &cardPan, &cardVersion, &newBalance, &cwCode))
            {
              ActivityManager::CardValidationLog  log;
              memset(log.samSerialId, 0, sizeof(log.samSerialId));

              log.time          = now.time();
              log.transactionId = _transactionId;
              log.mediumType = 400 <= cardVersion ? ActivityManager::MediumType::Ja4 : ActivityManager::MediumType::Ja3;
              memcpy(log.mediumSerialId, cardSerial.constData(), cardSerial.size());
              memcpy(log.intrinsicMediumSerialId, cardPan.constData(), cardPan.size());
              log.ticketType          = Settings::TICKET_TYPE_CITYPAY;
              log.contractId          = Settings::CONTRACT_ID;
              log.prevValidationMode  = ActivityManager::ValidationMode::ValidateOnEntry;
              log.prevValidationDate  = QDateTime(QDate(1, 1, 1), QTime());
              log.prevCompanyId       = Settings::COMPANY_ID;
              log.prevStationId       = 0;
              log.ticketEvd           = QDate(2099, 1, 1);
              log.discountRate        = 0;
              log.numValidSections    = 0;
              log.amountDecreased     = amountToDecrease;
              log.tripDecreased       = 0;
              log.remainingAmount     = newBalance;
              log.remainingTrips      = 0;
              log.prevRemainingAmount = newBalance + amountToDecrease;
              memcpy(log.samSerialId + 2, _citypaySam->getSamSerial().constData(), _citypaySam->getSamSerial().size());
              memcpy(log.cwCode, cwCode.constData(), cwCode.size());

              ActivityManager::ValidationLogType  validationType;

              if (ActivityManager::AccessControlGateType::Exit == ActivityManager::getInstance()->getAcgType())
              {
                validationType = ActivityManager::ValidationLogType::Exit;
              }
              else
              {
                validationType = ActivityManager::ValidationLogType::Entry;
              }

              ActivityManager::getInstance()->appendValidation(validationType, log);

              ++_transactionId;
            }
            else
            {
              ActivityManager::RejectedValidationLog  rej;
              memset(rej.samSerialId, 0, sizeof(rej.samSerialId));

              rej.time       = QDateTime::currentDateTimeUtc().time();
              rej.mediumType = 400 <= cardVersion ? ActivityManager::MediumType::Ja4 : ActivityManager::MediumType::Ja3;
              rej.ticketType = Settings::TICKET_TYPE_CITYPAY;
              rej.rejectCode = 0;
              memcpy(rej.mediumSerialId, cardSerial.constData(), cardSerial.size());
              memcpy(rej.intrinsicMediumSerialId, cardPan.constData(), cardPan.size());
              memcpy(rej.samSerialId + 2, _citypaySam->getSamSerial().constData(), _citypaySam->getSamSerial().size());

              ActivityManager::getInstance()->appendRejectedValidation(rej);
            }
          }
        }
      }
    }
  }

  return res;
}

Result  MetroBusiness::transactWithCard(const QSharedPointer<CityWayCard> &card)
{
  Result                    res;
  CardReader::ApduResponse  response;
  const auto               &now = QDateTime::currentDateTimeUtc();
  const auto               &AM  = ActivityManager::getInstance();
  const auto               &PM  = ParameterManager::getInstance();

  res.message = ERR_TRANSACTION;

  if (_citypaySam->citywayInitForOperation(card->getId(), &response))
  {
    //const auto &keyA0 = response.data;

    if (_citypaySam->citywayGetSectorKey(CardReader::BlockAuthMode::B, 11, &response))
    {
      const auto                    &keyB44 = response.data;
      const auto                    &keyB45 = response.data;
      const auto                    &keyB46 = response.data;
      CardReader::MifareMemoryBlock  block44, block45, block46;

      if (_cardReader->mifareAuthBlock(CardReader::BlockAuthMode::B, keyB44, 44) && _cardReader->mifareReadBlock(44, &block44))
      {
        // Block 44.
        //        const quint8  &mask              = block44.readBits<0, 4>().to_ulong();
                const quint8  &ticketType        = block44.readBits<4, 8>().to_ulong();
        //        const quint32 &serialNo          = block44.readBits<16, 32>().to_ulong();
        //        const quint8  &equipmentType     = block44.readBits<48, 8>().to_ulong();
        //        const quint8  &equipmentSubType  = block44.readBits<56, 8>().to_ulong();
        //        const quint16 &equipmentSerialId = block44.readBits<64, 16>().to_ulong();

        QVector<ParameterManager::ValidationAction>  actionsList;

        if (PM->getValidationProperties().contains(ticketType))
        {
          auto  validationProps = PM->getValidationProperties()[ticketType];

          if (getValidationActions(validationProps, &actionsList))
          {

            if (_cardReader->mifareAuthBlock(CardReader::BlockAuthMode::B, keyB45, 45) && _cardReader->mifareReadBlock(45, &block45)
                && _cardReader->mifareAuthBlock(CardReader::BlockAuthMode::B, keyB46, 46) && _cardReader->mifareReadBlock(46, &block46))
            {
              CityWayCard::MemoryMapping  newCardData, oldCardData;
              quint8                      oldBlockNumber, nextBlockNumber;
              bool                        forceWriteOldBlock = false;

              // Check sequence id.
              //const auto seq45  = block45.readBits<24, 8>().to_ulong();
              //const auto seq46  = block46.readBits<24, 8>().to_ulong();

              if (block45.readBits<24, 8>().to_ulong() < block46.readBits<24, 8>().to_ulong())
              {
                // Block 46 is newer.
                oldCardData.endingValidityYear  = block46.readBits<0, 7>().to_ulong();
                oldCardData.endingValidityMonth = block46.readBits<7, 4>().to_ulong();
                oldCardData.endingValidityDay   = block46.readBits<11, 5>().to_ulong();
                oldCardData.blacklisted         = block46.readBits<16, 1>().to_ulong();
                oldCardData.companyId           = block46.readBits<20, 4>().to_ulong();
                oldCardData.sequenceID          = block46.readBits<24, 8>().to_ulong();
                oldCardData.totalAmount         = block46.readBits<32, 32>().to_ulong();
                oldCardData.remainingTravels    = block46.readBits<64, 8>().to_ulong();
                oldCardData.validationIndicator = block46.readBits<72, 2>().to_ulong();
                oldCardData.validationStation   = block46.readBits<74, 10>().to_ulong();
                oldCardData.validationYear      = block46.readBits<84, 7>().to_ulong();
                oldCardData.validationMonth     = block46.readBits<91, 4>().to_ulong();
                oldCardData.validationDay       = block46.readBits<95, 5>().to_ulong();
                oldCardData.validationHour      = block46.readBits<100, 5>().to_ulong();
                oldCardData.validationMinute    = block46.readBits<105, 6>().to_ulong();
                oldCardData.contractId          = block46.readBits<112, 8>().to_ulong();
                oldCardData.rejectCode          = block46.readBits<120, 8>().to_ulong();

                nextBlockNumber = 45;
                oldBlockNumber  = 46;
              }
              else
              {
                // Block 45 is newer.
                oldCardData.endingValidityYear  = block45.readBits<0, 7>().to_ulong();
                oldCardData.endingValidityMonth = block45.readBits<7, 4>().to_ulong();
                oldCardData.endingValidityDay   = block45.readBits<11, 5>().to_ulong();
                oldCardData.blacklisted         = block45.readBits<16, 1>().to_ulong();
                oldCardData.companyId           = block45.readBits<20, 4>().to_ulong();
                oldCardData.sequenceID          = block45.readBits<24, 8>().to_ulong();
                oldCardData.totalAmount         = block45.readBits<32, 32>().to_ulong();
                oldCardData.remainingTravels    = block45.readBits<64, 8>().to_ulong();
                oldCardData.validationIndicator = block45.readBits<72, 2>().to_ulong();
                oldCardData.validationStation   = block45.readBits<74, 10>().to_ulong();
                oldCardData.validationYear      = block45.readBits<84, 7>().to_ulong();
                oldCardData.validationMonth     = block45.readBits<91, 4>().to_ulong();
                oldCardData.validationDay       = block45.readBits<95, 5>().to_ulong();
                oldCardData.validationHour      = block45.readBits<100, 5>().to_ulong();
                oldCardData.validationMinute    = block45.readBits<105, 6>().to_ulong();
                oldCardData.contractId          = block45.readBits<112, 8>().to_ulong();
                oldCardData.rejectCode          = block45.readBits<120, 8>().to_ulong();

                nextBlockNumber = 46;
                oldBlockNumber  = 45;
              }

              newCardData = oldCardData;

              quint8                                  tableIdForValidity = validationProps.validityTableIdForRegularDays;
              quint8                                  tableIdForToken    = validationProps.tableIdForAmountOrTokenUseForRegularDays;
              quint8                                  timeSlotNumber     = 0;
              quint8                                  discountRate       = 0;
              ActivityManager::CardValidationLog      log;
              ActivityManager::RejectedValidationLog  rej;
              bool                                    blacklisted       = false;
              bool                                    forceWriteToCard  = false;
              bool                                    encodeRejectCode  = false;
              quint32                                 penalty           = 0;

              if (PM->getTicketParams().contains(validationProps.parameterTableId))
              {
                penalty = PM->getTicketParams()[validationProps.parameterTableId].fineValue;
              }

              memset(log.mediumSerialId, 0, sizeof(log.mediumSerialId));
              memset(log.intrinsicMediumSerialId, 0, sizeof(log.intrinsicMediumSerialId));
              memset(log.samSerialId, 0, sizeof(log.samSerialId));
              memset(log.cwCode, 0, sizeof(log.cwCode));
              log.amountDecreased = 0;

              memset(rej.mediumSerialId, 0, sizeof(rej.mediumSerialId));
              memset(rej.intrinsicMediumSerialId, 0, sizeof(rej.intrinsicMediumSerialId));
              memset(rej.samSerialId, 0, sizeof(rej.samSerialId));
              rej.rejectCode = 0;

              for (qint32 i = 0; validationProps.transferAllowed && i < actionsList.size(); ++i)
              {
                using AC = ParameterManager::ActionType;
                const ParameterManager::ValidationAction &action = actionsList[i];

                encodeRejectCode = PM->isEncodeEnabled(action.refusalTableId);
                forceWriteToCard = encodeRejectCode;

                switch (action.id)
                {
                case AC::CheckAndEncodeTicketEvd:
                {
                  if ((0 == newCardData.endingValidityYear) && (0 == newCardData.endingValidityMonth)
                      && (0 == newCardData.endingValidityDay))
                  {
                    auto  evd = now.date();

                    if (PM->getPeriods().contains(validationProps.periodTableId))
                    {
                      evd = evd.addDays(PM->getPeriods()[validationProps.periodTableId]);
                    }

                    // if (encodeRejectCode
                    {
                      newCardData.endingValidityYear  = evd.year() - 2000;
                      newCardData.endingValidityMonth = evd.month();
                      newCardData.endingValidityDay   = evd.day();
                    }

                    res.succeed = true;
                    res.message = "اعتبار " + Utilities::miladiToShamsi(evd).toString("yyyy/MM/dd");
                  }
                  else
                  {
                    const auto &evd = QDate(2000 + newCardData.endingValidityYear, newCardData.endingValidityMonth,
                                            newCardData.endingValidityDay);

                    if (now.date() <= evd)
                    {
#ifdef CLEAR_CITYWAY_EVD
                      newCardData.endingValidityYear  = 0;
                      newCardData.endingValidityMonth = 0;
                      newCardData.endingValidityDay   = 0;
#endif

                      res.succeed = true;
                      res.message = "اعتبار " + Utilities::miladiToShamsi(evd).toString("yyyy/MM/dd");
                    }
                    else
                    {
                      validationProps.transferAllowed = false;
                      rej.rejectCode                  = 0;// action.refusalTableId;
                      res.message                     = "اعتبار پایان یافته است.";
                    }
                  }
                }
                break;
                case AC::CheckAndDecreaseTheRemainingJourneys:
                {
                }
                break;
                case AC::CheckAndDecreaseTheAmountOfMoney:
                {
                  quint32  amountToDecrease = 0;

                  if (PM->getFlatFares().contains(tableIdForToken))
                  {
                    const auto  flatFares = PM->getFlatFares()[tableIdForToken];

                    //if (PM->getTimeSlotAndDiscountRate(tableIdForValidity, &timeSlotNumber, &discountRate))
                    {
                      if (timeSlotNumber < flatFares.size())
                      {
                        amountToDecrease = PM->getFare2Date() < now.date()
                                           ? flatFares[timeSlotNumber].fare2 : flatFares[timeSlotNumber].fare1;
                        amountToDecrease -= (amountToDecrease * discountRate) / 100;

                        QByteArray  cwCode;
                        qDebug() << "discount::::::::::::" << discountRate;
                        res = purchaseCityWay(card->getId(), amountToDecrease, amountToDecrease, &newCardData.totalAmount, &cwCode);

                        if (res.succeed)
                        {
                          log.amountDecreased = amountToDecrease;
                          memcpy(log.cwCode + 12, cwCode.constData(), cwCode.size());
                        }
                        else
                        {
                          validationProps.transferAllowed = false;
                          rej.rejectCode                  = 0;// action.refusalTableId;
                        }
                      }
                    }
                  }
                }
                break;
                case AC::CheckIfTicketIsOnBlackList:
                {
                  if (PM->getBlacklists().contains(reinterpret_cast<const quint32 &>(*card->getId().constData())))
                  {
                    validationProps.transferAllowed = false;

                    if (PM->isEncodeEnabled(action.refusalTableId))
                    {
                      forceWriteToCard        = true;
                      newCardData.blacklisted = true;
                    }

                    blacklisted    = true;
                    rej.rejectCode = 0;// action.refusalTableId;
                    res.message    = "کارت مسدود است.";
                  }
                }
                break;
                case AC::CheckIfBlackListFlagIsActivated:
                {
                  if (oldCardData.blacklisted)
                  {
                    validationProps.transferAllowed = false;
                    newCardData.blacklisted         = true;
                    blacklisted                     = true;
                    rej.rejectCode                  = 0;// action.refusalTableId;
                    res.message                     = "کارت مسدود است.";
                  }
                }
                break;
                case AC::CheckTheAntiPassBackTime:
                {
                  if ((quint8(ActivityManager::ValidationMode::ValidateOnEntry) == oldCardData.validationIndicator
                       && ActivityManager::AccessControlGateType::Entry == AM->getAcgType())
                      || (quint8(ActivityManager::ValidationMode::ValidateOnExit) == oldCardData.validationIndicator
                          && ActivityManager::AccessControlGateType::Exit == AM->getAcgType()))
                  {
                    if (PM->getTransferTables().contains(validationProps.transferTableId))
                    {
                      auto  lastCardTime =
                          QDateTime(QDate(2000 + oldCardData.validationYear, oldCardData.validationMonth, oldCardData.validationDay),
                                    QTime(oldCardData.validationHour, oldCardData.validationMinute));
                      auto  passBackTime =
                          lastCardTime.addSecs(60 * PM->getTransferTables()[validationProps.transferTableId].passBackDuration);

                      if ((lastCardTime <= now) && (now <= passBackTime))
                      {
                        validationProps.transferAllowed = false;
                        rej.rejectCode                  = 0;// action.refusalTableId;
                        res.message                     = "بلیت قبلا استفاده شده است.";
                      }
                    }
                  }
                }
                break;
                case AC::CheckTimeAndRemainingJourneysDecreaseRemainingJourneysOnExit:
                { }
                break;
                case AC::CheckTheSectionsAccordingToTheSectionsAllowed:
                { }
                break;
                case AC::CheckIfValidationStationIsValid:
                { }
                break;
                case AC::CheckTimeAndRemainingValueDecreaseRemainingValueOnExit:
                { }
                break;
                case AC::CheckAndDecreaseTheAmountOnExit:
                { }
                break;
                case AC::CheckLastValidationDecreaseTheRegularisationAmount:
                {
                  bool    paramIsValid      = false;
                  bool    doPurchase        = true;
                  quint32 amountToDecrease  = 0;

                  if (PM->getTicketParams().contains(validationProps.parameterTableId))
                  {
                    if (((quint8(ActivityManager::ValidationMode::ValidateOnEntry) == oldCardData.validationIndicator)
                         && (ActivityManager::AccessControlGateType::Exit != AM->getAcgType()))
                        || ((quint8(ActivityManager::ValidationMode::ValidateOnExit) == oldCardData.validationIndicator)
                            && (ActivityManager::AccessControlGateType::Exit == AM->getAcgType()))
                        || ((quint8(ActivityManager::ValidationMode::ValidateOnEntry) == oldCardData.validationIndicator)
                            && (ActivityManager::AccessControlGateType::Exit == AM->getAcgType())
                            && (now.date() > QDate(2000 + oldCardData.validationYear, oldCardData.validationMonth, oldCardData.validationDay))))
                    {
                      paramIsValid      = true;
                      amountToDecrease  = penalty;
                    }
                    else
                    {
                      if (ActivityManager::AccessControlGateType::Exit == AM->getAcgType())
                      {
                        const auto  stations = PM->getStations();

                        if (stations.contains(oldCardData.validationStation)
                            && stations.contains(AM->getStationId()))
                        {
                          auto  lastStation = stations[oldCardData.validationStation];
                          auto  thisStation = stations[AM->getStationId()];

                          if (lastStation.order > thisStation.order)
                          {
                            auto  tempStation = lastStation;
                            lastStation = thisStation;
                            thisStation = tempStation;
                          }

                          qint32  sectionNumber = stations.size() * (stations.size() + 1) / 2;
                          sectionNumber -= (stations.size() - lastStation.order) * (stations.size() - lastStation.order + 1) / 2
                                           + lastStation.order - thisStation.order;

                          if (sectionNumber < PM->getSectionIds().size())
                          {
                            const auto  sectionId = PM->getSectionIds()[sectionNumber];
                            const auto  costCategoryId = PM->getTicketParams()[validationProps.parameterTableId].costCategoryId;

                            if (PM->getCostCategories().contains(costCategoryId))
                            {
                              const auto  sections = PM->getCostCategories()[costCategoryId];

                              for (const ParameterManager::Section s : sections)
                              {
                                if (s.id == sectionId)
                                {
                                  paramIsValid = true;
                                  amountToDecrease = PM->getFare2Date() < now.date() ? s.fare2 : s.fare1;
                                  amountToDecrease -= (amountToDecrease * discountRate) / 100;

                                  break;
                                }
                              }
                            }
                          }
                        }
                      }
                      else
                      {
                        paramIsValid = true;
                        doPurchase   = false;
                      }
                    }
                  }

                  if (paramIsValid)
                  {
                    if (doPurchase)
                    {
                      QByteArray  cwCode;
                      qDebug() << "discount::::::::::::" << discountRate;
                      res = purchaseCityWay(card->getId(), amountToDecrease, amountToDecrease, &newCardData.totalAmount, &cwCode);

                      if (res.succeed)
                      {
                        log.amountDecreased = amountToDecrease;
                        memcpy(log.cwCode + 12, cwCode.constData(), cwCode.size());
                      }
                      else
                      {
                        validationProps.transferAllowed = false;
                        rej.rejectCode                  = 0;// action.refusalTableId;
                      }
                    }
                    else
                    {
                      log.amountDecreased = amountToDecrease;
                      res.succeed         = true;
                      res.message         = Utilities::prettyPrice(newCardData.totalAmount);
                    }
                  }
                  else
                  {
                    validationProps.transferAllowed = false;
                    rej.rejectCode                  = 0;// action.refusalTableId;
                    res.message                    += " P";
                  }
                }
                break;
                case AC::CheckLastValidationDecreaseTheRegularisationAmountPlusFlatFare:
                {
                  bool    paramIsValid      = false;
                  quint32 amountToDecrease  = 0;

                  // Compute flat fare.
                  {
                    if (PM->getFlatFares().contains(tableIdForToken))
                    {
                      const auto  flatFares = PM->getFlatFares()[tableIdForToken];

                      //if (PM->getTimeSlotAndDiscountRate(tableIdForValidity, &timeSlotNumber, &discountRate))
                      {
                        if (timeSlotNumber < flatFares.size())
                        {
                          amountToDecrease = PM->getFare2Date() < now.date()
                                             ? flatFares[timeSlotNumber].fare2 : flatFares[timeSlotNumber].fare1;

                          amountToDecrease -= (amountToDecrease * discountRate) / 100;
                          paramIsValid      = true;
                        }
                      }
                    }
                  }

                  if (PM->getTicketParams().contains(validationProps.parameterTableId))
                  {
                    if (((quint8(ActivityManager::ValidationMode::ValidateOnEntry) == oldCardData.validationIndicator)
                         && (ActivityManager::AccessControlGateType::Exit != AM->getAcgType()))
                        || ((quint8(ActivityManager::ValidationMode::ValidateOnExit) == oldCardData.validationIndicator)
                            && (ActivityManager::AccessControlGateType::Exit == AM->getAcgType())))
                    {
                      amountToDecrease += penalty;
                      paramIsValid      = true;

                    }
                    else
                    {

                      if (ActivityManager::AccessControlGateType::Exit == AM->getAcgType())
                      {
                        const auto  stations = PM->getStations();

                        if (stations.contains(oldCardData.validationStation)
                            && stations.contains(AM->getStationId()))
                        {
                          auto  lastStation = stations[oldCardData.validationStation];
                          auto  thisStation = stations[AM->getStationId()];

                          if (lastStation.order > thisStation.order)
                          {
                            auto  tempStation = lastStation;
                            lastStation = thisStation;
                            thisStation = tempStation;
                          }

                          qint32  sectionNumber = stations.size() * (stations.size() + 1) / 2;
                          sectionNumber -= (stations.size() - lastStation.order) * (stations.size() - lastStation.order + 1) / 2
                                           + lastStation.order - thisStation.order;

                          if (sectionNumber < PM->getSectionIds().size())
                          {
                            const auto  sectionId = PM->getSectionIds()[sectionNumber];

                            const auto  costCategoryId = PM->getTicketParams()[validationProps.parameterTableId].costCategoryId;

                            if (PM->getCostCategories().contains(costCategoryId))
                            {
                              const auto  sections = PM->getCostCategories()[costCategoryId];

                              for (const ParameterManager::Section &s : sections)
                              {
                                if (s.id == sectionId)
                                {
                                  amountToDecrease  = PM->getFare2Date() < now.date() ? s.fare2 : s.fare1;
                                  amountToDecrease -= (amountToDecrease * discountRate) / 100;
                                  paramIsValid      = true;

                                  break;
                                }
                              }
                            }
                          }
                        }
                      }
                    }
                  }

                  if (paramIsValid)
                  {
                    QByteArray  cwCode;
                    qDebug() << "discount::::::::::::" << discountRate;
                    res = purchaseCityWay(card->getId(), amountToDecrease, amountToDecrease + penalty, &newCardData.totalAmount, &cwCode);

                    if (res.succeed)
                    {
                      log.amountDecreased = amountToDecrease;
                      memcpy(log.cwCode + 12, cwCode.constData(), cwCode.size());
                    }
                    else
                    {
                      validationProps.transferAllowed = false;
                      rej.rejectCode                  = 0;// action.refusalTableId;
                    }
                  }
                  else
                  {
                    validationProps.transferAllowed = false;
                    rej.rejectCode                  = 0;// action.refusalTableId;
                    res.message                    += " P";
                  }
                }
                break;
                case AC::CalculateDiscount:
                {
                  if (PM->getTimeSlotAndDiscountRate(tableIdForValidity, &timeSlotNumber, &discountRate))
                  {
                    // TODO
                  }
                }
                break;
                case AC::CheckTheReturnedCardFlag:
                {
                  // if (0 < oldCardData.rejectCode)
                  // {
                  // validationProps.transferAllowed = false;
                  // rej.rejectCode                  = action.refusalTableId;
                  // res.message                     = "کارت مسدود است.";
                  // }
                }
                break;
                case AC::CheckIfRegularDay:
                {
                  // if (!PM->isPublicHolidays(now.date()) && !PM->isSchoolHoliday(now.date()))
                  {
                    tableIdForValidity = validationProps.validityTableIdForRegularDays;
                    tableIdForToken    = validationProps.tableIdForAmountOrTokenUseForRegularDays;
                  }
                  // else
                  {
                    // refusalTableId = action.refusalTableId;
                  }
                }
                break;
                case AC::CheckIfPublicHoliday:
                {
                  if (PM->isPublicHolidays(now.date()))
                  {
                    tableIdForValidity = validationProps.validityTableIdForPublicHolidays;
                    tableIdForToken    = validationProps.tableIdForAmountOrTokenUseForPublicHolidays;
                  }
                  else
                  {
                    validationProps.transferAllowed = false;
                    rej.rejectCode                  = 0;// action.refusalTableId;
                  }
                }
                break;
                case AC::CheckIfSchoolHoliday:
                {
                  if (PM->isSchoolHoliday(now.date()))
                  {
                    tableIdForValidity = validationProps.validityTableIdForSchoolDays;
                    tableIdForToken    = validationProps.tableIdForAmountOrTokenUseForSchoolDays;
                  }
                  else
                  {
                    validationProps.transferAllowed = false;
                    rej.rejectCode                  = 0;// action.refusalTableId;
                  }
                }
                break;
                }
              }

              if (validationProps.transferAllowed || forceWriteToCard)
              {
                newCardData.companyId  = Settings::COMPANY_ID;
                newCardData.contractId = Settings::CONTRACT_ID;

                if (encodeRejectCode)
                {
                  newCardData.rejectCode = 0;// rej.rejectCode;
                }

                newCardData.remainingTravels = 0;
                ++newCardData.sequenceID;

                if (0 == newCardData.sequenceID)
                {
                  newCardData.sequenceID = 1;
                  oldCardData.sequenceID = 0;
                  forceWriteOldBlock = true;
                }

                newCardData.validationYear      = now.date().year() - 2000;
                newCardData.validationMonth     = now.date().month();
                newCardData.validationDay       = now.date().day();
                newCardData.validationHour      = now.time().hour();
                newCardData.validationMinute    = now.time().minute();
                newCardData.validationStation   = AM->getStationId();
                newCardData.validationIndicator = (ActivityManager::AccessControlGateType::Exit == AM->getAcgType())
                                                  ? quint8(ActivityManager::ValidationMode::ValidateOnExit)
                                                  : quint8(ActivityManager::ValidationMode::ValidateOnEntry);

                {
                  CardReader::MifareMemoryBlock  blk;
                  blk.writeBits<0, 7>(newCardData.endingValidityYear);
                  blk.writeBits<7, 4>(newCardData.endingValidityMonth);
                  blk.writeBits<11, 5>(newCardData.endingValidityDay);
                  blk.writeBits<16, 1>(newCardData.blacklisted);
                  blk.writeBits<20, 4>(newCardData.companyId);
                  blk.writeBits<24, 8>(newCardData.sequenceID);
                  blk.writeBits<32, 32>(newCardData.totalAmount);
                  blk.writeBits<64, 8>(newCardData.remainingTravels);
                  blk.writeBits<72, 2>(newCardData.validationIndicator);
                  blk.writeBits<74, 10>(newCardData.validationStation);
                  blk.writeBits<84, 7>(newCardData.validationYear);
                  blk.writeBits<91, 4>(newCardData.validationMonth);
                  blk.writeBits<95, 5>(newCardData.validationDay);
                  blk.writeBits<100, 5>(newCardData.validationHour);
                  blk.writeBits<105, 6>(newCardData.validationMinute);
                  blk.writeBits<112, 8>(newCardData.contractId);
                  blk.writeBits<120, 8>(newCardData.rejectCode);

                  _citypaySam->citywayGetSectorKey(CardReader::BlockAuthMode::B, 11, &response);

                  if (_cardReader->mifareAuthBlock(CardReader::BlockAuthMode::B, response.data, nextBlockNumber)
                      && _cardReader->mifareWriteBlock(nextBlockNumber, blk))
                  {
                    qDebug() << "OLD";
                    qDebug() << "endingValidityYear " << oldCardData.endingValidityYear;
                    qDebug() << "endingValidityMonth" << oldCardData.endingValidityMonth;
                    qDebug() << "endingValidityDay  " << oldCardData.endingValidityDay;
                    qDebug() << "blacklisted        " << oldCardData.blacklisted;
                    qDebug() << "companyId          " << oldCardData.companyId;
                    qDebug() << "sequenceID         " << oldCardData.sequenceID;
                    qDebug() << "totalAmount        " << oldCardData.totalAmount;
                    qDebug() << "remainingTravels   " << oldCardData.remainingTravels;
                    qDebug() << "validationIndicator" << oldCardData.validationIndicator;
                    qDebug() << "validationStation  " << oldCardData.validationStation;
                    qDebug() << "validationYear     " << oldCardData.validationYear;
                    qDebug() << "validationMonth    " << oldCardData.validationMonth;
                    qDebug() << "validationDay      " << oldCardData.validationDay;
                    qDebug() << "validationHour     " << oldCardData.validationHour;
                    qDebug() << "validationMinute   " << oldCardData.validationMinute;
                    qDebug() << "contractId         " << oldCardData.contractId;
                    qDebug() << "rejectCode         " << oldCardData.rejectCode;
                    qDebug() << "NEW";
                    qDebug() << "endingValidityYear " << newCardData.endingValidityYear;
                    qDebug() << "endingValidityMonth" << newCardData.endingValidityMonth;
                    qDebug() << "endingValidityDay  " << newCardData.endingValidityDay;
                    qDebug() << "blacklisted        " << newCardData.blacklisted;
                    qDebug() << "companyId          " << newCardData.companyId;
                    qDebug() << "sequenceID         " << newCardData.sequenceID;
                    qDebug() << "totalAmount        " << newCardData.totalAmount;
                    qDebug() << "remainingTravels   " << newCardData.remainingTravels;
                    qDebug() << "validationIndicator" << newCardData.validationIndicator;
                    qDebug() << "validationStation  " << newCardData.validationStation;
                    qDebug() << "validationYear     " << newCardData.validationYear;
                    qDebug() << "validationMonth    " << newCardData.validationMonth;
                    qDebug() << "validationDay      " << newCardData.validationDay;
                    qDebug() << "validationHour     " << newCardData.validationHour;
                    qDebug() << "validationMinute   " << newCardData.validationMinute;
                    qDebug() << "contractId         " << newCardData.contractId;
                    qDebug() << "rejectCode         " << newCardData.rejectCode;
                  }
                  else
                  {
                    qDebug() << "Error writing data to card.";
                  }
                }

                if (true == forceWriteOldBlock)
                {
                  CardReader::MifareMemoryBlock  blk;
                  blk.writeBits<0, 7>(oldCardData.endingValidityYear);
                  blk.writeBits<7, 4>(oldCardData.endingValidityMonth);
                  blk.writeBits<11, 5>(oldCardData.endingValidityDay);
                  blk.writeBits<16, 1>(oldCardData.blacklisted);
                  blk.writeBits<20, 4>(oldCardData.companyId);
                  blk.writeBits<24, 8>(oldCardData.sequenceID);
                  blk.writeBits<32, 32>(oldCardData.totalAmount);
                  blk.writeBits<64, 8>(oldCardData.remainingTravels);
                  blk.writeBits<72, 2>(oldCardData.validationIndicator);
                  blk.writeBits<74, 10>(oldCardData.validationStation);
                  blk.writeBits<84, 7>(oldCardData.validationYear);
                  blk.writeBits<91, 4>(oldCardData.validationMonth);
                  blk.writeBits<95, 5>(oldCardData.validationDay);
                  blk.writeBits<100, 5>(oldCardData.validationHour);
                  blk.writeBits<105, 6>(oldCardData.validationMinute);
                  blk.writeBits<112, 8>(oldCardData.contractId);
                  blk.writeBits<120, 8>(oldCardData.rejectCode);
                  _citypaySam->citywayGetSectorKey(CardReader::BlockAuthMode::B, 11, &response);

                  if (_cardReader->mifareAuthBlock(CardReader::BlockAuthMode::B, response.data, oldBlockNumber)
                      && _cardReader->mifareWriteBlock(oldBlockNumber, blk))
                  {
                    qDebug() << "OLD";
                    qDebug() << "endingValidityYear " << oldCardData.endingValidityYear;
                    qDebug() << "endingValidityMonth" << oldCardData.endingValidityMonth;
                    qDebug() << "endingValidityDay  " << oldCardData.endingValidityDay;
                    qDebug() << "blacklisted        " << oldCardData.blacklisted;
                    qDebug() << "companyId          " << oldCardData.companyId;
                    qDebug() << "sequenceID         " << oldCardData.sequenceID;
                    qDebug() << "totalAmount        " << oldCardData.totalAmount;
                    qDebug() << "remainingTravels   " << oldCardData.remainingTravels;
                    qDebug() << "validationIndicator" << oldCardData.validationIndicator;
                    qDebug() << "validationStation  " << oldCardData.validationStation;
                    qDebug() << "validationYear     " << oldCardData.validationYear;
                    qDebug() << "validationMonth    " << oldCardData.validationMonth;
                    qDebug() << "validationDay      " << oldCardData.validationDay;
                    qDebug() << "validationHour     " << oldCardData.validationHour;
                    qDebug() << "validationMinute   " << oldCardData.validationMinute;
                    qDebug() << "contractId         " << oldCardData.contractId;
                    qDebug() << "rejectCode         " << oldCardData.rejectCode;
                    qDebug() << "NEW";
                    qDebug() << "endingValidityYear " << newCardData.endingValidityYear;
                    qDebug() << "endingValidityMonth" << newCardData.endingValidityMonth;
                    qDebug() << "endingValidityDay  " << newCardData.endingValidityDay;
                    qDebug() << "blacklisted        " << newCardData.blacklisted;
                    qDebug() << "companyId          " << newCardData.companyId;
                    qDebug() << "sequenceID         " << newCardData.sequenceID;
                    qDebug() << "totalAmount        " << newCardData.totalAmount;
                    qDebug() << "remainingTravels   " << newCardData.remainingTravels;
                    qDebug() << "validationIndicator" << newCardData.validationIndicator;
                    qDebug() << "validationStation  " << newCardData.validationStation;
                    qDebug() << "validationYear     " << newCardData.validationYear;
                    qDebug() << "validationMonth    " << newCardData.validationMonth;
                    qDebug() << "validationDay      " << newCardData.validationDay;
                    qDebug() << "validationHour     " << newCardData.validationHour;
                    qDebug() << "validationMinute   " << newCardData.validationMinute;
                    qDebug() << "contractId         " << newCardData.contractId;
                    qDebug() << "rejectCode         " << newCardData.rejectCode;
                  }
                  else
                  {
                    qDebug() << "Error writing data to card.";
                  }
                }

                log.time          = now.time();
                log.transactionId = newCardData.sequenceID;
                log.mediumType    = ActivityManager::MediumType::Csc;
                memcpy(log.mediumSerialId + 4, card->getId().constData(), card->getId().size());
                memcpy(log.intrinsicMediumSerialId, log.mediumSerialId, sizeof(log.mediumSerialId));
                // TODO: size mismatch!
                log.ticketType         = ticketType;
                log.contractId         = newCardData.contractId;
                log.prevValidationMode = ActivityManager::ValidationMode(oldCardData.validationIndicator);
                log.prevValidationDate =
                    QDateTime(QDate(2000 + oldCardData.validationYear, oldCardData.validationMonth, oldCardData.validationDay),
                              QTime(oldCardData.validationHour, oldCardData.validationMinute));
                log.prevCompanyId = oldCardData.companyId;
                log.prevStationId = oldCardData.validationStation;
                log.ticketEvd     = QDate(newCardData.endingValidityYear, newCardData.endingValidityMonth,
                                          newCardData.endingValidityDay);
                log.discountRate        = discountRate;
                log.numValidSections    = 0;
                log.remainingAmount     = newCardData.totalAmount;
                log.remainingTrips      = newCardData.remainingTravels;
                log.prevRemainingAmount = oldCardData.totalAmount;
                memcpy(log.samSerialId + 2, _citypaySam->getSamSerial().constData(), _citypaySam->getSamSerial().size());

                ActivityManager::ValidationLogType  validationType;

                if (ActivityManager::AccessControlGateType::Exit == AM->getAcgType())
                {
                  validationType = ActivityManager::ValidationLogType::Exit;
                }
                else
                {
                  validationType = ActivityManager::ValidationLogType::Entry;
                }

                AM->appendValidation(validationType, log);
              }
              else
              {
                ActivityManager::RejectedValidationLog  rej;

                rej.time       = now.time();
                rej.mediumType = ActivityManager::MediumType::Csc;
                rej.ticketType = ticketType;
                memcpy(rej.mediumSerialId + 4, card->getId().constData(), card->getId().size());
                memcpy(rej.intrinsicMediumSerialId, rej.mediumSerialId, sizeof(rej.mediumSerialId));
                memcpy(rej.samSerialId + 2, _citypaySam->getSamSerial().constData(), _citypaySam->getSamSerial().size());

                // if (newCardData.blacklisted)
                if (blacklisted)
                {
                  ActivityManager::BlackListLog  bl;
                  bl.time       = rej.time;
                  bl.mediumType = rej.mediumType;
                  bl.ticketType = rej.ticketType;
                  memcpy(bl.mediumSerialId, rej.mediumSerialId, sizeof(rej.mediumSerialId));
                  memcpy(bl.intrinsicMediumSerialId, rej.mediumSerialId, sizeof(rej.mediumSerialId));
                  memcpy(bl.samSerialId, rej.samSerialId, sizeof(rej.samSerialId));

                  AM->appendBlackList(bl);
                }
                else
                {
                  AM->appendRejectedValidation(rej);
                }
              }
            }
            else
            {
              res.message += "\n(خطای ۶)";
            }
          }
          else
          {
            res.message += "\n(خطای ۵)";
          }
        }
        else
        {
          res.message += "\n(خطای ۴)";
        }
      }
      else
      {
        res.message += "\n(خطای ۳)";
      }
    }
    else
    {
      res.message += "\n(خطای ۲)";
    }
  }
  else
  {
    res.message += "\n(خطای ۱)";
  }

  return res;
}

Result  MetroBusiness::transactWithQr(const QSharedPointer<QrTicket> &ticket)
{
  Result      res(false, ERR_INVALID_TICKET);
  const auto  &now = QDateTime::currentDateTimeUtc();
  const auto  &PM  = ParameterManager::getInstance();

  auto const ticketType = ticket->getTicketType();

  if (PM->getValidationProperties().contains(ticketType))
  {
    auto const validationProps      = PM->getValidationProperties()[ticketType];
    auto const issueDate            = ticket->getIssuanceDate();
    quint32 const amountToDecrease  = PM->getFare2Date() < issueDate ? validationProps.fare2 : validationProps.fare1;
    auto const evd = issueDate.addDays(ticket->getValidityDays().toInt());

    if (issueDate <= now.date() && now.date() <= evd && amountToDecrease == ticket->getPrice().toUInt())
    {
      res.succeed = true;
      QrServerManager::getInstance().validate(ticket, ActivityManager::getInstance()->getStationId());
    }
  }

  return res;
}

Result  MetroBusiness::purchaseCityWay(const QByteArray &cardId, quint32 amount, quint32 minBalance, quint32 *newBalance, QByteArray *cwCode)
{
  Result                         res;
  CardReader::ApduResponse       response;
  CardReader::MifareMemoryBlock  block1, block2;
  const auto                    &now = QDateTime::currentDateTimeUtc();

  res.message = ERR_TRANSACTION;

  _citypaySam->citywayInitForOperation(cardId, &response);

  if (_cardReader->mifareAuthBlock(CardReader::BlockAuthMode::A, response.data, 1)
      && _cardReader->mifareReadBlock(1, &block1) && _cardReader->mifareReadBlock(2, &block2))
  {
    const auto &counter = block2.getData().mid(0, 4);

    if (_citypaySam->citywayGetBalance(block1.getData(), counter, &response))
    {

      const auto  oldBalance = qFromBigEndian<quint32>(reinterpret_cast<const uchar *>(response.data.mid(0, 4).constData()));

      qDebug() << "AAAAAAAAAAAA" << oldBalance << minBalance << amount;
      if ((0 < amount) && (minBalance <= oldBalance)
          && _citypaySam->citywayPurchase(block1.getData(), counter, amount, now, &response))
      {
        if ((0x90 == response.sw1) && (0x00 == response.sw2))
        {
          *newBalance = qFromBigEndian<quint32>(reinterpret_cast<const uchar *>(response.data.mid(0, 4).constData()));

          const auto &keyB0        = response.data.mid(4, 6);
          const auto &newBlkNumber = quint8(response.data.at(10));
          const auto &newBlkKeyB   = response.data.mid(11, 6);
          block1 = response.data.mid(17, 16);
          const auto &newBlkData = CardReader::MifareMemoryBlock(response.data.mid(33, 16));
          *cwCode = response.data.mid(49, 33);

          const auto &newCounter = qFromLittleEndian<quint32>(reinterpret_cast<const uchar *>(response.data.mid(82, 4).constData()));
          const auto &mac        = response.data.mid(86, 8);

          if (_cardReader->mifareAuthBlock(CardReader::BlockAuthMode::B, newBlkKeyB, newBlkNumber)
              && _cardReader->mifareWriteBlock(newBlkNumber, newBlkData)
              && _cardReader->mifareAuthBlock(CardReader::BlockAuthMode::B, keyB0, 1)
              && _cardReader->mifareWriteBlock(1, block1)
              && _cardReader->mifareDecrementTransfer(2, newCounter, 2))
          {
            res.message = Utilities::prettyPrice(*newBalance);
            res.succeed = true;
          }
          else
          {
            res.message += "\n(خطای ۱۰)";
          }
        }
      }
      else
      {
        res.message = "موجودی کافی نیست";
      }
    }
    else
    {
      res.message += "\n(خطای ۸)";
    }
  }
  else
  {
    res.message += "\n(خطای ۷)";
  }

  return res;
}

Result  MetroBusiness::purchaseCityPay(const QByteArray &, quint32 amount, QByteArray *cardSerial, QByteArray *cardPan,
                                       quint32 *cardVersion, quint32 *newBalance, QByteArray *cwCode)
{
  Result                    res, apduRes;
  CardReader::ApduResponse  response;

  if (apduRes = execCityPayApdu(CityPayCard::selectIsd(), &response))
  {
    if (apduRes = execCityPayApdu(CityPayCard::getSerialNumber(), &response))
    {
      const QByteArray  serial(response.data.mid(0, 2) + response.data.mid(16, 2) + response.data.mid(12, 4));
      *cardSerial = serial;

      if (apduRes = execCityPayApdu(CityPayCard::selectApplet(), &response))
      {
        *cardPan = response.data.mid(12, 8);

        const quint32     version = quint32(QString(response.data.mid(7, 4)).toFloat() * 100);
        const QByteArray  cardPin("\x12\x34\x00\x00", 4);

        *cardVersion = version;

        if (apduRes = execCityPayApdu(CityPayCard::authorizePin(cardPin), &response))
        {
          if (apduRes = execCityPayApdu(CityPayCard::getRandom(), &response))
          {
            const auto &terminalRandom = Utilities::generateRandom(8);

            // TODO: check.
            _citypaySam->citypayEncryptForTransmission(serial + response.data + terminalRandom, &response);

            if (apduRes = execCityPayApdu(CityPayCard::secureChannel(response.data), &response))
            {
              if (apduRes = execCityPayApdu(CityPayCard::getRandom(), &response))
              {
                const auto   &now            = QDateTime::currentDateTimeUtc();
                const auto   &samBinTerminal = _citypaySam->citypayGetSamTerminal();
                const auto   &beAmount       = qToBigEndian(amount);
                const quint8  walletId       = 0;
                const quint8  type           = 0;
                QByteArray    mediaInfo;

                mediaInfo.append(samBinTerminal);
                mediaInfo.append(Utilities::convertDateToBcd(now));
                mediaInfo.append(reinterpret_cast<const char *>(&beAmount), 4);
                mediaInfo.append(_transactionId / (0x10000));
                mediaInfo.append(_transactionId / 0x100);
                mediaInfo.append(_transactionId % 0x100);
                mediaInfo.append(walletId);
                mediaInfo.append(type);

                if (400 <= version)
                {
                  mediaInfo.append(QByteArray("\x00\x00\x00\x00\x00\x00\x00\x00", 8));
                }

                auto        apduPurchase = CityPayCard::purchase(mediaInfo);
                QByteArray  signature(serial + response.data + apduPurchase.lc);

                _citypaySam->citypaySignatureForPayment(signature, &response);
                apduPurchase.lc.append(response.data);

                if (apduRes = execCityPayApdu(apduPurchase, &response))
                {
                  *newBalance = qFromBigEndian<quint32>(reinterpret_cast<const uchar *>(response.data.mid(4, 4).data()));

                  mediaInfo.append(response.data.mid(0, 16));

                  if (version < 400)
                  {
                    mediaInfo.prepend(QByteArray("\x00\x00\x00\x00\x00\x00\x00\x00", 8));
                  }

                  *cwCode = mediaInfo;

                  apduRes.message = Utilities::prettyPrice(*newBalance);
                  res.succeed = true;
                  res.message = apduRes.message;
                }
              }
            }
          }
        }
      }
    }
  }

  if (!res.succeed)
  {
    res.message = apduRes.message;
  }

  return res;
}

Result  MetroBusiness::execCityPayApdu(const CardReader::ApduCommand &apdu, CardReader::ApduResponse *response)
{
  Result  res;

  if (res = _cardReader->cardApdu(apdu, response))
  {
    res = CityPayCard::checkApduResponse(*response);
  }

  return res;
}

void  MetroBusiness::start()
{
  const auto &now = QDateTime::currentMSecsSinceEpoch();
  Result      samStatus = _citypaySam->initialize();
qDebug() << "QQ 1";
  _windowManager->setSamModuleError(_citypaySam->getSamModuleErrors());
  _windowManager->setSamCityPayError(_citypaySam->getCityPayErrors());;
  _windowManager->setSamCityWayError(_citypaySam->getCityWayErrors());
  _windowManager->setRfStatus(samStatus.succeed);
  _windowManager->setSamStatus(_citypaySam->getCityPayInitialized(), _citypaySam->getCityWayInitialized());
  _windowManager->setReaderId(Utilities::getIpAddress().toIPv4Address() & 0xFF);
  _windowManager->setStaticMsg("در حال بارگذاری پارامتر...", WindowManager::MessageMode::Yellow);

  (void)ParameterManager::getInstance();
  loadSession();
qDebug() << "QQ 2";
  const auto &AM = ActivityManager::getInstance();

  AM->setActivityMode(ActivityManager::ActivityMode::OperatingMode);

  if (ActivityManager::AccessControlGateType::Exit == AM->getAcgType())
  {
    AM->setAlarm(ActivityManager::AlarmGroup::ExitReader, ActivityManager::AlarmBitNumber::Bit6, !samStatus.succeed);
  }
  else
  {
    AM->setAlarm(ActivityManager::AlarmGroup::EntryReader, ActivityManager::AlarmBitNumber::Bit6, !samStatus.succeed);
  }
qDebug() << "QQ 3";
  connect(&QrServerManager::getInstance(), SIGNAL(validationReady(QSharedPointer<QrTicket>,QrServerManager::ValidationResult)),
          this, SLOT(onQrValidationReady(QSharedPointer<QrTicket>,QrServerManager::ValidationResult)));

  Scu::getInstance()->start();

  _lastCardDetectionTime = now;
  _timerSearchCard->start();
  _timerSession->start();
}

void  MetroBusiness::saveSession()
{
  const auto             &AM = ActivityManager::getInstance();
  GateController::Status  gateStatus;

  // TODO: move this to other position.
  if (GateController::getInstance()->getStatus(&gateStatus))
  {
    AM->setAlarm(ActivityManager::AlarmGroup::Logic, gateStatus.alarm);

    _windowManager->setGateStatus(0 == (quint16(ActivityManager::AlarmBitNumber::Bit5) & gateStatus.alarm));
  }
  else
  {
    AM->setAlarm(ActivityManager::AlarmGroup::Logic, ActivityManager::AlarmBitNumber::Bit5, true);
  }

  _fileSession.seek(0);
  _streamSession.setDevice(&_fileSession);
  _streamSession << QDateTime::currentDateTimeUtc() << quint8(AM->getConfigurationMode()) << AM->getStationId()
                 << AM->getMaxFileSize() << AM->getValidationType() << AM->getCompanyService()
                 << quint8(AM->getAcgType()) << AM->getLogicType() << AM->getStationStatus()
                 << AM->getShiftStatus() << quint8(gateStatus.configuration & Scu::ConfigurationMask::GateConfigMask);

  _fileSession.flush();
}

void  MetroBusiness::loadSession()
{
  _fileSession.setFileName(SESSION_FILE_NAME);
  bool  hasSession = _fileSession.exists();

  if (_fileSession.open(QIODevice::ReadWrite))
  {
    _streamSession.setDevice(&_fileSession);

    if (hasSession)
    {
      const auto &AM = ActivityManager::getInstance();
      QDateTime   time;
      quint8      configurationMode;
      quint16     stationId;
      quint16     maxFileSize;
      quint8      validationType;
      quint8      companyService;
      quint8      acgType;
      quint8      logicType;
      bool        stationStatus;
      bool        shiftStatus;
      quint8      gateConfiguration;

      _streamSession >> time >> configurationMode >> stationId >> maxFileSize >> validationType >> companyService
          >> acgType >> logicType >> stationStatus >> shiftStatus >> gateConfiguration;

      Utilities::setSystemDateTime(time);

      AM->setConfigurationMode(ActivityManager::ConfigurationMode(configurationMode));
      AM->setStationId(stationId);
      AM->setMaxFileSize(maxFileSize);
      AM->setValidationType(validationType);
      AM->setCompanyService(companyService);
      AM->setAcgType(static_cast<ActivityManager::AccessControlGateType>(acgType));
      AM->setLogicType(logicType);
      AM->setStationStatus(stationStatus);
      //AM->setShiftStatus(shiftStatus);
      AM->setAlarm(ActivityManager::AlarmGroup::Kitcas, ActivityManager::AlarmBitNumber::Bit11, 0 == stationId);

      GateController::getInstance()->setConfiguration(gateConfiguration & Scu::ConfigurationMask::GateConfigMask);
      Scu::getInstance()->setConfiguration(Scu::ConfigurationMask(gateConfiguration));
    }
  }
}

void  MetroBusiness::stop()
{
  _timerSession->stop();
  _timerSearchCard->stop();
}

void  MetroBusiness::onTimerSessionTimeout()
{
  saveSession();
  _windowManager->setRemainingActFiles(ActivityManager::getInstance()->getActFileCount());
}

void MetroBusiness::onQrValidationReady(const QSharedPointer<QrTicket> &ticket, const QrServerManager::ValidationResult &validation)
{
  Result res(false, ERR_INVALID_TICKET);

  if (nullptr != ticket)
  {
    auto const now  = QDateTime::currentDateTimeUtc();
    auto const &AM  = ActivityManager::getInstance();
    auto const &PM  = ParameterManager::getInstance();

    switch (validation.status)
    {
    case QrServerManager::ValidationStatus::Valid:
    {
      auto const ticketType = ticket->getTicketType();
      auto const validationProps      = PM->getValidationProperties()[ticketType];
      auto const issueDate            = ticket->getIssuanceDate();
      quint32 const amountToDecrease  = PM->getFare2Date() < issueDate ? validationProps.fare2 : validationProps.fare1;
      auto const evd = issueDate.addDays(ticket->getValidityDays().toInt());

      {
        ActivityManager::QrValidationLog log;

        // TODO: check all fields. lots of size mismatch!
        log.issuanceDate        = issueDate;
        log.issuerSerial        = (ticket->getIssuerType() + ticket->getIssuerId()).toLatin1();
        log.validationTime      = now;
        log.qrCounter           = ticket->getIssuerCounter().toUInt();
        log.qrSerial            = ticket->getSerialNumber().toLatin1();
        log.readerCounter       = QrServerManager::getInstance().getTicketCounter();
        log.readerSerial        = Utilities::getIpString().toLatin1();
        log.type                = ticket->getQrType().toUInt();
        log.validationStationId = AM->getStationId();
        log.ticketType          = ticket->getTicketType();
        // Server status code.
        log.ticketEvd           = evd;
        log.validityDays        = ticket->getValidityDays().toUInt();
        log.amount              = amountToDecrease / ticket->getTotalTripCount();
        log.remainingTrips      = validation.remainingTrips;
        log.keyNumber           = ticket->getKeyId().toUShort();
        log.mac                 = ticket->getMac().toLatin1();
        log.encryptedData       = ticket->getEncryptedSection().toLatin1();

        AM->appendValidation(static_cast<quint8>(validation.responseCode), log);

        QrDbManager::getInstance().insertTicket(ticket, QrServerManager::ServerResponseCode::OfflineMode == validation.responseCode);

        res.succeed = true;
        res.message = "سفر به خیر";
      }
    }
    break;
    case QrServerManager::ValidationStatus::NotValid:
    {
      if (QrServerManager::ServerResponseCode::OfflineMode == validation.responseCode)
      {
        res.message = "عدم پذیرش بلیت QR";
      }
    }
    break;
    case QrServerManager::ValidationStatus::OfflineQuotaReached:
    {
      res.message = "از کارتخوان دیگری استفاده کنید";
    }
    break;
    }

    _windowManager->setTransactionMsg(res.message, res.succeed ? WindowManager::MessageMode::Green : WindowManager::MessageMode::Red);

    if (res.succeed)
    {
      BuzzerController::getInstance()->singleBeep();
      GateController::getInstance()->authorize();
    }
    else
    {
      BuzzerController::getInstance()->tripleBeep();
    }
  }
}

void  MetroBusiness::onTimerTimeout()
{
  QSharedPointer<CreditCard>  card;
  QSharedPointer<Barcode>     barcode;

  bool        searchCard    = false;
  const auto &AM            = ActivityManager::getInstance();
  const auto &configuration = Scu::getInstance()->getConfiguration();

  _windowManager->setStationId(AM->getStationId());
  _windowManager->setScuStatus(Scu::getInstance()->isConnected());
  _windowManager->setRfStatus(_cardReader->isResponding());
  _windowManager->setGateMode(AM->getAcgType(), static_cast<Scu::ConfigurationMask>(configuration));
  _windowManager->setQrStatus(_barcodeReader->getVersion(nullptr).succeed,
                              ActivityManager::AccessControlGateType::Exit != AM->getAcgType());

#ifdef VERBOSE_LOG
  qDebug() << "ALARMS:" << AM->getAlarmsMask() << AM->getAlarm(ActivityManager::AlarmGroup::Kitcas)
           << AM->getAlarm(ActivityManager::AlarmGroup::EntryReader)
           << AM->getAlarm(ActivityManager::AlarmGroup::ExitReader)
           << AM->getAlarm(ActivityManager::AlarmGroup::Logic)
           << AM->getAlarm(ActivityManager::AlarmGroup::Sc);
#endif

  if (ActivityManager::AccessControlGateType::Exit == AM->getAcgType())
  {
    AM->setAlarm(ActivityManager::AlarmGroup::ExitReader, ActivityManager::AlarmBitNumber::Bit6, !_cardReader->isResponding());
  }
  else
  {
    AM->setAlarm(ActivityManager::AlarmGroup::EntryReader, ActivityManager::AlarmBitNumber::Bit6, !_cardReader->isResponding());
  }

  if (0 == AM->getAlarmsMask())
  {
    switch (ParameterManager::getInstance()->getStatus())
    {
    case ParameterManager::Status::Normal:
    {
      if (AM->getStationStatus())
      {
        switch (configuration & Scu::GateConfigMask)
        {
        case Scu::ConfigControlledBlocked:
        {
          if (ActivityManager::AccessControlGateType::Exit != AM->getAcgType())
          {
            _windowManager->setStaticMsg("لطفا بلیت ارائه دهید", WindowManager::MessageMode::Normal);
            searchCard = true;
          }
          else
          {
            _windowManager->setStaticMsg("عبور ممنوع", WindowManager::MessageMode::Red);
          }
        }
        break;
        case Scu::ConfigControlledControlled:
        {
          _windowManager->setStaticMsg("لطفا بلیت ارائه دهید", WindowManager::MessageMode::Normal);
          searchCard = true;
        }
        break;
        case Scu::ConfigControlledFree:
        {
          if (ActivityManager::AccessControlGateType::Exit != AM->getAcgType())
          {
            _windowManager->setStaticMsg("لطفا بلیت ارائه دهید", WindowManager::MessageMode::Normal);
            searchCard = true;
          }
          else
          {
            _windowManager->setStaticMsg("عبور آزاد", WindowManager::MessageMode::Green);
          }
        }
        break;
        case Scu::ConfigBlockedControlled:
        {
          if (ActivityManager::AccessControlGateType::Exit != AM->getAcgType())
          {
            _windowManager->setStaticMsg("عبور ممنوع", WindowManager::MessageMode::Red);
          }
          else
          {
            _windowManager->setStaticMsg("لطفا بلیت ارائه دهید", WindowManager::MessageMode::Normal);
            searchCard = true;
          }
        }
        break;
        case Scu::ConfigBlockedFree:
        {
          if (ActivityManager::AccessControlGateType::Exit != AM->getAcgType())
          {
            _windowManager->setStaticMsg("عبور ممنوع", WindowManager::MessageMode::Red);
          }
          else
          {
            _windowManager->setStaticMsg("عبور آزاد", WindowManager::MessageMode::Green);
          }
        }
        break;
        case Scu::ConfigBlockedBlocked:
        {
          _windowManager->setStaticMsg("عبور ممنوع", WindowManager::MessageMode::Red);
        }
        break;
        case Scu::ConfigFreeFree:
        {
          _windowManager->setStaticMsg("عبور آزاد", WindowManager::MessageMode::Green);
        }
        break;
        case Scu::ConfigFreeBlocked:
        {
          if (ActivityManager::AccessControlGateType::Exit != AM->getAcgType())
          {
            _windowManager->setStaticMsg("عبور آزاد", WindowManager::MessageMode::Green);
          }
          else
          {
            _windowManager->setStaticMsg("عبور ممنوع", WindowManager::MessageMode::Red);

          }
        }
        break;
        case Scu::ConfigFreeControlled:
        {
          if (ActivityManager::AccessControlGateType::Entry == AM->getAcgType())
          {
            _windowManager->setStaticMsg("عبور آزاد", WindowManager::MessageMode::Green);

          }
          else
          {
            _windowManager->setStaticMsg("لطفا بلیت ارائه دهید", WindowManager::MessageMode::Normal);
            searchCard = true;
          }
        }
        break;
        default:
        {
          _windowManager->setStaticMsg("خارج از سرویس", WindowManager::MessageMode::Yellow);
        }
        }
      }
      else
      {
        _windowManager->setStaticMsg("ایستگاه تعطیل است", WindowManager::MessageMode::Red);
      }
    }
    break;
    case ParameterManager::Status::LoadingParameters:
    {
      _windowManager->setStaticMsg("در حال بارگذاری پارامتر...", WindowManager::MessageMode::Yellow);
    }
    break;
    case ParameterManager::Status::DownloadingParameters:
    {
      _windowManager->setStaticMsg("در حال دریافت پارامتر...", WindowManager::MessageMode::Yellow);
    }
    break;
    }
  }
  else
  {
    switch (ParameterManager::getInstance()->getStatus())
    {
    case ParameterManager::Status::Normal:
    {
      _windowManager->setStaticMsg("خارج از سرویس", WindowManager::MessageMode::Yellow);
    }
    break;
    case ParameterManager::Status::LoadingParameters:
    {
      _windowManager->setStaticMsg("در حال بارگذاری پارامتر...", WindowManager::MessageMode::Yellow);
    }
    break;
    case ParameterManager::Status::DownloadingParameters:
    {
      _windowManager->setStaticMsg("در حال دریافت پارامتر...", WindowManager::MessageMode::Yellow);
    }
    break;
    }
  }
  // TODO: enable following line.
  //if (true)
  if (searchCard && (ActivityManager::ActivityMode::OperatingMode == AM->getActivityMode()))
  {
    const auto &cardSearchResult = _cardReader->searchCard(&card);

    if (cardSearchResult)
    {
      if (nullptr != card)
      {
        _lastCardDetectionTime = QDateTime::currentMSecsSinceEpoch();

        if (!_lastTransResult.succeed || (nullptr == _lastDetectedCard) || (_lastDetectedCard->getId() != card->getId()))
        {
          _lastDetectedCard = card;

#ifdef VERBOSE_LOG
          qDebug() << cardTypeToString(card->getType()) << "card with id " << card->getId().toHex() << "detected.";
#endif

          Result  transResult(false, "کارت نا معتبر");

          switch (card->getType())
          {
          case CreditCardType::Mifare:
          {
            transResult = transactWithCard(card.staticCast<CityWayCard>());
          }
          break;
          case CreditCardType::CityPay:
          {
            transResult = transactWithCard(card.staticCast<CityPayCard>());
          }
          break;
          default:
          break;
          }

          _lastTransResult = transResult;

          _windowManager->setTransactionMsg(transResult.message, transResult.succeed
                                            ? WindowManager::MessageMode::Green : WindowManager::MessageMode::Red);

          if (transResult.succeed)
          {
            BuzzerController::getInstance()->singleBeep();
            GateController::getInstance()->authorize();
          }
          else
          {
            BuzzerController::getInstance()->tripleBeep();
          }
        }
        else // Card id anti pass back.
        {
          // BuzzerController::getInstance()->tripleBeep();
        }
      }
    }
    else
    {
      if (ActivityManager::AccessControlGateType::Exit != ActivityManager::getInstance()->getAcgType())
      {
        const auto &barcodeSearchResult = _barcodeReader->search(&barcode);

        if (barcodeSearchResult)
        {
          const auto &now = QDateTime::currentMSecsSinceEpoch();

          if (_lastQrDetectionTime + 1000 < now)
          {
            auto const res = transactWithQr(barcode.dynamicCast<QrTicket>());

            if (!res)
            {
              _windowManager->setTransactionMsg(res.message, WindowManager::MessageMode::Red);
              BuzzerController::getInstance()->tripleBeep();
            }
          }

          _lastQrDetectionTime = now;
        }
      }
    }

    if (TRANSACTION_INTERVAL_MSEC < QDateTime::currentMSecsSinceEpoch() - _lastCardDetectionTime)
    {
      _lastDetectedCard.clear();
    }
  }
}

Result  MetroBusiness::getValidationActions(const ParameterManager::ValidationProperty  &validationProp,
                                            QVector<ParameterManager::ValidationAction> *actions)
{
  Result      res;
  const auto &now = QDateTime::currentDateTimeUtc();
  const auto &PM  = ParameterManager::getInstance();
  const auto &AM  = ActivityManager::getInstance();

#ifdef VERBOSE_LOG
  qDebug() << "T1";
  qDebug() << "actionTableIdforEntry                      " << validationProp.actionTableIdforEntry;
  qDebug() << "actionTableIdForExit                       " << validationProp.actionTableIdForExit;
  qDebug() << "encodingMaskId                             " << validationProp.encodingMaskId;
  qDebug() << "endOfValidationAuthorization               " << validationProp.endOfValidationAuthorization;
  qDebug() << "fareIndex                                  " << validationProp.fareIndex;
  qDebug() << "id                                         " << validationProp.id;
  qDebug() << "kindOfTicket                               " << validationProp.kindOfTicket;
  qDebug() << "kindOfValidity                             " << validationProp.kindOfValidity;
  qDebug() << "maxTrips                                   " << validationProp.maxTrips;
  qDebug() << "parameterTableId                           " << validationProp.parameterTableId;
  qDebug() << "periodTableId                              " << validationProp.periodTableId;
  qDebug() << "startOfValidationAuthorization             " << validationProp.startOfValidationAuthorization;
  qDebug() << "tableIdForAmountOrTokenUseForPublicHolidays" << validationProp.tableIdForAmountOrTokenUseForPublicHolidays;
  qDebug() << "tableIdForAmountOrTokenUseForRegularDays   " << validationProp.tableIdForAmountOrTokenUseForRegularDays;
  qDebug() << "tableIdForAmountOrTokenUseForSchoolDays    " << validationProp.tableIdForAmountOrTokenUseForSchoolDays;
  qDebug() << "transferAllowed                            " << validationProp.transferAllowed;
  qDebug() << "transferTableId                            " << validationProp.transferTableId;
  qDebug() << "validityTableIdForPublicHolidays           " << validationProp.validityTableIdForPublicHolidays;
  qDebug() << "validityTableIdForRegularDays              " << validationProp.validityTableIdForRegularDays;
  qDebug() << "validityTableIdForSchoolDays               " << validationProp.validityTableIdForSchoolDays;
  qDebug() << "writeActionIndex                           " << validationProp.writeActionIndex;
  qDebug() << "T2";
#endif

  decltype(PM->getValidationActions())validationActions = PM->getValidationActions();
  decltype(PM->getValidationActions())::iterator actionsList;

  if (ActivityManager::AccessControlGateType::Exit == AM->getAcgType())
  {
    actionsList = validationActions.find(validationProp.actionTableIdForExit);
  }
  else
  {
    actionsList = validationActions.find(validationProp.actionTableIdforEntry);
  }

  if (actionsList != validationActions.constEnd())
  {
    *actions    = actionsList.value();
    res.succeed = true;
  }

  return res;
}
}
