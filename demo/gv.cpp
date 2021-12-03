#include "QGrypho.hpp"

#include <QApplication>
#include <QFileDialog>
#include <iostream>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  QGrypho qg;
  auto f = QFileDialog::getOpenFileName(&qg,
                                        "Open Graph",
                                        "~/",
                                        "Graph Files (*.tgf *.dot)").toStdString();
  if (f != "") {
      Graph g(f);
      qg.drawGraph(&g);
      qg.show();
      return a.exec();
  } else {
      return 0;
  }
}
