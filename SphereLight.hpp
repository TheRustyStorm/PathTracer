//
// Created by Peter Zdankin on 27.03.18.
//

#ifndef PATHTRACER_SPHERELIGHT_HPP
#define PATHTRACER_SPHERELIGHT_HPP

#include "LightSource.hpp"

class SphereLight : public LightSource{
private:
    Point center;
    double radius;

public:
    SphereLight(Point& _ambient, Point& _diffuse, Point& _specular, Point& _center, double _radius): LightSource(_ambient,_diffuse,_specular),center(_center), radius(_radius){}
    const Point& getCenter() const;
    double getRadius() const;
    const Direction getDirection (const Point&) const override ;
    double getDistance(const Point&) const override ;
};

#endif //PATHTRACER_SPHERELIGHT_HPP
