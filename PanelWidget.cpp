#include <PanelWidget.h>
#include <ui_PanelWidget.h>
#include <Utilities.h>
#include <GateController.h>
#include <ActivityManager.h>
#include <Scu.h>
#include <Cryptography.h>
#include <Settings.h>
#include <QOnScreenKeyboard/qonscreenkeyboard.h>
#include <QProcess>
#include <QFile>
#include <QDebug>

static const auto STYLE_OK              = QString("color: rgb(0, 0, 255);");
static const auto STYLE_ERR             = QString("color: rgb(255, 0, 0);");
static const auto DEFAULT_QR_SERVER_IP  = QString("192.168.1.4");

static const QByteArray  AFC_MASTER_KEY_MAPRA("\x9E\x91\x84\x8B\xC2\x52\xE5\x1B\x48\x6C\x29\xC0\xAC\xD4\x2D\x32", 16);
enum TabIndex
{
  TAB_MAIN            = 0,
  TAB_GATE_MODE       = 1,
  TAB_LOGIN           = 2,
  TAB_OPERATOR        = 3,
  TAB_IP_SETTINGS     = 4,
  TAB_DATE_SETTINGS   = 5,
  TAB_GATE_SETTINGS   = 6,
  TAB_SAM_SETTINGS    = 7,
  TAB_FILE_OPERATIONS = 8,
  TAB_PARAMETERS      = 9,
  TAB_TRANSACTIONS    = 10,
  TAB_LOGS            = 11,
};

