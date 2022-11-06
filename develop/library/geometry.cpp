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

POINT2D POINT2D::matrixTrans(POINT2D &resultP, const double mat[4], const POINT2D &datumP){
    double tmpX, tmpY;

    tmpX = this->X - datumP.X;
    tmpY = this->Y - datumP.Y;

    resultP.X = mat[0] * tmpX + mat[1] * tmpY + datumP.X;
    resultP.Y = mat[2] * tmpX + mat[3] * tmpY + datumP.Y;
    return resultP;
};
POINT2D POINT2D::rotateR(POINT2D &resultP, double rad){
    double c = cos(rad), s = sin(rad), mat[4] = {c, -s, s, c};
    this->matrixTrans(resultP, mat, _coordinateOrigin);
    return *this;
};
POINT2D POINT2D::rotateD(POINT2D &resultP, double deg){
    double c = cos(deg/180.0*M_PI), s = sin(deg/180.0*M_PI), mat[4] = {c, -s, s, c};
    this->matrixTrans(resultP, mat, _coordinateOrigin);
    return *this;
};
POINT2D POINT2D::rotateR(POINT2D &resultP, double rad, POINT2D datumP){
    double c = cos(rad), s = sin(rad), mat[4] = {c, -s, s, c};
    this->matrixTrans(resultP, mat, datumP);
    return *this;
};
POINT2D POINT2D::rotateD(POINT2D &resultP, double deg, POINT2D datumP){
    double c = cos(deg/180.0*M_PI), s = sin(deg/180.0*M_PI), mat[4] = {c, -s, s, c};
    this->matrixTrans(resultP, mat, datumP);
    return *this;
};

POINT2D POINT2D::symmetry(POINT2D &resultP){
    resultP.X = -this->X;
    resultP.Y = -this->Y;
    return resultP;
};
POINT2D POINT2D::symmetry(POINT2D &resultP, POINT2D datumP){
    resultP.X = 2 * datumP.X - this->X;
    resultP.Y = 2 * datumP.Y - this->Y;
    return resultP;
};
POINT2D POINT2D::symmetry(POINT2D &resultP, LINE2D datumL){
    double tmpX, tmpY, A2 = datumL.A*datumL.A, B2 = datumL.B*datumL.B;

    // p,q = this->X, this->Y
    // x = -(b(aq-bp)+a(2c+ap+bq)) / (a^2 + b^2)
    // y = a(aq-bp)-b(2c+ap+bq) / (a^2 + b^2)

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
    resultP.X = -this->X;
    resultP.Y = this->Y;
    return resultP;
};
POINT2D POINT2D::telescopic(POINT2D &resultP, double multipleX, double multipleY){
    resultP.X = this->X * multipleX;
    resultP.Y = this->Y * multipleY;
    return resultP;
};
POINT2D POINT2D::telescopic(POINT2D &resultP, double multipleX, double multipleY, POINT2D datumP){
    resultP.X = (this->X - datumP.X) * multipleX;
    resultP.Y = (this->Y - datumP.Y) * multipleY;
    return resultP;
};
double POINT2D::distance(const POINT2D &point1){
    double dx = this->X - point1.X, dy = this->Y - point1.Y;
    return sqrt(dx*dx + dy*dy);
};
double POINT2D::distance(const LINE2D &line1){
    return abs((this->X*line1.A + this->Y*line1.B + line1.C) / (line1.A*line1.A + line1.B*line1.B));
};




LINE2D::LINE2D(){
    this->A = 0;
    this->B = 0;
    this->C = 0;
};
LINE2D::LINE2D(POINT2D P0, POINT2D P1){
    this->A = P1.Y - P0.Y;
    this->B = P0.X - P1.X;
    this->C = -(this->A * P0.X + this->B * P0.Y);
};
LINE2D::LINE2D(POINT2D P0, double m){
    this->A = -m;
    this->B = 1;
    this->C = m * P0.X - P0.Y;
};
LINE2D::LINE2D(double a, double b, double c){
    this->A = a;
    this->B = b;
    this->C = c;
};

