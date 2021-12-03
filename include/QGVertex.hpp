#ifndef QGVERTEX_H
#define QGVERTEX_H

#include <QPainterPath>
#include <QColor>

namespace QG {
class Vertex
{
public:
    Vertex(QPointF pos, QColor c, QString t = "", bool h = false);

    static const int defaultRadius = 10;
    static const int highlightedRadius = 30;

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
