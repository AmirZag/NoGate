#ifndef QFOCUSABLELINEEDIT_H
#define QFOCUSABLELINEEDIT_H

#include <QLineEdit>


namespace Afc
{
class QFocusableLineEdit: public QLineEdit
{
  Q_OBJECT

public:
  QFocusableLineEdit(QWidget *parent = 0);

  ~QFocusableLineEdit();

signals:
  void          focused();

protected:
  virtual void  mouseReleaseEvent(QMouseEvent *);
};
}

#endif // QFOCUSABLELINEEDIT_H
