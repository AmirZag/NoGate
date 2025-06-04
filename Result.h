#ifndef RESULT_H
#define RESULT_H

#include <QString>


namespace Afc
{
struct Result
{
  explicit Result(bool isSucceed = false, const QString &resultMsg = QString())
  {
    succeed = isSucceed;
    message = resultMsg;
  }

  operator bool() const
  {
    return succeed;
  }

  bool  operator!() const
  {
    return !succeed;
  }

  bool     succeed;
  QString  message;
};
}

#endif // RESULT
