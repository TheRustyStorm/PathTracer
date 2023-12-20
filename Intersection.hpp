//
// Created by Peter Zdankin on 26.03.18.
//

#ifndef PATHTRACER_INTERSECTION_HPP
#define PATHTRACER_INTERSECTION_HPP

#include "Material.hpp"

class Intersection{
private:
    const Material material;
    const Direction normal;
    double t;
public:
    Intersection(const Material& _material, const Direction& _normal, const double _t): material(_material), normal(_normal), t(_t) {}
    const Material& getMaterial() const;
    const Direction& getNormal() const;
    double getT() const;
};

#endif //PATHTRACER_INTERSECTION_HPP
