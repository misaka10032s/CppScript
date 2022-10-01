#define SYS_LINUX
// #define SYS_WINDOWS
#include "systemFnc.h"

#include <string>
#include <iostream>
#include <vector>
#include <unordered_map>
#define _USE_MATH_DEFINES
#include <cmath>
#include <algorithm>

#include "number2.h"

#include <chrono>
#include <time.h>

inline bool testExe(bool x){
    std::cout<<"testExe\n"<<std::endl;
    return x;
}

/*
    ###########################################################################################################
    #                                                                                                         #
    #                                                                                                         #
    #                                                  BIGNUM                                                #
    #                                                                                                         #
    #                                                                                                         #
    ###########################################################################################################
*/
BIGNUM abs(BIGNUM NUM0){
    NUM0.sign = 0;
    return NUM0;
}

int pow10List[10] = {
    1, 10, 100, 1000, 10000, 
    100000, 1000000, 10000000, 100000000, 1000000000
};
inline int quick_pow10(int n){
    return pow10List[n];
}

inline int getDigNeed(int NUM0){
    if(NUM0) return floor(log10(NUM0) + 1);
    return 0;
}

inline int getDigNeed(BIGNUM const &NUM0){
    for(int i=NUM0.value.size()-1; i>=0; i--){
        if(NUM0.value[i] != 0){
            return DIGITNUM * i + floor(log10(NUM0.value[i]) + 1);
        }
    }
    return 0;
}

inline int getDignum(BIGNUM const &NUM0){
    for(int i=NUM0.value.size()-1; i>=0; i--){
        if(NUM0.value[i] != 0){
            return DIGITNUM * i + floor(log10(NUM0.value[i]) + 1) - NUM0.point;
        }
    }
    
    return 0;
};

inline void simpleMinus(BIGNUM &NUM0, BIGNUM const &NUM1, int mul){ // NUM0 - NUM1 * mul
    int digit, size0, size1;
    int pointDiff, nextNum;
    int modNUM0, modNUM1, pointOver0, pointOver1, tmpNUM0, tmpNUM1;
    long long int tmp, nextSlotValue = 0;

    pointDiff = (NUM0.point - NUM1.point) % DIGITNUM;

    modNUM0 = quick_pow10(pointDiff < 0 ? DIGITNUM + pointDiff : DIGITNUM);
    modNUM1 = quick_pow10(pointDiff > 0 ? DIGITNUM - pointDiff : DIGITNUM);

    if(NUM0.point - NUM1.point < 0){
        pointOver0 = (NUM1.point - NUM0.point) / DIGITNUM;
        pointOver1 = 0;
    }
    else{
        pointOver0 = 0;
        pointOver1 = (NUM0.point - NUM1.point) / DIGITNUM;
    }

    size0 = (getDigNeed(NUM0) - 1) / DIGITNUM + 1;
    size1 = (getDigNeed(NUM1) - 1) / DIGITNUM + 1;
    digit = std::max(size0, size1);

    NUM0.point = std::max(NUM0.point, NUM1.point);
    if(!(NUM0.value.size() > digit + 1)) NUM0.value.resize(digit + 1);

    for(int i=0; i<digit; i++){
        tmpNUM0 = (0 <= i + pointOver0 && i + pointOver0 < size0) ? NUM0.value[i + pointOver0] : 0;
        tmpNUM1 = (0 <= i + pointOver1 && i + pointOver1 < size1) ? NUM1.value[i + pointOver1] : 0;

        tmp = (long long int)((tmpNUM0 % modNUM0) * (DIGIT10 / modNUM0)) - (long long int)((tmpNUM1 % modNUM1) * (DIGIT10 / modNUM1) * (long long int)mul) * (long long int)quick_pow10(exp%DIGITNUM) + nextSlotValue;
        nextNum = (tmp < 0 ? -(tmp) / (long long int)DIGIT10 + 1 : 0);
        nextSlotValue = (tmpNUM0 / modNUM0) - (tmpNUM1 / modNUM1) * mul - nextNum;
        NUM0.value[i] = tmp + (long long int)nextNum * (long long int)DIGIT10;
    }
    if(nextSlotValue) NUM0.value[digit] = nextSlotValue;
}

inline bool compareAbs(BIGNUM const &NUM0, BIGNUM const &NUM1){ // abs(NUM0) < abs(NUM1)
    if(NUM0.isInf || NUM1.isInf) return 0;

    int dig0 = getDigNeed(NUM0), dig1 = getDigNeed(NUM1), tmp;
    if(dig0 - NUM0.point != dig1 - NUM1.point) return dig0 - NUM0.point < dig1 - NUM1.point;

    bool rev = 0;
    int pointDiff = (NUM0.point - NUM1.point) % DIGITNUM, pointOver = abs(NUM0.point - NUM1.point) / DIGITNUM;
    int modNUM0, modNUM1, tmpNUM0, tmpNUM1;
    int idx0, idx1;
    BIGNUM const *_NUM0, *_NUM1;
    if(pointDiff < 0){
        _NUM0 = &NUM0;
        _NUM1 = &NUM1;
    }
    else{
        _NUM0 = &NUM1;
        _NUM1 = &NUM0;
        tmp = dig0;
        dig0 = dig1;
        dig1 = tmp;
        rev = 1;
    }

    idx0 = (dig0-1)/DIGITNUM + 1;
    idx1 = (dig1-1)/DIGITNUM + 1;
    modNUM1 = quick_pow10(abs(pointDiff));
    modNUM0 = (DIGIT10/modNUM1);
    
    for(int i=std::max(idx0, idx1) - 1; i>=0; i--){
        tmpNUM0 = i < idx0 ? _NUM0->value[i] % modNUM0 : 0;
        tmpNUM1 = (0 <= i - pointOver && i - pointOver < idx1) ? _NUM1->value[i - pointOver] / modNUM1 : 0;
        if(tmpNUM0 != tmpNUM1) return(tmpNUM0 < tmpNUM1) ^ rev;
        
        tmpNUM0 = (0 <= i - 1 && i - 1 < idx0) ? _NUM0->value[i - 1] / modNUM0 : 0;
        tmpNUM1 = (0 <= i - pointOver && i - pointOver < idx1) ? _NUM1->value[i - pointOver] % modNUM1 : 0;
        if(tmpNUM0 != tmpNUM1) return(tmpNUM0 < tmpNUM1) ^ rev;
    }
    return NUM0.point != NUM1.point ? (NUM0.point < NUM1.point) ^ rev : 0;
}

