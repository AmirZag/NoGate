#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <WindowManager.h>
#include <QMainWindow>
#include <QSettings>
#include <QSet>


namespace Afc
{
namespace Ui
{
class MainWindow;
}

class PanelWidget;
class HoneywellBarcodeReader;
class CardReader;
class Business;

class MainWindow: public QMainWindow, public WindowManager
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);

  ~MainWindow();

  void  saveSettings();

  void  loadSettings();


  // WindowManager interface

public:
  void  setDate(const QDateTime &date)              final;

  void  setStationId(quint32 id)                    final;

  void  setReaderId(quint32 id)                     final;

  void  setAppVersion(quint8 major, quint8 minor, quint8 revision, quint8 patch)       final;

  void  setFamilleVersion(quint32 version)             final;

  void  setGateMode(ActivityManager::AccessControlGateType acgType, Scu::ConfigurationMask configurationMode) final;

  void  setGateStatus(bool status)                   final;

  void  setScuStatus(bool status)                   final;

  void  setDbStatus(quint32 status) final;

  void  setQrStatus(bool status, bool enabled)                    final;

  void  setRemainingActFiles(quint32 count)         final;

  void  setRfStatus(quint32 status)                  final;

  void  setSamStatus(quint32 citypay, quint32 cityway)                 final;

  void  setSamModuleError(const QString &err) final;

  void  setSamCityPayError(const QString &err) final;

  void  setSamCityWayError(const QString &err) final;

  void  setStaticMsg(const QString &msg, MessageMode mode) final;

  void  setTransactionMsg(const QString &msg, MessageMode mode) final;

private slots:
  void  onTimerWindowMgr();

  void  onBtnPanelClicked();

  void  onQrServerIpChanged(const QString &ip);

private:
  void  initWm();

private:
  QScopedPointer<Ui::MainWindow>          _ui;
  QScopedPointer<PanelWidget>             _panelSettings;
  QSettings                               _settings;
  QFile                                   _settingsCityPayKey;
  QScopedPointer<QTimer>                  _timerWindowMgr;
  QString                                 _oldStyle;
  qint64                                  _lastTransactionMsgTime;
  QString                                 _gateControllerPortName;
  QString                                 _cardReaderPortName;
  QSharedPointer<CardReader>              _cardReader;
  QSharedPointer<HoneywellBarcodeReader>  _barcodeReader;
  QSharedPointer<Business>                _business;
  quint8                                  _citypaySamSlot;
  QByteArray                              _citypaySamKey;

  // WindowManager.
  QString      _wmStationId;
  QString      _wmReaderId;
  QString      _wmAppVersion;
  QString      _wmFamilleVersion;
  QString      _wmGateMode;
  QString      _wmGateStatus;
  QString      _wmScuStatus;
  QString      _wmDbStatus;
  QString      _wmQrStatus;
  QString      _wmRemainingActFile;
  QString      _wmRfStatus;
  QString      _wmSamCitypayStatus;
  QString      _wmSamCitywayStatus;
  QString      _wmStaticMsg;
  MessageMode  _wmStaticMsgMode;
};
}

#endif // MAINWINDOW_H
