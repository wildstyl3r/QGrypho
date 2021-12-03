#ifndef QGEDGE_H
#define QGEDGE_H

#include "grypho.hpp"

namespace QG {
class Edge
{
public:
    Edge(const vertex v, const vertex u, bool h = false) : _v(v), _u(u), _highlighted(h) {}
    Edge() : _v(0), _u(0), _highlighted(false) {}

    void highlight(bool h = true) { _highlighted = h; }
    void setVertices(const vertex v, const vertex u)
    {
        _v = v;
        _u = u;
    }
    
    vertex v() { return _v; }
    vertex u() { return _u; }
    bool highlighted() { return _highlighted; }
    
private:
    vertex _v, _u;
    bool _highlighted;
};
}

#endif // QGEDGE_H
