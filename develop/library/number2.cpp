#include <string>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <cmath>

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

int BIGNUM::degree(BIGNUM const &NUM0){
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
                if(i-1>0) x = 10*x + NUM0.value[i-1] / (DIGIT10 / 10);
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
    if(NUM0.sign > NUM1.sign) return 1;

    bool rev = 0;
    int pointDiff = (NUM0.point - NUM1.point) % DIGITNUM, pointOver = abs(NUM0.point - NUM1.point) / DIGITNUM;
    int modNUM0, modNUM1;
    int idx0, idx1;
    BIGNUM const *_NUM0, *_NUM1;
    if(pointDiff < 0){
        _NUM0 = &NUM0;
        _NUM1 = &NUM1;
        dig0 += NUM0.point;
        dig1 += NUM1.point;
    }
    else{
        _NUM0 = &NUM1;
        _NUM1 = &NUM0;
        tmp = dig0;
        dig0 = dig1 + NUM1.point;
        dig1 = tmp + NUM0.point;
        rev = 1;
    }

    idx0 = (dig0-1)/DIGITNUM + 1;
    idx1 = (dig1-1)/DIGITNUM + 1;
    modNUM1 = pow(10, abs(pointDiff));
    modNUM0 = (DIGIT10/modNUM1);
    
    for(int i=0;;i++){
        if(idx0 - i < 0 || idx1 - i - pointOver < 0) break;
        if(_NUM0->value[idx0 - i] % modNUM0 < _NUM1->value[idx1 - i - pointOver] / modNUM1) return 1 ^ rev;
        if(idx0 - i - 1 < 0) break;
        if(_NUM0->value[idx0 - i - 1] / modNUM0 < _NUM1->value[idx1 - i - pointOver] % modNUM1) return 1 ^ rev;
    }
    return (dig0 < dig1) ^ rev;
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
    this->value.resize(digit);
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

    this->value.resize(digitN + digitF);
    for(int i=digitF-1; i>=0; i--){
        this->value[idx++] = F[i];
    }
    for(int i=digitN-1; i>=0; i--){
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

    this->value.resize(digitN + digitF);
    for(int i=digitF-1; i>=0; i--){
        this->value[idx++] = F[i];
    }
    for(int i=digitN-1; i>=0; i--){
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
    int digitFix, digitN, idx, tmp;
    if(m.sign) std::cout << "-";
    if(m.isInf) {
        std::cout << "Inf" << std::flush;
        return os;
    }
    digitN = getDig(m);
    idx = (digitN - 1 ) / DIGITNUM + 1;
// std::cout << "\ndigitN " << digitN << ", " << idx << ", " << m.point << ", "; for(int i=idx-1; i>=0; i--) std::cout << m.value[i] << ", "; std::cout << "\n";
    if(idx){
        if(digitN <= m.point){
            std::cout << "0.";
            for(int i=digitN; i < m.point; i++) std::cout << "0";
        }
        for(int i=idx-1; i>=0; i--) {
            digitFix = DIGIT10/10;
            while(digitFix>1) {
                if(m.value[i] < digitFix && i != idx-1) {
                    std::cout << "0";
                }
                else break;
                digitFix /= 10;
            }
            if(i*DIGITNUM <= m.point && m.point < (i+1)*DIGITNUM && digitN > m.point) {
                std::cout << m.value[i] / (int)pow(10, m.point%DIGITNUM);
                if(i){
                    std::cout << ".";
                    std::cout << m.value[i] % (int)pow(10, m.point%DIGITNUM);
                }
            }
            else {
                if(i || m.point <= DIGITNUM) std::cout << m.value[i];
                else{
                    tmp = m.value[i];
                    while(!(tmp%10)) tmp/=10;
                    std::cout << tmp;
                }
            }
        }
    }
    else{
        std::cout << 0;
    }
    std::cout << std::flush;
    return os;
};

BIGNUM BIGNUM::operator << (DIGITDATATYPE NUM1){
    if(this->isInf) return *this;
    if(NUM1 < 0) return *this;
    BIGNUM res;
    DIGITDATATYPE digitSet, digitShift, digitUnit, idx;
    int multiple;

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
    idx = (getDig(*this) + NUM1 - 1) / DIGITNUM + 1;
    multiple = pow(10, (DIGITNUM - digitUnit) % DIGITNUM);

    res.value.resize(idx);
    for(int i=0; i<idx; i++) res.value[i] = 0;
// std::cout << "\nthis-> " << idx << ", " << NUM1 << ", " << this->point << ", "; for(int i=idx-1; i>=0; i--) std::cout << this->value[i] << ", "; std::cout << "\n";
    for(int i=0; i<this->value.size(); i++){
        if(0 <= i+digitShift-1) res.value[i+digitShift-1] = (this->value[i] % multiple) * (DIGIT10 / multiple);
        if(i+digitShift < idx) res.value[i+digitShift] += this->value[i] / multiple;
    }
// std::cout << "NUM1 " << res.point << ", "; for(int i=idx-1; i>=0; i--) std::cout << res.value[i] << ", "; std::cout << "\n";

    return res;
};
BIGNUM BIGNUM::operator >> (DIGITDATATYPE NUM1){
    if(this->isInf) return *this;
    if(NUM1 < 0) return *this;
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
    if(!zero) return res;

    digitSet = zero / DIGITNUM;
    digitUnit = zero % DIGITNUM;
    digitShift = digitSet + (digitUnit ? 1 : 0);
    idx = (getDig(*this) - NUM1 + zero - 1) / DIGITNUM + 1;
    multiple = pow(10, (DIGITNUM - digitUnit) % DIGITNUM);

    res.value.resize(idx);
    for(int i=0; i<idx; i++) res.value[i] = 0;

    for(int i=0; i<origSize; i++){
        if(0 <= i+digitShift-1) res.value[i] = this->value[i+digitShift-1] / multiple;
        if(i+digitShift < idx) res.value[i] +=  (this->value[i+digitShift] % multiple) * (DIGIT10 / multiple);
    }

    return res;
};

BIGNUM BIGNUM::operator <<= (DIGITDATATYPE NUM1){
    if(this->isInf) return *this;
    if(NUM1 < 0) return *this;
    DIGITDATATYPE digitSet, digitShift, digitUnit, idx;
    int multiple;

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
    idx = (getDig(*this) + NUM1 - 1) / DIGITNUM + 1;
    multiple = pow(10, (DIGITNUM - digitUnit) % DIGITNUM);

    this->value.resize(idx);
    for(int i=0; i<idx; i++) this->value[i] = 0;

    for(int i=0; i<this->value.size(); i++){
        if(0 <= i+digitShift-1) this->value[i+digitShift-1] = (this->value[i] % multiple) * (DIGIT10 / multiple);
        else this->value[i] = 0;
        if(i+digitShift < idx) this->value[i+digitShift] += this->value[i] / multiple;
    }
    return *this;
};
BIGNUM BIGNUM::operator >>= (DIGITDATATYPE NUM1){
    if(this->isInf) return *this;
    if(NUM1 < 0) return *this;
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
    this->point += NUM1 - zero;
    if(!zero) return *this;

    digitSet = zero / DIGITNUM;
    digitUnit = zero % DIGITNUM;
    digitShift = digitSet + (digitUnit ? 1 : 0);
    idx = (getDig(*this) - NUM1 + zero - 1) / DIGITNUM + 1;
    multiple = pow(10, (DIGITNUM - digitUnit) % DIGITNUM);

    for(int i=0; i<origSize; i++){
        if(0 <= i+digitShift-1) this->value[i] = this->value[i+digitShift-1] / multiple;
        else this->value[i] = 0;
        if(i+digitShift < idx) this->value[i] +=  (this->value[i+digitShift] % multiple) * (DIGIT10 / multiple);
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
    int pointDiff = (this->point - NUM1.point) % DIGITNUM, pointOver = abs(this->point - NUM1.point) / DIGITNUM;
    int modNUM0, modNUM1;
    int idx0, idx1;

    BIGNUM const *_NUM0, *_NUM1;
    if(pointDiff < 0){
        _NUM0 = this;
        _NUM1 = &NUM1;
    }
    else{
        _NUM0 = &NUM1;
        _NUM1 = this;
    }
    modNUM1 = pow(10, abs(pointDiff));
    modNUM0 = (DIGIT10/modNUM1);

    size0 = (getDig(*_NUM0) - 1) / DIGITNUM + 1;
    size1 = (getDig(*_NUM1) - 1) / DIGITNUM + 1;
    digit = std::max(size0, size1);

    res.sign = this->sign;
    res.point = std::max(this->point, NUM1.point);
    res.value.resize(digit + 1);

    res.value[0] = 0;
    for(int i=0; i<digit; i--){
        tmpNUM0 = _NUM0->value.size() <= i ? 0 : _NUM0->value[i + pointOver];
        tmpNUM1 = _NUM1->value.size() <= i ? 0 : _NUM1->value[i];

        res.value[i] += tmpNUM1 % modNUM1;
        res.value[i] += (tmpNUM0 % modNUM0 + tmpNUM1 / modNUM1) * modNUM1;
        res.value[i + 1] = tmpNUM0 / modNUM0 + (res.value[i] > DIGIT10);
        res.value[i] %= DIGIT10;
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
    
    int digit, tmpNUM0, tmpNUM1;
    int size0, size1;
    int pointDiff = (this->point - NUM1.point) % DIGITNUM, pointOver = abs(this->point - NUM1.point) / DIGITNUM;
    int modNUM0, modNUM1;
    int idx0, idx1;

    BIGNUM const *_NUM0, *_NUM1;
    if(pointDiff < 0){
        _NUM0 = this;
        _NUM1 = &NUM1;
    }
    else{
        _NUM0 = &NUM1;
        _NUM1 = this;
    }
    modNUM1 = pow(10, abs(pointDiff));
    modNUM0 = (DIGIT10/modNUM1);

    size0 = (getDig(*_NUM0) - 1) / DIGITNUM + 1;
    size1 = (getDig(*_NUM1) - 1) / DIGITNUM + 1;
    digit = std::max(size0, size1);

    res.sign = this->sign;
    res.point = std::max(this->point, NUM1.point);
    res.value.resize(digit + 1);

    res.value[0] = 0;
    for(int i=0; i<digit; i--){
        tmpNUM0 = _NUM0->value.size() <= i ? 0 : _NUM0->value[i + pointOver];
        tmpNUM1 = _NUM1->value.size() <= i ? 0 : _NUM1->value[i];

        res.value[i] += tmpNUM1 % modNUM1;
        res.value[i] += (tmpNUM0 % modNUM0 - tmpNUM1 / modNUM1) * modNUM1;
        res.value[i + 1] = tmpNUM0 / modNUM0 - (res.value[i] < 0);
        if(res.value[i] < 0) res.value[i] += DIGIT10;
    }

    return res;
}
BIGNUM BIGNUM::operator * (BIGNUM const &NUM1){
    BIGNUM res;
    int digit;
    long long int tmp;

    res.sign = this->sign ^ NUM1.sign;

    digit = getDig(*this) + getDig(NUM1);

    res.value.resize(digit);
    res.point = this->point + NUM1.point;

    for(int i=0; i<res.value.size(); i++) res.value[i] = 0;

    for(int i=0; i<(int)this->value.size(); i++){
        for(int j=0; j<(int)NUM1.value.size(); j++){
            tmp = (long long int)(this->value[i]) * (long long int)(NUM1.value[j]);
            
            res.value[i + j] += tmp % DIGIT10;
            res.value[i + j + 1] += tmp / DIGIT10;
        }
    }

    return res;
}
BIGNUM BIGNUM::operator / (BIGNUM NUM1){
    BIGNUM NUM0 = *this, res;
    int digitNUM0, digitNUM1, digitExact, nowDigit, rangeL, f0, f1;

    if(isZero(NUM1)) {
        res.isInf = 1;
        return res;
    }

    digitNUM0 = getDig(NUM0);
    digitNUM1 = getDig(NUM1);
    nowDigit = (digitNUM0 - NUM0.point) - (digitNUM1 - NUM1.point);
    digitExact = digitNUM0 + digitNUM1;

    res.sign = this->sign ^ NUM1.sign;
    res.point = digitExact;

    res.value.resize(std::max((nowDigit + digitExact - 1)/DIGITNUM + 1, 0));
    for(int i=0; i<res.value.size(); i++) res.value[i] = 0;

    f1 = firstDigit2(NUM1);
    if(digitNUM0 > digitNUM1) NUM1 <<= (digitNUM0 - digitNUM1);
    else NUM1 >>= (digitNUM0 - digitNUM1);
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
                res.value[(nowDigit + digitExact)/DIGITNUM] += (i-1) * pow(10, nowDigit % DIGITNUM);
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

    digitNUM0 = getDig(res);
    digitNUM1 = getDig(NUM1);

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
    if(dig0 - this->point != dig1 - NUM1.point) return dig0 - this->point < dig1 - NUM1.point;
    if(this->sign != NUM1.sign) return this->sign < NUM1.sign;

    bool rev = 0;
    int pointDiff = (this->point - NUM1.point) % DIGITNUM, pointOver = abs(this->point - NUM1.point) / DIGITNUM;
    int modNUM0, modNUM1;
    int idx0, idx1;
    BIGNUM const *_NUM0, *_NUM1;
    if(pointDiff < 0){
        _NUM0 = this;
        _NUM1 = &NUM1;
        dig0 += this->point;
        dig1 += NUM1.point;
    }
    else{
        _NUM0 = &NUM1;
        _NUM1 = this;
        tmp = dig0;
        dig0 = dig1 + NUM1.point;
        dig1 = tmp + this->point;
        rev = 1;
    }

    idx0 = (dig0-1)/DIGITNUM + 1;
    idx1 = (dig1-1)/DIGITNUM + 1;
    modNUM1 = pow(10, abs(pointDiff));
    modNUM0 = (DIGIT10/modNUM1);
    
    for(int i=0;;i++){
        if(idx0 - i < 0 || idx1 - i - pointOver < 0) break;
        if(_NUM0->value[idx0 - i] % modNUM0 < _NUM1->value[idx1 - i - pointOver] / modNUM1) return 1 ^ rev;
        if(idx0 - i - 1 < 0) break;
        if(_NUM0->value[idx0 - i - 1] / modNUM0 < _NUM1->value[idx1 - i - pointOver] % modNUM1) return 1 ^ rev;
    }
    return (dig0 < dig1) ^ rev;
}
bool BIGNUM::operator < (BIGNUM const &NUM1){
    if(this->isInf) return 0;

    int dig0 = getDig(*this), dig1 = getDig(NUM1), tmp;
    if(dig0 - this->point != dig1 - NUM1.point) return dig0 - this->point < dig1 - NUM1.point;
    if(this->sign != NUM1.sign) return this->sign > NUM1.sign;

    bool rev = 0;
    int pointDiff = (this->point - NUM1.point) % DIGITNUM, pointOver = abs(this->point - NUM1.point) / DIGITNUM;
    int modNUM0, modNUM1;
    int idx0, idx1;
    BIGNUM const *_NUM0, *_NUM1;
    if(pointDiff < 0){
        _NUM0 = this;
        _NUM1 = &NUM1;
        dig0 += this->point;
        dig1 += NUM1.point;
    }
    else{
        _NUM0 = &NUM1;
        _NUM1 = this;
        tmp = dig0;
        dig0 = dig1 + NUM1.point;
        dig1 = tmp + this->point;
        rev = 1;
    }

    idx0 = (dig0-1)/DIGITNUM + 1;
    idx1 = (dig1-1)/DIGITNUM + 1;
    modNUM1 = pow(10, abs(pointDiff));
    modNUM0 = (DIGIT10/modNUM1);
    
    for(int i=0;;i++){
        if(idx0 - i < 0 || idx1 - i - pointOver < 0) break;
        if(_NUM0->value[idx0 - i] % modNUM0 < _NUM1->value[idx1 - i - pointOver] / modNUM1) return 1 ^ rev;
        if(idx0 - i - 1 < 0) break;
        if(_NUM0->value[idx0 - i - 1] / modNUM0 < _NUM1->value[idx1 - i - pointOver] % modNUM1) return 1 ^ rev;
    }
    return (dig0 < dig1) ^ rev;
}
bool BIGNUM::operator == (BIGNUM const &NUM1){
    if(this->sign != NUM1.sign || degree(*this) != degree(NUM1)) return 0;

    int dig0 = getDig(*this), dig1 = getDig(NUM1), tmp;
    int pointDiff = (this->point - NUM1.point) % DIGITNUM, pointOver = abs(this->point - NUM1.point) / DIGITNUM;
    int modNUM0, modNUM1;
    int idx0, idx1;
    BIGNUM const *_NUM0, *_NUM1;
    if(pointDiff < 0){
        _NUM0 = this;
        _NUM1 = &NUM1;
        dig0 += this->point;
        dig1 += NUM1.point;
    }
    else{
        _NUM0 = &NUM1;
        _NUM1 = this;
        tmp = dig0;
        dig0 = dig1 + NUM1.point;
        dig1 = tmp + this->point;
    }

    idx0 = (dig0-1)/DIGITNUM + 1;
    idx1 = (dig1-1)/DIGITNUM + 1;
    modNUM1 = pow(10, abs(pointDiff));
    modNUM0 = (DIGIT10/modNUM1);
    
    for(int i=0;;i++){
        if(idx0 - i < 0 || idx1 - i - pointOver < 0) break;
        if(_NUM0->value[idx0 - i] % modNUM0 < _NUM1->value[idx1 - i - pointOver] / modNUM1) return 0;
        if(idx0 - i - 1 < 0) break;
        if(_NUM0->value[idx0 - i - 1] / modNUM0 < _NUM1->value[idx1 - i - pointOver] % modNUM1) return 0;
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