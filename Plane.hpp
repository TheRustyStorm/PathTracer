//
// Created by Peter Zdankin on 26.03.18.
//

#ifndef PATHTRACER_PLANE_HPP
#define PATHTRACER_PLANE_HPP

#include "IntersectableObject.hpp"

class Plane : public IntersectableObject{
private:
    const Direction normal;
    const double d;
    const Material materialPrimary;
    const Material materialSecondary;
    Direction frame1;
    Direction frame2;
    Point center;
    void buildLocalFrame();
public:
    Plane(const Direction& _normal, double _d, const Material& _materialPrimary, const Material& _materialSecondary): normal(_normal), d(_d), materialPrimary(_materialPrimary), materialSecondary(_materialSecondary){ buildLocalFrame();}
    const Material& getMaterial() const override;
    const std::unique_ptr<Intersection> intersect(const Ray& r) const override;
    void rotate(double angle);
};

#endif //PATHTRACER_PLANE_HPP
