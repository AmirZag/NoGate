#ifndef BUZZERCONTROLLER_H
#define BUZZERCONTROLLER_H

#include <QSharedPointer>


namespace Afc
{
class BuzzerController
{
public:
  static QSharedPointer<BuzzerController>  getInstance()
  {
    static const auto &ins = QSharedPointer<BuzzerController>(new BuzzerController());

    return ins;
  }

  void  singleBeep(quint32 msec = 200, quint32 freq = 1000);

  void  tripleBeep(quint32 msec = 100, quint32 freq = 1000);

  void  beep(quint32 count, quint32 msec = 100, quint32 freq = 1000);

private:
  BuzzerController();
};
}

#endif // BUZZERCONTROLLER_H