LINE2D LINE2D::rotateR(LINE2D &resultL, double rad){
    double tmpA, tmpB, c = cos(rad), s = sin(rad);

    tmpA = c * this->A - s * this->B;
    tmpB = s * this->A + c * this->B;

    resultL.A = tmpA;
    resultL.B = tmpB;
    resultL.C = this->C;

    return resultL;
};
LINE2D LINE2D::rotateD(LINE2D &resultL, double deg){
    double tmpA, tmpB, c = cos(deg*M_PI/180), s = sin(deg*M_PI/180);

    tmpA = c * this->A - s * this->B;
    tmpB = s * this->A + c * this->B;

    resultL.A = tmpA;
    resultL.B = tmpB;
    resultL.C = this->C;

    return resultL;
};
LINE2D LINE2D::rotateR(LINE2D &resultL, double rad, POINT2D datumP){
    double tmpA, tmpB, tmpC, c = cos(rad), s = sin(rad);

    tmpA = c * this->A - s * this->B;
    tmpB = s * this->A + c * this->B;
    tmpC = this->C + datumP.X * this->A + datumP.Y * this->B;

    resultL.A = tmpA;
    resultL.B = tmpB;
    resultL.C = tmpC - datumP.X * tmpA + datumP.Y * tmpB;

    return resultL;
};
LINE2D LINE2D::rotateD(LINE2D &resultL, double deg, POINT2D datumP){
    double tmpA, tmpB, tmpC, c = cos(deg*M_PI/180), s = sin(deg*M_PI/180);

    tmpA = c * this->A - s * this->B;
    tmpB = s * this->A + c * this->B;
    tmpC = this->C + datumP.X * this->A + datumP.Y * this->B;

    resultL.A = tmpA;
    resultL.B = tmpB;
    resultL.C = tmpC - datumP.X * tmpA + datumP.Y * tmpB;

    return resultL;
};
LINE2D LINE2D::symmetry(LINE2D &resultL){
    resultL.A = this->A;
    resultL.B = this->B;
    resultL.C = -this->C;

    return resultL;
};
LINE2D LINE2D::symmetry(LINE2D &resultL, POINT2D datumP){
    resultL.A = -this->A;
    resultL.B = -this->B;
    resultL.C = this->C + 2.0 * (this->A * datumP.X + this->B * datumP.Y);

    return resultL;
};
LINE2D LINE2D::symmetry(LINE2D &resultL, LINE2D datumL){
    double mul = 2 * (this->A * datumL.A + this->B * datumL.B) / (datumL.A * datumL.A + datumL.B * datumL.B);

    resultL.A = mul * datumL.A - this->A;
    resultL.B = mul * datumL.B - this->B;
    resultL.C = mul * datumL.C - this->C;
    
    return resultL;

        // angel symmetry 
        // L1: ax+by+c=0
        // L2: dx+ey+f=0
        // L : 2(ad+be)/(d^2+e^2)L2 - L1
};
LINE2D LINE2D::symmetryX(LINE2D &resultL){
    resultL.A = this->A;
    resultL.B = -this->B;
    resultL.C = this->C;

    return resultL;
};
LINE2D LINE2D::symmetryY(LINE2D &resultL){
    resultL.A = -this->A;
    resultL.B = this->B;
    resultL.C = this->C;
    
    return resultL;
};
LINE2D LINE2D::angelBisector(LINE2D &resultL, LINE2D datumL){
    double m = sqrt(this->A * this->A + this->B * this->B), n = sqrt(datumL.A * datumL.A + datumL.B * datumL.B);
    resultL.A = (n/m) * this->A + datumL.A;
    resultL.B = (n/m) * this->B + datumL.B;
    resultL.C = (n/m) * this->C + datumL.C;
    
    return resultL;
        // angle bisector
        // L1: ax+by+c=0
        // L2: dx+ey+f=0
        // m = sqrt(a^2+b^2), n = sqrt(d^2+e^2), K = ad+be, D = ae-bd
        // L: (n/m)L1 + L2
        // L: (n/m)(bx-ay+(Kc-fm^2)/D) + (ex-dy+(cn^2-Kf)/D)
};
LINE2D LINE2D::angelBisector2(LINE2D &resultL, LINE2D datumL){
    double m = sqrt(this->A * this->A + this->B * this->B), n = sqrt(datumL.A * datumL.A + datumL.B * datumL.B), K = this->A * datumL.A - this->B * datumL.B, D = this->A * datumL.B - this->B * datumL.A;
    resultL.A = (n/m) * this->B - datumL.B;
    resultL.B = (n/m) * this->A - datumL.A;
    resultL.C = (n/m) * (K * this->C - datumL.C * m * m) / D - (K * datumL.C - this->C * n * n);
    
    return resultL;

};
LINE2D LINE2D::telescopic(LINE2D &resultL, double multipleX, double multipleY){
    resultL.A = this->A / multipleX;
    resultL.B = this->B / multipleY;
    resultL.C = this->C;
    
    return resultL;
};
LINE2D LINE2D::telescopic(LINE2D &resultL, double multipleX, double multipleY, POINT2D datumP){
    resultL.A = this->A / multipleX;
    resultL.B = this->B / multipleY;
    resultL.C = this->C + (1.0 - 1.0/multipleX) * this->A * datumP.X + (1.0 - 1.0/multipleY) * this->B * datumP.Y;

    return resultL;
};

