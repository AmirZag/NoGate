#include <BuzzerController.h>
#include <GpioSunXi.h>
#include <thread>

#define PI06   SUNXI_GPI(6)


namespace Afc
{
static void  internalBeep(quint32 msec, quint32, quint32 count, quint32 offsetMsec)
{
  using namespace std::chrono;

  for (quint32 i = 0; i < count; ++i)
  {
      sunxi_gpio_output(PI06, 1);
      std::this_thread::sleep_for(milliseconds(msec));
      sunxi_gpio_output(PI06, 0);

    if (i < count - 1)
    {
      sunxi_gpio_output(PI06, 0);
      std::this_thread::sleep_for(milliseconds(offsetMsec));
    }
  }

  sunxi_gpio_output(PI06, 0);
}

BuzzerController::BuzzerController()
{
  sunxi_gpio_init();
  sunxi_gpio_set_cfgpin(PI06, SUNXI_GPIO_OUTPUT);
}

void  BuzzerController::singleBeep(quint32 msec, quint32 freq)
{
  std::thread  th(internalBeep, msec, freq, 1, 0);

  th.detach();
}

void  BuzzerController::tripleBeep(quint32 msec, quint32 freq)
{
  std::thread  th(internalBeep, msec, freq, 3, 100);

  th.detach();
}

void BuzzerController::beep(quint32 count, quint32 msec, quint32 freq)
{
  std::thread  th(internalBeep, msec, freq, count, 150);

  th.detach();
}
}
