#ifndef QGVERTEX_H
#define QGVERTEX_H
#include <grypho.hpp>
#include <QPainterPath>
#include <QColor>

namespace QG {
class Vertex
{
public:
    Vertex(QPointF pos = QPointF(), QColor c = Qt::black, QString t = "", bool h = false);

    static const int defaultRadius = 10;
    static const int highlightedRadius = 50;

    void setPath(const QPainterPath &path);
    void setToolTip(const QString &toolTip);
    void setPosition(const QPointF &position);
    void setColor(const QColor &color);
    void highlight(bool h = true);
    
    QPainterPath path() const;
    QPointF position() const;
    QColor color() const;
    QString toolTip() const;
    bool highlighted() const;
    
private:
    QPainterPath _path;
    QPointF _position;
    QColor _color;
    QString _tooltip;
    bool _highlighted;
};
}

#endif // QGVERTEX_H
