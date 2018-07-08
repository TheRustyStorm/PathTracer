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


const std::shared_ptr<Intersection> Plane::intersect(const Ray& r) const{
    double denom = r.getDirection().dot(normal);

    //Parallel Ray
    if (denom == 0)
        return nullptr;

    double t = -((r.getOrigin() - Point()).dot(normal) + d)/ denom;


    if (t < 0)
        return nullptr;

    Point p = r.getPosOnRay(t);
    Direction dir = p - center;
    double x = dir.dot(frame1);
    double y = dir.dot(frame2);

    if(isLimited){
        if( x > maxWidth || x < -maxWidth || y > maxHeight || y < -maxHeight){
            return nullptr;
        }
    }

    if(oneMaterial){
        return std::make_shared<Intersection>(materialPrimary, normal, t);
    }

    int w = (int)(round(x)) % 2; // w is either 0 or 1
    int h = (int)(round(y)) % 2; // h is either 0 or 1

    // if w and h are equal
    // 00|01
    // 10|11
    // Needed for checkerboard pattern
    if((w + h) % 2 == 0){
        return std::make_shared<Intersection>(materialPrimary, normal, t);
    }else{
        return std::make_shared<Intersection>(materialSecondary, normal, t);
    }

}

void Plane::rotate(double angle) {
    // Degree to Rad
    double _angle = angle * M_PI / 180;

    double ca = cos(_angle);
    double sa = sin(_angle);

    double x1 = frame1.getX() * ca + frame1.getZ()*sa;
    double z1 = frame1.getZ() * ca - frame1.getX()*sa;

    //We assume the plane is not using the Y Dimension
    frame1 = Direction(x1, 0, z1);
    frame2 = normal.cross(frame1);

}

const Point &Plane::getCenter() const {
    return center;
}
