//
// Created by Peter Zdankin on 26.03.18.
//

#include "Camera.hpp"

void Camera::setEyePoint(Point _eyePoint){
    this->eyePoint = _eyePoint;
}
void Camera::setViewDir(Direction _viewDir){
    this->viewDir = _viewDir;
}
void Camera::setUpDir(Direction _upDir){
    this->upDir = _upDir;
}
void Camera::setFoV(double _FoV){
    this->FoV = _FoV;
}
void Camera::setLookAt(Point _lookAt){
    viewDir = (_lookAt - eyePoint).normalize();
}

const Point& Camera::getEyePoint() const{
    return eyePoint;
}
const Direction& Camera::getViewDir() const{
    return viewDir;
}
const Direction& Camera::getUpDir() const{
    return upDir;
}
double Camera::getFoV() const{
    return FoV;
}