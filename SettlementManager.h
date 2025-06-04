#ifndef SETTLEMENTMANAGER
#define SETTLEMENTMANAGER

#include <QSharedPointer>


namespace Afc
{
class TransactionLog;

class SettlementManager
{
public:
  virtual ~SettlementManager()
  {
  }

  virtual void        appendLog(const QSharedPointer<TransactionLog> &log) = 0;

  virtual void        commit() = 0;
};
}

#endif // SETTLEMENTMANAGER
