//
// Created by Peter Zdankin on 26.03.18.
//

#ifndef PATHTRACER_INTERSECTABLEOBJECT_HPP
#define PATHTRACER_INTERSECTABLEOBJECT_HPP

#include "Material.hpp"
#include "Intersection.hpp"
#include "Ray.hpp"

class IntersectableObject{
public:
    virtual const Material& getMaterial() const = 0;
    virtual const std::unique_ptr<Intersection> intersect(const Ray& r) const = 0;
    virtual ~IntersectableObject() = default;
};

#endif //PATHTRACER_INTERSECTABLEOBJECT_HPP
