//
// Created by Peter Zdankin on 27.03.18.
//


#include "Point.hpp"
#include "SphereLight.hpp"

const Point& SphereLight::getCenter() const{
    return center;
}
double SphereLight::getRadius() const{
    return radius;
}
const Direction SphereLight::getDirection (const Point& _position) const {
    return (this->center - _position).normalize();
}
double SphereLight::getDistance(const Point& _position) const{
    return ((this->center - _position).length() - radius);
}