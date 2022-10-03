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

        POINT2D matrixTrans(const double &mat[4], const POINT2D &datumP, POINT2D &resultP);
        POINT2D rotateR(double rad, POINT2D &resultP);
        POINT2D rotateD(double deg, POINT2D &resultP);
        POINT2D rotateR(double rad, POINT2D datumP, POINT2D &resultP);
        POINT2D rotateD(double deg, POINT2D datumP, POINT2D &resultP);
        POINT2D symmetry(POINT2D &resultP);
        POINT2D symmetry(POINT2D datumP, POINT2D &resultP);
        POINT2D symmetry(LINE2D datumL, POINT2D &resultP);
        POINT2D symmetryX(POINT2D &resultP);
        POINT2D symmetryY(POINT2D &resultP);
        double distance(const POINT2D &point2);
        double distance(const LINE2D &line2);
};
const POINT2D _coordinateOrigin;

class LINE2D{ // Ax + By + C = 0
    public:
        LINE2D();
        LINE2D(double a, double b, double c);

        double A,B,C;

        LINE2D rotateR(double rad, LINE2D &resultP);
        LINE2D rotateD(double deg, LINE2D &resultP);
        LINE2D rotateR(double rad, POINT2D datumP, LINE2D &resultP);
        LINE2D rotateD(double deg, POINT2D datumP, LINE2D &resultP);
        LINE2D symmetry(LINE2D &resultP);
        LINE2D symmetry(POINT2D datumL, LINE2D &resultP);
        LINE2D symmetry(LINE2D datumL, LINE2D &resultP);
        LINE2D symmetryX(LINE2D &resultP);
        LINE2D symmetryY(LINE2D &resultP);
        LINE2D telescopic(double multipleX, double multipleY, LINE2D &resultP);
        double distance(POINT2D poine2);
        POINT2D intersection(LINE2D line2);

        // angle bisector
        // L1:ax+by+c=0, L2:dx+ey+f=0
        // m = sqrt(a^2+b^2), n = sqrt(d^2+e^2)
        // L: (n/m)L1 + L2
};


class POLYGON{
    public:
        POLYGON();
        POLYGON(std::vector<POINT2D> points);
        POLYGON(std::initializer_list<double> Xpoints, std::initializer_list<double> Ypoints);
        POLYGON(std::initializer_list<std::initializer_list<double>> points);

        std::vector<POINT2D> vertex;

        double area();
        bool contain(POINT2D point2);
};

class CONICSECTION{ // ax^2 + by^2 + cxy + dx + ey + f = 0
    public:
        CONICSECTION(double a, double b, double c, double d, double e, double f);
}

#endif