namespace Afc
{
PanelWidget::PanelWidget(QWidget *parent):
  QWidget(parent),
  _ui(new Ui::PanelWidget), _settings("config.ini", QSettings::IniFormat),
  _settingsSamKey("/home/" + QString(Settings::USER_NAME) +  "/samkey/key")
{
  _ui->setupUi(this);
  _ui->panelTab->setCurrentIndex(0);
  _ui->txtIp->setText(Utilities::getIpAddress().toString());
  _ui->txtQrIp->setText(_settings.value(Settings::QR_SERVER_IP, DEFAULT_QR_SERVER_IP).toString());
  _ui->spinSamSlot->setValue(_settings.value(Settings::CITYPAY_SAM_SLOT, 0).toInt());

  QString           ipRange = "(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])";
  QRegExp           ipRegex("^" + ipRange + "\\." + ipRange + "\\." + ipRange + "\\." + ipRange + "$");
  QRegExpValidator *ipValidator = new QRegExpValidator(ipRegex, this);

  _ui->txtIp->setValidator(ipValidator);
  _ui->txtQrIp->setValidator(ipValidator);
  _ui->txtStationId->setValidator(new QRegExpValidator(QRegExp("^[1-9]\\d*$"), this));

  connect(_ui->txtPassword, SIGNAL(focused()), this, SLOT(showKeyboard()));
  connect(_ui->txtIp, SIGNAL(focused()), this, SLOT(showKeyboard()));
  connect(_ui->txtQrIp, SIGNAL(focused()), this, SLOT(showKeyboard()));
  connect(_ui->txtSamKey, SIGNAL(focused()), this, SLOT(showKeyboard()));
  connect(_ui->txtStationId, SIGNAL(focused()), this, SLOT(showKeyboard()));
  connect(_ui->txtDateNew, SIGNAL(focused()), this, SLOT(showKeyboard()));
  connect(_ui->txtTimeNew, SIGNAL(focused()), this, SLOT(showKeyboard()));

  auto  groupReader = new QButtonGroup();
  groupReader->addButton(_ui->radioReaderA);
  groupReader->addButton(_ui->radioReaderB);

  connect(&_timer, SIGNAL(timeout()), this, SLOT(onTimerTimeout()));
  _timer.setInterval(500);
  _timer.start();
}

PanelWidget::~PanelWidget()
{
  delete _ui;
}

void  PanelWidget::setGateUi(bool isAuto, Scu::ConfigurationMask mode)
{
  if (isAuto)
  {
    _ui->radioGateModeAuto->setChecked(true);
  }
  else
  {
    switch (mode & Scu::ConfigurationMask::GateConfigMask)
    {
    case 1:
      _ui->radioGateMode1->setChecked(true);
    break;
    case 2:
      _ui->radioGateMode2->setChecked(true);
    break;
    case 3:
      _ui->radioGateMode3->setChecked(true);
    break;
    case 4:
      _ui->radioGateMode4->setChecked(true);
    break;
    case 5:
      _ui->radioGateMode5->setChecked(true);
    break;
    case 6:
      _ui->radioGateMode6->setChecked(true);
    break;
    case 7:
      _ui->radioGateMode7->setChecked(true);
    break;
    case 8:
      _ui->radioGateMode8->setChecked(true);
    break;
    case 9:
      _ui->radioGateMode9->setChecked(true);
    break;
    default:
    break;
    }
  }
}

void  PanelWidget::setAccessControlUi(ActivityManager::AccessControlGateType acgType)
{
  switch (acgType)
  {
  case ActivityManager::AccessControlGateType::Exit:
    _ui->radioReaderB->setChecked(true);
  break;
  default:
    _ui->radioReaderA->setChecked(true);
  break;
  }
}

void PanelWidget::setSamModuleError(const QString &err)
{
  _ui->lblSamModuleError->setText("SAM Module: " + (err.isEmpty() ? QString("OK"): err));
  _ui->lblSamModuleError->setStyleSheet(err.isEmpty() ? STYLE_OK : STYLE_ERR);
}

void PanelWidget::setSamCityPayError(const QString &err)
{
  _ui->lblSamCityPayError->setText("SAM CityPay: " + (err.isEmpty() ? QString("OK"): err));
  _ui->lblSamCityPayError->setStyleSheet(err.isEmpty() ? STYLE_OK : STYLE_ERR);
}

void PanelWidget::setSamCityWayError(const QString &err)
{
  _ui->lblSamCityWayError->setText("SAM CityWay: " + (err.isEmpty() ? QString("OK"): err));
  _ui->lblSamCityWayError->setStyleSheet(err.isEmpty() ? STYLE_OK : STYLE_ERR);
}

QString PanelWidget::getQrServerIp() const
{
  return _settings.value(Settings::QR_SERVER_IP, DEFAULT_QR_SERVER_IP).toString();
}

void PanelWidget::resetBtnSamKeyStyle()
{
  _ui->btnSaveSam->setStyleSheet("border: 4px solid black;"
                                 "border-radius: 8px;"
                                 "background-color: rgb(255, 255, 255);");
}

void PanelWidget::resetBtnClearGateCounterStyle()
{
  _ui->btnClearGateCounters->setStyleSheet("border: 4px solid black;"
                                 "border-radius: 8px;"
                                 "background-color: rgb(255, 255, 255);");
}

void  PanelWidget::showKeyboard()
{
  _touchKeyboard.reset(new QOnScreenKeyboard());
  _touchKeyboard->setReceiver(static_cast<QWidget *>(sender()));
  _touchKeyboard->show();
  _touchKeyboard->move(0, 240);
}

void  PanelWidget::onTimerTimeout()
{
  _ui->dateTimeCurrent->setDateTime(QDateTime::currentDateTimeUtc());

  GateController::Status status;
  GateController::getInstance()->getStatus(&status);

  _ui->lblGateEntryCounter->setText(QString::number(status.counterA));
  _ui->lblGateExitCounter->setText(QString::number(status.counterB));

  const auto lastClearedBy = GateController::getInstance()->getLastClearedBy();
  const auto lastClearedDate = GateController::getInstance()->getLastClearedDate();

  if (lastClearedDate.isValid())
  {
    QString txt("Last cleared by: ");
    txt += lastClearedBy + " at " + Utilities::miladiToShamsi(lastClearedDate.date()).toString("yyyy/MM/dd");
    txt += " " + lastClearedDate.time().toString("hh:mm:ss");
    _ui->lblLastGateReset->setText(txt);
  }
}

void  Afc::PanelWidget::on_btnExitOperator_clicked()
{
  _ui->panelTab->setCurrentIndex(TAB_MAIN);
}

void  Afc::PanelWidget::on_btnIpSettings_clicked()
{
  _ui->panelTab->setCurrentIndex(TAB_IP_SETTINGS);
}

void  Afc::PanelWidget::on_btnDateSettings_clicked()
{
  _ui->panelTab->setCurrentIndex(TAB_DATE_SETTINGS);
}

void  Afc::PanelWidget::on_btnGateSettings_clicked()
{
  _ui->panelTab->setCurrentIndex(TAB_GATE_SETTINGS);
}

void  Afc::PanelWidget::on_btnSamSettings_clicked()
{
  _ui->panelTab->setCurrentIndex(TAB_SAM_SETTINGS);
}

void  Afc::PanelWidget::on_btnFileOperations_clicked()
{
  _ui->panelTab->setCurrentIndex(TAB_FILE_OPERATIONS);
}

void  Afc::PanelWidget::on_btnExitIpSettings_clicked()
{
  _ui->panelTab->setCurrentIndex(TAB_OPERATOR);
}

void  Afc::PanelWidget::on_btnExitDateSettings_clicked()
{
  _ui->panelTab->setCurrentIndex(TAB_OPERATOR);
}

void  Afc::PanelWidget::on_btnSaveDate_clicked()
{
  const auto &dt = QDateTime::fromString(_ui->txtDateNew->text() + " " + _ui->txtTimeNew->text() + ":00", QString("yyyy/MM/dd hh:mm:ss"));

  Utilities::setSystemDateTime(dt);
}

void  Afc::PanelWidget::on_btnExitGateSettings_clicked()
{
  _ui->panelTab->setCurrentIndex(TAB_OPERATOR);
}

void  Afc::PanelWidget::on_btnExitSamSettings_clicked()
{
  _ui->panelTab->setCurrentIndex(TAB_OPERATOR);
}

void  Afc::PanelWidget::on_btnSaveSam_clicked()
{
  const auto &key = _ui->txtSamKey->text().toLatin1().replace("-", "").trimmed();

  if (32 == key.length())
  {
    const auto &encKey = Cryptography::encrypt2Des(AFC_MASTER_KEY_MAPRA, QByteArray::fromHex(key));
    if (_settingsSamKey.open(QIODevice::ReadWrite | QIODevice::Truncate))
    {
      _settingsSamKey.write(encKey);
      _settingsSamKey.close();
      _ui->btnSaveSam->setStyleSheet("border: 4px solid black; border-radius: 8px; background-color: rgb(0, 255, 0);");

      QTimer::singleShot(1000, this, SLOT(resetBtnSamKeyStyle()));
    }
  }

  _settings.setValue(Settings::CITYPAY_SAM_SLOT, _ui->spinSamSlot->value());
  _settings.sync();
}

void  Afc::PanelWidget::on_btnExitFileOperations_clicked()
{
  _ui->panelTab->setCurrentIndex(TAB_OPERATOR);
}

void  Afc::PanelWidget::on_btnParameters_clicked()
{
  _ui->panelTab->setCurrentIndex(TAB_PARAMETERS);
}

void  Afc::PanelWidget::on_btnTransactions_clicked()
{
  _ui->panelTab->setCurrentIndex(TAB_TRANSACTIONS);
}

void  Afc::PanelWidget::on_btnLogs_clicked()
{
  _ui->panelTab->setCurrentIndex(TAB_LOGS);
}

void  Afc::PanelWidget::on_btnExitTransactions_clicked()
{
  _ui->panelTab->setCurrentIndex(TAB_OPERATOR);
}

void  Afc::PanelWidget::on_btnSaveTransactions_clicked()
{
  _ui->panelTab->setCurrentIndex(TAB_FILE_OPERATIONS);
}

void  Afc::PanelWidget::on_btnExitLog_clicked()
{
  _ui->panelTab->setCurrentIndex(TAB_OPERATOR);
}

void  Afc::PanelWidget::on_btnSaveLogs_clicked()
{
  _ui->panelTab->setCurrentIndex(TAB_FILE_OPERATIONS);
}

void  Afc::PanelWidget::on_btnExitGateMode_clicked()
{
  _ui->panelTab->setCurrentIndex(TAB_MAIN);
}

void  Afc::PanelWidget::on_btnExitPanel_clicked()
{
  ActivityManager::getInstance()->setActivityMode(ActivityManager::ActivityMode::OperatingMode);
  hide();
}

void  Afc::PanelWidget::on_btnOperator_clicked()
{
  _ui->panelTab->setCurrentIndex(TAB_LOGIN);
}

void  Afc::PanelWidget::on_btnGateMode_clicked()
{
  _ui->panelTab->setCurrentIndex(TAB_GATE_MODE);
}

void  Afc::PanelWidget::on_btnExitLogin_clicked()
{
  _ui->panelTab->setCurrentIndex(TAB_MAIN);
}

void  Afc::PanelWidget::on_btnLogin_clicked()
{
  if ("" == _ui->txtPassword->text())
  {
    _ui->panelTab->setCurrentIndex(TAB_OPERATOR);
  }
}
}

