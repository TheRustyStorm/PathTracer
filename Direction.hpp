//
// Created by Peter Zdankin on 26.03.18.
//

#ifndef PATHTRACER_DIRECTION_H
#define PATHTRACER_DIRECTION_H
#include <iostream>
#include <memory>

class Direction{
private:
    double x;
    double y;
    double z;
public:
    Direction():x(0),y(0),z(0){};
    Direction(double _x, double _y, double _z): x(_x), y(_y), z(_z){};
    Direction operator+(const Direction&) const;
    Direction operator-(const Direction&) const;
    Direction operator*(const Direction& other) const;
    Direction operator/(const Direction& other) const;

    Direction operator+(double) const;
    Direction operator-(double) const;
    Direction operator*(double) const;
    Direction operator/(double) const;

    double dot(const Direction&) const;

    double length() const;
    Direction normalize() const;
    Direction cross(const Direction& other) const;
    std::unique_ptr<Direction> reflection(const Direction& normal) const;
    std::unique_ptr<Direction> refraction(const Direction& normal, double IORRatio) const;

    Direction rotateX(double a);
    Direction rotateY(double a);
    Direction rotateZ(double a);

    double getX() const;
    double getY() const;
    double getZ() const;

    friend std::ostream& operator<<(std::ostream&, const Direction&);

};
#endif //PATHTRACER_DIRECTION_H
