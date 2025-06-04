#include <MainWindow.h>
#include <QApplication>
#include <QFile>
#include <QFontDatabase>
#include <QTextCodec>
#include <QDir>
#include <Utilities.h>


int  main(int argc, char *argv[])
{
  QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
  QApplication  a(argc, argv);
  a.setOverrideCursor(QCursor(QPixmap(1, 1)));
  QFontDatabase::addApplicationFont(":/Fonts/LiberationSerif-Regular");
  QFontDatabase::addApplicationFont(":/Fonts/Sahel-FD.ttf");
  QFontDatabase::addApplicationFont(":/Fonts/Sahel-Black-FD.ttf");
  QDir::setCurrent("/root");
  Afc::Utilities::initUsers();

  Afc::MainWindow  w;
  w.showMaximized();

  QCursor::setPos(0, 0);

  return a.exec();
}