inline bool isZero(BIGNUM const &NUM0){
    if(NUM0.isInf) return 0;
    for(int i=NUM0.value.size()-1; i>=0; i--) if(NUM0.value[i]) return 0;
    return 1;
}

inline int BIGNUM::firstDigit(BIGNUM const &NUM0){
    int x = 0;
    for(int i=NUM0.value.size()-1; i>=0; i--){
        if(NUM0.value[i] != 0){
            x = NUM0.value[i];
            break;
        }
    }
    while(x >= 10) x /= 10;
    return x;
};

inline int BIGNUM::firstDigit2(BIGNUM const &NUM0){
    int x = 0;
    for(int i=NUM0.value.size()-1; i>=0; i--){
        if(NUM0.value[i] != 0){
            x = NUM0.value[i];
            if(x < 10){
                if(i-1>=0) x = 10*x + NUM0.value[i-1] / (DIGIT10 / 10);
                else x *= 10;
            }
            break;
        }
    }

    while(x >= 100) x /= 10;
    return x;
};







BIGNUM::BIGNUM(){
    this->sign = 0;
    this->isInf = 0;
    this->point = 0;
}
BIGNUM::BIGNUM(int x){
    int N[DIGITINT], digit=0;

    this->isInf = 0;
    this->point = 0;
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
    this->value.resize(std::max(digit, 1));
    for(int i=0; i<digit;i++){
        this->value[i] = N[i];
    }
};
BIGNUM::BIGNUM(float x){
    int idx = 0;
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

    for(int i=0; i<DIGITFLOAT; i++){
        N[i] = std::fmod(x, DIGIT10);
        x = floor(x/DIGIT10);
        if(N[i]) digitN++;
        else break;
    }

    dx *= DIGIT10;
    for(int i=0; i<DIGITFLOAT; i++){
        F[i] = floor(dx);
        dx = std::fmod(dx,1) * DIGIT10;
        if(F[i]) digitF++;
        else break;
    }

    this->value.resize(std::max(digitN + digitF, 1));
    for(int i=digitF-1; i>=0; i--){
        this->value[idx++] = F[i];
    }
    for(int i=0; i<digitN; i++){
        this->value[idx++] = N[i];
    }
    this->point = digitF * DIGITNUM;
};
BIGNUM::BIGNUM(double x){
    int idx = 0;
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

    for(int i=0; i<DIGITDOUBLE; i++){
        N[i] = std::fmod(x, DIGIT10);
        x = floor(x/DIGIT10);
        if(N[i]) digitN++;
        else break;
    }

    dx *= DIGIT10;
    for(int i=0; i<DIGITDOUBLE; i++){
        F[i] = floor(dx);
        dx = std::fmod(dx,1) * DIGIT10;
        if(F[i]) digitF++;
        else break;
    }

    this->value.resize(std::max(digitN + digitF, 1));
    for(int i=digitF-1; i>=0; i--){
        this->value[idx++] = F[i];
    }
    for(int i=0; i<digitN; i++){
        this->value[idx++] = N[i];
    }
    this->point = digitF * DIGITNUM;
};
BIGNUM::BIGNUM(std::string x){
    std::string num;
    int c, idx;
    bool firstDot = 1;

    this->isInf = 0;
    this->sign = x.substr(0,1) == (std::string)"-";
    for(idx=this->sign; idx<(int)x.length(); idx++){
        c = x[idx];
        if(48 <= c && c <= 57) num += x[idx];
        else if(c == 46 && firstDot) {
            this->point = (int)num.length();
            firstDot = 0;
        }
    }
    if(firstDot) this->point = 0;
    else this->point = (int)num.length() - this->point;

    if(num.length()){
        this->value.resize(((int)num.length()-1)/DIGITNUM + 1);
        for(int i=(int)this->value.size()-2; i>=0; i--) this->value[i] = std::stoi(num.substr((int)num.length() % DIGITNUM + ((int)this->value.size()-2-i) * DIGITNUM, DIGITNUM));
        this->value[this->value.size()-1] = std::stoi(num.substr(0, ((int)num.length() - 1) % DIGITNUM + 1));
    }
}

void const BIGNUM::retract(){
    int fltZero = 0;
    for(int i=this->value.size()-1; i>=0; i--){
        if(this->value[i] == 0) fltZero++;
        else break;
    }
    this->value.resize(std::max((int)this->value.size() - fltZero, 0));
};


std::ostream& operator << (std::ostream &os, const BIGNUM &m){
    std::string s = "";
    int digitN, idx, tmp, length, zero = 0, dig;
    if(m.sign) std::cout << "-";
    if(m.isInf) {
        std::cout << "Inf" << std::flush;
        return os;
    }
    if(!m.value.size()){
        std::cout << 0 << std::flush;
        return os;
    }
    if(!(m.value[0]%10)){
        for(int i=0; i<(int)m.value.size(); i++){
            dig = 1;
            while(!(m.value[i] % (dig *= 10))){
                zero++;
                if(zero >= m.point) break;
            }
            if(zero % DIGITNUM) break;
        }
    }
    digitN = getDigNeed(m);
    length = (digitN - 1) / DIGITNUM + 1;

    for(int i=0; i<length; i++) {
        tmp = m.value[i];
        for(int j=0; j<DIGITNUM; j++){
            idx = i * DIGITNUM + j + 1;
            if(idx > digitN) break;
            if(idx <= zero && idx < m.point) {
                tmp /= 10;
                continue;
            }
            if(idx == m.point){
                if(idx > zero || tmp%10) s += std::to_string(tmp%10) + ".";
            }
            else s += std::to_string(tmp%10);
            tmp /= 10;
        }
    }
    for(int i=digitN; i<m.point; i++) s += "0";
    if(digitN < m.point) s += ".0";
    else if(digitN == m.point) s += "0";
    std::reverse(s.begin(), s.end());
    std::cout << s << std::flush;
    return os;
};

