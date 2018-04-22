//
// Created by Peter Zdankin on 26.03.18.
//

#ifndef PATHTRACER_SPHERE_HPP
#define PATHTRACER_SPHERE_HPP

#include "Material.hpp"
#include "Intersection.hpp"
#include "Ray.hpp"
#include "IntersectableObject.hpp"

enum TextureType{ONE_MATERIAL, STRIPES, CHECKERBOARD};

class Sphere: public IntersectableObject {
private:
    const Point center;
    const double radius;
    const double sqradius;
    const Material primaryMaterial;
    const Material secondaryMaterial;
    Point angles;
    TextureType textureType;
public:
    Sphere(Point& _center, double _radius, const Material& _primaryMaterial,
           const Material& _secondaryMaterial, Point& _angles, TextureType _textureType) : center(_center), radius(_radius),
                                                                 sqradius(_radius * _radius),
                                                                 primaryMaterial(_primaryMaterial),
                                                                 secondaryMaterial(_secondaryMaterial),
                                                                 angles(_angles), textureType(_textureType){
    }
    Sphere(Point& _center, double _radius, const Material& _primaryMaterial, Point& _angles) : center(_center), radius(_radius),
                                                                 sqradius(_radius * _radius),
                                                                 primaryMaterial(_primaryMaterial),
                                                                 secondaryMaterial(_primaryMaterial),
                                                                 angles(_angles) {
        textureType = ONE_MATERIAL;
    }

    virtual const Material &getMaterial() const override;

    virtual const std::unique_ptr<Intersection> intersect(const Ray &r) const override;
    const Point getCenter() const;

};

#endif //PATHTRACER_SPHERE_HPP
