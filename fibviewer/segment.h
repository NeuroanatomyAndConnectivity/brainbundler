#ifndef SEGMENT_H
#define SEGMENT_H

#include "qvector3d.h"
#include "primitive.h"

class Segment : public Primitive
{
public:
    Segment(QVector3D *p1, QVector3D *p2);
    QVector3D *p1, *p2;
    void paintGL();
    bool operator<(const Segment& other) const;
};

#endif // SEGMENT_H
