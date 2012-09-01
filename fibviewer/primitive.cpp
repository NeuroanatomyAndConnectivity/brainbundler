#include "primitive.h"
#include <QDebug>

Primitive::Primitive()
{
}

bool Primitive::operator<(const Primitive& other) const{
    return other.depth > this->depth;
}
