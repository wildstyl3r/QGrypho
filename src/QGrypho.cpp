#include "QGrypho.hpp"
#include "ui_QGrypho.h"
#include "nodesoup.hpp"

QGrypho::QGrypho(QWidget *parent):
  QWidget(parent),
  g(nullptr),
  ui(new Ui::QGrypho)
{
  ui->setupUi(this);
  setMouseTracking(true);
  setAutoFillBackground(true);
  QPalette pal(palette());
  pal.setColor(QPalette::Background, QColor(23,27,33));
  setPalette(pal);
}

QGrypho::~QGrypho()
{
  delete ui;
}

void QGrypho::drawGraph(Graph *G)
{
    g = G;
    cached_height = height();
    cached_width = width();

    nodesoup::adj_list_t g_v(g->V().size());
    for (vertex v = 0; v < g->V().size(); ++v){
        for (vertex u : g->V()[v]){
            g_v[v].push_back(u);
        }
    }
    auto coords = nodesoup::fruchterman_reingold(g_v, cached_width, cached_height);
    vertices.clear();
    vertices.reserve(coords.size());

    int color_delta = 360 / g->count_colors();
    for (size_t i = 0; i < coords.size(); ++i){
        vertices.push_back(QG::Vertex(
                               QPointF(coords[i].x, coords[i].y),
                               QColor::fromHsl(color_delta * g->color(i), 255, 127),
                               QString::fromStdString(g->id(i) + (g->label(i) != "" ? ' ' + g->label(i) : ""))));
    }

    edges.clear();
    QSet<edge> e_xists;
    for(vertex v = 0; v < g->V().size(); ++v){
        for(vertex u = 0; u < g->V().size(); ++u){
            edge e = {std::min(u,v), std::max(u,v)};
            if (g->V()[v].count(u) && !e_xists.contains(e)){
                edges[e] = QG::Edge(e.first, e.second);
                e_xists.insert(e);
            }
        }
    }


  update();
}

void QGrypho::updateColoring()
{
    int color_delta = 360 / g->count_colors();
    for (size_t i = 0; i < vertices.size(); ++i){
        vertices[i].setColor(QColor::fromHsl(color_delta * g->color(i), 255, 127));
    }
    update();
}

void QGrypho::paintEvent(QPaintEvent *)
{
  QPainter canvas(this);
  canvas.setRenderHint(QPainter::Antialiasing);
  if(g != nullptr){
      int current_height = height();
      int current_width = width();

      canvas.translate(current_width / 2, current_height / 2);

      double scale_factor = std::min((double)current_width / cached_width, (double)current_height / cached_height);

      QPen normal(QColor(127,127,127)), highlighted(QColor(220,180,127));
      highlighted.setWidth(2);
      for (QG::Edge& e : edges){
          if (e.highlighted()) {
              canvas.setPen(highlighted);
          } else {
              canvas.setPen(normal);
          }
          canvas.drawLine(vertices[e.u()].position() * scale_factor, vertices[e.v()].position() * scale_factor);
      }

      for(QG::Vertex& v : vertices){
          QPointF current = v.position() * scale_factor;

          canvas.translate(current);

          QRadialGradient point_gradient (QPointF(), v.highlighted() ? QG::Vertex::highlightedRadius : QG::Vertex::defaultRadius);
          point_gradient.setColorAt(0, v.color());
          point_gradient.setColorAt(1, QColor(0,0,0,0));
          canvas.setBrush(point_gradient);
          canvas.setPen(QColor(0,0,0,0));
          canvas.drawPath(v.path());

          canvas.translate(-current);
        }
      canvas.translate(-cached_width/2, -cached_height/2);
    }
}

void QGrypho::highlight(QVector<vertex> vs)
{
    for(QG::Vertex& v : vertices){
        v.highlight(false);
    }
    for(vertex& v : vs){
        if (0 <= v && v <= vertices.size()){
            vertices[v].highlight(true);
        }
    }
}

void QGrypho::highlight(QVector<edge> es)
{
    for(QG::Edge& e : edges){
        e.highlight(false);
    }
    for(edge& e : es){
        if (edges.contains({std::min(e.first, e.second), std::max(e.first, e.second)})){
            edges[{std::min(e.first, e.second), std::max(e.first, e.second)}].highlight(true);
        }
    }
}

bool QGrypho::event(QEvent *event)
{
    if(event->type() == QEvent::ToolTip){
        QHelpEvent *helpEvent = static_cast<QHelpEvent *>(event);
        int index = vertexAt(helpEvent->pos());
        if (index != -1) {
            QToolTip::showText(helpEvent->globalPos(), vertices[index].toolTip());
        } else {
            QToolTip::hideText();
            event->ignore();
        }

        return true;
    }
    return QWidget::event(event);
}

int QGrypho::vertexAt(const QPointF &pos)
{
    int current_height = height();
    int current_width = width();
    double scale_factor = std::min((double)current_width / cached_width, (double)current_height / cached_height);

    for (int i = vertices.size() - 1; i >= 0; --i) {
        const QG::Vertex &v = vertices[i];
        if (v.path().contains(pos - QPointF(current_width/2, current_height/2) - v.position() * scale_factor))
            return i;
    }
    return -1;
}

void QGrypho::mousePressEvent(QMouseEvent *event){
    if (event->button() == Qt::LeftButton) {
        int index = vertexAt(event->pos());
        if (index != -1) {
            moving = index;
            prev_pos = event->pos();
            update();
        }
    } else if (event->button() == Qt::RightButton) {
        int index = vertexAt(event->pos());
        if (onselect) onselect(index);
        update();
    }
}

void QGrypho::mouseMoveEvent(QMouseEvent *event)
{
    if ((event->buttons() & Qt::LeftButton) && moving != -1)
        moveVertexTo(event->pos());
}

void QGrypho::moveVertexTo(const QPointF &pos)
{
    int current_height = height();
    int current_width = width();
    double scale_factor = std::min((double)current_width / cached_width, (double)current_height / cached_height);

    QPointF offset = pos - prev_pos;
    vertices[moving].setPosition(vertices[moving].position() + offset/scale_factor);
    prev_pos = pos;
    update();
}

void QGrypho::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && moving != -1) {
        moveVertexTo(event->pos());
        moving = -1;
    }
}

void QGrypho::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (ondblclick) ondblclick(vertexAt(event->pos()), event->button() == Qt::LeftButton);
}

void QGrypho::setDblClick(std::function<void (int, bool)> f)
{
    ondblclick = f;
}

void QGrypho::setSelect(std::function<void (int)> f)
{
    onselect = f;
}
