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
#include <QPainterPath>
#include <QStaticText>
#include <QTextDocument>
#include <QToolTip>
#include <functional>
#include "QGVertex.hpp"
#include "QGEdge.hpp"
#include "grypho.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class QGrypho; }
QT_END_NAMESPACE


class QGRYPHO_EXPORT QGrypho : public QWidget
{
  Q_OBJECT
public:
  QGrypho(QWidget *parent = nullptr);
  bool isGraphSet();
  void drawGraph(Graph *G);
  void updateColoring();
  QString info();
  void highlight(QVector<vertex> vs);
  void highlight(QVector<edge> es);
  bool highlighted(vertex vs);
  bool highlighted(edge es);
  void setDblClick(std::function<void(Graph*, int, bool)>);
  void setSelect(std::function<void(Graph*, int)>);
  void dblClick(Graph* g, int v, bool left);
  void select(Graph* g, int v);
  QString text;
  ~QGrypho();
  
private:
  QString _info;
  int vertexAt(const QPointF &pos);
  QColor makeColor(vertex v);
  std::function<void(Graph*, int)> onselect;
  std::function<void(Graph*, int, bool)> ondblclick;
  int vertex_size = 10;
  int cached_width, cached_height;
  vector<std::pair<double, double>> coords_cache;
  QVector<QG::Vertex> vertices;
  QMap<edge, QG::Edge> edges;
  vertex moving = -1;
  QPointF prev_pos;
  QPointF canvas_center;

  Graph      *g;
  Ui::QGrypho *ui;


  void moveVertexTo(const QPointF &pos);
  void moveCanvasTo(const QPointF &pos);

protected:
  bool event(QEvent *event) override;
  void paintEvent(QPaintEvent *) override;
  void mousePressEvent(QMouseEvent *event) override;
  void mouseMoveEvent(QMouseEvent *event) override;
  void mouseReleaseEvent(QMouseEvent *event) override;
  void mouseDoubleClickEvent(QMouseEvent *event) override;
};

#endif // QGRYPHO_H
