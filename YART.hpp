//
// Created by Peter Zdankin on 26.03.18.
//

#ifndef PATHTRACER_YART_HPP
#define PATHTRACER_YART_HPP

#include <cmath>
#include <thread>
#include "Camera.hpp"
#include "Scene.hpp"
#include "RaySetup.hpp"
#include "Screen.hpp"


class YART{
private:
    int recDepth;
    int numSamplesX;
    int numSamplesY;
    Camera camera;
    Scene scene;
    std::unique_ptr<RaySetup> computeRaySetup(Screen screen);

public:
    YART(int _recDepth, int _numSamples):recDepth(_recDepth){
        numSamplesX = sqrt(_numSamples);
        numSamplesY = _numSamples/numSamplesX;
    }
    void setCamera(Camera& camera);
    void setScene(Scene& scene);
    void render(Screen& screen);
    Point traceRay(const Ray& r);
    Ray computeRay(double x, double y, const RaySetup& rs);


};

#endif //PATHTRACER_YART_HPP
