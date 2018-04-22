//
// Created by Peter Zdankin on 19.04.18.
//

#ifndef PATHTRACER_MULTICOREYART_HPP
#define PATHTRACER_MULTICOREYART_HPP

#include <cmath>
#include "Camera.hpp"
#include "Scene.hpp"
#include "RaySetup.hpp"
#include "Screen.hpp"

class MultiCoreYART{
private:
    int recDepth;
    int numSamplesX;
    int numSamplesY;
    Camera camera;
    Scene scene;
    std::unique_ptr<RaySetup> computeRaySetup(Screen screen);

public:
    MultiCoreYART(int _recDepth, int _numSamples):recDepth(_recDepth){
        numSamplesX = sqrt(_numSamples);
        numSamplesY = _numSamples/numSamplesX;
    }
    void setCamera(Camera& camera);
    void setScene(Scene& scene);
    void render(Screen& screen, uint64_t minHeight, uint64_t maxHeight);
    Point traceRay(const Ray& r);
    Ray computeRay(double x, double y, const RaySetup& rs);


};

#endif //PATHTRACER_MULTICOREYART_HPP
