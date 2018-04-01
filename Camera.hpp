//
// Created by Peter Zdankin on 26.03.18.
//

#ifndef PATHTRACER_CAMERA_HPP
#define PATHTRACER_CAMERA_HPP

#include "Point.hpp"

class Camera{
private:
    Point eyePoint;
    Direction viewDir;
    Direction upDir;
    double FoV;
public:
    Camera():eyePoint(Point(0,0,0)),viewDir(Direction(0,0,-1)),upDir(Direction(0,1,0)), FoV(60){}
    Camera(const Point _eyePoint):eyePoint(_eyePoint),viewDir(Direction(0,0,-1)),upDir(Direction(0,1,0)), FoV(60){}
    Camera(const Point _eyePoint, const Direction _viewDir):eyePoint(_eyePoint),viewDir(_viewDir),upDir(Direction(0,1,0)), FoV(60){}
    Camera(const Point _eyePoint, const Direction _viewDir, const Direction _upDir):eyePoint(_eyePoint),viewDir(_viewDir),upDir(_upDir), FoV(60){}
    Camera(const Point _eyePoint, const Direction _viewDir, const Direction _upDir, const double _FoV):eyePoint(_eyePoint),viewDir(_viewDir),upDir(_upDir), FoV(_FoV){}

    void setEyePoint(Point _eyePoint);
    void setViewDir(Direction _viewDir);
    void setUpDir(Direction _upDir);
    void setFoV(double _FoV);
    void setLookAt(Point lookAt);

    const Point& getEyePoint() const;
    const Direction& getViewDir() const;
    const Direction& getUpDir() const;
    double getFoV() const;

};

#endif //PATHTRACER_CAMERA_HPP