BIGNUM BIGNUM::operator << (DIGITDATATYPE NUM1){
    if(NUM1 <= 0 || this->isInf) return *this;
    BIGNUM res;
    DIGITDATATYPE digitSet, digitShift, digitUnit, idx;
    int multiple, origSize = (getDigNeed(*this) - 1) / DIGITNUM + 1;

    res.sign = this->sign;
    if(this->point >= NUM1){
        res = *this;
        res.point -= NUM1;
        return res;
    }
    else {
        NUM1 -= this->point;
        res.point = 0;
    }

    digitSet = NUM1 / DIGITNUM;
    digitUnit = NUM1 % DIGITNUM;
    digitShift = digitSet + (digitUnit ? 1 : 0);
    idx = (getDigNeed(*this) - 1) / DIGITNUM + 1;
    multiple = quick_pow10((DIGITNUM - digitUnit) % DIGITNUM);

    res.value.resize(idx + digitShift);
    for(int i=0; i<idx; i++) res.value[i] = 0;

    for(int i=(int)res.value.size()-1; i>=0; i--){
        res.value[i] = (0 <= i-digitShift && i-digitShift < origSize) ? this->value[i-digitShift] / multiple : 0;
        res.value[i] += (0 <= i-digitShift+1 && i-digitShift+1 < origSize) ? this->value[i-digitShift+1] % multiple * (DIGIT10 / multiple) : 0;
    }

    return res;
};
BIGNUM BIGNUM::operator >> (DIGITDATATYPE NUM1){
    if(NUM1 <= 0 || this->isInf) return *this;
    BIGNUM res;
    DIGITDATATYPE zero = 0, digitSet, digitShift, digitUnit, idx;
    int dig, multiple, origSize = this->value.size();

    if(!(this->value[0]%10)){
        for(int i=0; i<(int)this->value.size(); i++){
            dig = 1;
            while(!(this->value[i] % (dig *= 10))){
                zero++;
                if(zero >= NUM1) break;
            }
            if(zero % DIGITNUM) break;
        }
    }
    res.point = this->point + NUM1 - zero;
    res.sign = this->sign;
    if(!zero) {
        res.value = this->value;
        return res;
    }

    digitSet = zero / DIGITNUM;
    digitUnit = zero % DIGITNUM;
    digitShift = digitSet + (digitUnit ? 1 : 0);
    idx = (getDigNeed(*this) - NUM1 + zero - 1) / DIGITNUM + 1;
    multiple = quick_pow10(DIGITNUM - digitUnit);

    res.value.resize(idx);
    for(int i=0; i<idx; i++) res.value[i] = 0;

    for(int i=0; i<origSize; i++){
        res.value[i] = i+digitShift-1 < (int)this->value.size() ? this->value[i+digitShift-1] / (DIGIT10 / multiple) : 0;
        res.value[i] += i+digitShift < (int)this->value.size() ? this->value[i+digitShift] % (DIGIT10 / multiple) * multiple : 0;
    }

    return res;
};

