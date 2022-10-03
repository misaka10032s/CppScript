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

POINT2D POINT2D::matrixTrans(const double &mat[4], const POINT2D &datumP, POINT2D &resultP){
    double tmpX, tmpY;

    tmpX = this->X - datumP.X;
    tmpY = this->Y - datumP.Y;

    resultP.X = mat[0] * tmpX + mat[1] * tmpY + datumP.X;
    resultP.Y = mat[2] * tmpX + mat[3] * tmpY + datumP.Y;
    return resultP;
};
POINT2D POINT2D::rotateR(double rad, POINT2D &resultP){
    double c = cos(rad), s = sin(rad), mat[4] = {c, -s, s, c};
    this->matrixTrans(mat, _coordinateOrigin, resultP);
    return *this;
};
POINT2D POINT2D::rotateD(double deg, POINT2D &resultP){
    double c = cos(deg/180.0*M_PI), s = sin(deg/180.0*M_PI), mat[4] = {c, -s, s, c};
    this->matrixTrans(mat, _coordinateOrigin, resultP);
    return *this;
};
POINT2D POINT2D::rotateR(double rad, POINT2D datumP, POINT2D &resultP){
    double c = cos(rad), s = sin(rad), mat[4] = {c, -s, s, c};
    this->matrixTrans(mat, datumP, resultP);
    return *this;
};
POINT2D POINT2D::rotateD(double deg, POINT2D datumP, POINT2D &resultP){
    double c = cos(deg/180.0*M_PI), s = sin(deg/180.0*M_PI), mat[4] = {c, -s, s, c};
    this->matrixTrans(mat, datumP, resultP);
    return *this;
};

POINT2D POINT2D::symmetry(POINT2D &resultP){
    resultP.X = -this->X;
    resultP.Y = this->Y;
    return resultP;
};
POINT2D POINT2D::symmetry(POINT2D datumP, POINT2D &resultP){
    resultP.X = 2 * datumP.X - this->X;
    resultP.Y = 2 * datumP.Y - this->Y;
    return resultP;
};
POINT2D POINT2D::symmetry(LINE2D datumL, POINT2D &resultP){
    double tmpX, tmpY, A2 = datumL.A*datumL.A, B2 = datumL.B*datumL.B;

    // p,q = this->X, this->Y
    // x = -(b(aq-bp)+a(2c+ap+bq)) / (x^2 + b^2)
    // y = a(aq-bp)-b(2c+ap+bq) / (x^2 + b^2)

    tmpX = (this->X*(B2 - A2) - 2*datumL.A*(datumL.B * this->Y + datumL.C)) / (A2 + B2);
    tmpY = (this->Y*(A2 - B2) - 2*datumL.B*(datumL.A * this->X + datumL.C)) / (A2 + B2);

    resultP.X = tmpX;
    resultP.Y = tmpY;

    return resultP;
};
POINT2D POINT2D::symmetryX(POINT2D &resultP){
    resultP.X = this->X;
    resultP.Y = -this->Y;
    return resultP;
};
POINT2D POINT2D::symmetryY(POINT2D &resultP){
    resultP.X = =this->X;
    resultP.Y = this->Y;
    return resultP;
};
double POINT2D::distance(const POINT2D &point2){
    double dx = this->X - point2.X, dy = this->Y - point2.Y;
    return sqrt(dx*dx + dy*dy);
};
double POINT2D::distance(const LINE2D &line2){
    return abs((this->X*line2.A + this->Y*line2.B + line2.C) / (line2.A*line2.A + line2.B*line2.B));
};





LINE2D LINE2D::rotateR(double rad, LINE2D &resultP){
    double tmpA, tmpB, c = cos(rad), s = sin(rad);

    tmpA = c * this->A - s * this->B;
    tmpB = s * this->A + c * this->B;

    resultP.A = tmpA;
    resultP.B = tmpB;
    resultP.C = this->C;

    return resultP;
};
LINE2D LINE2D::rotateD(double deg, LINE2D &resultP){
    double tmpA, tmpB, c = cos(deg*M_PI/180), s = sin(deg*M_PI/180);

    tmpA = c * this->A - s * this->B;
    tmpB = s * this->A + c * this->B;

    resultP.A = tmpA;
    resultP.B = tmpB;
    resultP.C = this->C;

    return resultP;
};
LINE2D LINE2D::rotateR(double rad, POINT2D datumP, LINE2D &resultP){
    double tmpA, tmpB, tmpC, c = cos(rad), s = sin(rad);

    tmpA = c * this->A - s * this->B;
    tmpB = s * this->A + c * this->B;
    tmpC = this->C + datumP.X * this->A + datumP.Y * this->B;

    resultP.A = tmpA;
    resultP.B = tmpB;
    resultP.C = tmpC - datumP.X * tmpA + datumP.Y * tmpB;

    return resultP;
};
LINE2D LINE2D::rotateD(double deg, POINT2D datumP, LINE2D &resultP){
    double tmpA, tmpB, tmpC, c = cos(deg*M_PI/180), s = sin(deg*M_PI/180);

    tmpA = c * this->A - s * this->B;
    tmpB = s * this->A + c * this->B;
    tmpC = this->C + datumP.X * this->A + datumP.Y * this->B;

    resultP.A = tmpA;
    resultP.B = tmpB;
    resultP.C = tmpC - datumP.X * tmpA + datumP.Y * tmpB;

    return resultP;
};
LINE2D LINE2D::symmetry(LINE2D &resultP){
    resultP.A = this->A;
    resultP.B = this->B;
    resultP.C = -this->C;

    return resultP;
};
LINE2D LINE2D::symmetry(POINT2D datumL, LINE2D &resultP){
    double tmpC;

    tmpC = -(this->C + datumP.X * this->A + datumP.Y * this->B) - datumP.X * this->A + datumP.Y * this->B;

    return resultP;
};
LINE2D LINE2D::symmetry(LINE2D datumL, LINE2D &resultP){
    
    return resultP;
};
LINE2D LINE2D::symmetryX(LINE2D &resultP){
    resultP.A = this->A;
    resultP.B = -this->B;
    resultP.C = -this->C;

    return resultP;
};
LINE2D LINE2D::symmetryY(LINE2D &resultP){
    resultP.A = -this->A;
    resultP.B = this->B;
    resultP.C = this->C;
    
    return resultP;
};
LINE2D LINE2D::telescopic(double multipleX, double multipleY, LINE2D &resultP){
    resultP.A = this->A * multipleX;
    resultP.B = this->B * multipleY;
    resultP.C = this->C;
    
    return resultP;
};
double LINE2D::distance(POINT2D poine2);
POINT2D LINE2D::intersection(LINE2D line2);