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

QT_BEGIN_NAMESPACE
namespace Ui { class QGrypho; }
QT_END_NAMESPACE

namespace std
{
  inline auto qHash(const std::string& key, uint seed = 0)
  {
    return qHash(QByteArray::fromRawData(key.data(), key.length()), seed);
  }
}


class QGRYPHO_EXPORT QGrypho : public QWidget
{
  Q_OBJECT
public:
  QGrypho(QWidget *parent = nullptr);
  bool isGraphSet();
  void drawGraph(const Graph *G);
  void updateColoring();
  QString info();
  void highlight(QVector<vertex> vs);
  void highlight(QVector<edge> es);
  bool highlighted(vertex vs);
  bool highlighted(edge es);
  void setDblClick(std::function<void(const Graph*, vertex, bool)>);
  void setSelect(std::function<void(const Graph*, vertex)>);
  void dblClick(Graph* g, vertex v, bool left);
  void select(Graph* g, vertex v);
  QString text;
  ~QGrypho();
  
private:
  QString _info;
  const vertex vertexAt(const QPointF &pos) const;
  QColor makeColor(vertex v);
  std::function<void(const Graph*, vertex)> onselect;
  std::function<void(const Graph*, vertex, bool)> ondblclick;
  int vertex_size = 10;
  int cached_width, cached_height;
  vector<std::pair<double, double>> coords_cache;
  QMap<vertex, QG::Vertex> vertices;
  QMap<edge, QG::Edge> edges;
  vertex moving = "";
  QPointF prev_pos;
  QPointF canvas_center;

  const Graph  *g;
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
