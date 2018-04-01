//
// Created by Peter Zdankin on 26.03.18.
//

#include "Direction.hpp"
#include "Point.hpp"
#include "PointLight.hpp"

const Direction PointLight::getDirection (const Point& _position) const {
    return (this->position - _position).normalize();
}
double PointLight::getDistance(const Point& _position) const {
    return (this->position - _position).length();
}