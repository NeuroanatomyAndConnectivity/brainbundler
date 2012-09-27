#ifndef CONNECTIONS_H
#define CONNECTIONS_H

#include <QString>
#include <QList>
#include "edge.h"
#include <QVector3D>
#include "primitive.h"

#include "segment.h"

#include <qfile.h>
#include <qtextstream.h>

class Connections
{
public:
    Connections();
    Connections(QString nname, QString ename);
    Connections(QString fib);
    void paintPoints();
    void paintGL();
    void sortCons();
    void writeVTK();
    void writeOBJ(QString filename);
    void writeBinaryVTK(QString filename);
    void writeCSVs();
    QVector3D piv;
    QVector3D max, min;
    QList<QVector3D> nodes;
    QList<Edge*> edges;
    double vis_c(Edge* e1, Edge* e2);
    QList<Primitive*> prims;
    void createPrims();
    void calculateBounds();
};

#endif // CONNECTIONS_H
