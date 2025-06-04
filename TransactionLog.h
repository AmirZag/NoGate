#ifndef TRANSACTIONLOG_H
#define TRANSACTIONLOG_H

#include <QByteArray>


namespace Afc
{
class TransactionLog
{
public:
  virtual ~TransactionLog()
  {
  }

  virtual QByteArray serialize() const = 0;
};
}

#endif // TRANSACTIONLOG_H
