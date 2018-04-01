//
// Created by Peter Zdankin on 26.03.18.
//

#include <cmath>
#include "Plane.hpp"

void Plane::buildLocalFrame() {
    double x = normal.getX();
    double y = normal.getY();
    double z = normal.getZ();
    Direction v;
    if(x != y){
        double temp = x;
        x = y;
        y = temp;
        v = Direction(x,y,z);
    }else if(x != z){
        double temp = x;
        x = z;
        z = temp;
        v = Direction(x,y,z);
    }else if(y != z){
        double temp = y;
        y = z;
        z = temp;
        v = Direction(x,y,z);
    }else{
        v = Direction(0,1,0);
    }



    frame1 = normal.cross(v).normalize();
    frame2 = normal.cross(frame1).normalize();

    center = Point() + normal*(-d);
}

const Material& Plane::getMaterial() const{
    return materialPrimary;
}


const std::unique_ptr<Intersection> Plane::intersect(const Ray& r) const{
    double denom = r.getDirection().dot(normal);
    if (denom == 0)
        return nullptr;

    //double t = -(new Direction(ray.getOrigin()).dot(normal) + d) / denom;
    double t = -((r.getOrigin() - Point()).dot(normal) + d)/ denom;

    if (t < 0)
        return nullptr;
    Point p = r.getPosOnRay(t);
    Direction dir = p - center;
    double x = dir.dot(frame1);
    double y = dir.dot(frame2);
    int w = (int)(round(x)) % 2;
    int h = (int)(round(y)) % 2;

    if((w + h) % 2 == 0){
        return std::unique_ptr<Intersection>(new Intersection(materialPrimary, normal, t));
    }else{
        return std::unique_ptr<Intersection>(new Intersection(materialSecondary, normal, t));
    }

}

void Plane::rotate(double angle) {
    double _angle = angle * M_PI / 180;
    double ca = cos(_angle);
    double sa = sin(_angle);

    double x1 = frame1.getX() * ca + frame1.getZ()*sa;
    double z1 = frame1.getZ() * ca - frame1.getX()*sa;
    frame1 = Direction(x1, 0, z1);
    frame2 = normal.cross(frame1);

}
