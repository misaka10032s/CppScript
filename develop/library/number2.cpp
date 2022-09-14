#include <string>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <cmath>
#include <algorithm>

#include "number2.h"

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
    if(NUM0.isInf) return 0;
    for(int i=NUM0.value.size()-1; i>=0; i--) if(NUM0.value[i]) return 0;
    return 1;
}

inline int getDig(int const &NUM0){
    int dig0 = 0;
    for(int i=1; i<DIGIT10; i*=10){
        if(NUM0>=i) dig0++;
    }
    return dig0;
}

inline int getDig(BIGNUM const &NUM0){
    int dig0 = 0;
    for(int i=NUM0.value.size()-1; i>=0; i--){
        if(NUM0.value[i] != 0){
            dig0 = DIGITNUM * i;
            for(int j=1; j<DIGIT10; j*=10){
                if(NUM0.value[i]>=j) dig0++;
            }
            break;
        }
    }
    return dig0;
}

inline int BIGNUM::degree(BIGNUM const &NUM0){
    int dig0 = 0;
    for(int i=NUM0.value.size()-1; i>=0; i--){
        if(NUM0.value[i] != 0){
            dig0 = DIGITNUM * i - NUM0.point;
            for(int j=1; j<DIGIT10; j*=10){
                if(NUM0.value[i]>=j) dig0++;
            }
            break;
        }
    }
    
    return dig0;
};

