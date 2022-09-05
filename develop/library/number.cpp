#include <string>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <cmath>

#include "number.h"

#define DIGITNUM 9
#define DIGIT10 (int)pow(10, (int)DIGITNUM)
#define DIGITINT 10/DIGITNUM + 1
#define DIGITFLOAT 38/DIGITNUM + 1
#define DIGITDOUBLE 308/DIGITNUM + 1

BIGNUM::BIGNUM(){
    this->sign = 0;
    this->INT.resize(1);
    this->FLT.resize(1);
    this->INT[0] = 0;
    this->FLT[0] = 0;
}
BIGNUM::BIGNUM(int x){
    int N[DIGITINT], digit=0;
    if(x<0){
        x *= -1;
        this->sign = 1;
    }
    else{
        this->sign = 0;
    }
    for(int i=0; i<DIGITINT;i++){
        N[i] = x%DIGIT10;
        x /= DIGIT10;
        if(N[i]) digit++;
        else break;
    }
    this->INT.resize(digit);
    for(int i=0; i<digit;i++){
        this->INT[i] = N[i];
    }

    this->FLT.resize(1);
    this->FLT[0] = 0;
};
BIGNUM::BIGNUM(float x){
    int N[DIGITFLOAT]={0}, digitN=0;
    int F[DIGITFLOAT]={0}, digitF=0;
    float dx;
    if(x<0){
        x *= -1;
        this->sign = 1;
    }
    else{
        this->sign = 0;
    }

    dx = std::fmod(x,1);
    x = floor(x);

    for(int i=0; i<DIGITFLOAT;i++){
        N[i] = std::fmod(x, DIGIT10);
        x = floor(x/DIGIT10);
        if(N[i]) digitN++;
        else break;
    }
    this->INT.resize(std::max(digitN, 1));
    for(int i=0; i<digitN;i++){
        this->INT[i] = N[i];
    }

    dx *= DIGIT10;
    for(int i=0; i<DIGITFLOAT;i++){
        F[i] = floor(dx);
        dx = std::fmod(dx,1) * DIGIT10;
        if(F[i]) digitF++;
        else break;
    }
    this->FLT.resize(std::max(digitF, 1));
    for(int i=0; i<digitF;i++){
        this->FLT[i] = F[i];
    }
};
BIGNUM::BIGNUM(double x){
    int N[DIGITDOUBLE]={0}, digitN=0;
    int F[DIGITDOUBLE]={0}, digitF=0;
    float dx;
    if(x<0){
        x *= -1;
        this->sign = 1;
    }
    else{
        this->sign = 0;
    }

    dx = std::fmod(x,1);
    x = floor(x);

    for(int i=0; i<DIGITDOUBLE;i++){
        N[i] = std::fmod(x,DIGIT10);
        x = floor(x/DIGIT10);
        if(N[i]) digitN++;
        else break;
    }
    this->INT.resize(std::max(digitN, 1));
    for(int i=0; i<digitN;i++){
        this->INT[i] = N[i];
    }

    dx *= DIGIT10;
    for(int i=0; i<DIGITDOUBLE;i++){
        F[i] = floor(dx);
        dx = std::fmod(dx,1) * DIGIT10;
        if(F[i]) digitF++;
        else break;
    }
    this->FLT.resize(std::max(digitF, 1));
    for(int i=0; i<digitF;i++){
        this->FLT[i] = F[i];
    }
};

std::ostream& operator<<(std::ostream &os, const BIGNUM &m){
    int digitFix;
    if(m.sign) std::cout << "-";

    std::cout << m.INT.back();
    for(int i=m.INT.size()-2; i>=0; i--) {
        digitFix = DIGIT10/10;
        while(digitFix>1) {
            if(m.INT[i] < digitFix) std::cout << "0";
            else break;
            digitFix /= 10;
        }
        std::cout << m.INT[i];
    }
    if(m.FLT.back() != 0){
        std::cout << ".";
        for(int i=0; i<m.FLT.size()-1; i++) {
            digitFix = DIGIT10/10;
            while(digitFix>1) {
                if(m.FLT[i] < digitFix) std::cout << "0";
                else break;
                digitFix /= 10;
            }
            std::cout << m.FLT[i];
        }
        digitFix = DIGIT10/10;
        while(digitFix>1) {
            if(m.FLT.back() < digitFix) std::cout << "0";
            else break;
            digitFix /= 10;
        }
        digitFix = m.FLT.back();
        while(digitFix % 10  == 0 && digitFix != 0) digitFix /= 10;
        std::cout << digitFix;
    }

    return os;
}

