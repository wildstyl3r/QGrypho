#include "QGrypho.h"
#include "ui_QGrypho.h"

QGrypho::QGrypho(QWidget *parent):
  QWidget(parent),
  g(nullptr),
  ui(new Ui::QGrypho)
{
  ui->setupUi(this);
}

QGrypho::~QGrypho()
{
  delete ui;
}

void QGrypho::drawGraph(Graph *G){
  if (g != G ||
      width() != cached_width ||
      height() != cached_height ||
      coords_cache.size() == 0){
      g = G;
      cached_height = height();
      cached_width = width();
      coords_cache = g->draw_coords_FR(cached_width, cached_height);
      update();
    }
}

void QGrypho::paintEvent(QPaintEvent *){
  QPainter canvas(this);

  for(vertex v = 0; v < g->V().size(); ++v){
      canvas.setPen(QColor::fromHsl(10, 255, 127));
      auto& v_p = coords_cache[v];
      canvas.drawEllipse(QPointF(v_p.x, v_p.y), vertex_size, vertex_size);
      for(vertex u = v + 1; u < g->V().size(); ++u){
          if (g->V()[v].count(u)){
              auto& u_p = coords_cache[u];
              canvas.drawArc(v_p.x, v_p.y, u_p.x-v_p.x, u_p.y-v_p.y, 1, 1);
            }
        }
    }
}
