#include <QFocusableLineEdit.h>

namespace Afc
{
QFocusableLineEdit::QFocusableLineEdit(QWidget *parent):
  QLineEdit(parent)
{
}

QFocusableLineEdit::~QFocusableLineEdit()
{
}

void  QFocusableLineEdit::mouseReleaseEvent(QMouseEvent *e)
{
  QLineEdit::mouseReleaseEvent(e);
  emit(focused());
}
}
