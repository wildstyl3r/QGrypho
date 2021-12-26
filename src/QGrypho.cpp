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
  pal.setColor(QPalette::Window, QColor(23,27,33));
  setPalette(pal);
}

QGrypho::~QGrypho()
{
  delete ui;
}

void QGrypho::drawGraph(const Graph *G)
{
    g = G;
    cached_height = height();
    cached_width = width();

    nodesoup::adj_list_t g_v(g->size());
    map<vertex, size_t> num;
    for (auto& [v, _] : g->V()){
        size_t n = num.size();
        num[v] = n;
    }
    for (auto& [v, _] : g->V()){
        for (auto& [u, _] : g->V())
            if (g->has({v,u})){
                g_v[num[v]].push_back(num[u]);
            }
    }
    auto coords = nodesoup::fruchterman_reingold(g_v, cached_width, cached_height);
    vertices.clear();

    int total_colors = g->count_colors();
    for (auto& [v,_] : g->V()){
        vertices[v] = QG::Vertex(
                QPointF(coords[num[v]].x, coords[num[v]].y),
                makeColor(v),
                QString::fromStdString(v + " (" + std::to_string(g->color(v)) + (g->label(v) != "" ? ") " + g->label(v) : ")")));
    }

    edges.clear();
    QSet<edge> e_xists;
    for (auto& [v, _] : g->V()){
        for (auto& [u, _] : g->V()){
            edge e = {std::min(u,v), std::max(u,v)};
            if (g->has({v, u}) && !e_xists.contains(e)){
                edges[e] = QG::Edge(e.first, e.second, false, g->weight(e));
                e_xists.insert(e);
            }
        }
    }
    _info = "вершин: " + QString::number(vertices.size()) + "\n" +
            "ребер: " + QString::number(edges.size()) + "\n" +
            "цветов: " + QString::number(total_colors);


  update();
}

QColor QGrypho::makeColor(vertex v) {
    if (g->has(v)){
        int total_colors = g->count_colors();
        return QColor::fromHsl((360 / total_colors * g->color(v)) % 360, (128 + 127 / (g->color(v) % 3 + 1)) % 256, (80 + 95 / (3 - g->color(v) % 3)) % 256);
    }
    return Qt::black;
}

void QGrypho::updateColoring()
{
    for (auto& [v, _] : g->V()){
        vertices[v].setColor(makeColor(v));
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
      canvas.translate(canvas_center);

      double scale_factor = std::min((double)current_width / cached_width, (double)current_height / cached_height);

      QPen normal(QColor(127,127,127)), highlighted(QColor(220,180,127));
      highlighted.setWidth(2);
      for (QG::Edge& e : edges){
          if (e.highlighted()) {
              canvas.setPen(highlighted);
          } else {
              canvas.setPen(normal);
          }
          QPointF v_scr = vertices[e.v()].position() * scale_factor;
          QPointF u_scr = vertices[e.u()].position() * scale_factor;
          canvas.drawLine(u_scr, v_scr);
          if (e.weight() != 1){
              canvas.drawText((u_scr+v_scr)/2, QString::number(e.weight()));
          }
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
      canvas.translate(-canvas_center);
      canvas.translate(-current_width/2, -current_height/2);
      canvas.setPen(Qt::white);
      canvas.drawText(QRectF(10, 10, width(), height()), Qt::AlignLeft, _info+"\n"+text);
    }
}

void QGrypho::highlight(QVector<vertex> vs)
{
    for(QG::Vertex& v : vertices){
        v.highlight(false);
    }
    for(vertex& v : vs){
        if (vertices.count(v)){
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
        vertex index = vertexAt(helpEvent->pos());
        if (index != "") {
            QToolTip::showText(helpEvent->globalPos(), vertices[index].toolTip());
        } else {
            QToolTip::hideText();
            event->ignore();
        }

        return true;
    }
    return QWidget::event(event);
}

const vertex QGrypho::vertexAt(const QPointF &pos) const
{
    int current_height = height();
    int current_width = width();
    double scale_factor = std::min((double)current_width / cached_width, (double)current_height / cached_height);

    for (vertex& k : vertices.keys()) {
        //const QG::Vertex &v = vertices[i];
        if (vertices[k].path().contains(pos - canvas_center - QPointF(current_width/2, current_height/2) - vertices[k].position() * scale_factor))
            return k;
    }
    return "";
}

void QGrypho::mousePressEvent(QMouseEvent *event){
    if (event->button() == Qt::LeftButton) {
        vertex v = vertexAt(event->pos());
        if (v != "") {
            moving = v;
        }
        prev_pos = event->pos();
        update();
    } else if (event->button() == Qt::RightButton) {
        vertex v = vertexAt(event->pos());
        if (onselect) onselect(g, v);
        update();
    }
}

void QGrypho::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton){
        if(moving != ""){
            moveVertexTo(event->pos());
        } else {
            moveCanvasTo(event->pos());
        }
    }
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

void QGrypho::moveCanvasTo(const QPointF &pos)
{
    QPointF offset = pos - prev_pos;
    canvas_center += offset;
    prev_pos = pos;
    update();
}

void QGrypho::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && moving != "") {
        moveVertexTo(event->pos());
        moving = "";
    }
}

void QGrypho::mouseDoubleClickEvent(QMouseEvent *event)
{
    vertex target = vertexAt(event->pos());
    if (target == ""){
        canvas_center = QPointF();
    } else {
        if (ondblclick) ondblclick(g, target, event->button() == Qt::LeftButton);
    }
}

void QGrypho::setDblClick(std::function<void (const Graph*, vertex, bool)> f)
{
    ondblclick = f;
}

void QGrypho::setSelect(std::function<void (const Graph*, vertex)> f)
{
    onselect = f;
}

bool QGrypho::highlighted(vertex v)
{
    if(vertices.count(v)){
        return vertices[v].highlighted();
    }
    return false;
}

bool QGrypho::highlighted(edge e)
{
    if(edges.contains({std::min(e.first, e.second), std::max(e.first, e.second)})){
        return  edges[e].highlighted();
    }
    return false;
}

bool QGrypho::isGraphSet(){
    return g != nullptr;
}

void QGrypho::select(Graph * g, vertex v){
    if (onselect) onselect(g, v);
}

void QGrypho::dblClick(Graph *g, vertex v, bool left){
    if (ondblclick) ondblclick(g, v, left);
}