// ===========================================  BIGNUM  ===========================================
BIGNUM BIGNUM::operator + (BIGNUM NUM1){
    BIGNUM res;
    if(this->sign != NUM1.sign) {
        res = NUM1;
        res.sign ^= 1;
        return *this - res;
    }
    
    int digitInt, digitFlt, fltZero = 0;
    bool isCarry = 0;

    digitInt = std::max(this->INT.size(), NUM1.INT.size());
    digitFlt = std::max(this->FLT.size(), NUM1.FLT.size());

    res.INT.resize(digitInt);
    res.FLT.resize(digitFlt);

    for(int i=digitFlt-1; i>=0; i--){
        if(NUM1.FLT.size() <= i || this->FLT.size() <= i) continue;
        res.FLT[i] = NUM1.FLT[i] + this->FLT[i] + isCarry;
        isCarry = (res.FLT[i] > DIGIT10);
        res.FLT[i] %= DIGIT10;
    }
    for(int i=0; i<digitInt; i++){
        if(NUM1.INT.size() <= i || this->INT.size() <= i) continue;
        res.INT[i] = NUM1.INT[i] + this->INT[i] + isCarry;
        isCarry = (res.INT[i] > DIGIT10);
        res.INT[i] %= DIGIT10;
    }
    if(isCarry){
        res.INT.resize(digitInt + 1);
        res.INT[digitInt] = 1;
    }
    for(int i=digitFlt-1; i>=0; i--){
        if(res.FLT[i] == 0) fltZero++;
    }
    res.FLT.resize(std::max(digitFlt-fltZero, 1));
    return res;
}
BIGNUM BIGNUM::operator - (BIGNUM NUM1){
    BIGNUM res;
    if(this->sign != NUM1.sign) {
        res = NUM1;
        res.sign ^= 1;
        return *this + res;
    }
    
    int digitInt, digitFlt, fltZero = 0;
    bool isCarry = 0;

    digitInt = std::max(this->INT.size(), NUM1.INT.size());
    digitFlt = std::max(this->FLT.size(), NUM1.FLT.size());

    res.INT.resize(digitInt);
    res.FLT.resize(digitFlt);

    for(int i=digitFlt-1; i>=0; i--){
        if(NUM1.FLT.size() <= i || this->FLT.size() <= i) continue;
        res.FLT[i] = NUM1.FLT[i] - this->FLT[i] - isCarry;
        isCarry = (res.FLT[i] < 0);
        if(isCarry) res.FLT[i] += DIGIT10;
    }
    for(int i=0; i<digitInt; i++){
        if(NUM1.INT.size() <= i || this->INT.size() <= i) continue;
        res.INT[i] = NUM1.INT[i] - this->INT[i] - isCarry;
        isCarry = (res.INT[i] < 0);
        res.INT[i] += DIGIT10;
    }
    if(isCarry){
        res.INT.resize(digitInt - 1);
        res.sign ^= 1;
    }
    for(int i=digitFlt-1; i>=0; i--){
        if(res.FLT[i] == 0) fltZero++;
    }
    res.FLT.resize(std::max(digitFlt-fltZero, 1));
    return res;
}
BIGNUM BIGNUM::operator * (BIGNUM NUM1){
    BIGNUM res;
    int digitInt, digitFlt, digitFill;
    float tmp;

    res.sign = this->sign ^ NUM1.sign;

    digitInt = this->INT.size() + NUM1.INT.size();
    digitFlt = this->FLT.size() + NUM1.FLT.size();

    res.INT.resize(digitInt);
    res.FLT.resize(digitFlt);

    for(int i=0; i<res.INT.size(); i++) res.INT[i] = 0;
    for(int i=0; i<res.FLT.size(); i++) res.FLT[i] = 0;

std::cout<<"@@ "<<this->INT.size()<<", "<<this->FLT.size()<<"\n";
std::cout<<"@@ "<<NUM1.INT.size()<<", "<<NUM1.FLT.size()<<"\n";
std::cout<<"@@ "<<-1 * (int)this->FLT.size()<<"\n";
std::cout<<"@@ "<<-1 * (int)NUM1.FLT.size()<<"\n";

    for(int i=-1 * (int)this->FLT.size(); i<this->INT.size(); i++){
std::cout<<"i "<<i<<"\n";
        for(int j=-1 * (int)NUM1.FLT.size(); j<NUM1.INT.size(); j++){
std::cout<<"ij "<<i<<", "<<j<<"\n";
            tmp = (float)(i >= 0 ? this->INT[i] : this->FLT[-1 * i - 1]) * (float)(j >= 0 ? NUM1.INT[j] : NUM1.FLT[-1 * j - 1]);
            digitFill = i + j;
            if(digitFill >= 0){
                res.INT[i + j] += std::fmod(tmp, DIGIT10);
                res.INT[i + j + 1] += floor(tmp / DIGIT10);

                res.INT[i + j + 1] += res.INT[i + j] / DIGIT10;
                res.INT[i + j] %= DIGIT10;
            }
            else if(digitFill == -1){
                res.FLT[-1 * (i + j) - 1] += std::fmod(tmp, DIGIT10);
                res.INT[i + j + 1] += floor(tmp / DIGIT10);

                res.INT[i + j + 1] += res.FLT[-1 * (i + j) - 1] / DIGIT10;
                res.FLT[-1 * (i + j) - 1] %= DIGIT10;
            }
            else{
                res.FLT[-1 * (i + j) - 1] += std::fmod(tmp, DIGIT10);
                res.FLT[-1 * (i + j + 1) - 1] += floor(tmp / DIGIT10);

                res.FLT[-1 * (i + j + 1) - 1] += res.FLT[-1 * (i + j) - 1] / DIGIT10;
                res.FLT[-1 * (i + j) - 1] %= DIGIT10;
            }
        }
    }

    if(res.INT.back() == 0) res.INT.resize(std::max(digitInt, 1));

    return res;
}
BIGNUM BIGNUM::operator / (BIGNUM NUM1){ // @@@@
    return (BIGNUM)0;
}
BIGNUM BIGNUM::operator % (BIGNUM NUM1){ // @@@@
    return (BIGNUM)0;
}
BIGNUM BIGNUM::operator = (BIGNUM NUM1){
    this->INT.resize(NUM1.INT.size());
    this->FLT.resize(NUM1.FLT.size());
    for(int i=0; i<NUM1.INT.size(); i++){
        this->INT[i] = NUM1.INT[i];
    }
    for(int i=0; i<NUM1.FLT.size(); i++){
        this->FLT[i] = NUM1.FLT[i];
    }
    return *this;
}
bool BIGNUM::operator > (BIGNUM NUM1){
    if(this->sign != NUM1.sign) return this->sign < NUM1.sign;
    if(this->INT.size() != NUM1.INT.size()) return this->INT.size() > NUM1.INT.size();
    int digitInt, digitFlt;
    digitInt = this->INT.size();
    digitFlt = std::min(this->FLT.size(), NUM1.FLT.size());

    for(int i=digitInt-1; i>=0; i--) if(this->INT[i] != NUM1.INT[i]) return this->INT[i] > NUM1.INT[i];
    for(int i=0; i<digitFlt; i++) if(this->FLT[i] != NUM1.FLT[i]) return this->FLT[i] > NUM1.FLT[i];
    return this->FLT.size() > NUM1.FLT.size();
}
bool BIGNUM::operator < (BIGNUM NUM1){
    if(this->sign != NUM1.sign) return this->sign > NUM1.sign;
    if(this->INT.size() != NUM1.INT.size()) return this->INT.size() < NUM1.INT.size();
    int digitInt, digitFlt;
    digitInt = this->INT.size();
    digitFlt = std::min(this->FLT.size(), NUM1.FLT.size());

    for(int i=digitInt-1; i>=0; i--) if(this->INT[i] != NUM1.INT[i]) return this->INT[i] < NUM1.INT[i];
    for(int i=0; i<digitFlt; i++) if(this->FLT[i] != NUM1.FLT[i]) return this->FLT[i] < NUM1.FLT[i];
    return this->FLT.size() < NUM1.FLT.size();
}
bool BIGNUM::operator == (BIGNUM NUM1){
    if(this->sign != NUM1.sign || this->INT.size() != NUM1.INT.size() || this->FLT.size() != NUM1.FLT.size()) return 0;
    for(int i=0; i<this->INT.size(); i++) if(this->INT[i] != NUM1.INT[i]) return 0;
    for(int i=0; i<this->FLT.size(); i++) if(this->FLT[i] != NUM1.FLT[i]) return 0;
    return 1;
}
// @@@@
bool BIGNUM::operator >= (BIGNUM NUM1){
    return (*this == NUM1 || *this > NUM1);
}
bool BIGNUM::operator <= (BIGNUM NUM1){
    return (*this == NUM1 || *this < NUM1);
}
BIGNUM BIGNUM::operator += (BIGNUM NUM1){
    BIGNUM res = *this + NUM1;
    *this = res;
    return res;
}
BIGNUM BIGNUM::operator -= (BIGNUM NUM1){
    BIGNUM res = *this - NUM1;
    *this = res;
    return res;
}
BIGNUM BIGNUM::operator *= (BIGNUM NUM1){
    BIGNUM res = *this * NUM1;
    *this = res;
    return res;
}
BIGNUM BIGNUM::operator /= (BIGNUM NUM1){
    BIGNUM res = *this / NUM1;
    *this = res;
    return res;
}
BIGNUM BIGNUM::operator %= (BIGNUM NUM1){
    BIGNUM res = *this % NUM1;
    *this = res;
    return res;
}