BIGNUM BIGNUM::operator <<= (DIGITDATATYPE NUM1){
    if(NUM1 <= 0 || this->isInf) return *this;
    DIGITDATATYPE digitSet, digitShift, digitUnit, idx;
    int multiple, origSize = (getDigNeed(*this) - 1) / DIGITNUM + 1, tmp;

    if(this->point >= NUM1){
        this->point -= NUM1;
        return *this;
    }
    else {
        NUM1 -= this->point;
        this->point = 0;
    }

    digitSet = NUM1 / DIGITNUM;
    digitUnit = NUM1 % DIGITNUM;
    digitShift = digitSet + (digitUnit ? 1 : 0);
    idx = (getDigNeed(*this) - 1) / DIGITNUM + 1;
    multiple = quick_pow10((DIGITNUM - digitUnit) % DIGITNUM);

    if(origSize < idx + digitShift) this->value.resize(idx + digitShift);
    for(int i=origSize; i<idx + digitShift; i++) this->value[i] = 0;

    for(int i=(int)this->value.size()-1; i>=0; i--){
        tmp = (0 <= i-digitShift && i-digitShift < origSize) ? this->value[i-digitShift] / multiple : 0;
        tmp += (0 <= i-digitShift+1 && i-digitShift+1 < origSize) ? this->value[i-digitShift+1] % multiple * (DIGIT10 / multiple) : 0;
        this->value[i] = tmp;
    }
    return *this;
};
BIGNUM BIGNUM::operator >>= (DIGITDATATYPE NUM1){
    if(NUM1 <= 0 || this->isInf) return *this;
    DIGITDATATYPE zero = 0, digitSet, digitShift, digitUnit, idx;
    int dig, multiple, origSize = this->value.size();

    if(!(this->value[0]%10)){
        for(int i=0; i<origSize; i++){
            dig = 1;
            while(!(this->value[i] % (dig *= 10))){
                zero++;
                if(zero >= NUM1) break;
            }
            if(zero % DIGITNUM) break;
        }
    }
    this->point += NUM1 - zero;
    if(!zero) return *this;

    digitSet = zero / DIGITNUM;
    digitUnit = zero % DIGITNUM;
    digitShift = digitSet + (digitUnit ? 1 : 0);
    idx = (getDigNeed(*this) - NUM1 + zero - 1) / DIGITNUM + 1;
    multiple = quick_pow10(DIGITNUM - digitUnit);

    for(int i=0; i<origSize; i++){
        this->value[i] = i+digitShift-1 < (int)this->value.size() ? this->value[i+digitShift-1] / (DIGIT10 / multiple) : 0;
        this->value[i] += i+digitShift < (int)this->value.size() ? this->value[i+digitShift] % (DIGIT10 / multiple) * multiple : 0;
    }

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

    int digit, tmpNUM0, tmpNUM1, nextSlotValue = 0;
    int size0, size1;
    int pointDiff = (this->point - NUM1.point) % DIGITNUM;
    int modNUM0, modNUM1, pointOver0, pointOver1;

    modNUM0 = quick_pow10(pointDiff < 0 ? DIGITNUM + pointDiff : DIGITNUM);
    modNUM1 = quick_pow10(pointDiff > 0 ? DIGITNUM - pointDiff : DIGITNUM);

    if(this->point - NUM1.point < 0){
        pointOver0 = (NUM1.point - this->point) / DIGITNUM;
        pointOver1 = 0;
    }
    else{
        pointOver0 = 0;
        pointOver1 = (this->point - NUM1.point) / DIGITNUM;
    }

    size0 = (getDigNeed(*this) - 1) / DIGITNUM + 1;
    size1 = (getDigNeed(NUM1) - 1) / DIGITNUM + 1;
    digit = std::max(size0, size1);

    res.sign = this->sign;
    res.point = std::max(this->point, NUM1.point);
    res.value.resize(digit + 1);

    for(int i=0; i<digit; i++){
        tmpNUM0 = i + pointOver0 < size0 ? this->value[i + pointOver0] : 0;
        tmpNUM1 = i + pointOver1 < size1 ? NUM1.value[i + pointOver1] : 0;

        res.value[i] = tmpNUM0 % modNUM0 * (DIGIT10 / modNUM0) + tmpNUM1 % modNUM1 * (DIGIT10 / modNUM1) + nextSlotValue;
        nextSlotValue = tmpNUM0 / modNUM0 + tmpNUM1 / modNUM1 + (res.value[i] > DIGIT10);
        res.value[i] = res.value[i] % DIGIT10;
    }

    return res;
}
BIGNUM BIGNUM::operator - (BIGNUM const &NUM1){
    BIGNUM res;
    if(this->sign != NUM1.sign) {
        res = NUM1;
        res.sign ^= 1;
        return *this + res;
    }
    
    int digit, tmpNUM0, tmpNUM1, nextSlotValue = 0;
    int size0, size1;
    int pointDiff;
    int modNUM0, modNUM1, pointOver0, pointOver1;

    BIGNUM const *_NUM0, *_NUM1;
    if(compareAbs(*this, NUM1)){
        _NUM0 = &NUM1;
        _NUM1 = this;
        res.sign = !this->sign;
    }
    else{
        _NUM0 = this;
        _NUM1 = &NUM1;
        res.sign = this->sign;
    }

    pointDiff = (_NUM0->point - _NUM1->point) % DIGITNUM;

    modNUM0 = quick_pow10(pointDiff < 0 ? DIGITNUM + pointDiff : DIGITNUM);
    modNUM1 = quick_pow10(pointDiff > 0 ? DIGITNUM - pointDiff : DIGITNUM);

    if(_NUM0->point - _NUM1->point < 0){
        pointOver0 = (_NUM1->point - _NUM0->point) / DIGITNUM;
        pointOver1 = 0;
    }
    else{
        pointOver0 = 0;
        pointOver1 = (_NUM0->point - _NUM1->point) / DIGITNUM;
    }

    size0 = (getDigNeed(*_NUM0) - 1) / DIGITNUM + 1;
    size1 = (getDigNeed(*_NUM1) - 1) / DIGITNUM + 1;
    digit = std::max(size0, size1);

    res.point = std::max(this->point, NUM1.point);
    res.value.resize(digit + 1);

    for(int i=0; i<digit; i++){
        tmpNUM0 = i + pointOver0 < size0 ? _NUM0->value[i + pointOver0] : 0;
        tmpNUM1 = i + pointOver1 < size1 ? _NUM1->value[i + pointOver1] : 0;

        res.value[i] = (tmpNUM0 % modNUM0) * (DIGIT10 / modNUM0) - (tmpNUM1 % modNUM1) * (DIGIT10 / modNUM1) + nextSlotValue;
        nextSlotValue = tmpNUM0 / modNUM0 - tmpNUM1 / modNUM1 - (res.value[i] < 0);
        if(res.value[i] < 0) res.value[i] += DIGIT10;
    }

    return res;
}
BIGNUM BIGNUM::operator * (BIGNUM const &NUM1){
    BIGNUM res;
    int digit, size0, size1;
    long long int tmp, thisSlotValue = 0, nextSlotValue = 0;

    res.sign = this->sign ^ NUM1.sign;

    size0 = (getDigNeed(*this) - 1) / DIGITNUM + 1;
    size1 = (getDigNeed(NUM1) - 1) / DIGITNUM + 1;

    digit = size0 + size1;

    res.value.resize(digit);
    res.point = this->point + NUM1.point;

    for(int i=0; i<digit; i++){
        for(int j=0; j<std::min(i + 1, size1); j++){
            if(i-j >= size0) continue;
            tmp = (long long int)(this->value[i-j]) * (long long int)(NUM1.value[j]);

            thisSlotValue += tmp;
        }
        res.value[i] = (thisSlotValue + nextSlotValue) % DIGIT10;
        nextSlotValue = (thisSlotValue + nextSlotValue) / DIGIT10;
        thisSlotValue = 0;
    }

    return res;
}
BIGNUM BIGNUM::operator / (BIGNUM NUM1){
    BIGNUM res;
    BIGNUM NUM0, res;
    DIGITDATATYPE digitNUM0, digitNUM1, digitExact, nowDigit;
    int rangeL, f0, f1, tmp;

    if(isZero(NUM1)) {
        res.isInf = 1;
        return res;
    }

    digitNUM0 = getDignum(*this);
    digitNUM1 = getDignum(NUM1);
    nowDigit = digitNUM0 - digitNUM1 + 1;
    digitExact = digitNUM1 + NUM1.point;

    NUM0.value.resize((digitNUM0 + digitExact - 1) / DIGITNUM + 1);
    for(int i=0; i<this->value.size(); i++){
        NUM0.value[i] = this->value[i];
    }
    NUM0.sign = this->sign;
    NUM0.isInf = this->isInf;
    NUM0.point = this->point;

    res.sign = this->sign ^ NUM1.sign;
    res.point = digitExact;

    res.value.resize(std::max((nowDigit + digitExact - 1)/DIGITNUM + 1, 0));
    for(int i=0; i<res.value.size(); i++) res.value[i] = 0;

    f1 = firstDigit2(NUM1);
    if(digitNUM0 > digitNUM1) NUM1 <<= (digitNUM0 - digitNUM1);
    else NUM1 >>= (digitNUM1 - digitNUM0);
    digitNUM1 = digitNUM0;

    while(-nowDigit < digitExact && !isZero(NUM0)){
        if(NUM0 < NUM1) {
            NUM1 >>= 1;
            digitNUM1--;
            nowDigit--;
            continue;
        }
        f0 = firstDigit2(NUM0);
        rangeL = f0>=f1 ? f0/f1 : 10*f0/f1;
        
        tmp = (nowDigit + digitExact - 1) % DIGITNUM;
        res.value[(nowDigit + digitExact - 1)/DIGITNUM] += (rangeL-1) * quick_pow10(tmp < 0 ? tmp + DIGITNUM : tmp);
        simpleMinus(NUM0, NUM1, rangeL-1);
        if(NUM0 >= NUM1){
            simpleMinus(NUM0, NUM1, 1);
            res.value[(nowDigit + digitExact - 1)/DIGITNUM] += quick_pow10(tmp < 0 ? tmp + DIGITNUM : tmp);
        }
        NUM1 >>= 1;
        digitNUM0 = getDignum(NUM0);
        digitNUM1--;
        nowDigit--;
    }
    
    return res;
}
BIGNUM BIGNUM::operator % (BIGNUM const &NUM1){
    BIGNUM res;
    BIGNUM res, divNUM = abs(NUM1);
    DIGITDATATYPE digitNUM0, digitNUM1;
    int rangeL, f0, f1;

    if(isZero(NUM1)) {
        res.isInf = 1;
        return res;
    }

    digitNUM0 = getDignum(*this);
    digitNUM1 = getDignum(NUM1);

    res.value.resize(std::max(this->value.size(), NUM1.value.size()));
    for(int i=0; i<this->value.size(); i++){
        res.value[i] = this->value[i];
    }
    res.sign = 0;
    res.isInf = this->isInf;
    res.point = this->point;
    if(res < NUM1) return *this;

    f1 = firstDigit2(NUM1);
    divNUM = divNUM << (digitNUM0 - digitNUM1);
    digitNUM1 = digitNUM0;

    while(compareAbs(NUM1, res)){
        if(res < divNUM) {
            divNUM >>= 1;
            digitNUM1--;
            continue;
        }
        f0 = firstDigit2(res);
        rangeL = f0>=f1 ? f0/f1 : 10*f0/f1;
        
        simpleMinus(res, divNUM, rangeL-1);
        if(res >= divNUM){
            simpleMinus(res, divNUM, 1);
        }
        divNUM >>= 1;
        digitNUM0 = getDignum(res);
        digitNUM1--;
    }

    return res;
}
BIGNUM BIGNUM::operator = (BIGNUM const &NUM1){
    this->value.resize(NUM1.value.size());
    for(int i=0; i<NUM1.value.size(); i++){
        this->value[i] = NUM1.value[i];
    }
    this->sign = NUM1.sign;
    this->isInf = NUM1.isInf;
    this->point = NUM1.point;
    this->retract();
    return *this;
}
bool BIGNUM::operator > (BIGNUM const &NUM1){
    if(NUM1.isInf) return 0;

    int dig0 = getDigNeed(*this), dig1 = getDigNeed(NUM1), tmp;
    if(dig0 - this->point != dig1 - NUM1.point) return dig0 - this->point > dig1 - NUM1.point;
    if(this->sign != NUM1.sign) return this->sign < NUM1.sign;

    bool rev = 0;
    int pointDiff = (this->point - NUM1.point) % DIGITNUM, pointOver = abs(this->point - NUM1.point) / DIGITNUM;
    int modNUM0, modNUM1, tmpNUM0, tmpNUM1;
    int idx0, idx1;
    BIGNUM const *_NUM0, *_NUM1;
    if(pointDiff < 0){
        _NUM0 = this;
        _NUM1 = &NUM1;
    }
    else{
        _NUM0 = &NUM1;
        _NUM1 = this;
        tmp = dig0;
        dig0 = dig1;
        dig1 = tmp;
        rev = 1;
    }

    idx0 = (dig0-1)/DIGITNUM + 1;
    idx1 = (dig1-1)/DIGITNUM + 1;
    modNUM1 = quick_pow10(abs(pointDiff));
    modNUM0 = (DIGIT10/modNUM1);

    for(int i=std::max(idx0, idx1) - 1; i>=0; i--){
        tmpNUM0 = i < idx0 ? _NUM0->value[i] % modNUM0 : 0;
        tmpNUM1 = (0 <= i - pointOver && i - pointOver < idx1) ? _NUM1->value[i - pointOver] / modNUM1 : 0;
        if(tmpNUM0 != tmpNUM1) return (tmpNUM0 > tmpNUM1) ^ rev;
        
        tmpNUM0 = (0 <= i - 1 && i - 1 < idx0) ? _NUM0->value[i - 1] / modNUM0 : 0;
        tmpNUM1 = (0 <= i - pointOver && i - pointOver < idx1) ? _NUM1->value[i - pointOver] % modNUM1 : 0;
        if(tmpNUM0 != tmpNUM1) return(tmpNUM0 > tmpNUM1) ^ rev;
    }
    return 0;
}
bool BIGNUM::operator < (BIGNUM const &NUM1){
    if(this->isInf) return 0;

    int dig0 = getDigNeed(*this), dig1 = getDigNeed(NUM1), tmp;
    if(dig0 - this->point != dig1 - NUM1.point) return dig0 - this->point < dig1 - NUM1.point;
    if(this->sign != NUM1.sign) return this->sign > NUM1.sign;

    bool rev = 0;
    int pointDiff = (this->point - NUM1.point) % DIGITNUM, pointOver = abs(this->point - NUM1.point) / DIGITNUM;
    int modNUM0, modNUM1, tmpNUM0, tmpNUM1;
    int idx0, idx1;
    BIGNUM const *_NUM0, *_NUM1;
    if(pointDiff < 0){
        _NUM0 = this;
        _NUM1 = &NUM1;
    }
    else{
        _NUM0 = &NUM1;
        _NUM1 = this;
        tmp = dig0;
        dig0 = dig1;
        dig1 = tmp;
        rev = 1;
    }

    idx0 = (dig0-1)/DIGITNUM + 1;
    idx1 = (dig1-1)/DIGITNUM + 1;
    modNUM1 = quick_pow10(abs(pointDiff));
    modNUM0 = (DIGIT10/modNUM1);
    
    for(int i=std::max(idx0, idx1) - 1; i>=0; i--){
        tmpNUM0 = i < idx0 ? _NUM0->value[i] % modNUM0 : 0;
        tmpNUM1 = (0 <= i - pointOver && i - pointOver < idx1) ? _NUM1->value[i - pointOver] / modNUM1 : 0;
        if(tmpNUM0 != tmpNUM1) return(tmpNUM0 < tmpNUM1) ^ rev;
        
        tmpNUM0 = (0 <= i - 1 && i - 1 < idx0) ? _NUM0->value[i - 1] / modNUM0 : 0;
        tmpNUM1 = (0 <= i - pointOver && i - pointOver < idx1) ? _NUM1->value[i - pointOver] % modNUM1 : 0;
        if(tmpNUM0 != tmpNUM1) return(tmpNUM0 < tmpNUM1) ^ rev;
    }
    return 0;
}
bool BIGNUM::operator == (BIGNUM const &NUM1){
    if(this->sign != NUM1.sign || getDignum(*this) != getDignum(NUM1)) return 0;

    int dig0 = getDigNeed(*this), dig1 = getDigNeed(NUM1), tmp;
    int pointDiff = (this->point - NUM1.point) % DIGITNUM, pointOver = abs(this->point - NUM1.point) / DIGITNUM;
    int modNUM0, modNUM1, tmpNUM0, tmpNUM1;
    int idx0, idx1;
    BIGNUM const *_NUM0, *_NUM1;
    if(pointDiff < 0){
        _NUM0 = this;
        _NUM1 = &NUM1;
    }
    else{
        _NUM0 = &NUM1;
        _NUM1 = this;
        tmp = dig0;
        dig0 = dig1;
        dig1 = tmp;
    }

    idx0 = (dig0-1)/DIGITNUM + 1;
    idx1 = (dig1-1)/DIGITNUM + 1;
    modNUM1 = quick_pow10(abs(pointDiff));
    modNUM0 = (DIGIT10/modNUM1);

    for(int i=std::max(idx0, idx1) - 1; i>=0; i--){
        tmpNUM0 = i < idx0 ? _NUM0->value[i] % modNUM0 : 0;
        tmpNUM1 = (0 <= i - pointOver && i - pointOver < idx1) ? _NUM1->value[i - pointOver] / modNUM1 : 0;
        if(tmpNUM0 != tmpNUM1) return 0;
        
        tmpNUM0 = (0 <= i - 1 && i - 1 < idx0) ? _NUM0->value[i - 1] / modNUM0 : 0;
        tmpNUM1 = (0 <= i - pointOver && i - pointOver < idx1) ? _NUM1->value[i - pointOver] % modNUM1 : 0;
        if(tmpNUM0 != tmpNUM1) return 0;
    }
    return 1;
}
bool BIGNUM::operator != (BIGNUM const &NUM1){
    return !(*this==NUM1);
}
bool BIGNUM::operator >= (BIGNUM const &NUM1){
    return !(*this < NUM1);
}
bool BIGNUM::operator <= (BIGNUM const &NUM1){
    return !(*this > NUM1);
}