void  Afc::PanelWidget::on_radioGateMode1_clicked()
{
  setGateMode(1);
}

void  Afc::PanelWidget::on_radioGateMode2_clicked()
{
  setGateMode(2);
}

void  Afc::PanelWidget::on_radioGateMode3_clicked()
{
  setGateMode(3);
}

void  Afc::PanelWidget::on_radioGateMode4_clicked()
{
  setGateMode(4);
}

void  Afc::PanelWidget::on_radioGateMode5_clicked()
{
  setGateMode(5);
}

void  Afc::PanelWidget::on_radioGateMode6_clicked()
{
  setGateMode(6);
}

void  Afc::PanelWidget::on_radioGateMode7_clicked()
{
  setGateMode(7);
}

void  Afc::PanelWidget::on_radioGateMode8_clicked()
{
  setGateMode(8);
}

void  Afc::PanelWidget::on_radioGateMode9_clicked()
{
  setGateMode(9);
}

void  Afc::PanelWidget::on_radioGateModeAuto_clicked()
{
  ActivityManager::getInstance()->setConfigurationMode(ActivityManager::ConfigurationMode::Online);
}

void  Afc::PanelWidget::on_btnSaveIp_clicked()
{
  Utilities::setIpAddress(_ui->txtIp->text());
  _settings.setValue(Settings::QR_SERVER_IP, _ui->txtQrIp->text());
  _settings.sync();
  
  emit qrServerIpChanged(_ui->txtQrIp->text());
}

