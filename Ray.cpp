//
// Created by Peter Zdankin on 26.03.18.
//

#include "Ray.hpp"

const Point& Ray::getOrigin() const{
    return origin;
}
const Direction& Ray::getDirection() const{
    return direction;
}
const Point Ray::getPosOnRay(double t) const{
    return origin+direction*t;
}