BIGNUM BIGNUM::operator += (BIGNUM const &NUM1){
    *this = *this + NUM1;
    return *this;
}
BIGNUM BIGNUM::operator -= (BIGNUM const &NUM1){
    *this = *this - NUM1;
    return *this;
}
BIGNUM BIGNUM::operator *= (BIGNUM const &NUM1){
    *this = *this * NUM1;
    return *this;
}
BIGNUM BIGNUM::operator /= (BIGNUM const &NUM1){
    *this = *this / NUM1;
    return *this;
}
BIGNUM BIGNUM::operator %= (BIGNUM const &NUM1){
    *this = *this % NUM1;
    return *this;
}


// ===========================================  int  ===========================================
BIGNUM BIGNUM::operator + (int NUM1){
    return *this + BIGNUM(NUM1);
}
BIGNUM BIGNUM::operator - (int NUM1){
    return *this - BIGNUM(NUM1);
}
BIGNUM BIGNUM::operator * (int NUM1){
    return *this * BIGNUM(NUM1);
}
BIGNUM BIGNUM::operator / (int NUM1){
    return *this / BIGNUM(NUM1);
}
BIGNUM BIGNUM::operator % (int NUM1){
    return *this % BIGNUM(NUM1);
}
BIGNUM BIGNUM::operator = (int NUM1){
    return *this = BIGNUM(NUM1);
}
bool BIGNUM::operator > (int NUM1){
    return *this > BIGNUM(NUM1);
}
bool BIGNUM::operator < (int NUM1){
    return *this < BIGNUM(NUM1);
}
bool BIGNUM::operator == (int NUM1){
    return *this == BIGNUM(NUM1);
}
bool BIGNUM::operator != (int NUM1){
    return !(*this == BIGNUM(NUM1));
}
bool BIGNUM::operator >= (int NUM1){
    return *this >= BIGNUM(NUM1);
}
bool BIGNUM::operator <= (int NUM1){
    return *this <= BIGNUM(NUM1);
}
BIGNUM BIGNUM::operator += (int NUM1){
    BIGNUM res = *this + BIGNUM(NUM1);
    *this = res;
    return res;
}
BIGNUM BIGNUM::operator -= (int NUM1){
    BIGNUM res = *this - BIGNUM(NUM1);
    *this = res;
    return res;
}
BIGNUM BIGNUM::operator *= (int NUM1){
    BIGNUM res = *this * BIGNUM(NUM1);
    *this = res;
    return res;
}
BIGNUM BIGNUM::operator /= (int NUM1){
    BIGNUM res = *this / BIGNUM(NUM1);
    *this = res;
    return res;
}
BIGNUM BIGNUM::operator %= (int NUM1){
    BIGNUM res = *this % BIGNUM(NUM1);
    *this = res;
    return res;
}



