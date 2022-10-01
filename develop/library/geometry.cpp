#include <vector>
#include <iostream>
#define _USE_MATH_DEFINES
#include <cmath>

#include "geometry.h"




POINT2D::POINT2D(){
    this->X = 0;
    this->Y = 0;
};
POINT2D::POINT2D(double x, double y){
    this->X = x;
    this->Y = y;
};

POINT2D POINT2D::rotateR(double rad){
    return *this;
};
POINT2D POINT2D::rotateD(double degree){
    return *this;
};
POINT2D POINT2D::rotateR(double rad, POINT2D P){
    return *this;
};
POINT2D POINT2D::rotateD(double degree, POINT2D P){
    return *this;
};