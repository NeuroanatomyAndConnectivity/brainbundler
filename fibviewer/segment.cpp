#include "segment.h"
#include "glwidget.h"

#include <QtDebug>

Segment::Segment(QVector3D *p1, QVector3D *p2) {
    this->p1 = p1;
    this->p2 = p2;
}

void Segment::paintGL(){
    glBegin(GL_LINES);
    QVector3D nor = *p1-*p2;
    glNormal3f(nor.x(),nor.y(),nor.z());
    glVertex3f(p1->x(),p1->y(),p1->z());
    glVertex3f(p2->x(),p2->y(),p2->z());
    glEnd();
}

bool Segment::operator<(const Segment& other) const{
    return other.depth > this->depth;
}