// ===========================================  double  ===========================================
BIGNUM BIGNUM::operator + (double NUM1){
    return *this + BIGNUM(NUM1);
}
BIGNUM BIGNUM::operator - (double NUM1){
    return *this - BIGNUM(NUM1);
}
BIGNUM BIGNUM::operator * (double NUM1){
    return *this * BIGNUM(NUM1);
}
BIGNUM BIGNUM::operator / (double NUM1){
    return *this / BIGNUM(NUM1);
}
BIGNUM BIGNUM::operator % (double NUM1){
    return *this % BIGNUM(NUM1);
}
BIGNUM BIGNUM::operator = (double NUM1){
    return *this = BIGNUM(NUM1);
}
bool BIGNUM::operator > (double NUM1){
    return *this > BIGNUM(NUM1);
}
bool BIGNUM::operator < (double NUM1){
    return *this < BIGNUM(NUM1);
}
bool BIGNUM::operator == (double NUM1){
    return *this == BIGNUM(NUM1);
}
bool BIGNUM::operator != (double NUM1){
    return !(*this == BIGNUM(NUM1));
}
bool BIGNUM::operator >= (double NUM1){
    return *this >= BIGNUM(NUM1);
}
bool BIGNUM::operator <= (double NUM1){
    return *this <= BIGNUM(NUM1);
}
BIGNUM BIGNUM::operator += (double NUM1){
    BIGNUM res = *this + BIGNUM(NUM1);
    *this = res;
    return res;
}
BIGNUM BIGNUM::operator -= (double NUM1){
    BIGNUM res = *this - BIGNUM(NUM1);
    *this = res;
    return res;
}
BIGNUM BIGNUM::operator *= (double NUM1){
    BIGNUM res = *this * BIGNUM(NUM1);
    *this = res;
    return res;
}
BIGNUM BIGNUM::operator /= (double NUM1){
    BIGNUM res = *this / BIGNUM(NUM1);
    *this = res;
    return res;
}
BIGNUM BIGNUM::operator %= (double NUM1){
    BIGNUM res = *this % BIGNUM(NUM1);
    *this = res;
    return res;
}

/*
    ###########################################################################################################
    #                                                                                                         #
    #                                                                                                         #
    #                                                  COMPLEX                                                #
    #                                                                                                         #
    #                                                                                                         #
    ###########################################################################################################
*/

