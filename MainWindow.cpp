#include <MainWindow.h>
#include <ui_MainWindow.h>
#include <PanelWidget.h>
#include <Settings.h>
#include <DualiCardReader.h>
#include <HoneywellBarcodeReader.h>
#include <GateController.h>
#include <BuzzerController.h>
#include <MetroBusiness.h>
#include <Scu.h>
#include <CityPaySam.h>
#include <QrKeyStore.h>
#include <QrDbManager.h>
#include <Utilities.h>
#include <Cryptography.h>
#include <QFile>
#include <QTimer>
#include <QDateTime>
#include <QKeyEvent>
#include <QDir>
#include <QDebug>

#define MESSAGE_CLEAR_TIMEOUT   2500


static const QByteArray  AFC_MASTER_KEY_MAPRA("\x9E\x91\x84\x8B\xC2\x52\xE5\x1B\x48\x6C\x29\xC0\xAC\xD4\x2D\x32", 16);


namespace Afc
{
MainWindow::MainWindow(QWidget *parent):
  QMainWindow(parent), _ui(new Ui::MainWindow), _settings("config.ini", QSettings::IniFormat),
  _settingsCityPayKey("/home/" + QString(Settings::USER_NAME) +  "/samkey/key"),
  _lastTransactionMsgTime(QDateTime::currentMSecsSinceEpoch())
{
  _ui->setupUi(this);
  _ui->lblTransactionShadow->stackUnder(_ui->lblTransaction);

  _panelSettings.reset(new PanelWidget());

  QrDbManager::getInstance();
  QrServerManager::getInstance().setServerIp(_panelSettings->getQrServerIp());

  connect(_panelSettings.data() , SIGNAL(qrServerIpChanged(QString)), this, SLOT(onQrServerIpChanged(QString)));

  if (!QDir("/home/" + QString(Settings::USER_NAME) +  "/samkey").exists())
  {
    QDir().mkdir("/home/" + QString(Settings::USER_NAME) + "/samkey");
    Utilities::setOwner("/home/" + QString(Settings::USER_NAME) +  "/samkey", Settings::USER_NAME);
  }

  initWm();

  connect(_ui->btnPanel, SIGNAL(clicked(bool)), this, SLOT(onBtnPanelClicked()));

  _oldStyle = _ui->lblTransactionShadow->styleSheet();
  setWindowFlags(Qt::FramelessWindowHint);
  loadSettings();

  _timerWindowMgr.reset(new QTimer());
  _timerWindowMgr->setInterval(200);
  connect(_timerWindowMgr.data(), SIGNAL(timeout()), this, SLOT(onTimerWindowMgr()));
  _timerWindowMgr->start();

  GateController::getInstance()->initializeDevice(_gateControllerPortName);

  _cardReader = QSharedPointer<DualiCardReader>(new DualiCardReader(_cardReaderPortName));
  _cardReader->initializeDevice();

  _barcodeReader = QSharedPointer<HoneywellBarcodeReader>::create();
  _barcodeReader->initializeDevice();
  const auto &citypaySam = QSharedPointer<CityPaySam>(new CityPaySam(_cardReader, _citypaySamSlot, _citypaySamKey));
  QrKeyStore::initialize(AFC_MASTER_KEY_MAPRA);
  _business = QSharedPointer<MetroBusiness>(new MetroBusiness(this, _cardReader, _barcodeReader, citypaySam));
  _business->start();

  BuzzerController::getInstance()->singleBeep(150);
}

MainWindow::~MainWindow()
{
  saveSettings();
}

void  MainWindow::saveSettings()
{
  _settings.setValue(Settings::GATE_CONTROLLER_PORT_NAME, _gateControllerPortName);
  _settings.setValue(Settings::CARD_READER_PORT_NAME, _cardReaderPortName);
  _settings.setValue(Settings::CITYPAY_SAM_SLOT, _citypaySamSlot);

  if (16 == _citypaySamKey.size())
  {
    const auto &samKey = Cryptography::encrypt2Des(AFC_MASTER_KEY_MAPRA, _citypaySamKey);

    if (_settingsCityPayKey.open(QIODevice::ReadWrite | QIODevice::Truncate))
    {
      _settingsCityPayKey.write(samKey);
      _settingsCityPayKey.close();
    }
  }
  else
  {
    if (_settingsCityPayKey.open(QIODevice::ReadWrite | QIODevice::Truncate))
    {
      _settingsCityPayKey.write(QString("00").toLatin1());
      _settingsCityPayKey.close();
    }
  }

  _settings.sync();
}

void  MainWindow::loadSettings()
{
  _gateControllerPortName = _settings.value(Settings::GATE_CONTROLLER_PORT_NAME, "ttyS5").toString();
  _cardReaderPortName     = _settings.value(Settings::CARD_READER_PORT_NAME, "ttyS6").toString();
  _citypaySamSlot         = _settings.value(Settings::CITYPAY_SAM_SLOT, 1).toUInt();

  if (_settingsCityPayKey.open(QIODevice::ReadOnly))
  {
    _citypaySamKey = _settingsCityPayKey.readAll();
    _settingsCityPayKey.close();
  }

  if (16 == _citypaySamKey.size())
  {
    _citypaySamKey = Cryptography::decrypt2Des(AFC_MASTER_KEY_MAPRA, _citypaySamKey);
  }

  if (!QFile::exists(_settings.fileName()))
  {
    saveSettings();
  }
}

void  MainWindow::setDate(const QDateTime &date)
{
  _ui->lblTime->setText(Utilities::miladiToShamsi(date.date()).toString("yyyy/MM/dd")
                        + " | " + date.time().toString("hh:mm:ss"));
}

void  MainWindow::setStationId(quint32 id)
{
  _wmStationId = QString::number(id);
}

void  MainWindow::setReaderId(quint32 id)
{
  _wmReaderId = QString::number(id);
}

void  MainWindow::setAppVersion(quint8 major, quint8 minor, quint8, quint8)
{
  _wmAppVersion = QString::number(major) + "." + QString::number(minor);
}

void  MainWindow::setFamilleVersion(quint32 version)
{
  _wmFamilleVersion = QString::number(version);
}

void  MainWindow::setGateMode(ActivityManager::AccessControlGateType acgType, Scu::ConfigurationMask configurationMode)
{
  bool  isOnline = (0 != (configurationMode & Scu::Online));

  _panelSettings->setGateUi(isOnline, configurationMode);
  _panelSettings->setAccessControlUi(acgType);
  _wmGateMode = isOnline ? "O" : "L";

  switch (acgType)
  {
  case ActivityManager::AccessControlGateType::Entry:
    _wmGateMode += "A";
    break;
  case ActivityManager::AccessControlGateType::Exit:
    _wmGateMode += "B";
    break;
  case ActivityManager::AccessControlGateType::EntryExit:
    _wmGateMode += "C";
    break;
  }

  _wmGateMode += QString::number(configurationMode & Scu::GateConfigMask);
}

void  MainWindow::setGateStatus(bool status)
{
  _wmGateStatus = status ? "G" : "0";
}

void  MainWindow::setScuStatus(bool status)
{
  _wmScuStatus = status ? "C" : "0";
}

void  MainWindow::setDbStatus(quint32 status)
{
  _wmDbStatus = status ? "C" : "0";
}

void  MainWindow::setQrStatus(bool status, bool enabled)
{
  _wmQrStatus = enabled ? (status ? "Q" : "0") : "*";
}

void  MainWindow::setRemainingActFiles(quint32 count)
{
  _wmRemainingActFile = QString::number(count);
}

void  MainWindow::setRfStatus(quint32 status)
{
  _wmRfStatus = status ? "R" : "0";
}

void  MainWindow::setSamStatus(quint32 citypay, quint32 cityway)
{
  _wmSamCitypayStatus = citypay ? "P" : "0";
  _wmSamCitywayStatus = cityway ? "W" : "0";
}

void MainWindow::setSamModuleError(const QString &err)
{
  _panelSettings->setSamModuleError(err);
}

void MainWindow::setSamCityPayError(const QString &err)
{
  _panelSettings->setSamCityPayError(err);
}

void MainWindow::setSamCityWayError(const QString &err)
{
  _panelSettings->setSamCityWayError(err);
}

void  MainWindow::setStaticMsg(const QString &msg, MessageMode mode)
{
  _wmStaticMsg     = msg;
  _wmStaticMsgMode = mode;
}

void  MainWindow::setTransactionMsg(const QString &msg, MessageMode mode)
{
  _lastTransactionMsgTime = QDateTime::currentMSecsSinceEpoch();
  _ui->lblTransaction->setText(msg);
  _ui->lblTransactionShadow->setText(msg);

  switch (mode)
  {
  case MessageMode::Normal:
  {
    _ui->lblTransaction->setStyleSheet(Settings::STYLE_WHITE_BLACK);
    _ui->lblTransactionShadow->setStyleSheet(Settings::STYLE_WHITE_BLACK_S);
    _ui->lblTime->setStyleSheet(Settings::STYLE_TIME_BLACK);
    _ui->lblLeft->setStyleSheet(Settings::STYLE2_BLACK);
    _ui->lblRight->setStyleSheet(Settings::STYLE2_BLACK);
    _ui->btnPanel->setStyleSheet(Settings::STYLE_SETTING_BLACK);
    _ui->lblLogo1->setStyleSheet(Settings::STYLE_LOGO_BLACK);
    _ui->lblLogo2->setStyleSheet(Settings::STYLE_LOGO_BLACK);
  }
  break;
  case MessageMode::Green:
  {
    _ui->lblTransaction->setStyleSheet(Settings::STYLE_GREEN_WHITE);
    _ui->lblTransactionShadow->setStyleSheet(Settings::STYLE_GREEN_WHITE_S);
    _ui->lblTime->setStyleSheet(Settings::STYLE_TIME_WHITE);
    _ui->lblLeft->setStyleSheet(Settings::STYLE2_WHITE);
    _ui->lblRight->setStyleSheet(Settings::STYLE2_WHITE);
    _ui->btnPanel->setStyleSheet(Settings::STYLE_SETTING_WHITE);
    _ui->lblLogo1->setStyleSheet(Settings::STYLE_LOGO_WHITE);
    _ui->lblLogo2->setStyleSheet(Settings::STYLE_LOGO_WHITE);
  }
  break;
  case MessageMode::Yellow:
  {
    _ui->lblTransaction->setStyleSheet(Settings::STYLE_YELLOW_WHITE);
    _ui->lblTransactionShadow->setStyleSheet(Settings::STYLE_YELLOW_WHITE_S);
    _ui->lblTime->setStyleSheet(Settings::STYLE_TIME_WHITE);
    _ui->lblLeft->setStyleSheet(Settings::STYLE2_WHITE);
    _ui->lblRight->setStyleSheet(Settings::STYLE2_WHITE);
    _ui->btnPanel->setStyleSheet(Settings::STYLE_SETTING_WHITE);
    _ui->lblLogo1->setStyleSheet(Settings::STYLE_LOGO_WHITE);
    _ui->lblLogo2->setStyleSheet(Settings::STYLE_LOGO_WHITE);
  }
  break;
  case MessageMode::Red:
  {
    _ui->lblTransaction->setStyleSheet(Settings::STYLE_RED_WHITE);
    _ui->lblTransactionShadow->setStyleSheet(Settings::STYLE_RED_WHITE_S);
    _ui->lblTime->setStyleSheet(Settings::STYLE_TIME_WHITE);
    _ui->lblLeft->setStyleSheet(Settings::STYLE2_WHITE);
    _ui->lblRight->setStyleSheet(Settings::STYLE2_WHITE);
    _ui->btnPanel->setStyleSheet(Settings::STYLE_SETTING_WHITE);
    _ui->lblLogo1->setStyleSheet(Settings::STYLE_LOGO_WHITE);
    _ui->lblLogo2->setStyleSheet(Settings::STYLE_LOGO_WHITE);
  }
  break;
  }

  repaint();
}

void  MainWindow::onTimerWindowMgr()
{
  auto  now = QDateTime::currentMSecsSinceEpoch();

  setDate(QDateTime::currentDateTimeUtc());

  if (_lastTransactionMsgTime + MESSAGE_CLEAR_TIMEOUT < now)
  {
    setTransactionMsg(_wmStaticMsg, _wmStaticMsgMode);
  }

  _ui->lblLeft->setText(_wmStationId + "-" + _wmReaderId + "-" + _wmAppVersion
                        + "-" + QString::number(ParameterManager::getInstance()->getParamsVersion()));
  _ui->lblRight->setText(_wmGateMode + "-" + _wmScuStatus + _wmDbStatus + "-" + _wmRemainingActFile + "-"
                         + _wmGateStatus + _wmRfStatus + _wmSamCitypayStatus + _wmSamCitywayStatus + _wmQrStatus);
}

void  MainWindow::onBtnPanelClicked()
{
  ActivityManager::getInstance()->setActivityMode(ActivityManager::ActivityMode::MaintenanceMode);
  _panelSettings->showFullScreen();
}

void MainWindow::onQrServerIpChanged(const QString &ip)
{
  QrServerManager::getInstance().setServerIp(ip);
}

void  MainWindow::initWm()
{
  setStationId(0);
  setReaderId(0);
  setAppVersion(Settings::MAIN_SOFTWARE_VERSION / 256, Settings::MAIN_SOFTWARE_VERSION % 256,
                Settings::AUX_SOFTWARE_VERSION / 256, Settings::AUX_SOFTWARE_VERSION % 256);
  setFamilleVersion(0);
  setGateMode(ActivityManager::AccessControlGateType::Entry, Scu::UnknownConfiguration);
  setGateStatus(false);
  setScuStatus(false);
  setDbStatus(false);
  setQrStatus(false, false);
  setRemainingActFiles(0);
  setRfStatus(false);
  setSamStatus(false, false);
  setStaticMsg("لطفا بلیت ارائه دهید", MessageMode::Normal);
}
}
