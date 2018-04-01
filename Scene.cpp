//
// Created by Peter Zdankin on 26.03.18.
//

#include <cmath>
#include "IntersectableObject.hpp"
#include "Scene.hpp"
#include "PointLight.hpp"
#include "Sphere.hpp"
#include "Plane.hpp"
#include "SphereLight.hpp"
#include <algorithm>
#define NUM_SAMPLES 20

void Scene::addSphere(Sphere *object){
    spheres.push_back(object);
}

void Scene::addPlane(Plane *object) {
    planes.push_back(object);
}

void Scene::addLight(PointLight* light){
    pointLights.push_back(light);
}

void Scene::addSphereLight(SphereLight *light) {
    sphereLights.push_back(light);
}

const Point Scene::getBackgroundColor() const{
    return backgroundColor;
}

std::unique_ptr<Intersection> Scene::intersect(const Ray& ray, bool shadowRay) const{

    std::unique_ptr<Intersection> result = nullptr;

    for(auto& object: planes){
        if(shadowRay && !object->getMaterial().isShadowCaster()){
            continue;
        }
        std::unique_ptr<Intersection> i = object->intersect(ray);
        if( i == nullptr){
            continue;
        }
        if(result == nullptr || i->getT() < result->getT()){
            result = std::move(i);
        }
    }

    for(auto& object: spheres){
        if(shadowRay && !object->getMaterial().isShadowCaster()){
            continue;
        }
        std::unique_ptr<Intersection> i = object->intersect(ray);
            if( i == nullptr){
                continue;
            }

        if(result == nullptr || i->getT() < result->getT()){
                result = std::move(i);
            }
        }
    return result;
}


Point Scene::traceRay(const Ray& ray, const double IoR, int recDepth) const {
    if (recDepth == 0) {
        return Point();
    }
    std::unique_ptr<Intersection> intersection = intersect(ray, false);
    if (intersection == nullptr) {
        return backgroundColor;
    }

    if (intersection->getMaterial().isEmitting()) {
        return intersection->getMaterial().getEmission();
    }

    Point intersectionPoint = ray.getPosOnRay(intersection->getT() - epsilon);

    Direction normal = intersection->getNormal();
    Point reflection;
    if (intersection->getMaterial().reflects()) {
        Ray reflRay = Ray(intersectionPoint + normal * epsilon, *ray.getDirection().reflection(normal));
        reflection = traceRay(reflRay, IoR, recDepth - 1);
    } else {
        reflection = Point();
    }

    Point refraction;
    if (intersection->getMaterial().refracts()) {

        std::unique_ptr<Direction> refractionDir = ray.getDirection().refraction(normal,
                                                                                 intersection->getMaterial().getIndexOfRefraction());
        if (refractionDir != nullptr) {
            if (IoR == 1.0) {
                Point inSurfacePos = intersectionPoint + normal * (-epsilon);
                Ray refractionRay = Ray(inSurfacePos, *refractionDir);
                refraction = traceRay(refractionRay, intersection->getMaterial().getIndexOfRefraction(), recDepth - 1);
            } else {
                Ray refractionRay = Ray(intersectionPoint, *refractionDir);
                refraction = traceRay(refractionRay, 1.0, recDepth - 1);
            }
        } else {
            refraction = Point();
        }
    } else {
        refraction = Point();
    }


    Point localColor = Point();


    for (auto& object: spheres) {
        if(object->getMaterial().isEmitting()) {
            Direction sphereDir = Direction(object->getCenter() - intersectionPoint).normalize();
            double dot = intersection->getNormal().dot(sphereDir);
            for (int i = 0; i < NUM_SAMPLES; ++i) {
                double r = ((double) rand() / (RAND_MAX));
                if (dot > r) {
                    Ray randRay = Ray(intersectionPoint, sphereDir);
                    localColor = localColor + traceRay(randRay, IoR, recDepth - 1);
                }
            }

        }
    }
    localColor = localColor / NUM_SAMPLES;

    /**
    for (int i = 0; i < NUM_SAMPLES; i++) {
        Direction randDir;
        double dot = 0;
        int xDir = std::rand() % 50;
        int yDir = std::rand() % 50;
        int zDir = std::rand() % 50;
        randDir = Direction(xDir - 25, yDir - 25, zDir - 25).normalize();
        dot = randDir.dot(normal);
        if (dot < 0) {
            randDir = randDir * (-1);
        }
        Ray randRay = Ray(intersectionPoint, randDir);
        localColor = localColor + traceRay(randRay, IoR, recDepth - 1) * 1.5;
    }
    localColor = localColor / NUM_SAMPLES;
    */
    /**
    for (PointLight *ls: pointLights) {
        Ray shadowRay = Ray(intersectionPoint, ls->getDirection(intersectionPoint));

        std::unique_ptr<Intersection> shadowIntersection = intersect(shadowRay, true);
        Point ambient = intersection->getMaterial().getAmbient() * ls->getAmbient();
        double distance = log(ls->getDistance(intersectionPoint));

        if (shadowIntersection == nullptr || shadowIntersection->getT() >= ls->getDistance(intersectionPoint)) {
            Point diffuse = intersection->getMaterial().getDiffuse() *
                            (ls->getDiffuse() * ls->getDirection(intersectionPoint).dot(normal));
            double val = ls->getDirection(intersectionPoint).dot(*ray.getDirection().reflection(normal));
            if (val < 0) {
                val = 0;
            }
            Point specular = intersection->getMaterial().getSpecular() *
                             (ls->getSpecular() * pow(val, intersection->getMaterial().getExponent()));
            localColor = localColor + (ambient + diffuse + specular) / (distance * distance);
        } else {
            localColor = localColor + ambient / (distance * distance);
        }
    }
     */


    localColor.clamp(0.0, 1.0);

    double cosI = ray.getDirection().dot(intersection->getNormal());
    double l = 0, r = 0, t = 0;
    if (intersection->getMaterial().refracts()) {
        l = intersection->getMaterial().getLocalReflectivity();
        r = intersection->getMaterial().getReflectivity(cosI);
        t = 1 - r;
        r = (1 - l) * r;
        t = (1 - l) * t;
    } else if (intersection->getMaterial().reflects()) {
        r = intersection->getMaterial().getReflectivity(cosI);
        l = 1 - r;
    } else {
        l = 1;
    }

    return localColor * l + reflection * r + refraction * t;
}


