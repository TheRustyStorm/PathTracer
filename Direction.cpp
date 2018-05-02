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

Direction Direction::reflection(const Direction &normal) const {
    auto d = *(this)-normal*(dot(normal)*2);
    return d;
}

std::experimental::optional<Direction> Direction::refraction(const Direction &normal, double IORRatio) const {
    double cosI = this->dot(normal);
    int sign = (cosI < 0) ? -1 : 1;
    double n = (sign == 1) ? IORRatio : 1.0 / IORRatio;
    double sinT2 = n*n*(1-cosI*cosI);
    if(sinT2 > 1){
        return {};
    }
    Direction d = *(this)*n-normal*(n*cosI - sign * sqrt(1 - sinT2));
    return d;
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

unsigned Direction::my_rand(void)
{
    static unsigned next1=1151752134u, next2=2070363486u;
    next1 = next1 * 1701532575u + 571550083u;
    next2 = next2 * 3145804233u + 4178903934u;
    return (next1<<16)^next2;
}

float Direction::U_m1_p1(){
    return float(my_rand())*(1.0f/2147483648.0f) - 1.0f;
}

Direction Direction::pick_random_point_in_sphere(){
    float x0,x1,x2,x3,d2;
    do{
        x0=U_m1_p1();
        x1=U_m1_p1();
        x2=U_m1_p1();
        x3=U_m1_p1();
        d2=x0*x0+x1*x1+x2*x2+x3*x3;
    }while(d2>1.0f);
    float scale = 1.0f/d2;
    return Direction(2*(x1*x3+x0*x2)*scale,
                    2*(x2*x3+x0*x1)*scale,
                    (x0*x0+x3*x3-x1*x1-x2*x2)*scale);
}

Direction Direction::pick_random_point_in_semisphere(){
    Direction result=pick_random_point_in_sphere();
    if(result.dot(*this)<0){
        result.x=-result.x;
        result.y=-result.y;
        result.z=-result.z;
    }
    return result;
}