// ===========================================  int  ===========================================
BIGNUM BIGNUM::operator + (int NUM1){
    return *this + *(new BIGNUM(NUM1));
}
BIGNUM BIGNUM::operator - (int NUM1){
    return *this - *(new BIGNUM(NUM1));
}
BIGNUM BIGNUM::operator * (int NUM1){
    return *this * *(new BIGNUM(NUM1));
}
BIGNUM BIGNUM::operator / (int NUM1){
    return *this / *(new BIGNUM(NUM1));
}
BIGNUM BIGNUM::operator % (int NUM1){
    return *this % *(new BIGNUM(NUM1));
}
BIGNUM BIGNUM::operator = (int NUM1){
    return *this = *(new BIGNUM(NUM1));
}
bool BIGNUM::operator > (int NUM1){
    return *this > *(new BIGNUM(NUM1));
}
bool BIGNUM::operator < (int NUM1){
    return *this < *(new BIGNUM(NUM1));
}
bool BIGNUM::operator == (int NUM1){
    return *this == *(new BIGNUM(NUM1));
}
bool BIGNUM::operator >= (int NUM1){
    return *this >= *(new BIGNUM(NUM1));
}
bool BIGNUM::operator <= (int NUM1){
    return *this <= *(new BIGNUM(NUM1));
}
BIGNUM BIGNUM::operator += (int NUM1){
    BIGNUM res = *this + *(new BIGNUM(NUM1));
    *this = res;
    return res;
}
BIGNUM BIGNUM::operator -= (int NUM1){
    BIGNUM res = *this - *(new BIGNUM(NUM1));
    *this = res;
    return res;
}
BIGNUM BIGNUM::operator *= (int NUM1){
    BIGNUM res = *this * *(new BIGNUM(NUM1));
    *this = res;
    return res;
}
BIGNUM BIGNUM::operator /= (int NUM1){
    BIGNUM res = *this / *(new BIGNUM(NUM1));
    *this = res;
    return res;
}
BIGNUM BIGNUM::operator %= (int NUM1){
    BIGNUM res = *this % *(new BIGNUM(NUM1));
    *this = res;
    return res;
}


