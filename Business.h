#ifndef BUSINESS_H
#define BUSINESS_H


class QString;


namespace Afc
{
class Business
{
public:
  virtual ~Business()
  {
  }

  virtual void  start() = 0;

  virtual void  stop() = 0;
};
}

#endif // BUSINESS_H
