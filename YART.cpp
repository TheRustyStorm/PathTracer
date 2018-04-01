//
// Created by Peter Zdankin on 26.03.18.
//

#include "RaySetup.hpp"
#include "YART.hpp"
#include <time.h>
#include <iomanip>

static uint64_t maxNumber;
static void showProgress(uint64_t x, uint64_t y);

std::unique_ptr<RaySetup> YART::computeRaySetup(const Screen screen){
    std::unique_ptr<RaySetup> rs = std::unique_ptr<RaySetup>(new RaySetup);
    Direction forwardDir = camera.getViewDir();
    Direction upDir = camera.getUpDir();
    double openingAngle = camera.getFoV()* M_PI / 180;
    rs->rayOrigin = camera.getEyePoint();
    double aspectRatio = ((double) screen.getWidth())/ ((double) screen.getHeight());
    Direction rightDir = forwardDir.cross(upDir);
    Direction rowVector = rightDir*2.0*tan(openingAngle/2.0) * aspectRatio;
    Direction columnVector = upDir * 2.0 * tan(openingAngle/2.0);
    rs->dX = rowVector/((double)screen.getWidth());
    rs->dY = columnVector*(-1.0)/((double)screen.getHeight());
    rs->topLeft = Point() + (forwardDir - (rowVector - columnVector)*0.5);

    return rs;
}

void YART::setCamera(Camera& _camera){
    this->camera = _camera;
}
void YART::setScene(Scene& _scene){
    this->scene = _scene;
}
void YART::render(Screen& screen){
    std::clock_t c1, c2;
    c1 = clock();
    std::unique_ptr<RaySetup> rs = computeRaySetup(screen);
    int numSamples = numSamplesX * numSamplesY;
    maxNumber = screen.getHeight() * screen.getWidth();
    for(uint64_t y = 0; y < screen.getHeight(); ++y){
        for(uint64_t x = 0; x < screen.getWidth(); ++x){
            Point color;
            if(numSamples == 1){
                Ray r = computeRay(x,y,*rs);
                color = traceRay(r);
            }else{
                color = Point();
                for(int sY = 0; sY < numSamplesY; ++sY){
                    for(int sX = 0; sX < numSamplesX; ++sX){
                        Ray r = computeRay(x + sX/((double)numSamplesX), y + sY/((double)numSamplesY),*rs);
                        color = color + traceRay(r);
                    }
                }
                color = color / numSamples;
            }
            screen.setPixel(x, y, color);
            if(x % 128 == 0) {
                double _x = x;
                double _y = y;
                double progress = (_y) / double(screen.getHeight());
                c2 = clock();
                float diff((float) c2 - (float) c1);
                float seconds = diff / CLOCKS_PER_SEC;
                int eta = seconds * (1/progress) - seconds ;
                int eta_s = eta % 60;
                int eta_m = (eta / 60) % 60;
                int eta_h = (eta / 3600);

                int eta_total = seconds * (1/progress);
                int eta_total_s = eta_total % 60;
                int eta_total_m = (eta_total/60) % 60;
                int eta_total_h = (eta_total/3600);

                std::cout << "Y:" << y << " Progress " << int(progress * 100.0) << "% ETA "
                          << std::setfill('0') << std::setw(2) << eta_h << ":" << std::setfill('0') << std::setw(2)
                          << eta_m << ":" << std::setfill('0') << std::setw(2) << eta_s
                          << " TOTAL " << std::setfill('0') << std::setw(2) << eta_total_h << ":"
                          << std::setfill('0') << std::setw(2) << eta_total_m << ":"<< std::setfill('0') << std::setw(2)
                            << eta_total_s << " \r";
                std::cout.flush();
            }
        }
    }
}

Point YART::traceRay(const Ray& r){
    return scene.traceRay(r,1.0,recDepth);
}

Ray YART::computeRay(double x, double y, const RaySetup& rs){
    Direction direction = ((rs.topLeft + rs.dX*x + rs.dY * y) - Point()).normalize();
    return Ray(rs.rayOrigin, direction);
}