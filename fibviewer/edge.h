#ifndef EDGE_H
#define EDGE_H

#include <QList>
#include <QVector3D>

#include <QGLWidget>

class Edge
{
public:
    Edge(QVector3D fn, QVector3D tn);
    QVector3D fn, tn;
    QList<QVector3D> points;

    void paintGL();
    void glVertex(QVector3D v);
    double length();
    double minDist(QVector3D p1, QVector3D p2, QVector3D p);
    double minDist(QVector3D p);
};

#endif // EDGE_H
