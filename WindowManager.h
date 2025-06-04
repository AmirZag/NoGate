#ifndef WINDOWMANAGER_H
#define WINDOWMANAGER_H

#include <QtGlobal>
#include <ActivityManager.h>
#include <Scu.h>


class QDateTime;

namespace Afc
{
class WindowManager
{
public:
  enum class MessageMode
  {
    Normal,
    Green,
    Yellow,
    Red,
  };

  virtual ~WindowManager()
  {
  }

  virtual void  setDate(const QDateTime &date) = 0;

  virtual void  setStationId(quint32 id) = 0;

  virtual void  setReaderId(quint32 id) = 0;

  virtual void  setAppVersion(quint8 major, quint8 minor, quint8 revision, quint8 patch) = 0;

  virtual void  setFamilleVersion(quint32 version) = 0;

  virtual void  setGateMode(ActivityManager::AccessControlGateType acgType, Scu::ConfigurationMask configurationMode) = 0;

  virtual void  setGateStatus(bool status) = 0;

  virtual void  setScuStatus(bool status) = 0;

  virtual void  setQrStatus(bool status, bool enabled) = 0;

  virtual void  setRemainingActFiles(quint32 count) = 0;

  virtual void  setRfStatus(quint32 status) = 0;

  virtual void  setSamStatus(quint32 citypay, quint32 cityway) = 0;

  virtual void  setSamModuleError(const QString &err) = 0;

  virtual void  setSamCityPayError(const QString &err) = 0;

  virtual void  setSamCityWayError(const QString &err) = 0;

  virtual void  setDbStatus(quint32 status) = 0;

  virtual void  setStaticMsg(const QString &msg, MessageMode mode) = 0;

  virtual void  setTransactionMsg(const QString &msg, MessageMode mode) = 0;
};
}

#endif // WINDOWMANAGER_H
