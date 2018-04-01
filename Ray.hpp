//
// Created by Peter Zdankin on 26.03.18.
//

#ifndef PATHTRACER_RAY_HPP
#define PATHTRACER_RAY_HPP

#include "Direction.hpp"
#include "Point.hpp"

class Ray{
private:
    const Point origin;
    const Direction direction;
public:
    Ray(const Point& _p, const Direction& _dir):origin(_p),direction(_dir){}
    const Point& getOrigin() const;
    const Direction& getDirection() const;
    const Point getPosOnRay(double) const;
};

#endif //PATHTRACER_RAY_HPP
