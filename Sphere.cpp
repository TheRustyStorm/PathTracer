//
// Created by Peter Zdankin on 26.03.18.
//

#include <cmath>
#include "Intersection.hpp"
#include "Ray.hpp"
#include "Sphere.hpp"

const Material& Sphere::getMaterial() const{
    return primaryMaterial;
}

static double toRadians(double degree){
    return degree/180 * M_PI;
}

static double toDegree(double radians){
    return radians / M_PI * 180;
}

const std::unique_ptr<Intersection> Sphere::intersect(const Ray& r) const {
    Direction l = center - r.getOrigin();

    double tCenter = l.dot(r.getDirection());
    if (tCenter < 0)
        return nullptr; // no intersection

    double dSq = l.dot(l) - tCenter * tCenter;
    if (dSq > sqradius)
        return nullptr; // no intersection

    double dist = sqrt(sqradius - dSq);
    double t = tCenter - dist;

    if (t < 0) {
        t = tCenter + dist; // when inside the sphere
    }

    Direction normal = (r.getPosOnRay(t) - center).normalize();

    if (textureType == ONE_MATERIAL) {
        return std::unique_ptr<Intersection>(new Intersection(primaryMaterial, normal, t));
    }

    Direction dir = Direction(r.getPosOnRay(t) - center).rotateX(angles.getX()).rotateY(
            toRadians(angles.getY())).rotateZ(toRadians(angles.getZ()));
    double theta = atan2(dir.getY(), dir.getX());
    double phi = acos(dir.getZ() / sqrt(sqradius));

    phi = toDegree(phi);
    theta = toDegree(theta+M_PI);


    if (textureType == STRIPES) {
        if ((phi >= 30 && phi <= 60) || (phi >= 120 && phi <= 150)) {
            return std::unique_ptr<Intersection>(new Intersection(secondaryMaterial, normal, t));
        } else {
            return std::unique_ptr<Intersection>(new Intersection(primaryMaterial, normal, t));
        }
    }
    else {//if(textureType == CHECKERBOARD){
        if(((int)(phi) % 60 <= 30 && (int)(theta) % 60 <= 30) || ((int)(phi) % 60 > 30 && (int)(theta) % 60 > 30) ){
            return std::unique_ptr<Intersection>(new Intersection(secondaryMaterial, normal, t));
        }
        else{
            return std::unique_ptr<Intersection>(new Intersection(primaryMaterial, normal, t));
        }
    }
    //atan2 calculates value from -PI to PI
    //add PI to the value to get a value between 0 and 2PI
    //divide by 2PI to get a value between 0 and 1
    // double u = ((theta+M_PI)/(2*M_PI));

    //acos calculates value from 0 to PI
    //divide by PI to get value between 0 and 1
    //double v = (phi/M_PI);

}

const Point Sphere::getCenter() const {
    return center;
}