#ifndef PANELWIDGET_H
#define PANELWIDGET_H

#include <QWidget>
#include <QTimer>
#include <ActivityManager.h>
#include <Scu.h>
#include <QSettings>


class QOnScreenKeyboard;

namespace Afc
{
namespace Ui
{
class PanelWidget;
}

class PanelWidget: public QWidget
{
  Q_OBJECT

public:
  explicit PanelWidget(QWidget *parent = 0);

  ~PanelWidget();

  void  setGateUi(bool isAuto, Scu::ConfigurationMask mode);

  void  setAccessControlUi(ActivityManager::AccessControlGateType acgType);

  void  setSamModuleError(const QString &err);

  void  setSamCityPayError(const QString &err);

  void  setSamCityWayError(const QString &err);

  QString getQrServerIp() const;

signals:
  void qrServerIpChanged(const QString &ip);

private slots:
  void  resetBtnSamKeyStyle();

  void  resetBtnClearGateCounterStyle();

  void  showKeyboard();

  void  onTimerTimeout();

  void  on_btnExitOperator_clicked();

  void  on_btnIpSettings_clicked();

  void  on_btnDateSettings_clicked();

  void  on_btnGateSettings_clicked();

  void  on_btnSamSettings_clicked();

  void  on_btnFileOperations_clicked();

  void  on_btnExitIpSettings_clicked();

  void  on_btnExitDateSettings_clicked();

  void  on_btnSaveDate_clicked();

  void  on_btnExitGateSettings_clicked();

  void  on_btnExitSamSettings_clicked();

  void  on_btnSaveSam_clicked();

  void  on_btnExitFileOperations_clicked();

  void  on_btnParameters_clicked();

  void  on_btnTransactions_clicked();

  void  on_btnLogs_clicked();

  void  on_btnExitTransactions_clicked();

  void  on_btnSaveTransactions_clicked();

  void  on_btnExitLog_clicked();

  void  on_btnSaveLogs_clicked();

  void  on_btnExitGateMode_clicked();

  void  on_btnExitPanel_clicked();

  void  on_btnOperator_clicked();

  void  on_btnGateMode_clicked();

  void  on_btnExitLogin_clicked();

  void  on_btnLogin_clicked();

  void  on_radioGateMode1_clicked();

  void  on_radioGateMode2_clicked();

  void  on_radioGateMode3_clicked();

  void  on_radioGateMode4_clicked();

  void  on_radioGateMode5_clicked();

  void  on_radioGateMode6_clicked();

  void  on_radioGateMode7_clicked();

  void  on_radioGateMode8_clicked();

  void  on_radioGateMode9_clicked();

  void  on_radioGateModeAuto_clicked();

  void  on_btnSaveIp_clicked();

  void  on_btnExitParameters_clicked();

  void on_btnClearGateCounters_clicked();

private:
  void  setGateMode(quint8 mode);

private:
  Ui::PanelWidget                   *_ui;
  QScopedPointer<QOnScreenKeyboard>  _touchKeyboard;
  QTimer                             _timer;
  QSettings                          _settings;
  QFile                              _settingsSamKey;
};
}

#endif // PANELWIDGET_H
