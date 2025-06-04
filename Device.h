#ifndef DEVICE_H
#define DEVICE_H

#include <Result.h>


class QByteArray;

namespace Afc
{
class Device
{
public:
  virtual ~Device()
  {
  }

  virtual Result  initializeDevice() = 0;

  virtual Result  restartDevice() = 0;

  virtual Result  getVersion(QByteArray *version) = 0;
};
}

#endif // DEVICE_H
