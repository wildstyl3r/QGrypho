#include "QGrypho.h"

#include <QApplication>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  QGrypho qg;
  qg.show();
  return a.exec();
}
