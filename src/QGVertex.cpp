#include "QGVertex.hpp"

QG::Vertex::Vertex(QPointF pos, QColor c, QString t, bool h)
    : _position(pos), _color(c), _tooltip(t), _highlighted(h) {
    _path.addEllipse(QPointF(), Vertex::defaultRadius, Vertex::defaultRadius);
}
void QG::Vertex::setPath(const QPainterPath &path) { _path = path; }
void QG::Vertex::setToolTip(const QString &toolTip) { _tooltip = toolTip; }
void QG::Vertex::setPosition(const QPointF &position) {_position = position; }
void QG::Vertex::setColor(const QColor &color) { _color = color; }
void QG::Vertex::highlight(bool h) {
    _highlighted = h;
    if (h){
        _path.clear();
        _path.addEllipse(QPointF(), Vertex::highlightedRadius, Vertex::highlightedRadius);
    } else {
        _path.clear();
        _path.addEllipse(QPointF(), Vertex::defaultRadius, Vertex::defaultRadius);
    }

}

QPainterPath QG::Vertex::path() const { return _path; }
QPointF QG::Vertex::position() const { return _position; }
QColor QG::Vertex::color() const { return _color; }
QString QG::Vertex::toolTip() const { return _tooltip; }
bool QG::Vertex::highlighted() const { return _highlighted; }

