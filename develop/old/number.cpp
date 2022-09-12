#include <string>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <cmath>

#include "number.h"

#include <chrono>
#include <time.h>

#define DIGITNUM 9
#define DIGIT10 (int)pow(10, (int)DIGITNUM)
#define DIGITINT 10/DIGITNUM + 1
#define DIGITFLOAT 38/DIGITNUM + 1
#define DIGITDOUBLE 308/DIGITNUM + 1

BIGNUM abs(BIGNUM NUM0){
    NUM0.sign = 0;
    return NUM0;
}

bool isZero(BIGNUM const &NUM0){
    return !NUM0.isInf && !NUM0.INT.size() && !NUM0.FLT.size();
}

bool compareAbs(BIGNUM &NUM0, BIGNUM const &NUM1){
    if(NUM0.INT.size() != NUM1.INT.size()) return NUM0.INT.size() < NUM1.INT.size();
    int digitInt, digitFlt;
    digitInt = NUM0.INT.size();
    digitFlt = std::min(NUM0.FLT.size(), NUM1.FLT.size());

    for(int i=digitInt-1; i>=0; i--) if(NUM0.INT[i] != NUM1.INT[i]) return NUM0.INT[i] < NUM1.INT[i];
    for(int i=0; i<digitFlt; i++) if(NUM0.FLT[i] != NUM1.FLT[i]) return NUM0.FLT[i] < NUM1.FLT[i];
    return NUM0.FLT.size() < NUM1.FLT.size();
}


BIGNUM::BIGNUM(){
    this->sign = 0;
    this->isInf = 0;
}
BIGNUM::BIGNUM(int x){
    int N[DIGITINT], digit=0;

    this->isInf = 0;
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
};
BIGNUM::BIGNUM(float x){
    int N[DIGITFLOAT]={0}, digitN=0;
    int F[DIGITFLOAT]={0}, digitF=0;
    float dx;

    this->isInf = 0;
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
    this->INT.resize(std::max(digitN, 0));
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
    this->FLT.resize(std::max(digitF, 0));
    for(int i=0; i<digitF;i++){
        this->FLT[i] = F[i];
    }
};
BIGNUM::BIGNUM(double x){
    int N[DIGITDOUBLE]={0}, digitN=0;
    int F[DIGITDOUBLE]={0}, digitF=0;
    float dx;
    
    this->isInf = 0;
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
    this->INT.resize(std::max(digitN, 0));
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
    this->FLT.resize(std::max(digitF, 0));
    for(int i=0; i<digitF;i++){
        this->FLT[i] = F[i];
    }
};
BIGNUM::BIGNUM(std::string x){
    std::string n, f;
    int c, idx;

    this->isInf = 0;
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

    this->INT.resize(std::max(((int)n.length() - 1)/DIGITNUM + 1, 0));
    this->INT[this->INT.size()-1] = std::stoi(n.substr(0, ((int)n.length() - 1) % DIGITNUM + 1));
    for(int i=(int)this->INT.size()-2; i>=0; i--) this->INT[i] = std::stoi(n.substr((int)n.length() % DIGITNUM + ((int)this->INT.size()-2-i) * DIGITNUM, DIGITNUM));

    if(!f.length()) return;
    this->FLT.resize(std::max(((int)f.length() - 1)/DIGITNUM + 1, 0));
    for(int i=0; i<(int)this->FLT.size()-1; i++) this->FLT[i] = std::stoi(f.substr(i * DIGITNUM, DIGITNUM));
    this->FLT[this->FLT.size()-1] = std::stoi(f.substr((this->FLT.size()-1) * DIGITNUM, ((int)f.length() - 1) % DIGITNUM + 1)) * pow(10, DIGITNUM - (int)f.length() % DIGITNUM);

    this->retract();
}