int BIGNUM::firstDigit(BIGNUM const &NUM0){
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

int BIGNUM::firstDigit2(BIGNUM const &NUM0){
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

bool compareAbs(BIGNUM const &NUM0, BIGNUM const &NUM1){
    if(NUM0.isInf) return 0;

    int dig0 = getDig(NUM0), dig1 = getDig(NUM1), tmp;
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
    modNUM1 = pow(10, abs(pointDiff));
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

// ================================================================================================

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
    digitN = getDig(m);
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
    if(this->isInf) return *this;
    if(NUM1 <= 0) return *this;
    BIGNUM res;
    DIGITDATATYPE digitSet, digitShift, digitUnit, idx;
    int multiple, origSize = (getDig(*this) - 1) / DIGITNUM + 1;

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
    idx = (getDig(*this) - 1) / DIGITNUM + 1;
    multiple = pow(10, (DIGITNUM - digitUnit) % DIGITNUM);

    res.value.resize(idx + digitShift);
    for(int i=0; i<idx; i++) res.value[i] = 0;

    for(int i=(int)res.value.size()-1; i>=0; i--){
        res.value[i] = (0 <= i-digitShift && i-digitShift < origSize) ? this->value[i-digitShift] / multiple : 0;
        res.value[i] += (0 <= i-digitShift+1 && i-digitShift+1 < origSize) ? (this->value[i-digitShift+1] % multiple) * (DIGIT10 / multiple) : 0;
    }

    return res;
};
BIGNUM BIGNUM::operator >> (DIGITDATATYPE NUM1){
    if(this->isInf) return *this;
    if(NUM1 <= 0) return *this;
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
    idx = (getDig(*this) - NUM1 + zero - 1) / DIGITNUM + 1;
    multiple = pow(10, DIGITNUM - digitUnit);

    res.value.resize(idx);
    for(int i=0; i<idx; i++) res.value[i] = 0;

    for(int i=0; i<origSize; i++){
        res.value[i] = i+digitShift-1 < (int)this->value.size() ? this->value[i+digitShift-1] / (DIGIT10 / multiple) : 0;
        res.value[i] += i+digitShift < (int)this->value.size() ? (this->value[i+digitShift] % (DIGIT10 / multiple)) * multiple : 0;
    }

    return res;
};

BIGNUM BIGNUM::operator <<= (DIGITDATATYPE NUM1){
    if(this->isInf) return *this;
    if(NUM1 <= 0) return *this;
    DIGITDATATYPE digitSet, digitShift, digitUnit, idx;
    int multiple, origSize = (getDig(*this) - 1) / DIGITNUM + 1, tmp;

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
    idx = (getDig(*this) - 1) / DIGITNUM + 1;
    multiple = pow(10, (DIGITNUM - digitUnit) % DIGITNUM);

    if(origSize < idx + digitShift) this->value.resize(idx + digitShift);
    for(int i=origSize; i<idx + digitShift; i++) this->value[i] = 0;

    for(int i=(int)this->value.size()-1; i>=0; i--){
        tmp = (0 <= i-digitShift && i-digitShift < origSize) ? this->value[i-digitShift] / multiple : 0;
        tmp += (0 <= i-digitShift+1 && i-digitShift+1 < origSize) ? (this->value[i-digitShift+1] % multiple) * (DIGIT10 / multiple) : 0;
        this->value[i] = tmp;
    }
    return *this;
};
BIGNUM BIGNUM::operator >>= (DIGITDATATYPE NUM1){
    if(this->isInf) return *this;
    if(NUM1 <= 0) return *this;
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
    idx = (getDig(*this) - NUM1 + zero - 1) / DIGITNUM + 1;
    multiple = pow(10, DIGITNUM - digitUnit);

    for(int i=0; i<origSize; i++){
        this->value[i] = i+digitShift-1 < (int)this->value.size() ? this->value[i+digitShift-1] / (DIGIT10 / multiple) : 0;
        this->value[i] += i+digitShift < (int)this->value.size() ? (this->value[i+digitShift] % (DIGIT10 / multiple)) * multiple : 0;
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
    
    int digit, tmpNUM0, tmpNUM1;
    int size0, size1;
    int pointDiff = (this->point - NUM1.point) % DIGITNUM;
    int modNUM0, modNUM1, pointOver0, pointOver1;

    modNUM0 = pow(10, pointDiff < 0 ? DIGITNUM + pointDiff : DIGITNUM);
    modNUM1 = pow(10, pointDiff > 0 ? DIGITNUM - pointDiff : DIGITNUM);

    if(this->point - NUM1.point < 0){
        pointOver0 = (NUM1.point - this->point) / DIGITNUM;
        pointOver1 = 0;
    }
    else{
        pointOver0 = 0;
        pointOver1 = (this->point - NUM1.point) / DIGITNUM;
    }

    size0 = (getDig(*this) - 1) / DIGITNUM + 1;
    size1 = (getDig(NUM1) - 1) / DIGITNUM + 1;
    digit = std::max(size0, size1);

// std::cout << "\nthis ";
// for(int i=size0-1; i>=0; i--) std::cout << this->value[i] << ", "; std::cout << "\n";
// for(int i=size1-1; i>=0; i--) std::cout << NUM1.value[i] << ", "; std::cout << "\n";
    res.sign = this->sign;
    res.point = std::max(this->point, NUM1.point);
    res.value.resize(digit + 1);
// std::cout << "pointDiff " << pointDiff << ", " << pointOver0 << ", " << pointOver1<< ", " << modNUM0 << ", " << modNUM1 << ", " << digit << ", " << res.point << "\n";

    res.value[0] = 0;
    for(int i=0; i<digit; i++){
        tmpNUM0 = i + pointOver0 < this->value.size() ? this->value[i + pointOver0] : 0;
        tmpNUM1 = i + pointOver1 < NUM1.value.size() ? NUM1.value[i + pointOver1] : 0;

        res.value[i] += (tmpNUM0 % modNUM0) * (DIGIT10 / modNUM0) + (tmpNUM1 % modNUM1) * (DIGIT10 / modNUM1);
        res.value[i + 1] = tmpNUM0 / modNUM0 + tmpNUM1 / modNUM1 + (res.value[i] > DIGIT10);
        res.value[i] %= DIGIT10;
    }
// for(int i=digit; i>=0; i--) std::cout << res.value[i] << ", "; std::cout << "\n";

    return res;
}
BIGNUM BIGNUM::operator - (BIGNUM const &NUM1){
    BIGNUM res;
    if(this->sign != NUM1.sign) {
        res = NUM1;
        res.sign ^= 1;
        return *this + res;
    }
    
    int digit, tmpNUM0, tmpNUM1;
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

    modNUM0 = pow(10, pointDiff < 0 ? DIGITNUM + pointDiff : DIGITNUM);
    modNUM1 = pow(10, pointDiff > 0 ? DIGITNUM - pointDiff : DIGITNUM);

    if(_NUM0->point - _NUM1->point < 0){
        pointOver0 = (_NUM1->point - _NUM0->point) / DIGITNUM;
        pointOver1 = 0;
    }
    else{
        pointOver0 = 0;
        pointOver1 = (_NUM0->point - _NUM1->point) / DIGITNUM;
    }

    size0 = (getDig(*_NUM0) - 1) / DIGITNUM + 1;
    size1 = (getDig(*_NUM1) - 1) / DIGITNUM + 1;
    digit = std::max(size0, size1);

    res.point = std::max(this->point, NUM1.point);
    res.value.resize(digit + 1);

    res.value[0] = 0;
    for(int i=0; i<digit; i++){
        tmpNUM0 = i + pointOver0 < _NUM0->value.size() ? _NUM0->value[i + pointOver0] : 0;
        tmpNUM1 = i + pointOver1 < _NUM1->value.size() ? _NUM1->value[i + pointOver1] : 0;

        res.value[i] += (tmpNUM0 % modNUM0) * (DIGIT10 / modNUM0) - (tmpNUM1 % modNUM1) * (DIGIT10 / modNUM1);
        res.value[i + 1] = tmpNUM0 / modNUM0 - tmpNUM1 / modNUM1 - (res.value[i] < 0);
        if(res.value[i] < 0) res.value[i] += DIGIT10;
    }

    return res;
}
BIGNUM BIGNUM::operator * (BIGNUM const &NUM1){
    BIGNUM res;
    int digit;
    long long int tmp;

    res.sign = this->sign ^ NUM1.sign;

    digit = std::max(getDig(*this) + getDig(NUM1), 1);

    res.value.resize((digit - 1) / DIGITNUM + 1);
    res.point = this->point + NUM1.point;

    for(int i=0; i<res.value.size(); i++) res.value[i] = 0;

    for(int i=0; i<(int)this->value.size(); i++){
        for(int j=0; j<(int)NUM1.value.size(); j++){
            tmp = (long long int)(this->value[i]) * (long long int)(NUM1.value[j]);
            
            res.value[i + j] += tmp % DIGIT10;
            res.value[i + j + 1] += tmp / DIGIT10 + res.value[i + j] / DIGIT10;
            res.value[i + j ] %= DIGIT10;
        }
    }

    return res;
}
BIGNUM BIGNUM::operator / (BIGNUM NUM1){
    BIGNUM NUM0 = *this, res;
    int digitNUM0, digitNUM1, digitExact, nowDigit, rangeL, f0, f1, tmp;

    if(isZero(NUM1)) {
        res.isInf = 1;
        return res;
    }

    digitNUM0 = degree(NUM0);
    digitNUM1 = degree(NUM1);
    nowDigit = digitNUM0 - digitNUM1 + 1;
    digitExact = digitNUM0 + digitNUM1;

    res.sign = this->sign ^ NUM1.sign;
    res.point = digitExact;

    res.value.resize(std::max((nowDigit + digitExact - 1)/DIGITNUM + 1, 0));
    for(int i=0; i<res.value.size(); i++) res.value[i] = 0;

    f1 = firstDigit2(NUM1);
    if(digitNUM0 > digitNUM1) NUM1 <<= (digitNUM0 - digitNUM1);
    else NUM1 >>= (digitNUM1 - digitNUM0);
    digitNUM1 = digitNUM0;

    while(-nowDigit < digitExact && NUM0 != 0){
        if(NUM0 < NUM1) {
            NUM1 >>= 1;
            digitNUM1--;
            nowDigit--;
            continue;
        }
        f0 = firstDigit2(NUM0);
        // rangeL = f0>=f1 ? f0/(f1 + 1) : (10*f0 - 1)/(f1 + 1);
        rangeL = f0>=f1 ? f0/f1 : (10*f0 - 1)/(f1 + 1);
        for(int i=rangeL; i<=10; i++){
            // if(NUM1 * i > NUM0){
            if((f1 * i > f0 * pow(10, digitNUM0 - digitNUM1) + i) || (NUM1 * i > NUM0)){
// std::cout<<"/4 "<<f0<<", "<<f1<<", "<<digitNUM0<<", "<<digitNUM1<<", "<<NUM0<<", "<<NUM1<<"\n";
// std::cout<<"/5 "<<NUM1 * i<<", "<<(NUM1 * i > NUM0)<<"\n";
                tmp = (nowDigit + digitExact - 1) % DIGITNUM;
                res.value[(nowDigit + digitExact - 1)/DIGITNUM] += (i-1) * pow(10, tmp < 0 ? tmp + DIGITNUM : tmp);
                NUM0 -= NUM1 * (i-1);
                NUM1 >>= 1;
                digitNUM0 = degree(NUM0);
                digitNUM1--;
                break;
            }
        }
        nowDigit--;
    }

    return res;
}
BIGNUM BIGNUM::operator % (BIGNUM const &NUM1){
    BIGNUM res = abs(*this), divNUM = abs(NUM1);
    DIGITDATATYPE digitNUM0, digitNUM1;
    int rangeL, f0, f1;
    if(isZero(NUM1)) {
        res.isInf = 1;
        return res;
    }
    if(res < NUM1) return *this;

    digitNUM0 = degree(res);
    digitNUM1 = degree(NUM1);

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
        // rangeL = f0>=f1 ? f0/(f1 + 1) : (10*f0 - 1)/(f1 + 1);
        rangeL = f0>=f1 ? f0/f1 : (10*f0 - 1)/(f1 + 1);
        for(int i=rangeL; i<=10; i++){
            // if(divNUM * i > res){
            if((f1 * i > f0 * pow(10, digitNUM0 - digitNUM1) + i) || (divNUM * i > res)){
                res -= divNUM * (i-1);
                divNUM >>= 1;
                digitNUM0 = degree(res);
                digitNUM1--;
                break;
            }
        }
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

    int dig0 = getDig(*this), dig1 = getDig(NUM1), tmp;
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
    modNUM1 = pow(10, abs(pointDiff));
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

    int dig0 = getDig(*this), dig1 = getDig(NUM1), tmp;
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
    modNUM1 = pow(10, abs(pointDiff));
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
    if(this->sign != NUM1.sign || degree(*this) != degree(NUM1)) return 0;

    int dig0 = getDig(*this), dig1 = getDig(NUM1), tmp;
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
    modNUM1 = pow(10, abs(pointDiff));
    modNUM0 = (DIGIT10/modNUM1);

    for(int i=std::max(idx0, idx1) - 1; i>=0; i--){
        tmpNUM0 = i < (int)_NUM0->value.size() ? _NUM0->value[i] % modNUM0 : 0;
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