//
// Created by Peter Zdankin on 26.03.18.
//

#include "Point.hpp"


Direction Point::operator-(const Point& other) const {
    return Direction(x-other.x, y - other.y, z - other.z);
}

Point Point::operator+(const Direction& other) const {
    return Point(x + other.getX(), y + other.getY(), z + other.getZ());
}

Point Point::operator-(const Direction& other) const {
    return Point(x - other.getX(), y - other.getY(), z - other.getZ());
}

Point Point::operator+(const Point& other) const {
    return Point(x + other.x, y + other.y, z + other.z);
}

Point Point::operator*(const Point& other) const {
    return Point(x * other.x, y * other.y, z * other.z);
}

Point Point::operator/(const Point& other) const {
    return Point(x / other.x, y / other.y, z / other.z);
}

Point Point::operator+(double d) const {
    return Point(x + d, y + d, z + d);
}

Point Point::operator-(double d) const {
    return Point(x - d, y - d, z - d);
}

Point Point::operator*(double d) const {
    return Point(x * d, y * d, z * d);
}

Point Point::operator/(double d) const {
    return Point(x / d, y / d, z / d);
}

bool Point::operator==(const Point& other) const{
    return other.x == x && other.y == y && other.z == z;
}

double Point::getX() const{
    return x;
}
double Point::getY() const{
    return y;
}
double Point::getZ() const{
    return z;
}

void Point::clamp(double min, double max) {
    if(x < min){
        x = min;
    }
    if(y < min){
        y = min;
    }
    if(z < min){
        z = min;
    }

    if(x > max){
        x = max;
    }
    if(y > max){
        y = max;
    }
    if(z > max){
        z = max;
    }

}
