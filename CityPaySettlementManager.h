#ifndef CITYPAYSETTLEMENTMANAGER_H
#define CITYPAYSETTLEMENTMANAGER_H

#include <SettlementManager.h>
#include <QByteArray>

class QDate;

namespace Afc
{
class CityPaySettlementManager: public SettlementManager
{
public:
  static QSharedPointer<CityPaySettlementManager>  getInstance()
  {
    static const auto &ins = QSharedPointer<CityPaySettlementManager>(new CityPaySettlementManager());

    return ins;
  }

  ~CityPaySettlementManager() final;

  // TransactionSettlement interface

public:
  void  appendLog(const QSharedPointer<TransactionLog> &log) final;

  void  commit() final;

private:
  CityPaySettlementManager();

  QString  generateFileName(const QDate &date, quint32 counter);

  QString  generateNewFileName();
};
}

#endif // CITYPAYSETTLEMENTMANAGER_H
