//
// Created by Peter Zdankin on 26.03.18.
//

#include "Direction.hpp"
#include <cmath>

Direction Direction::operator+(const Direction &other) const{
    return Direction(x + other.x, y + other.y, z + other.z);
}

Direction Direction::operator-(const Direction &other) const{
    return Direction(x - other.x, y - other.y, z - other.z);
}

Direction Direction::operator*(const Direction &other) const{
    return Direction(x * other.x, y * other.y, z * other.z);
}

Direction Direction::operator/(const Direction &other) const{
    return Direction(x - other.x, y - other.y, z - other.z);
}

Direction Direction::operator+(double d) const{
    return Direction(x + d, y + d, z + d);
}

Direction Direction::operator*(double d) const{
    return Direction(x * d, y * d, z * d);
}

Direction Direction::operator-(double d) const{
    return Direction(x - d, y - d, z - d);
}

Direction Direction::operator/(double d) const{
    return Direction(x / d, y / d, z / d);
}

double Direction::length() const {
    return std::sqrt(x * x + y * y + z * z);
}

Direction Direction::normalize() const{
    return *(this)/length();
}

double Direction::dot(const Direction& other) const {
    return x*other.x + y*other.y + z*other.z;
}

Direction Direction::cross(const Direction &other) const {
    return Direction(y * other.z - z*other.y, z * other.x - x * other.z, x * other.y - y * other.x);
}

std::unique_ptr<Direction> Direction::reflection(const Direction &normal) const {
    auto d = *(this)-normal*(dot(normal)*2);
    return std::unique_ptr<Direction>(new Direction(d.x,d.y,d.z));
}

std::unique_ptr<Direction> Direction::refraction(const Direction &normal, double IORRatio) const {
    double cosI = this->dot(normal);
    int sign = (cosI < 0) ? -1 : 1;
    double n = (sign == 1) ? IORRatio : 1.0 / IORRatio;
    double sinT2 = n*n*(1-cosI*cosI);
    if(sinT2 > 1){
        return nullptr;
    }
    Direction d = *(this)*n-normal*(n*cosI - sign * sqrt(1 - sinT2));
    return std::unique_ptr<Direction>(new Direction(d.getX(), d.getY(), d.getZ()));
}

Direction Direction::rotateX(double a) {
    return Direction(x, y*cos(a)-z*sin(a), y*sin(a)+z*cos(a));
}

Direction Direction::rotateY(double a) {
    return Direction(x*cos(a)+z*sin(a),y, z*cos(a)-x*sin(a));
}

Direction Direction::rotateZ(double a) {
    return Direction(x*cos(a)-y*sin(a), x*sin(a)+y*cos(a),z);
}


double Direction::getX() const {
    return x;
}

double Direction::getY() const {
    return y;
}

double Direction::getZ() const {
    return z;
}

std::ostream& operator<<(std::ostream& os, const Direction& d){
    std::cout << "X: " << d.getX() << std::endl;
    std::cout << "Y: " << d.getY() << std::endl;
    std::cout << "Z: " << d.getZ() << std::endl;
    return os;
}

