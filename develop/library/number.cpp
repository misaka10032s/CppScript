#include <string>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <cmath>
// #include <stdexcept>

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
BIGNUM::BIGNUM(std::string x){
    std::string n, f;
    int c, idx;

    this->sign = x.substr(0,1) == (std::string)"-";
    for(idx=this->sign; idx<x.length(); idx++){
        c = x[idx];
        if(48 <= c && c <= 57) n += x[idx];
        else if(c == 46) break;
    }
    for(; idx<x.length(); idx++){
        c = x[idx];
        if(48 <= c && c <= 57) f += x[idx];
    }
    this->INT.resize(std::max(((int)n.length() - 1)/DIGITNUM + 1, 1));
    this->FLT.resize(std::max(((int)f.length() - 1)/DIGITNUM + 1, 1));

    this->INT[this->INT.size()-1] = std::stoi(n.substr(0, ((int)n.length() - 1) % DIGITNUM + 1));
    for(int i=(int)this->INT.size()-2; i>=0; i--) this->INT[i] = std::stoi(n.substr((int)n.length() % DIGITNUM + i * DIGITNUM, DIGITNUM));

    if(!f.length()) return;
    for(int i=0; i<(int)this->FLT.size()-1; i++) this->FLT[i] = std::stoi(f.substr(i * DIGITNUM, DIGITNUM));
    this->FLT[this->FLT.size()-1] = std::stoi(f.substr((this->FLT.size()-1) * DIGITNUM, ((int)f.length() - 1) % DIGITNUM + 1)) * pow(10, DIGITNUM - (int)f.length() % DIGITNUM);
}