void  Afc::PanelWidget::setGateMode(quint8 mode)
{
  const auto &AM = ActivityManager::getInstance();

  AM->setConfigurationMode(ActivityManager::ConfigurationMode::Local);
  Scu::getInstance()->setConfiguration(Scu::ConfigurationMask(mode));

  if (ActivityManager::AccessControlGateType::Exit != AM->getAcgType())
  {
    GateController::getInstance()->setConfiguration(mode & Scu::ConfigurationMask::GateConfigMask);
  }
  else
  {
    if (Scu::ConfigBlockedControlled != (mode & Scu::ConfigurationMask::GateConfigMask))
    {
      GateController::getInstance()->setConfiguration(mode & Scu::ConfigurationMask::GateConfigMask);
    }
    else
    {
      GateController::getInstance()->setConfiguration(Scu::ConfigBlockedFree);
    }
  }
}

void  Afc::PanelWidget::on_btnExitParameters_clicked()
{
  _ui->panelTab->setCurrentIndex(TAB_FILE_OPERATIONS);
}

void Afc::PanelWidget::on_btnClearGateCounters_clicked()
{
  GateController::getInstance()->resetCounters("Reader");

  _ui->btnClearGateCounters->setStyleSheet("border: 4px solid black;"
                                           "border-radius: 8px;"
                                           "background-color: rgb(0, 255, 0);");

  QTimer::singleShot(1000, this, SLOT(resetBtnClearGateCounterStyle()));
}
