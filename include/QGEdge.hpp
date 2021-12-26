#ifndef QGEDGE_H
#define QGEDGE_H

#include "grypho.hpp"

namespace QG {
class Edge
{
public:
    Edge(const vertex v, const vertex u, bool h = false, double weight = 0) : _v(v), _u(u), _highlighted(h), _weight(weight) {}
    Edge() : _v(""), _u(""), _highlighted(false) {}

    void highlight(bool h = true) { _highlighted = h; }
    void setVertices(const vertex v, const vertex u)
    {
        _v = v;
        _u = u;
    }
    void setWeight(const double w){
        _weight = w;
    }
    
    vertex v() { return _v; }
    vertex u() { return _u; }
    double weight() { return _weight; }
    bool highlighted() { return _highlighted; }
    
private:
    vertex _v, _u;
    bool _highlighted;
    double _weight;
};
}

#endif // QGEDGE_H