int BIGNUM::degree(){
    int d =  DIGITNUM * (this->INT.size()-1), tmp = 1;
    while(tmp <= this->INT.back()){
        d++;
        tmp *= 10;
    }
    if(d == 0 && this->INT[0] == 0) d -= this->subdegree();
    return d;
};
int BIGNUM::subdegree(){
    int d =  DIGITNUM * (this->FLT.size()-1), tmp = DIGIT10;
    while(tmp > this->FLT.back()){
        d++;
        tmp /= 10;
    }
    return d;
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
    if(NUM1 > *this) {
        res = NUM1 - *this;
        res.sign ^= 1;
        return res;
    }
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
        res.FLT[i] = this->FLT[i] - NUM1.FLT[i] - isCarry;
        isCarry = (res.FLT[i] < 0);
        if(isCarry) res.FLT[i] += DIGIT10;
    }
    for(int i=0; i<digitInt; i++){
        if(NUM1.INT.size() <= i || this->INT.size() <= i) continue;
        res.INT[i] = this->INT[i] - NUM1.INT[i] - isCarry;
        isCarry = (res.INT[i] < 0);
        if(isCarry) res.INT[i] += DIGIT10;
    }
    if(isCarry){
        res.INT.resize(std::max(digitInt - 1, 1));
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
    int digitInt, digitFlt, fltZero;
    long long int tmp;

    res.sign = this->sign ^ NUM1.sign;

    digitInt = this->INT.size() + NUM1.INT.size();
    digitFlt = this->FLT.size() + NUM1.FLT.size();

    res.INT.resize(digitInt);
    res.FLT.resize(digitFlt);

    for(int i=0; i<res.INT.size(); i++) res.INT[i] = 0;
    for(int i=0; i<res.FLT.size(); i++) res.FLT[i] = 0;

    for(int i=-1 * (int)this->FLT.size(); i<(int)this->INT.size(); i++){
        for(int j=-1 * (int)NUM1.FLT.size(); j<(int)NUM1.INT.size(); j++){
            tmp = (long long int)(i >= 0 ? this->INT[i] : this->FLT[-1 * i - 1]) * (long long int)(j >= 0 ? NUM1.INT[j] : NUM1.FLT[-1 * j - 1]);
            
            if(i + j >= 0){
                res.INT[i + j] += tmp % DIGIT10;
                res.INT[i + j + 1] += floor(tmp / DIGIT10);

                res.INT[i + j + 1] += res.INT[i + j] / DIGIT10;
                res.INT[i + j] %= DIGIT10;
            }
            else if(i + j == -1){
                res.FLT[-1 * (i + j) - 1] += tmp % DIGIT10;
                res.INT[i + j + 1] += floor(tmp / DIGIT10);

                res.INT[i + j + 1] += res.FLT[-1 * (i + j) - 1] / DIGIT10;
                res.FLT[-1 * (i + j) - 1] %= DIGIT10;
            }
            else{
                res.FLT[-1 * (i + j) - 1] += tmp % DIGIT10;
                res.FLT[-1 * (i + j + 1) - 1] += floor(tmp / DIGIT10);

                res.FLT[-1 * (i + j + 1) - 1] += res.FLT[-1 * (i + j) - 1] / DIGIT10;
                res.FLT[-1 * (i + j) - 1] %= DIGIT10;
            }
        }
    }

    fltZero = 0;
    for(int i=digitInt-1; i>=0; i--){
        if(res.INT[i] == 0) fltZero++;
    }
    res.INT.resize(std::max(digitInt - fltZero, 1));

    fltZero = 0;
    for(int i=digitFlt-1; i>=0; i--){
        if(res.FLT[i] == 0) fltZero++;
    }
    res.FLT.resize(std::max(digitFlt-fltZero, 1));

    return res;
}
BIGNUM BIGNUM::operator / (BIGNUM NUM1){ // @@@@
    // if(NUM1 == 0) throw std::invalid_argument("division by zero");
    BIGNUM NUM0 = *this, res;
    int digitNUM0, digitNUM1, digitExact, nowDigit;
    std::string digitShift;

    digitNUM0 = NUM0.degree();
    digitNUM1 = NUM1.degree();
    nowDigit = digitNUM0 - digitNUM1;
    digitExact = abs(digitNUM0) + abs(digitNUM1);

    if(digitNUM0 > digitNUM1){
        digitShift = "1";
        for(int i=0; i<digitNUM0 - digitNUM1; i++) digitShift += "0";
    }
    else if(digitNUM0 < digitNUM1){
        digitShift = "0.";
        for(int i=0; i<digitNUM1 - digitNUM0; i++) digitShift += "0";
        digitShift += "1";
    }

    res.INT.resize(std::max((nowDigit - 1)/DIGITNUM + 1, 1));
    res.FLT.resize(std::max((digitExact - 1)/DIGITNUM + 1, 1));
    for(int i=0; i<res.INT.size(); i++) res.INT[i] = 0;
    for(int i=0; i<res.FLT.size(); i++) res.FLT[i] = 0;

    NUM1 *= (BIGNUM)digitShift;
// std::cout<<"digitExact "<<digitNUM0<<", "<<digitNUM1<<", "<<nowDigit<<", "<<digitExact<<", "<<digitShift<<", "<<NUM1<<"\n";
    while(nowDigit*(-1) < digitExact && NUM0 != 0){
        for(int i=1; i<10; i++){
// std::cout<<"NUMa "<<NUM0<<", "<<NUM1<<"\n";
            if(NUM1 * i > NUM0){
                if(nowDigit >= 0){
                    res.INT[nowDigit/DIGITNUM] += (i-1) * pow(10, nowDigit % DIGITNUM);
                }
                else{
                    res.FLT[-(nowDigit+1)/DIGITNUM] += (i-1) * pow(10, DIGITNUM + nowDigit % DIGITNUM);
                }
// std::cout<<"NUMx "<<NUM1 * (i-1)<<"\n";
                NUM0 -= NUM1 * (i-1);
// std::cout<<"NUMb "<<NUM0<<", "<<NUM1<<"\n";
                NUM1 *= (BIGNUM)"0.1";
                break;
            }
        }
// std::cout<<"NUMc "<<NUM0<<", "<<NUM1<<"\n";
        nowDigit--;
    }

    return res;
}
BIGNUM BIGNUM::operator % (BIGNUM NUM1){ // @@@@
    // if(NUM1 == 0) throw std::invalid_argument("division by zero");
    BIGNUM NUM0 = *this, res;
    return res;
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
bool BIGNUM::operator != (BIGNUM NUM1){
    return !(*this==NUM1);
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
// std::cout<<"-= "<<*this<<"\n";
// std::cout<<"-= "<<this->INT.size()<<", "<<this->FLT.size()<<"\n";
// std::cout<<"-= "<<NUM1<<"\n";
// std::cout<<"-= "<<NUM1.INT.size()<<", "<<NUM1.FLT.size()<<"\n";
// std::cout<<"-= "<<&res<<"\n";
// std::cout<<"-= "<<res.INT.size()<<", "<<res.FLT.size()<<"\n";
// std::cout<<"-= "<<res<<"\n";
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
bool BIGNUM::operator != (int NUM1){
    return !(*this == *(new BIGNUM(NUM1)));
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
bool BIGNUM::operator != (float NUM1){
    return !(*this == *(new BIGNUM(NUM1)));
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
bool BIGNUM::operator != (double NUM1){
    return !(*this == *(new BIGNUM(NUM1)));
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