COMPLEX::COMPLEX(){
    this->Re = 0;
    this->Im = 0;
};
COMPLEX::COMPLEX(int NUM0, int NUM1){
    this->Re = NUM0;
    this->Im = NUM1;
};
COMPLEX::COMPLEX(float NUM0, float NUM1){
    this->Re = NUM0;
    this->Im = NUM1;
};
COMPLEX::COMPLEX(double NUM0, double NUM1){
    this->Re = NUM0;
    this->Im = NUM1;
};

COMPLEX_DATA_TYPE abs(COMPLEX NUM0){
    return sqrt(NUM0.Re * NUM0.Re + NUM0.Im * NUM0.Im);
};
COMPLEX bar(COMPLEX NUM0){
    NUM0.Im = -NUM0.Im;
    return NUM0;
};
COMPLEX COMPLEX::bar(){
    this->Im = -this->Im;
    return *this;
};

std::ostream& operator<<(std::ostream &os, const COMPLEX &m){
    os << m.Re << " + " << m.Im << "i" << std::flush;
    return os;
};


COMPLEX COMPLEX::operator + (COMPLEX const &NUM1){
    COMPLEX res;
    res.Re = this->Re + NUM1.Re;
    res.Im = this->Im + NUM1.Im;
    return res;
};
COMPLEX COMPLEX::operator - (COMPLEX const &NUM1){
    COMPLEX res;
    res.Re = this->Re - NUM1.Re;
    res.Im = this->Im - NUM1.Im;
    return res;
};
COMPLEX COMPLEX::operator * (COMPLEX const &NUM1){
    COMPLEX res;
    res.Re = this->Re * NUM1.Re - this->Im * NUM1.Im;
    res.Im = this->Re * NUM1.Im + this->Im * NUM1.Re;
    return res;
};
COMPLEX COMPLEX::operator / (COMPLEX const &NUM1){
    COMPLEX res;
    res.Re = (this->Re * NUM1.Re + this->Im * NUM1.Im) / (NUM1.Re * NUM1.Re + NUM1.Im * NUM1.Im);
    res.Im = (-this->Re * NUM1.Im + this->Im * NUM1.Re) / (NUM1.Re * NUM1.Re + NUM1.Im * NUM1.Im);
    return res;
};
COMPLEX COMPLEX::operator = (COMPLEX const &NUM1){
    this->Re = NUM1.Re;
    this->Im = NUM1.Im;
    return *this;
};
bool COMPLEX::operator == (COMPLEX const &NUM1){
    return (this->Re == NUM1.Re) && (this->Im == NUM1.Im);
};
bool COMPLEX::operator != (COMPLEX const &NUM1){
    return (this->Re != NUM1.Re) || (this->Im != NUM1.Im);
};
COMPLEX COMPLEX::operator += (COMPLEX const &NUM1){
    this->Re = this->Re + NUM1.Re;
    this->Im = this->Im + NUM1.Im;
    return *this;
};
COMPLEX COMPLEX::operator -= (COMPLEX const &NUM1){
    this->Re = this->Re - NUM1.Re;
    this->Im = this->Im - NUM1.Im;
    return *this;
};
COMPLEX COMPLEX::operator *= (COMPLEX const &NUM1){
    this->Re = this->Re * NUM1.Re - this->Im * NUM1.Im;
    this->Im = this->Re * NUM1.Im + this->Im * NUM1.Re;
    return *this;
};
COMPLEX COMPLEX::operator /= (COMPLEX const &NUM1){
    this->Re = (this->Re * NUM1.Re + this->Im * NUM1.Im) / (NUM1.Re * NUM1.Re + NUM1.Im * NUM1.Im);
    this->Im = (-this->Re * NUM1.Im + this->Im * NUM1.Re) / (NUM1.Re * NUM1.Re + NUM1.Im * NUM1.Im);
    return *this;
};


// ===========================================  double  ===========================================
COMPLEX COMPLEX::operator + (double const &NUM1){
    COMPLEX res;
    res.Re = this->Re + NUM1;
    return res;
};
COMPLEX COMPLEX::operator - (double const &NUM1){
    COMPLEX res;
    res.Re = this->Re - NUM1;
    return res;
};
COMPLEX COMPLEX::operator * (double const &NUM1){
    COMPLEX res;
    res.Re = this->Re * NUM1;
    res.Im = this->Im * NUM1;
    return res;
};
COMPLEX COMPLEX::operator / (double const &NUM1){
    COMPLEX res;
    res.Re = this->Re / NUM1;
    res.Im = this->Im / NUM1;
    return res;
};
COMPLEX COMPLEX::operator = (double const &NUM1){
    this->Re = NUM1;
    this->Im = 0;
    return *this;
};
bool COMPLEX::operator == (double const &NUM1){
    return (this->Re == NUM1) && (this->Im == 0);
};
bool COMPLEX::operator != (double const &NUM1){
    return (this->Re != NUM1) || (this->Im != 0);
};
COMPLEX COMPLEX::operator += (double const &NUM1){
    this->Re = this->Re + NUM1;
    return *this;
};
COMPLEX COMPLEX::operator -= (double const &NUM1){
    this->Re = this->Re - NUM1;
    return *this;
};
COMPLEX COMPLEX::operator *= (double const &NUM1){
    this->Re = this->Re * NUM1;
    this->Im = this->Im * NUM1;
    return *this;
};
COMPLEX COMPLEX::operator /= (double const &NUM1){
    this->Re = this->Re / NUM1;
    this->Im = this->Im / NUM1;
    return *this;
};

/*
    ###########################################################################################################
    #                                                                                                         #
    #                                                                                                         #
    #                                                   MATRIX                                                #
    #                                                                                                         #
    #                                                                                                         #
    ###########################################################################################################
*/

MATRIX::MATRIX(){};
MATRIX::MATRIX(int colNum, int rowNum){
    this->col = colNum;
    this->row = rowNum;
    this->value.resize(colNum * rowNum);
    for(int i=0; i<this->col * this->row; i++) this->value[i] = 0;
};
MATRIX::MATRIX(int colNum, int rowNum, std::initializer_list<double> NUM1){
    this->col = colNum;
    this->row = rowNum;
    this->value.resize(colNum * rowNum);
    this->value = NUM1;
};

std::ostream& operator<<(std::ostream &os, const MATRIX &m){
    os << "\n";
    for(int i=0; i<m.row; i++){
        for(int j=0; j<m.col; j++) os << "\t" << m.value[i*m.col + j] << std::flush;
        os << "\n";
    }
    
    return os;
};