// ===========================================  float  ===========================================
BIGNUM BIGNUM::operator + (float NUM1){
    return *this + *(new BIGNUM(NUM1));
}
BIGNUM BIGNUM::operator - (float NUM1){
    return *this - *(new BIGNUM(NUM1));
}
BIGNUM BIGNUM::operator * (float NUM1){
    return *this * *(new BIGNUM(NUM1));
}
BIGNUM BIGNUM::operator / (float NUM1){
    return *this / *(new BIGNUM(NUM1));
}
BIGNUM BIGNUM::operator % (float NUM1){
    return *this % *(new BIGNUM(NUM1));
}
BIGNUM BIGNUM::operator = (float NUM1){
    return *this = *(new BIGNUM(NUM1));
}
bool BIGNUM::operator > (float NUM1){
    return *this > *(new BIGNUM(NUM1));
}
bool BIGNUM::operator < (float NUM1){
    return *this < *(new BIGNUM(NUM1));
}
bool BIGNUM::operator == (float NUM1){
    return *this == *(new BIGNUM(NUM1));
}
bool BIGNUM::operator >= (float NUM1){
    return *this >= *(new BIGNUM(NUM1));
}
bool BIGNUM::operator <= (float NUM1){
    return *this <= *(new BIGNUM(NUM1));
}
BIGNUM BIGNUM::operator += (float NUM1){
    BIGNUM res = *this + *(new BIGNUM(NUM1));
    *this = res;
    return res;
}
BIGNUM BIGNUM::operator -= (float NUM1){
    BIGNUM res = *this - *(new BIGNUM(NUM1));
    *this = res;
    return res;
}
BIGNUM BIGNUM::operator *= (float NUM1){
    BIGNUM res = *this * *(new BIGNUM(NUM1));
    *this = res;
    return res;
}
BIGNUM BIGNUM::operator /= (float NUM1){
    BIGNUM res = *this / *(new BIGNUM(NUM1));
    *this = res;
    return res;
}
BIGNUM BIGNUM::operator %= (float NUM1){
    BIGNUM res = *this % *(new BIGNUM(NUM1));
    *this = res;
    return res;
}