Scene genScene() {
    // create an empty scene
    Scene s = Scene();

    // and God said, let there be light and there was light
    PointLight *l = new PointLight(Point(2, 4, -2), Point(.5,.5,.5), Point(.5,.5,.5), Point(.5,.5,.5));

    // attach the light source to the scene
    s.addLight(l);

    Point ambient = Point(1, 0, 0);
    Point diffuse = Point(1, 0, 0);
    Point specular = Point(1, 0, 0);
    Point center = Point(5, 1, 6.2);
    SphereLight *sl = new SphereLight(ambient, diffuse, specular, center, 1.0);
    //s.addSphereLight(sl);


    Material black = Material(Point(0.1, 0.1, 0.1), Point(0.3, 0.3, 0.3), Point(1, 1, 1), Point(0, 0, 0), 8, 0.3);

    Point rotation1 = Point(std::rand() % 360, std::rand() % 360, std::rand() % 360);
    Point rotation2 = Point(std::rand() % 360, std::rand() % 360, std::rand() % 360);
    Point rotation3 = Point(std::rand() % 360, std::rand() % 360, std::rand() % 360);
    Point rotation4 = Point(std::rand() % 360, std::rand() % 360, std::rand() % 360);
    Point rotation5 = Point(std::rand() % 360, std::rand() % 360, std::rand() % 360);

    // create the bluish material for the right sphere
    // points are treated as color values in the range [0, 1]
    Material m1 = Material(Point(0.3, 0.3, 0.3), Point(0.5, 0.5, 0.5), Point(1, 1, 1), Point(0, 0, 0), 8, 0.2, 1.52);


    // create a sphere, apply the material above to it and attach it to the scene
    Point center1 = Point(2, -1, -2.5);
    Sphere *sphere1 = new Sphere(center1, 1, m1, m1, rotation1);
    s.addSphere(sphere1);

    // create the red material and apply it to the left sphere
    Material m2 = Material(Point(1, 0, 0), Point(1, 0, 0), Point(1, 1, 1), Point(1, 0, 0), 8, 1);
    Point center2 = Point(-5, -1, -6.2);
    Sphere *sphere2 = new Sphere(center2, 1, m2, black, rotation2);
    s.addSphere(sphere2);

    Material m2_c = Material(Point(0, 1, 1), Point(0, 1, 1), Point(1, 1, 1), Point(0, 1, 1), 8, 1);
    Point center2_c = Point(7, -1, -8);
    Sphere *sphere2_c = new Sphere(center2_c, 1, m2_c, black, rotation3);
    s.addSphere(sphere2_c);

    Material m2_y = Material(Point(1, 1, 0), Point(1, 1, 0), Point(1, 1, 1), Point(1, 1, 0), 8, 1);
    Point center2_y = Point(-12.9, -1, -25.2);
    Sphere *sphere2_y = new Sphere(center2_y, 1, m2_y, black, rotation4);
    s.addSphere(sphere2_y);

    Material m2_g = Material(Point(0, 1, 0), Point(0, 1, 0), Point(1, 1, 1), Point(0, 1, 0), 8, 1);
    Point center2_g = Point(2.9, -1, -15.2);
    Sphere *sphere2_g = new Sphere(center2_g, 1, m2_g, black, rotation5);
    s.addSphere(sphere2_g);

    // create the yellowish material and apply it to the big sphere in the back
    //Material m3 = Material(Point(0.3, 0.3, 0), Point(0.7, 0.7, 0), Point(1, 1, 0), 8, 0.3);
    //Point center3 = Point(0.0, 4.0, -8.0);
    //Sphere* sphere3 = new Sphere(center3, 3.9, m3);
    //s.addSphere(sphere3);

    // create the white ground plane
    //Material m4 = Material(Point(0.3, 0.3, 0.3), Point(0.5, 0.5, 0.5), Point(1, 1, 1), 32, 0.5);
    Material m4 = Material(Point(0.3, 0.3, 0.3), Point(0.5, 0.5, 0.5), Point(1, 1, 1), Point(0, 0, 0), 8, 1);
    Material m5 = Material(Point(0.1, 0.1, 0.1), Point(0.3, 0.3, 0.3), Point(1, 1, 1), Point(0, 0, 0), 8, 0.3);

    Plane *p = new Plane(Direction(0.0, 1.0, 0.0).normalize(), 2, m4, m5);
    p->rotate(345);
    s.addPlane(p);
    return s;
}