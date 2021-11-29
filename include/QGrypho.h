#ifndef QGRYPHO_H
#define QGRYPHO_H

#if defined(QGRYPHO_LIBRARY)
#  define QGRYPHO_EXPORT Q_DECL_EXPORT
#else
#  define QGRYPHO_EXPORT Q_DECL_IMPORT
#endif

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include "grypho.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class QGrypho; }
QT_END_NAMESPACE

class QGRYPHO_EXPORT QGrypho : public QWidget
{
  Q_OBJECT
public:
  QGrypho(QWidget *parent = nullptr);
  void drawGraph(Graph *G);
  ~QGrypho();
  
private:
  int vertex_size;
  int cached_width, cached_height;
  vector<nodesoup::Point2D> coords_cache;
  Graph      *g;
  Ui::QGrypho *ui;

protected:
  void paintEvent(QPaintEvent *) override;
};

#endif // QGRYPHO_H
