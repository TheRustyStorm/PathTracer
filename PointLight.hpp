//
// Created by Peter Zdankin on 26.03.18.
//

#ifndef PATHTRACER_POINTLIGHT_HPP
#define PATHTRACER_POINTLIGHT_HPP

#include "Point.hpp"
#include "LightSource.hpp"

class PointLight : public LightSource{
private:
    const Point position;
public:
    PointLight(const Point& _pos, const Point& _ambient, const Point& _diffuse, const Point& _specular):LightSource(_ambient, _diffuse, _specular), position(_pos) {}
    const Direction getDirection (const Point&) const override ;
    double getDistance(const Point&) const override ;
};

#endif //PATHTRACER_POINTLIGHT_HPP
