//
// Created by Peter Zdankin on 26.03.18.
//

#ifndef PATHTRACER_POINT_HPP
#define PATHTRACER_POINT_HPP

#include "Direction.hpp"

class Point{
private:
    double x;
    double y;
    double z;
public:
    Point(): x(0),y(0),z(0){}
    Point(double _x, double _y, double _z): x(_x),y(_y),z(_z){}

    Direction operator-(const Point&) const;
    Point operator+(const Direction&) const;
    Point operator-(const Direction&) const;
    void operator+=(const Direction&);
    void operator+=(const Point&);
    Point operator+(const Point&) const;
    Point operator*(const Point&) const;
    Point operator/(const Point&) const;
    bool operator==(const Point&) const;

    Point operator+(double) const;
    Point operator-(double) const;
    Point operator*(double) const;
    Point operator/(double) const;

    double getX() const;
    double getY() const;
    double getZ() const;

    void clamp(double min, double max);
};

#endif //PATHTRACER_POINT_HPP