double LINE2D::distance(POINT2D point1){
    return abs(this->A * point1.X + this->B * point1.Y + this->C) / (this->A * this->A + this->B * this->B);
};
POINT2D LINE2D::intersection(LINE2D line1){
    double delta = this->A * line1.B - this->B * line1.A;
    POINT2D res((this->B * line1.C - this->C * line1.B) / delta, (this->A * line1.C - this->C * line1.A) / delta);
    return res;
};








POLYGON::POLYGON(){
    this->vertexNum = 0;
    this->center.X = 0;
    this->center.Y = 0;
};
POLYGON::POLYGON(std::vector<POINT2D> points){
    this->vertexNum = (int)points.size();
    this->vertex = points;
    this->center.X = 0;
    this->center.Y = 0;
    this->radius = 0;
    for(int i=0; i<this->vertexNum; i++){
        this->center.X += this->vertex[i].X / (double)this->vertexNum;
        this->center.Y += this->vertex[i].Y / (double)this->vertexNum;
    }
    for(int i=0; i<this->vertexNum; i++) this->radius = std::max(sqrt((this->vertex[i].X-this->center.X)*(this->vertex[i].X-this->center.X) + (this->vertex[i].Y-this->center.Y)*(this->vertex[i].Y-this->center.Y)), this->radius);
};
POLYGON::POLYGON(std::initializer_list<double> Xpoints, std::initializer_list<double> Ypoints){
    this->vertexNum = std::min(Xpoints.size(), Ypoints.size());
    if((int)this->vertex.size() < this->vertexNum) this->vertex.resize(this->vertexNum);
    this->center.X = 0;
    this->center.Y = 0;
    this->radius = 0;
    for(int i=0; i<this->vertexNum; i++) {
        this->vertex[i].X = *(Xpoints.begin() + i);
        this->vertex[i].Y = *(Ypoints.begin() + i);
        this->center.X += this->vertex[i].X / (double)this->vertexNum;
        this->center.Y += this->vertex[i].Y / (double)this->vertexNum;
    }
    for(int i=0; i<this->vertexNum; i++) this->radius = std::max(sqrt((this->vertex[i].X-this->center.X)*(this->vertex[i].X-this->center.X) + (this->vertex[i].Y-this->center.Y)*(this->vertex[i].Y-this->center.Y)), this->radius);
};
POLYGON::POLYGON(std::initializer_list<std::initializer_list<double>> points){
    this->vertexNum = points.size();
    if((int)this->vertex.size() < this->vertexNum) this->vertex.resize(this->vertexNum);
    this->center.X = 0;
    this->center.Y = 0;
    this->radius = 0;
    for(int i=0; i<this->vertexNum; i++) {
        this->vertex[i].X = (*(points.begin() + i)->begin());
        this->vertex[i].Y = (int)(points.begin() + i)->size()>1 ? (*(points.begin() + i)->begin()+1) : 0;
        this->center.X += this->vertex[i].X / (double)this->vertexNum;
        this->center.Y += this->vertex[i].Y / (double)this->vertexNum;
    }
    for(int i=0; i<this->vertexNum; i++) this->radius = std::max(sqrt((this->vertex[i].X-this->center.X)*(this->vertex[i].X-this->center.X) + (this->vertex[i].Y-this->center.Y)*(this->vertex[i].Y-this->center.Y)), this->radius);
};