int BIGNUM::degree(){
    int d = 0, tmp = 1;
    if(this->INT.size()){
        d = DIGITNUM * (this->INT.size()-1);
        while(tmp <= this->INT.back()){
            d++;
            tmp *= 10;
        }
    }
    if(d == 0) d -= this->subdegree();
    return d;
};
int BIGNUM::subdegree(){
    if(!this->FLT.size()) return 0;
    int d = DIGITNUM * (this->FLT.size()-1), tmp = DIGIT10;
    while(tmp > this->FLT.back()){
        d++;
        tmp /= 10;
    }
    return d-1;
};
void BIGNUM::retract(){
    int fltZero = 0;
    for(int i=this->INT.size()-1; i>=0; i--){
        if(this->INT[i] == 0) fltZero++;
        else break;
    }
    this->INT.resize(std::max((int)this->INT.size() - fltZero, 0));
    fltZero = 0;
    for(int i=this->FLT.size()-1; i>=0; i--){
        if(this->FLT[i] == 0) fltZero++;
        else break;
    }
    this->FLT.resize(std::max((int)this->FLT.size() - fltZero, 0));
};
int BIGNUM::firstDigit(){
    int x = 0;
    if(this->INT.size()){
        x = this->INT.back();
    }
    else if(this->FLT.size()){
        for(int i=0; i<this->FLT.size(); i++) if(this->FLT[i] != 0) {x = this->FLT[i]; break;}
    }
    while(x >= 10) x /= 10;
    return x;
};
int BIGNUM::firstDigit2(){
    int x = 0, sizeI = this->INT.size(), sizeF = this->FLT.size();
    if(sizeI){
        x = this->INT.back();
        if(x < 10){
            if(sizeI > 1) x = 10*x + this->INT[sizeI-2] / (DIGIT10 / 10);
            else if(sizeF) x = 10*x + this->FLT[0] / (DIGIT10 / 10);
            else x *= 10;
        }
    }
    else if(sizeF){
        for(int i=0; i<sizeF; i++) if(this->FLT[i] != 0) {x = this->FLT[i]; break;}
    }
    while(x >= 100) x /= 10;
    return x;
};

// ================================================================================================

std::ostream& operator << (std::ostream &os, const BIGNUM &m){
    int digitFix;
    if(m.sign) std::cout << "-" << std::flush;
    if(m.isInf) {
        std::cout << "Inf" << std::flush;
        return os;
    }

    if(m.INT.size()){
        std::cout << m.INT.back() << std::flush;
        for(int i=m.INT.size()-2; i>=0; i--) {
            digitFix = DIGIT10/10;
            while(digitFix>1) {
                if(m.INT[i] < digitFix) std::cout << "0" << std::flush;
                else break;
                digitFix /= 10;
            }
            std::cout << m.INT[i] << std::flush;
        }
    }
    else{
        std::cout << 0 << std::flush;
    }

    if(m.FLT.size()){
        std::cout << "." << std::flush;
        for(int i=0; i<m.FLT.size()-1; i++) {
            digitFix = DIGIT10/10;
            while(digitFix>1) {
                if(m.FLT[i] < digitFix) std::cout << "0" << std::flush;
                else break;
                digitFix /= 10;
            }
            std::cout << m.FLT[i] << std::flush;
        }
        digitFix = DIGIT10/10;
        while(digitFix>1) {
            if(m.FLT.back() < digitFix) std::cout << "0" << std::flush;
            else break;
            digitFix /= 10;
        }
        digitFix = m.FLT.back();
        while(digitFix % 10  == 0 && digitFix != 0) digitFix /= 10;
        std::cout << digitFix << std::flush;
    }

    return os;
};