// ===========================================  double  ===========================================
BIGNUM BIGNUM::operator + (double NUM1){
    return *this + *(new BIGNUM(NUM1));
}
BIGNUM BIGNUM::operator - (double NUM1){
    return *this - *(new BIGNUM(NUM1));
}
BIGNUM BIGNUM::operator * (double NUM1){
    return *this * *(new BIGNUM(NUM1));
}
BIGNUM BIGNUM::operator / (double NUM1){
    return *this / *(new BIGNUM(NUM1));
}
BIGNUM BIGNUM::operator % (double NUM1){
    return *this % *(new BIGNUM(NUM1));
}
BIGNUM BIGNUM::operator = (double NUM1){
    return *this = *(new BIGNUM(NUM1));
}
bool BIGNUM::operator > (double NUM1){
    return *this > *(new BIGNUM(NUM1));
}
bool BIGNUM::operator < (double NUM1){
    return *this < *(new BIGNUM(NUM1));
}
bool BIGNUM::operator == (double NUM1){
    return *this == *(new BIGNUM(NUM1));
}
bool BIGNUM::operator >= (double NUM1){
    return *this >= *(new BIGNUM(NUM1));
}
bool BIGNUM::operator <= (double NUM1){
    return *this <= *(new BIGNUM(NUM1));
}
BIGNUM BIGNUM::operator += (double NUM1){
    BIGNUM res = *this + *(new BIGNUM(NUM1));
    *this = res;
    return res;
}
BIGNUM BIGNUM::operator -= (double NUM1){
    BIGNUM res = *this - *(new BIGNUM(NUM1));
    *this = res;
    return res;
}
BIGNUM BIGNUM::operator *= (double NUM1){
    BIGNUM res = *this * *(new BIGNUM(NUM1));
    *this = res;
    return res;
}
BIGNUM BIGNUM::operator /= (double NUM1){
    BIGNUM res = *this / *(new BIGNUM(NUM1));
    *this = res;
    return res;
}
BIGNUM BIGNUM::operator %= (double NUM1){
    BIGNUM res = *this % *(new BIGNUM(NUM1));
    *this = res;
    return res;
}