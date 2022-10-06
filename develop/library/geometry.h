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

        POINT2D matrixTrans(POINT2D &resultP, const double mat[4], const POINT2D &datumP);
        POINT2D rotateR(POINT2D &resultP, double rad);
        POINT2D rotateD(POINT2D &resultP, double deg);
        POINT2D rotateR(POINT2D &resultP, double rad, POINT2D datumP);
        POINT2D rotateD(POINT2D &resultP, double deg, POINT2D datumP);
        POINT2D symmetry(POINT2D &resultP);
        POINT2D symmetry(POINT2D &resultP, POINT2D datumP);
        POINT2D symmetry(POINT2D &resultP, LINE2D datumL);
        POINT2D symmetryX(POINT2D &resultP);
        POINT2D symmetryY(POINT2D &resultP);
        POINT2D telescopic(POINT2D &resultP, double multipleX, double multipleY);
        POINT2D telescopic(POINT2D &resultP, double multipleX, double multipleY, POINT2D datumP);
        double distance(const POINT2D &point1);
        double distance(const LINE2D &line1);
};
const POINT2D _coordinateOrigin;

class LINE2D{ // Ax + By + C = 0
    public:
        LINE2D();
        LINE2D(POINT2D a, POINT2D b);
        LINE2D(POINT2D a, double m);
        LINE2D(double a, double b, double c);

        double A,B,C;

        LINE2D rotateR(LINE2D &resultL, double rad);
        LINE2D rotateD(LINE2D &resultL, double deg);
        LINE2D rotateR(LINE2D &resultL, double rad, POINT2D datumP);
        LINE2D rotateD(LINE2D &resultL, double deg, POINT2D datumP);
        LINE2D symmetry(LINE2D &resultL);
        LINE2D symmetry(LINE2D &resultL, POINT2D datumP);
        LINE2D symmetry(LINE2D &resultL, LINE2D datumL);
        LINE2D symmetryX(LINE2D &resultL);
        LINE2D symmetryY(LINE2D &resultL);
        LINE2D angelBisector(LINE2D &resultL, LINE2D datumL);
        LINE2D angelBisector2(LINE2D &resultL,LINE2D datumL);
        LINE2D telescopic(LINE2D &resultL, double multipleX, double multipleY);
        LINE2D telescopic(LINE2D &resultL, double multipleX, double multipleY, POINT2D datumP);
        double distance(POINT2D point1);
        POINT2D intersection(LINE2D line1);
};


class POLYGON{
    public:
        POLYGON();
        POLYGON(std::vector<POINT2D> points);
        POLYGON(std::initializer_list<double> Xpoints, std::initializer_list<double> Ypoints);
        POLYGON(std::initializer_list<std::initializer_list<double>> points);

        std::vector<POINT2D> vertex;
        int vertexNum;
        POINT2D center;
        double radius, area;

        POLYGON rotateR(POLYGON &resultP, double rad);
        POLYGON rotateD(POLYGON &resultP, double deg);
        POLYGON rotateR(POLYGON &resultP, double rad, POINT2D datumP);
        POLYGON rotateD(POLYGON &resultP, double deg, POINT2D datumP);
        POLYGON symmetry(POLYGON &resultP);
        POLYGON symmetry(POLYGON &resultP, POINT2D datumP);
        POLYGON symmetry(POLYGON &resultP, LINE2D datumL);
        POLYGON symmetryX(POLYGON &resultP);
        POLYGON symmetryY(POLYGON &resultP);
        POLYGON telescopic(POLYGON &resultP, double multipleX, double multipleY);
        POLYGON telescopic(POLYGON &resultP, double multipleX, double multipleY, POINT2D datumP);
        double getRadius();
        double getArea();
        POINT2D getCenter();
        POLYGON update();
        bool isContain(POINT2D point1);
        bool isCollide(POLYGON polygon1);
};

class CONICSECTION{ // ax^2 + by^2 + cxy + dx + ey + f = 0
    public:
        CONICSECTION(double a, double b, double c, double d, double e, double f);
};

#endif