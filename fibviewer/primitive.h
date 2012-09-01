#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include "qvector3d.h"

class Primitive
{
public:
    Primitive();
    double depth;
    bool operator<(const Primitive& other) const;
    virtual void paintGL() = 0;
};

#endif // PRIMITIVE_H


