//
// Created by Peter Zdankin on 26.03.18.
//

#ifndef PATHTRACER_SCENE_HPP
#define PATHTRACER_SCENE_HPP
#include <vector>
#include "IntersectableObject.hpp"
#include "LightSource.hpp"
#include "Sphere.hpp"
#include "Plane.hpp"
#include "PointLight.hpp"
#include "SphereLight.hpp"

class Scene{
private:
    std::vector<Sphere*> spheres;
    std::vector<Plane*> planes;
    std::vector<PointLight*> pointLights;
    std::vector<SphereLight*> sphereLights;
    Point backgroundColor;
    double epsilon = 0.000000001;
public:
    explicit Scene(Point _backgroundColor):backgroundColor(_backgroundColor){}
    Scene():backgroundColor(Point(0,0,0)){}
    void addSphere(Sphere *object);
    void addPlane(Plane *object);
    void addLight(PointLight *light);
    void addSphereLight(SphereLight *light);
    const Point getBackgroundColor() const;
    std::unique_ptr<Intersection> intersect(const Ray& ray, bool shadowRay) const;
    Point traceRay(const Ray& ray, const double IoR, int recDepth) const;



};

Scene genScene();

#endif //PATHTRACER_SCENE_HPP
