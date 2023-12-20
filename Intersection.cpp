//
// Created by Peter Zdankin on 26.03.18.
//

#include "Intersection.hpp"

const Material& Intersection::getMaterial()const{
    return material;
}

const Direction& Intersection::getNormal() const {
    return normal;
}

double Intersection::getT() const {
    return t;
}