MATRIX MATRIX::operator + (MATRIX const &MTX1){
    if(this->col != MTX1.col || this->row != MTX1.row) {
        // error here
        return *this;
    }
    MATRIX res = *this;
    for(int i=0; i<this->col * this->row; i++) res.value[i] += MTX1.value[i];
    return res;
};
MATRIX MATRIX::operator - (MATRIX const &MTX1){
    if(this->col != MTX1.col || this->row != MTX1.row) {
        // error here
        return *this;
    }
    MATRIX res = *this;
    for(int i=0; i<this->col * this->row; i++) res.value[i] -= MTX1.value[i];
    return res;
};
MATRIX MATRIX::operator * (MATRIX const &MTX1){
    if(this->row != MTX1.col) {
        // error here
        return *this;
    }
    MATRIX res(this->row, MTX1.col);
    for(int i=0; i<res.row; i++){
        for(int j=0; j<res.col; j++){
            for(int k=0; k<this->row; k++) res.value[i * res.col + j] += this->value[i * this->col + k] * MTX1.value[k * MTX1.col + j];
        }
    }
    return res;
};
MATRIX MATRIX::operator = (MATRIX const &MTX1){
    if(this->value.size() != MTX1.value.size()) this->value.resize(MTX1.value.size());
    this->col = MTX1.col;
    this->row = MTX1.row;
    for(int i=0; i<this->col * this->row; i++) this->value[i] = MTX1.value[i];
    return *this;
};
bool MATRIX::operator == (MATRIX const &MTX1){
    if(this->col != MTX1.col || this->row != MTX1.row) return 0;
    for(int i=0; i<this->col * this->row; i++) if(this->value[i] != MTX1.value[i]) return 0;
    return 1;
};
bool MATRIX::operator != (MATRIX const &MTX1){
    if(this->col != MTX1.col || this->row != MTX1.row) return 1;
    for(int i=0; i<this->col * this->row; i++) if(this->value[i] != MTX1.value[i]) return 1;
    return 0;
};
MATRIX MATRIX::operator += (MATRIX const &MTX1){
    if(this->col != MTX1.col || this->row != MTX1.row) return *this;
    for(int i=0; i<this->col * this->row; i++) this->value[i] += MTX1.value[i];
    return *this;
};
MATRIX MATRIX::operator -= (MATRIX const &MTX1){
    if(this->col != MTX1.col || this->row != MTX1.row) return *this;
    for(int i=0; i<this->col * this->row; i++) this->value[i] -= MTX1.value[i];
    return *this;
};
MATRIX MATRIX::operator *= (MATRIX const &MTX1){
    *this = *this * MTX1;
    return *this;
};

MATRIX MATRIX::operator = (double* const NUM1){
    int s = this->col * this->row;
    for(int i=0; i<s; i++) this->value[i] = NUM1[i];
    return *this;
};
MATRIX MATRIX::operator = (std::initializer_list<double> NUM1){
    this->value = NUM1;
    return *this;
};
// MATRIX MATRIX::operator = (std::initializer_list<std::std::initializer_list<std::initializer_list<double>> NUM1);<double>> NUM1){
//     this->value = NUM1;
//     return *this;
// };

MATRIX MATRIX::operator + (double const &NUM1){
    MATRIX res(this->row, this->col);
    int s = this->col * this->row;
    if(this->row != this->col) for(int i=0; i<s; i++) res.value[i] = this->value[i] + NUM1;
    else for(int i=0; i<s; i++) res.value[i] = this->value[i] + (i%(this->row+1) ? 0 : NUM1);
    return res;
};
MATRIX MATRIX::operator - (double const &NUM1){
    MATRIX res(this->row, this->col);
    int s = this->col * this->row;
    if(this->row != this->col) for(int i=0; i<s; i++) res.value[i] = this->value[i] - NUM1;
    else for(int i=0; i<s; i+=this->row+1) res.value[i] = this->value[i] - (i%(this->row+1) ? 0 : NUM1);
    return res;
};
MATRIX MATRIX::operator * (double const &NUM1){
    MATRIX res(this->row, this->col);
    int s = this->col * this->row;
    for(int i=0; i<s; i++) res.value[i] = this->value[i] * NUM1;
    return res;
};
MATRIX MATRIX::operator / (double const &NUM1){
    MATRIX res(this->row, this->col);
    int s = this->col * this->row;
    for(int i=0; i<s; i++) res.value[i] = this->value[i] / NUM1;
    return res;
};
MATRIX MATRIX::operator += (double const &NUM1){
    int s = this->col * this->row;
    if(this->row != this->col) for(int i=0; i<s; i++) this->value[i] += NUM1;
    else for(int i=0; i<s; i+=this->row+1) this->value[i] += NUM1;
    return *this;
};
MATRIX MATRIX::operator -= (double const &NUM1){
    int s = this->col * this->row;
    if(this->row != this->col) for(int i=0; i<s; i++) this->value[i] -= NUM1;
    else for(int i=0; i<s; i+=this->row+1) this->value[i] -= NUM1;
    return *this;
};
MATRIX MATRIX::operator *= (double const &NUM1){
    int s = this->col * this->row;
    for(int i=0; i<s; i++) this->value[i] *= NUM1;
    return *this;
};
MATRIX MATRIX::operator /= (double const &NUM1){
    int s = this->col * this->row;
    for(int i=0; i<s; i++) this->value[i] /= NUM1;
    return *this;
};







MATRIX transposed(MATRIX MTX1){
    MATRIX res(MTX1.row, MTX1.col);
    for(int i=0; i<MTX1.row; i++){ // 2
        for(int j=0; j<MTX1.col; j++){ // 3
            res.value[j*MTX1.row+i] = MTX1.value[i*MTX1.col+j];
        }
    }
    return res;
};
MATRIX unitMTX(int order){
    int s = order * order;
    MATRIX res(order, order);
    for(int i=0; i<s; i+=order+1){
        res.value[i] = 1;
    }
    return res;
};
MATRIX rotateMTXd(double deg){
    MATRIX res(2, 2);
    double theta = deg * M_PI / 180.0;
    res.value[0] = cos(theta);
    res.value[1] = -sin(theta);
    res.value[2] = sin(theta);
    res.value[3] = cos(theta);
    return res;
};
MATRIX rotateMTXr(double rad){
    MATRIX res(2, 2);
    res.value[0] = cos(rad);
    res.value[1] = -sin(rad);
    res.value[2] = sin(rad);
    res.value[3] = cos(rad);
    return res;
};