POLYGON POLYGON::rotateR(POLYGON &resultP, double rad){
    resultP.vertex = this->vertex;
    resultP.vertexNum = this->vertexNum;
    for(int i=0; i<resultP.vertexNum; i++){
        resultP.vertex[i].rotateR(resultP.vertex[i], rad);
    }
    this->center.rotateR(resultP.center, rad);
    return resultP;
};
POLYGON POLYGON::rotateD(POLYGON &resultP, double deg){
    resultP.vertex = this->vertex;
    resultP.vertexNum = this->vertexNum;
    for(int i=0; i<resultP.vertexNum; i++){
        resultP.vertex[i].rotateD(resultP.vertex[i], deg);
    }
    this->center.rotateD(resultP.center, deg);
    return resultP;
};
POLYGON POLYGON::rotateR(POLYGON &resultP, double rad, POINT2D datumP){
    resultP.vertex = this->vertex;
    resultP.vertexNum = this->vertexNum;
    for(int i=0; i<resultP.vertexNum; i++){
        resultP.vertex[i].rotateR(resultP.vertex[i], rad, datumP);
    }
    this->center.rotateR(resultP.center, rad, datumP);
    return resultP;
};
POLYGON POLYGON::rotateD(POLYGON &resultP, double deg, POINT2D datumP){
    resultP.vertex = this->vertex;
    resultP.vertexNum = this->vertexNum;
    for(int i=0; i<resultP.vertexNum; i++){
        resultP.vertex[i].rotateD(resultP.vertex[i], deg, datumP);
    }
    this->center.rotateD(resultP.center, deg, datumP);
    return resultP;
};
POLYGON POLYGON::symmetry(POLYGON &resultP){
    resultP.vertex = this->vertex;
    resultP.vertexNum = this->vertexNum;
    for(int i=0; i<resultP.vertexNum; i++){
        resultP.vertex[i].symmetry(resultP.vertex[i]);
    }
    this->center.symmetry(resultP.center);
    return resultP;
};
POLYGON POLYGON::symmetry(POLYGON &resultP, POINT2D datumP){
    resultP.vertex = this->vertex;
    resultP.vertexNum = this->vertexNum;
    for(int i=0; i<resultP.vertexNum; i++){
        resultP.vertex[i].symmetry(datumP, resultP.vertex[i]);
    }
    this->center.symmetry(datumP, resultP.center);
    return resultP;
};
POLYGON POLYGON::symmetry(POLYGON &resultP, LINE2D datumL){
    resultP.vertex = this->vertex;
    resultP.vertexNum = this->vertexNum;
    for(int i=0; i<resultP.vertexNum; i++){
        resultP.vertex[i].symmetry(resultP.vertex[i], datumL);
    }
    this->center.symmetry(resultP.center, datumL);
    return resultP;
};
POLYGON POLYGON::symmetryX(POLYGON &resultP){
    resultP.vertex = this->vertex;
    resultP.vertexNum = this->vertexNum;
    for(int i=0; i<resultP.vertexNum; i++){
        resultP.vertex[i].symmetryX(resultP.vertex[i]);
    }
    this->center.symmetryX(resultP.center);
    return resultP;
};
POLYGON POLYGON::symmetryY(POLYGON &resultP){
    resultP.vertex = this->vertex;
    resultP.vertexNum = this->vertexNum;
    for(int i=0; i<resultP.vertexNum; i++){
        resultP.vertex[i].symmetryY(resultP.vertex[i]);
    }
    this->center.symmetryY(resultP.center);
    return resultP;
};
POLYGON POLYGON::telescopic(POLYGON &resultP, double multipleX, double multipleY){
    resultP.vertex = this->vertex;
    resultP.vertexNum = this->vertexNum;
    for(int i=0; i<resultP.vertexNum; i++){
        resultP.vertex[i].telescopic(resultP.vertex[i], multipleX, multipleY);
    }
    this->center.telescopic(resultP.center, multipleX, multipleY);
    this->radius *= sqrt(multipleX * multipleY);
    return resultP;
};
POLYGON POLYGON::telescopic(POLYGON &resultP, double multipleX, double multipleY, POINT2D datumP){
    resultP.vertex = this->vertex;
    resultP.vertexNum = this->vertexNum;
    for(int i=0; i<resultP.vertexNum; i++){
        resultP.vertex[i].telescopic(resultP.vertex[i], multipleX, multipleY, datumP);
    }
    this->center.telescopic(resultP.center, multipleX, multipleY, datumP);
    this->radius *= sqrt(multipleX * multipleY);
    return resultP;
};


