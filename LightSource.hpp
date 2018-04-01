//
// Created by Peter Zdankin on 26.03.18.
//

#ifndef PATHTRACER_LIGHTSOURCE_HPP
#define PATHTRACER_LIGHTSOURCE_HPP

#include "Point.hpp"

class LightSource{
protected:
    const Point ambient;
    const Point diffuse;
    const Point specular;

public:
    LightSource(const Point& _ambient, const Point& _diffuse, const Point& _specular):ambient(_ambient),diffuse(_diffuse),specular(_specular){}
    const Point& getAmbient () const {return ambient;}
    const Point& getDiffuse () const {return diffuse;}
    const Point& getSpecular () const {return specular;}
    virtual const Direction getDirection (const Point&) const = 0;
    virtual double getDistance(const Point&) const = 0;
};

#endif //PATHTRACER_LIGHTSOURCE_HPP
