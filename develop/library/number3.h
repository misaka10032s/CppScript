#include <string>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <type_traits>

#include "systemOS.h"

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
        void const removeZero();

        friend std::ostream& operator<<(std::ostream &os, const BIGNUM &m);

        BIGNUM leftShift(DIGITDATATYPE NUM1, BIGNUM &res);
        BIGNUM rightShift(DIGITDATATYPE NUM1, BIGNUM &res);
        BIGNUM plus(BIGNUM const &NUM1, BIGNUM &res);
        BIGNUM minus(BIGNUM const &NUM1, BIGNUM &res);
        BIGNUM multiply(BIGNUM const &NUM1, BIGNUM &res);
        BIGNUM divideBy(BIGNUM const &NUM1, BIGNUM &res);
        // BIGNUM divideBy(BIGNUM const &NUM1, BIGNUM &res, int digitExact);
        BIGNUM modulus(BIGNUM const &NUM1, BIGNUM &res);

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

        friend BIGNUM operator + (double NUM0, const BIGNUM &NUM1);
        friend BIGNUM operator - (double NUM0, const BIGNUM &NUM1);
        friend BIGNUM operator * (double NUM0, const BIGNUM &NUM1);
        friend BIGNUM operator / (double NUM0, const BIGNUM &NUM1);
        friend bool operator > (double NUM0, const BIGNUM &NUM1);
        friend bool operator < (double NUM0, const BIGNUM &NUM1);
        friend bool operator == (double NUM0, const BIGNUM &NUM1);
        friend bool operator != (double NUM0, const BIGNUM &NUM1);
        friend bool operator >= (double NUM0, const BIGNUM &NUM1);
        friend bool operator <= (double NUM0, const BIGNUM &NUM1);
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
        COMPLEX(double NUM0, double NUM1);

        friend std::ostream& operator<<(std::ostream &os, const COMPLEX &m);

        COMPLEX bar();

        // COMPLEX plus(COMPLEX const &NUM1, COMPLEX &res);
        // COMPLEX minus(COMPLEX const &NUM1, COMPLEX &res);
        // COMPLEX multiply(COMPLEX const &NUM1, COMPLEX &res);
        // COMPLEX divideBy(COMPLEX const &NUM1, COMPLEX &res);

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

        // friend COMPLEX operator + (double NUM0, const COMPLEX &NUM1);
        // friend COMPLEX operator - (double NUM0, const COMPLEX &NUM1);
        // friend COMPLEX operator * (double NUM0, const COMPLEX &NUM1);
        // friend COMPLEX operator / (double NUM0, const COMPLEX &NUM1);
        // friend bool operator == (double NUM0, const COMPLEX &NUM1);
        // friend bool operator != (double NUM0, const COMPLEX &NUM1);
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
template <typename T> class MATRIX{
    private:
    public:

        int col, row;
        std::vector<T> value;

        MATRIX();
        MATRIX(int colNum, int rowNum);
        MATRIX(int colNum, int rowNum, std::initializer_list<T> NUM1);
        
        template<typename U> friend std::ostream& operator<<(std::ostream &os, const MATRIX<U> &m);

        MATRIX transposed();

        // cube matrix only
        // MATRIX rotate_d(double degree); // 2D
        // MATRIX rotate_r(double radian); // 2D
        // MATRIX rotate_d(double degree, int coor1, int coor2);
        // MATRIX rotate_r(double radian, int coor1, int coor2);
        // MATRIX diagonalization(double radian);
        // MATRIX triangularL(double radian);
        // MATRIX triangularU(double radian);
        // MATRIX_DATA_TYPE determinant();

        // MATRIX plus(MATRIX const &MTX1, MATRIX &res);
        // MATRIX minus(MATRIX const &MTX1, MATRIX &res);
        // MATRIX multiply(MATRIX const &MTX1, MATRIX &res);

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

        // friend MATRIX operator + (double NUM0, const MATRIX &NUM1);
        // friend MATRIX operator - (double NUM0, const MATRIX &NUM1);
        // friend MATRIX operator * (double NUM0, const MATRIX &NUM1);
};

template <typename T> MATRIX<T> transposed(MATRIX<T> MTX1);
template <typename T> MATRIX<T> unitMTX(int order);
template <typename T> MATRIX<T> rotateMTX_d(double degree);
template <typename T> MATRIX<T> rotateMTX_r(double radian);

// MATRIX<uint8_t>;
// MATRIX<uint16_t>;
// MATRIX<uint32_t>;
// MATRIX<uint64_t>;
// MATRIX<int>;
// MATRIX<double>;
// MATRIX<char>;

#include "number3.tpp"

#endif