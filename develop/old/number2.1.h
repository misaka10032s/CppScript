#include <string>
#include <iostream>
#include <vector>
#include <unordered_map>

#ifndef LIB_NUMBER_H_
#define LIB_NUMBER_H_
// numerical

/*
    ###########################################################################################################
    #                                                                                                         #
    #                                                                                                         #
    #                                                  BIGNUM                                                #
    #                                                                                                         #
    #                                                                                                         #
    ###########################################################################################################
*/
#define DIGITDATATYPE int
#define DEFAULT_DIGIT_SIZE 128
#define DIGITNUM 9
#define DIGIT10 (int)pow(10, (int)DIGITNUM)
#define DIGITINT 10/DIGITNUM + 1
#define DIGITFLOAT 38/DIGITNUM + 1
#define DIGITDOUBLE 308/DIGITNUM + 1
class BIGNUM{
    private:

    public:
        bool sign, isInf;
        std::vector<int> value;
        DIGITDATATYPE point;

        BIGNUM();
        BIGNUM(int x);
        BIGNUM(float x);
        BIGNUM(double x);
        BIGNUM(std::string x);

        // inline int degree(BIGNUM const &NUM0);
        int firstDigit(BIGNUM const &NUM0);
        int firstDigit2(BIGNUM const &NUM0);
        void const retract();

        friend std::ostream& operator<<(std::ostream &os, const BIGNUM &m);

        BIGNUM operator << (DIGITDATATYPE NUM1);
        BIGNUM operator >> (DIGITDATATYPE NUM1);
        BIGNUM operator <<= (DIGITDATATYPE NUM1);
        BIGNUM operator >>= (DIGITDATATYPE NUM1);

        BIGNUM operator + (BIGNUM const &NUM1);
        BIGNUM operator - (BIGNUM const &NUM1);
        BIGNUM operator * (BIGNUM const &NUM1);
        BIGNUM operator / (BIGNUM NUM1);
        BIGNUM operator % (BIGNUM const &NUM1);
        BIGNUM operator = (BIGNUM const &NUM1);
        bool operator > (BIGNUM const &NUM1);
        bool operator < (BIGNUM const &NUM1);
        bool operator == (BIGNUM const &NUM1);
        bool operator != (BIGNUM const &NUM1);
        bool operator >= (BIGNUM const &NUM1);
        bool operator <= (BIGNUM const &NUM1);
        BIGNUM operator += (BIGNUM const &NUM1);
        BIGNUM operator -= (BIGNUM const &NUM1);
        BIGNUM operator *= (BIGNUM const &NUM1);
        BIGNUM operator /= (BIGNUM const &NUM1);
        BIGNUM operator %= (BIGNUM const &NUM1);

        BIGNUM operator + (int NUM1);
        BIGNUM operator - (int NUM1);
        BIGNUM operator * (int NUM1);
        BIGNUM operator / (int NUM1);
        BIGNUM operator % (int NUM1);
        BIGNUM operator = (int NUM1);
        bool operator > (int NUM1);
        bool operator < (int NUM1);
        bool operator == (int NUM1);
        bool operator != (int NUM1);
        bool operator >= (int NUM1);
        bool operator <= (int NUM1);
        BIGNUM operator += (int NUM1);
        BIGNUM operator -= (int NUM1);
        BIGNUM operator *= (int NUM1);
        BIGNUM operator /= (int NUM1);
        BIGNUM operator %= (int NUM1);
        
        BIGNUM operator + (double NUM1);
        BIGNUM operator - (double NUM1);
        BIGNUM operator * (double NUM1);
        BIGNUM operator / (double NUM1);
        BIGNUM operator % (double NUM1);
        BIGNUM operator = (double NUM1);
        bool operator > (double NUM1);
        bool operator < (double NUM1);
        bool operator == (double NUM1);
        bool operator != (double NUM1);
        bool operator >= (double NUM1);
        bool operator <= (double NUM1);
        BIGNUM operator += (double NUM1);
        BIGNUM operator -= (double NUM1);
        BIGNUM operator *= (double NUM1);
        BIGNUM operator /= (double NUM1);
        BIGNUM operator %= (double NUM1);
};
BIGNUM abs(BIGNUM NUM0);

