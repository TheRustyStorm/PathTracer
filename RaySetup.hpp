//
// Created by Peter Zdankin on 26.03.18.
//

#ifndef PATHTRACER_RAYSETUP_HPP
#define PATHTRACER_RAYSETUP_HPP

#include "Point.hpp"
#include "Screen.hpp"

class RaySetup{
public:
    Point topLeft;
    Point rayOrigin;
    Direction dX;
    Direction dY;
};

#endif //PATHTRACER_RAYSETUP_HPP