BIGNUM BIGNUM::operator << (unsigned long long int NUM1){
    unsigned long long int digitSet, digitUnit;
    int *changeSet, *changeSetNext, *targetSet;
    int digitShift, multiple, digitClear;
    BIGNUM res = *this;

    digitSet = NUM1 / DIGITNUM;
    digitUnit = NUM1 % DIGITNUM;
    digitShift =  digitSet + (digitUnit ? 1 : 0);
    multiple = pow(10, digitUnit);
    res.INT.resize(res.INT.size() + digitShift);

    for(int i=0; i<digitShift; i++) res.INT[(int)res.INT.size() - 1 - i] = 0;

    for(int i=(int)res.INT.size()-1; i>=-(int)res.FLT.size()+digitShift; i--){
        changeSet = i<0 ? &res.FLT[-(i+1)] : &res.INT[i];
        changeSetNext = (i-1)<0 ? &res.FLT[-i] : &res.INT[i-1];
        targetSet = (i-digitShift)<0 ? &res.FLT[-(i-digitShift+1)] : &res.INT[i-digitShift];
        *changeSet += *targetSet / (DIGIT10/multiple);
        *changeSetNext = multiple * (*targetSet % (DIGIT10/multiple));
    }
    
    if((int)res.FLT.size() - digitShift + 1 < 0){
        digitClear = -(res.FLT.size() - digitShift + 1) - 1;
        for(int i=0; i<=digitClear; i++) res.INT[i] = 0;
    }
    res.FLT.resize(std::max((long long int)(res.FLT.size() - digitShift + 1), (long long int)0));

    res.retract();
    return res;
};
BIGNUM BIGNUM::operator >> (unsigned long long int NUM1){
    unsigned long long int digitSet, digitUnit;
    int *changeSet, *changeSetNext, *targetSet;
    int digitShift, multiple, digitClear;
    BIGNUM res = *this;

    digitSet = NUM1 / DIGITNUM;
    digitUnit = NUM1 % DIGITNUM;
    digitShift =  digitSet + (digitUnit ? 1 : 0);
    multiple = pow(10, digitUnit);
    res.FLT.resize(res.FLT.size() + digitShift);
    for(int i=0; i<digitShift; i++) res.FLT[(int)res.FLT.size() - 1 - i] = 0;

    for(int i=-1 * (int)res.FLT.size(); i<(int)res.INT.size()-digitShift; i++){
        changeSet = i<0 ? &res.FLT[-(i+1)] : &res.INT[i];
        changeSetNext = (i+1)<0 ? &res.FLT[-(i+2)] : &res.INT[i+1];
        targetSet = (i+digitShift)<0 ? &res.FLT[-(i+digitShift+1)] : &res.INT[i+digitShift];
        *changeSet += (DIGIT10/multiple) * (*targetSet % multiple);
        *changeSetNext = (*targetSet / multiple);
    }

    if((int)res.INT.size() - digitShift + 1 < 0){
        digitClear = -(res.INT.size() - digitShift + 1) - 1;
        for(int i=0; i<=digitClear; i++) res.FLT[i] = 0;
    }
    res.INT.resize(std::max((long long int)(res.INT.size() - digitShift + 1), (long long int)0));

    res.retract();
    return res;
};
BIGNUM BIGNUM::operator <<= (unsigned long long int NUM1){
    unsigned long long int digitSet, digitUnit;
    int *changeSet, *changeSetNext, *targetSet;
    int digitShift, multiple, digitClear;

    digitSet = NUM1 / DIGITNUM;
    digitUnit = NUM1 % DIGITNUM;
    digitShift =  digitSet + (digitUnit ? 1 : 0);
    multiple = pow(10, digitUnit);
    this->INT.resize(this->INT.size() + digitShift);

    for(int i=0; i<digitShift; i++) this->INT[(int)this->INT.size() - 1 - i] = 0;

    for(int i=(int)this->INT.size()-1; i>=-(int)this->FLT.size()+digitShift; i--){
        changeSet = i<0 ? &this->FLT[-(i+1)] : &this->INT[i];
        changeSetNext = (i-1)<0 ? &this->FLT[-i] : &this->INT[i-1];
        targetSet = (i-digitShift)<0 ? &this->FLT[-(i-digitShift+1)] : &this->INT[i-digitShift];
        *changeSet += *targetSet / (DIGIT10/multiple);
        *changeSetNext = multiple * (*targetSet % (DIGIT10/multiple));
    }
    
    if((int)this->FLT.size() - digitShift + 1 < 0){
        digitClear = -(this->FLT.size() - digitShift + 1) - 1;
        for(int i=0; i<=digitClear; i++) this->INT[i] = 0;
    }
    this->FLT.resize(std::max((long long int)(this->FLT.size() - digitShift + 1), (long long int)0));

    this->retract();
    return *this;
};
BIGNUM BIGNUM::operator >>= (unsigned long long int NUM1){
    unsigned long long int digitSet, digitUnit;
    int *changeSet, *changeSetNext, *targetSet;
    int digitShift, multiple, digitClear;

    digitSet = NUM1 / DIGITNUM;
    digitUnit = NUM1 % DIGITNUM;
    digitShift =  digitSet + (digitUnit ? 1 : 0);
    multiple = pow(10, digitUnit);
    this->FLT.resize(this->FLT.size() + digitShift);
    for(int i=0; i<digitShift; i++) this->FLT[(int)this->FLT.size() - 1 - i] = 0;

    for(int i=-1 * (int)this->FLT.size(); i<(int)this->INT.size()-digitShift; i++){
        changeSet = i<0 ? &this->FLT[-(i+1)] : &this->INT[i];
        changeSetNext = (i+1)<0 ? &this->FLT[-(i+2)] : &this->INT[i+1];
        targetSet = (i+digitShift)<0 ? &this->FLT[-(i+digitShift+1)] : &this->INT[i+digitShift];
        *changeSet += (DIGIT10/multiple) * (*targetSet % multiple);
        *changeSetNext = (*targetSet / multiple);
    }

    if((int)this->INT.size() - digitShift + 1 < 0){
        digitClear = -(this->INT.size() - digitShift + 1) - 1;
        for(int i=0; i<=digitClear; i++) this->FLT[i] = 0;
    }
    this->INT.resize(std::max((long long int)(this->INT.size() - digitShift + 1), (long long int)0));

    this->retract();
    return *this;
};

