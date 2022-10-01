#include <vector>

#ifndef LIB_GEOMETRY_H_
#define LIB_GEOMETRY_H_

// POINT LINE SURFACE CELL

class POINT2D;
class LINE2D;


class POINT2D{ // (X, Y)
    public:
        POINT2D();
        POINT2D(double x, double y);
        
        double X, Y;

        POINT2D rotateR(double rad);
        POINT2D rotateD(double degree);
        POINT2D rotateR(double rad, POINT2D datumP);
        POINT2D rotateD(double degree, POINT2D datumP);
        POINT2D symmetry();
        POINT2D symmetry(POINT2D datumP);
        POINT2D symmetry(LINE2D datumL);
        POINT2D symmetryX();
        POINT2D symmetryY();
        double distance(LINE2D datumL);
};

class LINE2D{ // Ax + By + C = 0
    public:
        LINE2D();
        LINE2D(double a, double b, double c);

        double A,B,C;

        LINE2D rotateR(double rad);
        LINE2D rotateD(double degree);
        LINE2D rotateR(double rad, POINT2D datumP);
        LINE2D rotateD(double degree, POINT2D datumP);
        LINE2D symmetry();
        LINE2D symmetry(POINT2D datumL);
        LINE2D symmetry(LINE2D datumL);
        LINE2D symmetryX();
        LINE2D symmetryY();
        LINE2D telescopicX();
        LINE2D telescopicY();
        double distance(POINT2D datumP);
};

class POLYGON{
    public:
        POLYGON();
        POLYGON(std::vector<POINT2D> points);
        POLYGON(std::initializer_list<double> Xpoints, std::initializer_list<double> Ypoints);
        POLYGON(std::initializer_list<std::initializer_list<double>> points);

        std::vector<POINT2D> vertex;


};

#endif