#include "QGrypho.hpp"

#include <QApplication>
#include <QFileDialog>
#include <iostream>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  QGrypho qg, aqg;
  auto f = QFileDialog::getOpenFileName(&qg,
                                        "Open Graph",
                                        "~/",
                                        "Graph Files (*.tgf *.dot *.cgf *.*)").toStdString();
  if (f != "") {
      Graph g(f);
      Graph ag = !g;
      qg.drawGraph(&g);
      qg.show();
      aqg.drawGraph(&ag);
      aqg.show();
      return a.exec();
  } else {
      return 0;
  }
}