// ===========================================  BIGNUM  ===========================================
BIGNUM BIGNUM::operator + (BIGNUM const &NUM1){
    BIGNUM res;
    if(this->sign != NUM1.sign) {
        res = NUM1;
        res.sign ^= 1;
        return *this - res;
    }
    
    int digitInt, digitFlt, tmpNUM0, tmpNUM1;
    int sizeINT0, sizeFLT0, sizeINT1, sizeFLT1;
    bool isCarry = 0;

    sizeINT0 = this->INT.size();
    sizeFLT0 = this->FLT.size();
    sizeINT1 = NUM1.INT.size();
    sizeFLT1 = NUM1.FLT.size();

    res.sign = this->sign;

    digitInt = std::max(sizeINT0, sizeINT1);
    digitFlt = std::max(sizeFLT0, sizeFLT1);

    res.INT.resize(digitInt);
    res.FLT.resize(digitFlt);

    for(int i=digitFlt-1; i>=0; i--){
        tmpNUM0 = sizeFLT0 <= i ? 0 : this->FLT[i];
        tmpNUM1 = sizeFLT1 <= i ? 0 : NUM1.FLT[i];

        res.FLT[i] = tmpNUM0 + tmpNUM1 + isCarry;
        isCarry = (res.FLT[i] > DIGIT10);
        res.FLT[i] %= DIGIT10;
    }
    for(int i=0; i<digitInt; i++){
        tmpNUM0 = sizeINT0 <= i ? 0 : this->INT[i];
        tmpNUM1 = sizeINT1 <= i ? 0 : NUM1.INT[i];
        
        res.INT[i] = tmpNUM0 + tmpNUM1 + isCarry;
        isCarry = (res.INT[i] > DIGIT10);
        res.INT[i] %= DIGIT10;
    }
    if(isCarry){
        res.INT.resize(digitInt + 1);
        res.INT[digitInt] = 1;
    }
    res.retract();
    return res;
}
BIGNUM BIGNUM::operator - (BIGNUM const &NUM1){
    BIGNUM res;
    BIGNUM const *_NUM0, *_NUM1;
    if(this->sign != NUM1.sign) {
        res = NUM1;
        res.sign ^= 1;
        return *this + res;
    }

    if(compareAbs(*this, NUM1)) {
        _NUM0 = &NUM1;
        _NUM1 = this;
        res.sign = !this->sign;
    }
    else{
        _NUM0 = this;
        _NUM1 = &NUM1;
        res.sign = this->sign;
    }

    int digitInt, digitFlt, tmpNUM0, tmpNUM1;
    int sizeINT0, sizeFLT0, sizeINT1, sizeFLT1;
    bool isCarry = 0;

    sizeINT0 = _NUM0->INT.size();
    sizeFLT0 = _NUM0->FLT.size();
    sizeINT1 = _NUM1->INT.size();
    sizeFLT1 = _NUM1->FLT.size();

    digitInt = std::max(sizeINT0, sizeINT1);
    digitFlt = std::max(sizeFLT0, sizeFLT1);

    res.INT.resize(digitInt);
    res.FLT.resize(digitFlt);

    for(int i=digitFlt-1; i>=0; i--){
        tmpNUM0 = sizeFLT0 <= i ? 0 : _NUM0->FLT[i];
        tmpNUM1 = sizeFLT1 <= i ? 0 : _NUM1->FLT[i];

        res.FLT[i] = tmpNUM0 - tmpNUM1 - isCarry;
        isCarry = (res.FLT[i] < 0);
        if(isCarry) res.FLT[i] += DIGIT10;
    }

    for(int i=0; i<digitInt; i++){
        tmpNUM0 = sizeINT0 <= i ? 0 : _NUM0->INT[i];
        tmpNUM1 = sizeINT1 <= i ? 0 : _NUM1->INT[i];

        res.INT[i] = tmpNUM0 - tmpNUM1 - isCarry;
        isCarry = (res.INT[i] < 0);
        if(isCarry) res.INT[i] += DIGIT10;
    }

    res.retract();
    return res;
}
BIGNUM BIGNUM::operator * (BIGNUM const &NUM1){
    BIGNUM res;
    int digitInt, digitFlt;
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

    res.retract();
    return res;
}
BIGNUM BIGNUM::operator / (BIGNUM NUM1){
    BIGNUM NUM0 = *this, res;
    int digitNUM0, digitNUM1, digitExact, nowDigit, rangeL, f0, f1;

    if(isZero(NUM1)) {
        res.isInf = 1;
        return res;
    }

    digitNUM0 = NUM0.degree();
    digitNUM1 = NUM1.degree();
    nowDigit = digitNUM0 - digitNUM1;
    digitExact = abs(digitNUM0) + abs(digitNUM1);

    res.INT.resize(std::max((nowDigit - 1)/DIGITNUM + 1, 0));
    res.FLT.resize(std::max((digitExact - 1)/DIGITNUM + 1, 0));
    for(int i=0; i<res.INT.size(); i++) res.INT[i] = 0;
    for(int i=0; i<res.FLT.size(); i++) res.FLT[i] = 0;

    f1 = NUM1.firstDigit2();
    if(digitNUM0 > digitNUM1) NUM1 <<= (digitNUM0 - digitNUM1);
    else NUM1 >>= (digitNUM0 - digitNUM1);
    digitNUM1 = digitNUM0;

    while(nowDigit*(-1) < digitExact && NUM0 != 0){
        if(NUM0 < NUM1) {
            NUM1 >>= 1;
            digitNUM1--;
            nowDigit--;
            continue;
        }
        f0 = NUM0.firstDigit2();
        // rangeL = f0>=f1 ? f0/(f1 + 1) : (10*f0 - 1)/(f1 + 1);
        rangeL = f0>=f1 ? f0/f1 : (10*f0 - 1)/(f1 + 1);

        for(int i=rangeL; i<=10; i++){
            // if(NUM1 * i > NUM0){
            if((f1 * i > f0 * pow(10, digitNUM0 - digitNUM1) + i) || (NUM1 * i > NUM0)){
                if(nowDigit >= 0){
                    res.INT[nowDigit/DIGITNUM] += (i-1) * pow(10, nowDigit % DIGITNUM);
                }
                else{
                    res.FLT[-(nowDigit+1)/DIGITNUM] += (i-1) * pow(10, DIGITNUM + nowDigit % DIGITNUM);
                }
                NUM0 -= NUM1 * (i-1);
                NUM1 >>= 1;
                digitNUM0 = NUM0.degree();
                digitNUM1--;
                break;
            }
        }
        nowDigit--;
    }

    res.retract();
    return res;
}
BIGNUM BIGNUM::operator % (BIGNUM NUM1){
    BIGNUM res = abs(*this), divNUM = abs(NUM1);
    int digitNUM0, digitNUM1, rangeL, f0, f1;
    if(isZero(NUM1)) {
        res.isInf = 1;
        return res;
    }
    if(res < NUM1) return *this;

    digitNUM0 = res.degree();
    digitNUM1 = NUM1.degree();

    res.FLT.resize(std::max(this->FLT.size(), NUM1.FLT.size()));

    f1 = NUM1.firstDigit2();
    divNUM = NUM1 << (digitNUM0 - digitNUM1);
    digitNUM1 = digitNUM0;

    while(res > NUM1){
        if(res < divNUM) {
            divNUM >>= 1;
            digitNUM1--;
            continue;
        }
        f0 = res.firstDigit2();
        // rangeL = f0>=f1 ? f0/(f1 + 1) : (10*f0 - 1)/(f1 + 1);
        rangeL = f0>=f1 ? f0/f1 : (10*f0 - 1)/(f1 + 1);
        for(int i=rangeL; i<=10; i++){
            // if(divNUM * i > res){
            if((f1 * i > f0 * pow(10, digitNUM0 - digitNUM1) + i) || (divNUM * i > res)){
                res -= divNUM * (i-1);
                divNUM >>= 1;
                digitNUM0 = res.degree();
                digitNUM1--;
                break;
            }
        }
    }

    res.retract();
    return res;
}
// BIGNUM BIGNUM::operator = (BIGNUM NUM1){
//     this->INT.resize(NUM1.INT.size());
//     this->FLT.resize(NUM1.FLT.size());
//     for(int i=0; i<NUM1.INT.size(); i++){
//         this->INT[i] = NUM1.INT[i];
//     }
//     for(int i=0; i<NUM1.FLT.size(); i++){
//         this->FLT[i] = NUM1.FLT[i];
//     }
//     this->sign = NUM1.sign;
//     this->isInf = NUM1.isInf;
//     return *this;
// }
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
    *this = *this + NUM1;
    return *this;
}
BIGNUM BIGNUM::operator -= (BIGNUM NUM1){
    *this = *this - NUM1;
    return *this;
}
BIGNUM BIGNUM::operator *= (BIGNUM NUM1){
    *this = *this * NUM1;
    return *this;
}
BIGNUM BIGNUM::operator /= (BIGNUM NUM1){
    *this = *this / NUM1;
    return *this;
}
BIGNUM BIGNUM::operator %= (BIGNUM NUM1){
    *this = *this % NUM1;
    return *this;
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