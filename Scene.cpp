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
#include <experimental/optional>
#include "config.hpp"

int min(int a, int b){
    return (a<b)?a:b;
}

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


    return result;
}


Point Scene::traceRay(const Ray& ray, const double IoR, int recDepth) const {

    //Recursion End
    if (recDepth <= 0) {
        return Point();
    }

    std::unique_ptr<Intersection> intersection = intersect(ray, false);
    //Nothing hit
    if (intersection == nullptr) {
        return backgroundColor;
    }

    //If Ray hits an emitting object, return the emission color
    if (intersection->getMaterial().isEmitting()) {
        return intersection->getMaterial().getEmission();
    }

    Point intersectionPoint = ray.getPosOnRay(intersection->getT() - epsilon);
    Direction normal = intersection->getNormal();


    Point reflection;

    //Get reflected if any
    if (intersection->getMaterial().reflects()) {
        Ray reflRay = Ray(intersectionPoint + normal * epsilon, ray.getDirection().reflection(normal));
        reflection = traceRay(reflRay, IoR, recDepth - 1);
    } else {
        reflection = Point();
    }

    Point refraction;

    //Get refracted if any
    if (intersection->getMaterial().refracts()) {

        std::experimental::optional<Direction> refractionDir = ray.getDirection().refraction(normal,
                                                                                             intersection->getMaterial().getIndexOfRefraction());
        //Total Internal Reflection May occur
        if (refractionDir) {
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


    Point localColor = intersection->getMaterial().getDiffuse();

    if(intersection->getMaterial().getMaterialType() == DIFFUSE){
        long ran = rand();
        long x = (ran % 10) - 5;
        long y = (ran >> 8) % 10 - 5;
        long z = (ran >> 16) % 10 - 5;
        Direction tempDir = Direction(x,y,z);
        Direction rayDir = normal + tempDir;
        if(rayDir.dot(normal) < 1){
            rayDir = rayDir * (-1);
        }
        Ray randRay = Ray(intersectionPoint, rayDir);
        localColor = localColor + traceRay(randRay, IoR, recDepth - 2);
    }


    /**
     * For each sphere
     * If the PRIMARY COLOR of a sphere is emitting
     * Calculate the dotproduct between the sphere and the surface normal
     * The following is done as often as NUM_SAMPLES is big
     *
     * get a random number between 0 and 1
     * If the dotproduct is bigger than that number, trace a ray to the sphere
     *
     */
/*
        for (auto &object: spheres) {
            if (object->getMaterial().isEmitting()) {
                Direction sphereDir = Direction(object->getCenter() - intersectionPoint).normalize();
                double dot = intersection->getNormal().dot(sphereDir);

                for (int i = 0; i < RAY_SAMPLES; ++i) {
                    double r = ((double) rand() / (RAND_MAX));
                    if (dot > r) {
#ifdef IGNORE_EVERYTHING_BUT_EMISSION
//                        if(recDepth == 1 || (intersection->getMaterial().getAmbient() == Point(1,1,1))) {
                            localColor = localColor + object->getMaterial().getEmission();

  //                      }
   //                     else {
#else
                            Ray randRay = Ray(intersectionPoint, sphereDir);
                        localColor = localColor + intersection->getMaterial().getAmbient();
                                localColor = localColor + traceRay(randRay, IoR, recDepth-1);
#endif
                    }
                }

            }
        }localColor = localColor / RAY_SAMPLES;

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
    //PointLight *l = new PointLight(Point(2, 4, -2), Point(.5,.5,.5), Point(.5,.5,.5), Point(.5,.5,.5));

    // attach the light source to the scene
    //s.addLight(l);

    //Point ambient = Point(1, 0, 0);
    //Point diffuse = Point(1, 0, 0);
    //Point specular = Point(1, 0, 0);
    //Point center = Point(5, 1, 6.2);
    //SphereLight *sl = new SphereLight(ambient, diffuse, specular, center, 1.0);
    //s.addSphereLight(sl);



    Point rotation1 = Point(std::rand() % 360, std::rand() % 360, std::rand() % 360);
    Point rotation2 = Point(std::rand() % 360, std::rand() % 360, std::rand() % 360);
    Point rotation3 = Point(std::rand() % 360, std::rand() % 360, std::rand() % 360);
    Point rotation4 = Point(std::rand() % 360, std::rand() % 360, std::rand() % 360);
    Point rotation5 = Point(std::rand() % 360, std::rand() % 360, std::rand() % 360);

    // points are treated as color values in the range [0, 1]
    Material black = Material(Point(0.1, 0.1, 0.1), Point(0.3, 0.3, 0.3), Point(1, 1, 1), Point(0, 0, 0), 8, 0.3);
    Material glass = Material(Point(0.3, 0.3, 0.3), Point(0.5, 0.5, 0.5), Point(1, 1, 1), Point(0, 0, 0), 8, 0.2, 1.52);
    Material red = Material(Point(1, 0, 0), Point(1, 0, 0), Point(1, 1, 1), Point(1, 0, 0), 8, 1);
    Material cyan = Material(Point(0, 1, 1), Point(0, 1, 1), Point(1, 1, 1), Point(0, 1, 1), 8, 1);
    Material yellow = Material(Point(1, 1, 0), Point(1, 1, 0), Point(1, 1, 1), Point(1, 1, 0), 8, 1);
    Material green = Material(Point(0, 1, 0), Point(0, 1, 0), Point(1, 1, 1), Point(0, 1, 0), 8, 1);
    Material white = Material(Point(0.3, 0.3, 0.3), Point(0.5, 0.5, 0.5), Point(1, 1, 1), Point(0, 0, 0), 8, 1);
    Material blue = Material(Point(0,0,1),Point(0,0,1),Point(0,0,1),Point(0,0,1),8,1);
    Material magenta = Material(Point(1,0,1),Point(1,0,1),Point(1,0,1),Point(1,0,1),8,1);


    // Glass Sphere
    Point center1 = Point(2, -1, -2.5);
    Sphere *sphere1 = new Sphere(center1, 1, glass, glass, rotation1, STRIPES);
    s.addSphere(sphere1);

    // Red Light Emitting Sphere
    Point center_r = Point(-5, -1, -6.2);
    Sphere *sphere_r = new Sphere(center_r, 1, red, black, rotation2, CHECKERBOARD);
    s.addSphere(sphere_r);

    //Cyan Light Emitting Sphere
    Point center_c = Point(7, -1, -8);
    Sphere *sphere_c = new Sphere(center_c, 1, cyan, black, rotation3, STRIPES);
    s.addSphere(sphere_c);

    //Magenta Light Emitting Sphere
    Point center_m = Point(15, -1, -38);
    Sphere *sphere_m = new Sphere(center_m, 1, magenta, black, rotation3, STRIPES);
    s.addSphere(sphere_m);

    //Yellow Light Emitting Sphere
    Point center_y = Point(-12.9, -1, -25.2);
    Sphere *sphere_y = new Sphere(center_y, 1, yellow, black, rotation4, STRIPES);
    s.addSphere(sphere_y);

    //Green Light Emitting Sphere
    Point center_g = Point(2.9, -1, -15.2);
    Sphere *sphere_g = new Sphere(center_g, 1, green, black, rotation5, CHECKERBOARD);
    s.addSphere(sphere_g);

    //Blue Light Emitting Sphere
    Point center_b = Point(-3, -1, 1.2);
    Sphere *sphere_b = new Sphere(center_b, 1, blue, black, rotation5, CHECKERBOARD);
    s.addSphere(sphere_b);

    Plane *p = new Plane(Direction(0.0, 1.0, 0.0).normalize(), 2, white, black, 0, 0);
    p->rotate(345);
    s.addPlane(p);
    return s;
}

Scene genSimpleScene() {

    // create an empty scene
    Scene s = Scene();

    // and God said, let there be light and there was light
    PointLight *l = new PointLight(Point(0, 4, -2), Point(1, 1, 1), Point(1, 1, 1), Point(1, 1, 1));

    // attach the light source to the scene
    //s.addLight(l);

    Point rotation = Point();

    // create the bluish material for the right sphere
    // points are treated as color values in the range [0, 1]
    Material m = Material(Point(0, 0, 0.3), Point(0, 0, 0.5), Point(1, 1, 1), Point(0,0,0),8, 0.2, 1.52);

    // create a sphere, apply the material above to it and attach it to the scene
    Point center = Point(0.7, -0.4, -2);
    Sphere *sphere1 = new Sphere(center, 0.9, m, rotation);
    s.addSphere(sphere1);

    // create the red material and apply it to the left sphere
    Material m2 = Material(Point(0.3, 0, 0), Point(0.5, 0, 0), Point(1, 1, 1),Point(1,0,0), 8, 1);
    Point center2 = Point(-0.9, -0.1, -2.2);
    Sphere *sphere2 = new Sphere(center2, 0.6, m2, rotation);
    s.addSphere(sphere2);

    // create the yellowish material and apply it to the big sphere in the back
    Material m3 = Material(Point(0.3, 0.3, 0), Point(0.7, 0.7, 0), Point(1, 1, 0),Point(0,0,0), 8, 0.3);
    Point center3 = Point(0,4,-8);
    Sphere *sphere3 = new Sphere(center3, 3.9, m3, rotation);
    s.addSphere(sphere3);

    // create the white ground plane
    Material m4 = Material(Point(0.3, 0.3, 0.3), Point(0.5, 0.5, 0.5), Point(1, 1, 1), Point(0,0,0), 32, 0.5);
    Plane *p = new Plane(Direction(0,1,0),2,m4, m4, 0, 0);
    s.addPlane(p);

    // return the created scene to the caller
    return s;
}

Scene genOtherScene() {
    // create an empty scene
    Scene s = Scene();

    // and God said, let there be light and there was light
    PointLight *l = new PointLight(Point(2, 4, -2), Point(1,1,1), Point(1,1,1), Point(1,1,1));

    // attach the light source to the scene
    s.addLight(l);

    Point rotation1 = Point(std::rand() % 360, std::rand() % 360, std::rand() % 360);
    Point rotation2 = Point(std::rand() % 360, std::rand() % 360, std::rand() % 360);

    // points are treated as color values in the range [0, 1]
    Material black = Material(Point(0.1, 0.1, 0.1), Point(0.3, 0.3, 0.3), Point(1, 1, 1), Point(0, 0, 0), 8, 0.5);
    Material glass = Material(Point(0.3, 0.3, 0.3), Point(0.5, 0.5, 0.5), Point(1, 1, 1), Point(0, 0, 0), 8, 0.2, 1.52);
    Material white = Material(Point(1, 1, 1), Point(1, 1, 1), Point(1, 1, 1), Point(1, 1, 1), 8, 1);


    // Glass Sphere
    Point center1 = Point(-1, 0, -3.2);
    Sphere *sphere1 = new Sphere(center1, 1, glass, rotation1);
    s.addSphere(sphere1);

    // White Light Emitting Sphere
    Point center_r = Point(0, 4, -3.2);
    Sphere *sphere_r = new Sphere(center_r, 1, white, rotation2);
    s.addSphere(sphere_r);


    Plane *p = new Plane(Direction(0.0, 1.0, 0.0).normalize(), 2, black, black, 0, 0);
    p->rotate(345);
    s.addPlane(p);
    return s;
}

Scene genBoxScene(){
    Scene s = Scene();


    Material black = Material(Point(0.1, 0.1, 0.1), Point(0.3, 0.3, 0.3), Point(1, 1, 1), Point(0, 0, 0), 8, 0.5);
    Material glass = Material(Point(0.3, 0.3, 0.3), Point(0.5, 0.5, 0.5), Point(1, 1, 1), Point(0, 0, 0), 8, 0.2, 1.52);
    Material white = Material(Point(1, 1, 1), Point(1, 1, 1), Point(1, 1, 1), Point(1, 1, 1), 8, 1);
    Material magenta = Material(Point(1, 0, 1), Point(1, 0, 1), Point(1, 0, 1), Point(1, 0, 1), 8, 1);
    Material green = Material(Point(0, 1, 0), Point(0, 1, 0), Point(1, 1, 1), Point(0, 1, 0), 8, 1);
    Material red = Material(Point(1, 0, 0), Point(1, 0, 0), Point(1, 1, 1), Point(1, 0, 0), 8, 1);
    Material blue = Material(Point(0, 0, 1), Point(0, 0, 1), Point(1, 1, 1), Point(0, 0, 1), 8, 1);



    Material whitePlane = Material(Point(0.3, 0.3, 0.3), Point(0.5, 0.5, 0.5), Point(1, 1, 1), Point(0, 0, 0), 8, 1);
    Point rotation1 = Point(std::rand() % 360, std::rand() % 360, std::rand() % 360);
    Point rotation2 = Point(std::rand() % 360, std::rand() % 360, std::rand() % 360);

//    Point center_green = Point(0, 2, -3.2);
//    Point center_red = Point(1.73205, 2, -6.2);
//    Point center_blue = Point(-1.73205, 2, -6.2);
//    Point center_glass = Point(0, -1, -5.2);

    Point center_green = Point(0, 1, -5.2);
    Point center_red = Point(3, 1, -5.2);
    Point center_blue = Point(-3, 1, -5.2);
    Point center_glass = Point(2, -2, -5.2);
    Point center_mirror = Point(-2, -2, -5.2);



    Sphere *sphere_green = new Sphere(center_green, 1, green, rotation2);
    s.addSphere(sphere_green);

    Sphere *sphere_red = new Sphere(center_red, 1, red, rotation2);
    s.addSphere(sphere_red);

    Sphere *sphere_blue = new Sphere(center_blue, 1, blue, rotation2);
    s.addSphere(sphere_blue);



    Sphere *sphere_glass = new Sphere(center_glass, 1.5, black, rotation1);
    s.addSphere(sphere_glass);

    Sphere *sphere_mirror = new Sphere(center_mirror, 1.5, black, rotation1);
    s.addSphere(sphere_mirror);


    Plane *bottom = new Plane(Direction(0.0, 1.0, 0.0).normalize(), 5, whitePlane, 10, 7);
    //p->rotate(345);
    s.addPlane(bottom);

    Plane *top = new Plane(Direction(0.0, -1.0, 0.0).normalize(), 4, whitePlane, 10, 7);
    //p->rotate(345);
    s.addPlane(top);

    Plane *right = new Plane(Direction(-1.0, 0.0, 0.0).normalize(), 7, whitePlane, 10, 5);
    //p->rotate(345);
    s.addPlane(right);

    Plane *left = new Plane(Direction(1.0, 0.0, 0.0).normalize(), 7, whitePlane, 10, 5);
    //p->rotate(345);
    s.addPlane(left);

    Plane *back = new Plane(Direction(0.0, 0.0, 1.0).normalize(), 10, black, 5, 7);
    //p->rotate(345);
    s.addPlane(back);

    return s;
}

Scene genWeirdBoxScene(){
    Scene s = Scene();


    Material black = Material(Point(0.1, 0.1, 0.1), Point(0.3, 0.3, 0.3), Point(1, 1, 1), Point(0, 0, 0), 8, 0.5);
    Material glass = Material(Point(0.3, 0.3, 0.3), Point(0.5, 0.5, 0.5), Point(1, 1, 1), Point(0, 0, 0), 8, 0.2, 1.52);
    Material white = Material(Point(1, 1, 1), Point(1, 1, 1), Point(1, 1, 1), Point(1, 1, 1), 8, 1);
    Material magenta = Material(Point(1, 0, 1), Point(1, 0, 1), Point(1, 0, 1), Point(1, 0, 1), 8, 1);
    Material green = Material(Point(0, 1, 0), Point(0, 1, 0), Point(1, 1, 1), Point(0, 1, 0), 8, 1);
    Material red = Material(Point(1, 0, 0), Point(1, 0, 0), Point(1, 1, 1), Point(1, 0, 0), 8, 1);
    Material blue = Material(Point(0, 0, 1), Point(0, 0, 1), Point(1, 1, 1), Point(0, 0, 1), 8, 1);
    Material blackLight = Material(Point(0, 0, 0), Point(0, 0, 0), Point(0, 0, 0), Point(0.01, 0.01, 0.01), 8, 1);



    Material whitePlane = Material(Point(0.3, 0.3, 0.3), Point(0.5, 0.5, 0.5), Point(1, 1, 1), Point(0, 0, 0), 8, 1);
    Material blackPlane = Material(Point(0, 0, 0), Point(0.2, 0.2, 0.2), Point(.4, .4, .4), Point(0, 0, 0), 8, 1);

    Point rotation1 = Point(std::rand() % 360, std::rand() % 360, std::rand() % 360);
    Point rotation2 = Point(std::rand() % 360, std::rand() % 360, std::rand() % 360);

//    Point center_green = Point(0, 2, -3.2);
//    Point center_red = Point(1.73205, 2, -6.2);
//    Point center_blue = Point(-1.73205, 2, -6.2);
//    Point center_glass = Point(0, -1, -5.2);

    Point center_green = Point(0, 1, -5.2);
    Point center_red = Point(3, 1, -5.2);
    Point center_blue = Point(-3, 1, -5.2);
    Point center_glass = Point(2, -2, -5.2);
    Point center_mirror = Point(-2, -2, -5.2);



    Sphere *sphere_green = new Sphere(center_green, 1, white, rotation2);
   s.addSphere(sphere_green);

    Sphere *sphere_red = new Sphere(center_red, 1, glass, rotation2);
    s.addSphere(sphere_red);

    Sphere *sphere_blue = new Sphere(center_blue, 1, glass, rotation2);
    s.addSphere(sphere_blue);



    Sphere *sphere_glass = new Sphere(center_glass, 1.5, black, rotation1);
    s.addSphere(sphere_glass);

    Sphere *sphere_mirror = new Sphere(center_mirror, 1.5, black, rotation1);
    s.addSphere(sphere_mirror);


    Plane *bottom = new Plane(Direction(0.0, 1.0, 0.0).normalize(), 5, blackPlane, 10, 7);
    //p->rotate(345);
    s.addPlane(bottom);

    Plane *top = new Plane(Direction(0.0, -1.0, 0.0).normalize(), 4, blackPlane, 10, 7);
    //p->rotate(345);
    s.addPlane(top);

    Plane *right = new Plane(Direction(-1.0, 0.0, 0.0).normalize(), 7, blackPlane, 10, 5);
    //p->rotate(345);
    s.addPlane(right);

    Plane *left = new Plane(Direction(1.0, 0.0, 0.0).normalize(), 7, blackPlane, 10, 5);
    //p->rotate(345);
    s.addPlane(left);

    Plane *back = new Plane(Direction(0.0, 0.0, 1.0).normalize(), 10, blackPlane, 5, 7);
    //p->rotate(345);
    s.addPlane(back);

    return s;
}


Scene genSmallptScene(){
    Scene s = Scene();

    Material black = Material(Point(0.1, 0.1, 0.1), Point(0.3, 0.3, 0.3), Point(1, 1, 1), Point(0, 0, 0), 8, 0.5);
    Material glass = Material(Point(0.3, 0.3, 0.3), Point(0.5, 0.5, 0.5), Point(1, 1, 1), Point(0, 0, 0), 8, 0.2, 1.52);
    Material mirror = Material(Point(0.3, 0.3, 0.3), Point(0.5, 0.5, 0.5), Point(1, 1, 1), Point(0, 0, 0), 8, 0.2);
    Material white = Material(Point(1, 1, 1), Point(1, 1, 1), Point(1, 1, 1), Point(1, 1, 1), 8, 1);
    Material magenta = Material(Point(1, 0, 1), Point(1, 0, 1), Point(1, 0, 1), Point(1, 0, 1), 8, 1);
    Material green = Material(Point(0, 1, 0), Point(0, 1, 0), Point(1, 1, 1), Point(0, 1, 0), 8, 1);

    Material redWall = Material(Point(0.2, 0,0), Point(0.5,0,0), Point(1,1,1), Point(),8, 1);
    Material blueWall = Material(Point(0, 0,0.2), Point(0,0,0.5), Point(1,1,1), Point(),8, 1);


    Material whitePlane = Material(Point(0.3, 0.3, 0.3), Point(0.5, 0.5, 0.5), Point(1, 1, 1), Point(0, 0, 0), 8, 1);
    Material blackPlane = Material(Point(0, 0, 0), Point(0.2, 0.2, 0.2), Point(.4, .4, .4), Point(0, 0, 0), 8, 1);

    Point rotation1 = Point(std::rand() % 360, std::rand() % 360, std::rand() % 360);
    Point rotation2 = Point(std::rand() % 360, std::rand() % 360, std::rand() % 360);

    Point center_light = Point(0, 3, -5.2);
    Point center_glass = Point(3, -3, -3.2);
    Point center_mirror = Point(-3, -3, -7.2);



    Sphere *sphere_light = new Sphere(center_light, 1, white, rotation2);
    s.addSphere(sphere_light);



    Sphere *sphere_glass = new Sphere(center_glass, 1.5, glass, rotation1);
    s.addSphere(sphere_glass);

    Sphere *sphere_mirror = new Sphere(center_mirror, 1.5, mirror, rotation1);
    s.addSphere(sphere_mirror);


    Plane *bottom = new Plane(Direction(0.0, 1.0, 0.0).normalize(), 5, whitePlane, 10, 7);
    //p->rotate(345);
    s.addPlane(bottom);

    Plane *top = new Plane(Direction(0.0, -1.0, 0.0).normalize(), 4, whitePlane, 10, 7);
    //p->rotate(345);
    s.addPlane(top);

    Plane *right = new Plane(Direction(-1.0, 0.0, 0.0).normalize(), 7, redWall, 10, 5);
    //p->rotate(345);
    s.addPlane(right);

    Plane *left = new Plane(Direction(1.0, 0.0, 0.0).normalize(), 7, blueWall, 10, 5);
    //p->rotate(345);
    s.addPlane(left);

    Plane *back = new Plane(Direction(0.0, 0.0, 1.0).normalize(), 10, whitePlane, 5, 7);
    //p->rotate(345);
    s.addPlane(back);

    return s;
}