double POLYGON::getRadius(){
    for(int i=0; i<this->vertexNum; i++) this->radius = std::max(sqrt((this->vertex[i].X-this->center.X)*(this->vertex[i].X-this->center.X) + (this->vertex[i].Y-this->center.Y)*(this->vertex[i].Y-this->center.Y)), this->radius);
    return this->radius;
};
double POLYGON::getArea(){
    int next;
    double res = 0;
    for(int i=0; i<this->vertexNum; i++){
        next = (i+1)%this->vertexNum;
        res += this->vertex[i].X * this->vertex[next].Y - this->vertex[i].Y * this->vertex[next].X;
    }
    this->area = abs(res/2.0);
    return this->area;
};
POINT2D POLYGON::getCenter(){
    POINT2D C;
    this->center.X = this->center.Y = 0;
    for(int i=0; i<this->vertexNum; i++){
        this->center.X += this->vertex[i].X / (double)this->vertexNum;
        this->center.Y += this->vertex[i].Y / (double)this->vertexNum;
    }
    C.X = this->center.X;
    C.Y = this->center.Y;
    return C;
};
POLYGON POLYGON::update(){
    this->vertexNum = this->vertex.size();
    this->getRadius();
    this->getArea();
    this->getCenter();
    return *this;
};
bool POLYGON::isContain(POINT2D point1){
    if(this->center.distance(point1) > this->radius) return 0;
    int next, crs = 0;
    double A, B, C;
    for(int i=0; i<this->vertexNum; i++){
        next = (i+1)%this->vertexNum;
        if(((this->vertex[i].X - point1.X >= 0) || (this->vertex[next].X - point1.X >= 0)) && (this->vertex[i].Y - point1.Y <= 0) == (this->vertex[next].Y - point1.Y >= 0)){
            A = this->vertex[next].Y - this->vertex[i].Y;
            B = this->vertex[i].X - this->vertex[next].X;
            C = -(A * this->vertex[i].X + B * this->vertex[i].Y);
            if(A * (A * point1.X + B * point1.Y + C) <= 0) crs++;
        }
        else continue;
    }
    return crs%2;
};
bool POLYGON::isCollide(POLYGON polygon1){
    if(this->center.distance(polygon1.center) > this->radius + polygon1.radius) return 0;
    for(int i=0; i<this->vertexNum; i++){
        if(polygon1.isContain(this->vertex[i])) return 1;
    }
    for(int i=0; i<polygon1.vertexNum; i++){
        if(this->isContain(polygon1.vertex[i])) return 1;
    }
    return 0;
};