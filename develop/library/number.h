#include <string>
#include <iostream>
#include <vector>
#include <unordered_map>

#ifndef LIB_NUMBER_H_
#define LIB_NUMBER_H_

// BIGNUM

class BIGNUM{
    private:

    public:
        bool sign;
        std::vector<int> INT, FLT;

        BIGNUM();
        BIGNUM(int x);
        BIGNUM(float x);
        BIGNUM(double x);

        friend std::ostream& operator<<(std::ostream &os, const BIGNUM &m);

        BIGNUM operator + (BIGNUM NUM1);
        BIGNUM operator - (BIGNUM NUM1);
        BIGNUM operator * (BIGNUM NUM1);
        BIGNUM operator / (BIGNUM NUM1);
        BIGNUM operator % (BIGNUM NUM1);
        BIGNUM operator = (BIGNUM NUM1);
        bool operator > (BIGNUM NUM1);
        bool operator < (BIGNUM NUM1);
        bool operator == (BIGNUM NUM1);
        bool operator >= (BIGNUM NUM1);
        bool operator <= (BIGNUM NUM1);
        BIGNUM operator += (BIGNUM NUM1);
        BIGNUM operator -= (BIGNUM NUM1);
        BIGNUM operator *= (BIGNUM NUM1);
        BIGNUM operator /= (BIGNUM NUM1);
        BIGNUM operator %= (BIGNUM NUM1);

        BIGNUM operator + (int NUM1);
        BIGNUM operator - (int NUM1);
        BIGNUM operator * (int NUM1);
        BIGNUM operator / (int NUM1);
        BIGNUM operator % (int NUM1);
        BIGNUM operator = (int NUM1);
        bool operator > (int NUM1);
        bool operator < (int NUM1);
        bool operator == (int NUM1);
        bool operator >= (int NUM1);
        bool operator <= (int NUM1);
        BIGNUM operator += (int NUM1);
        BIGNUM operator -= (int NUM1);
        BIGNUM operator *= (int NUM1);
        BIGNUM operator /= (int NUM1);
        BIGNUM operator %= (int NUM1);

        BIGNUM operator + (float NUM1);
        BIGNUM operator - (float NUM1);
        BIGNUM operator * (float NUM1);
        BIGNUM operator / (float NUM1);
        BIGNUM operator % (float NUM1);
        BIGNUM operator = (float NUM1);
        bool operator > (float NUM1);
        bool operator < (float NUM1);
        bool operator == (float NUM1);
        bool operator >= (float NUM1);
        bool operator <= (float NUM1);
        BIGNUM operator += (float NUM1);
        BIGNUM operator -= (float NUM1);
        BIGNUM operator *= (float NUM1);
        BIGNUM operator /= (float NUM1);
        BIGNUM operator %= (float NUM1);
        
        BIGNUM operator + (double NUM1);
        BIGNUM operator - (double NUM1);
        BIGNUM operator * (double NUM1);
        BIGNUM operator / (double NUM1);
        BIGNUM operator % (double NUM1);
        BIGNUM operator = (double NUM1);
        bool operator > (double NUM1);
        bool operator < (double NUM1);
        bool operator == (double NUM1);
        bool operator >= (double NUM1);
        bool operator <= (double NUM1);
        BIGNUM operator += (double NUM1);
        BIGNUM operator -= (double NUM1);
        BIGNUM operator *= (double NUM1);
        BIGNUM operator /= (double NUM1);
        BIGNUM operator %= (double NUM1);
};

// COMPLEX

#endif