/*
    ###########################################################################################################
    #                                                                                                         #
    #                                                                                                         #
    #                                                  COMPLEX                                                #
    #                                                                                                         #
    #                                                                                                         #
    ###########################################################################################################
*/
#define COMPLEX_DATA_TYPE double
class COMPLEX{
    private:
    public:
        COMPLEX_DATA_TYPE Im, Re;

        COMPLEX();
        COMPLEX(int NUM0, int NUM1);
        COMPLEX(float NUM0, float NUM1);
        COMPLEX(double NUM0, double NUM1);

        COMPLEX bar();

        friend std::ostream& operator<<(std::ostream &os, const COMPLEX &m);

        COMPLEX operator + (COMPLEX const &NUM1);
        COMPLEX operator - (COMPLEX const &NUM1);
        COMPLEX operator * (COMPLEX const &NUM1);
        COMPLEX operator / (COMPLEX const &NUM1);
        COMPLEX operator = (COMPLEX const &NUM1);
        bool operator == (COMPLEX const &NUM1);
        bool operator != (COMPLEX const &NUM1);
        COMPLEX operator += (COMPLEX const &NUM1);
        COMPLEX operator -= (COMPLEX const &NUM1);
        COMPLEX operator *= (COMPLEX const &NUM1);
        COMPLEX operator /= (COMPLEX const &NUM1);

        COMPLEX operator + (double const &NUM1);
        COMPLEX operator - (double const &NUM1);
        COMPLEX operator * (double const &NUM1);
        COMPLEX operator / (double const &NUM1);
        COMPLEX operator = (double const &NUM1);
        bool operator == (double const &NUM1);
        bool operator != (double const &NUM1);
        COMPLEX operator += (double const &NUM1);
        COMPLEX operator -= (double const &NUM1);
        COMPLEX operator *= (double const &NUM1);
        COMPLEX operator /= (double const &NUM1);
};

COMPLEX bar(COMPLEX NUM0);
COMPLEX_DATA_TYPE abs(COMPLEX NUM0);

/*
    ###########################################################################################################
    #                                                                                                         #
    #                                                                                                         #
    #                                                   MATRIX                                                #
    #                                                                                                         #
    #                                                                                                         #
    ###########################################################################################################
*/

#define MATRIX_DATA_TYPE double
class MATRIX{
    private:
    public:
        int col, row;
        std::vector<MATRIX_DATA_TYPE> value;

        MATRIX();
        MATRIX(int colNum, int rowNum);
        MATRIX(int colNum, int rowNum, std::initializer_list<double> NUM1);

        MATRIX transposed();
        
        friend std::ostream& operator<<(std::ostream &os, const MATRIX &m);

        MATRIX operator + (MATRIX const &MTX1);
        MATRIX operator - (MATRIX const &MTX1);
        MATRIX operator * (MATRIX const &MTX1);
        MATRIX operator = (MATRIX const &MTX1);
        bool operator == (MATRIX const &MTX1);
        bool operator != (MATRIX const &MTX1);
        MATRIX operator += (MATRIX const &MTX1);
        MATRIX operator -= (MATRIX const &MTX1);
        MATRIX operator *= (MATRIX const &MTX1);

        MATRIX operator = (double* const NUM1);
        MATRIX operator = (std::initializer_list<double> NUM1);
        // MATRIX operator = (std::initializer_list<std::initializer_list<double>> NUM1);

        MATRIX operator + (double const &NUM1);
        MATRIX operator - (double const &NUM1);
        MATRIX operator * (double const &NUM1);
        MATRIX operator / (double const &NUM1);
        MATRIX operator += (double const &NUM1);
        MATRIX operator -= (double const &NUM1);
        MATRIX operator *= (double const &NUM1);
        MATRIX operator /= (double const &NUM1);
};

MATRIX transposed(MATRIX MTX1);
MATRIX unitMTX(int order);
MATRIX rotateMTXd(double degree);
MATRIX rotateMTXr(double radian);
// #include "number2.tpp"

#endif