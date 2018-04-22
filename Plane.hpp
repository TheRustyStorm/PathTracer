//
// Created by Peter Zdankin on 26.03.18.
//

#ifndef PATHTRACER_PLANE_HPP
#define PATHTRACER_PLANE_HPP

#include "IntersectableObject.hpp"

class Plane : public IntersectableObject {
private:
    const Direction normal;
    const double d;
    const Material materialPrimary;
    const Material materialSecondary;
    Direction frame1;
    Direction frame2;
    Point center;
    const double maxWidth;
    const double maxHeight;
    bool isLimited;
    bool oneMaterial;

    void buildLocalFrame();

public:
    Plane(const Direction &_normal, double _d, const Material &_materialPrimary, const Material &_materialSecondary,
          double _maxWidth, double _maxHeight) : normal(_normal), d(_d), materialPrimary(_materialPrimary),
                                                 materialSecondary(_materialSecondary), maxWidth(_maxWidth),
                                                 maxHeight(_maxHeight) {
        oneMaterial = false;
        buildLocalFrame();
        isLimited = !(_maxWidth == 0 || _maxHeight == 0);
    }

    Plane(const Direction &_normal, double _d, const Material &_materialPrimary,
          double _maxWidth, double _maxHeight) : normal(_normal), d(_d), materialPrimary(_materialPrimary),
                                                 materialSecondary(_materialPrimary), maxWidth(_maxWidth),
                                                 maxHeight(_maxHeight) {
        oneMaterial = true;
        buildLocalFrame();
        isLimited = !(_maxWidth == 0 || _maxHeight == 0);
    }

    const Material &getMaterial() const override;

    const std::unique_ptr<Intersection> intersect(const Ray &r) const override;

    void rotate(double angle);
    const Point& getCenter() const;
};

#endif //PATHTRACER_PLANE_HPP
