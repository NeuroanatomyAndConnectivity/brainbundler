#include "edge.h"

#include <QtDebug>

Edge::Edge(QVector3D fn, QVector3D tn)
{
    this->fn = fn;
    this->tn = tn;
    points << fn << tn;
    QVector3D nv(0,0,0);
    forces << nv << nv;
}

void Edge::subdivide(int newp){

    //newp = number of subdivision points, excluding start and end!

    QList<QVector3D> newpoints;
    newpoints << points.first();
    QList<QVector3D> newforces;
    newforces << forces.first();

    double polyLength = 0;
    for (int i=0; i < points.length()-1; i++){
        polyLength += segLength(i);
    }
    double newl = polyLength / (double)(newp+1);
    int seg = 0;
    double lengthSoFar = 0;

    QVector3D p1 = points.at(0);
    QVector3D p2 = points.at(1);

    for (int j = 0; j<newp; j++) {
        while (segLength(seg) < newl*(j+1)-lengthSoFar) {
            lengthSoFar += segLength(seg);
            seg++;
            p1 = points.at(seg);
            p2 = points.at(seg+1);
        }
        double ib = newl *(j+1)-lengthSoFar;
        QVector3D newpoint = p1+ib*((p2-p1).normalized());
        newpoints << newpoint;
        QVector3D nv(0,0,0);
        newforces << nv;

    }
    newpoints << points.last();
    newforces << forces.last();

    points = newpoints;
    forces = newforces;
}

double Edge::segLength(int n){
    //length of (n+1)th segment (n=0 is the first one...)
    QVector3D p1 = points.at(n);
    QVector3D p2 = points.at(n+1);
    return (p1-p2).length();
}

bool Edge::flip(Edge* other) {
    //minimizes sum of connections between endpoints...
    QVector3D ff = this->fn - other->fn;
    QVector3D tt = this->tn - other->tn;

    QVector3D ft = this->fn - other->tn;
    QVector3D tf = this->tn - other->fn;

    bool flip = ff.length()+tt.length() < ft.length()+tf.length();
    return flip;
}

double Edge::length(){
    return (fn-tn).length();
}

void Edge::applyForces(){
    for (int i=0; i<points.size(); i++){
        points.replace(i,points.at(i)+forces.at(i));
    }
}
