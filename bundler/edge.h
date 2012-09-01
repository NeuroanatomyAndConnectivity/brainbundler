#ifndef EDGE_H
#define EDGE_H

#include <QList>
#include <QVector3D>

class Edge
{
public:
    Edge(QVector3D fn, QVector3D tn);
    QVector3D fn, tn;
    QList<QVector3D> points;
    QList<QVector3D> forces;

    void subdivide(int newp);
    void attract();
    void applyForces();
    bool flip(Edge* other);
    double length();
    double segLength(int n);
};

#endif // EDGE_H
