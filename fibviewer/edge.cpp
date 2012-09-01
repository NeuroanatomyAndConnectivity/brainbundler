#include "edge.h"

#include <QtDebug>

Edge::Edge(QVector3D fn, QVector3D tn)
{
    this->fn = fn;
    this->tn = tn;
    points << fn << tn;
}

void Edge::paintGL() {

    glPointSize(2);
    glColor3f(1.0,0.0,0.0);
    glBegin(GL_POINTS);
    glVertex(fn);
    glVertex(tn);
    glEnd();

    glColor4f(0.5,0.0,0.0,1);
    glLineWidth(3);
    glPointSize(6);

    for (int i=0; i<points.length()-1; i++){
        QVector3D p1 = points.at(i);
        QVector3D p2 = points.at(i+1);

        glBegin(GL_POINTS);
        if (i!=0) glVertex(p1);
        glEnd();

        glBegin(GL_LINES);
        QVector3D nor = p1-p2;
        glNormal3f(nor.x(),nor.y(),nor.z());
        glVertex(p1);
        glVertex(p2);
        glEnd();

        glBegin(GL_LINES);
        glVertex(p1);
        glVertex(p2);
        glEnd();
    }
}

void Edge::glVertex(QVector3D v){
    glVertex3f(v.x(),v.y(),v.z());
}

double Edge::length(){
    return (fn-tn).length();
}

double Edge::minDist(QVector3D p1, QVector3D p2, QVector3D p){
    double l2 = (p1-p2).lengthSquared();
    if (l2 == 0.0) return (p-p1).length();
    QVector3D bla1 = p-p1;
    QVector3D bla2 = p2-p1;
    double t = QVector3D::dotProduct(bla1,bla2)/l2;
    if (t < 0.0) return (p1-p).length();
    else if (t > 1.0) return (p2-p).length();
    return (p-(p1+t*(p2-p1))).length();
}

double Edge::minDist(QVector3D p){
    double minD;
    for (int i = 0; i<points.length()-1;i++){
        double distSeg = minDist(points.at(i),points.at(i+1),p);
        if (i==0) minD = distSeg;
        if (distSeg<minD) minD = distSeg;
    }
    return minD;
}
