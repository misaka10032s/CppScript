// #include <functional>
#include <iostream>
#include <type_traits> // https://stackoverflow.com/questions/14294267/class-template-for-numeric-types

#include "systemOS.h"

template <typename T> MATRIX<T>::MATRIX(){
    static_assert(std::is_arithmetic<T>::value, "NumericType must be numeric");
};
template <typename T> MATRIX<T>::MATRIX(int colNum, int rowNum){
    static_assert(std::is_arithmetic<T>::value, "NumericType must be numeric");
    this->col = colNum;
    this->row = rowNum;
    this->value.resize(colNum * rowNum);
    for(int i=0; i<this->col * this->row; i++) this->value[i] = 0;
};
template <typename T> MATRIX<T>::MATRIX(int colNum, int rowNum, std::initializer_list<T> NUM1){
    static_assert(std::is_arithmetic<T>::value, "NumericType must be numeric");
    this->col = colNum;
    this->row = rowNum;
    this->value.resize(colNum * rowNum);
    this->value = NUM1;
};

template <typename U> std::ostream& operator<<(std::ostream &os, const MATRIX<U> &m){
    os << "\n";
    for(int i=0; i<m.row; i++){
        for(int j=0; j<m.col; j++) os << "\t" << m.value[i*m.col + j] << std::flush;
        os << "\n";
    }
    
    return os;
};


template <typename T> MATRIX<T> MATRIX<T>::operator + (MATRIX<T> const &MTX1){
    if(this->col != MTX1.col || this->row != MTX1.row) {
        // error here
        return *this;
    }
    MATRIX res = *this;
    for(int i=0; i<this->col * this->row; i++) res.value[i] += MTX1.value[i];
    return res;
};
template <typename T> MATRIX<T> MATRIX<T>::operator - (MATRIX<T> const &MTX1){
    if(this->col != MTX1.col || this->row != MTX1.row) {
        // error here
        return *this;
    }
    MATRIX res = *this;
    for(int i=0; i<this->col * this->row; i++) res.value[i] -= MTX1.value[i];
    return res;
};
template <typename T> MATRIX<T> MATRIX<T>::operator * (MATRIX<T> const &MTX1){
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
template <typename T> MATRIX<T> MATRIX<T>::operator = (MATRIX<T> const &MTX1){
    if(this->value.size() != MTX1.value.size()) this->value.resize(MTX1.value.size());
    this->col = MTX1.col;
    this->row = MTX1.row;
    int s = this->col * this->row;
    for(int i=0; i<s; i++) this->value[i] = MTX1.value[i];
    return *this;
};
template <typename T> bool MATRIX<T>::operator == (MATRIX<T> const &MTX1){
    if(this->col != MTX1.col || this->row != MTX1.row) return 0;
    int s = this->col * this->row;
    for(int i=0; i<s; i++) if(this->value[i] != MTX1.value[i]) return 0;
    return 1;
};
template <typename T> bool MATRIX<T>::operator != (MATRIX<T> const &MTX1){
    if(this->col != MTX1.col || this->row != MTX1.row) return 1;
    int s = this->col * this->row;
    for(int i=0; i<s; i++)  if(this->value[i] != MTX1.value[i]) return 1;
    return 0;
};
template <typename T> MATRIX<T> MATRIX<T>::operator += (MATRIX<T> const &MTX1){
    if(this->col != MTX1.col || this->row != MTX1.row) return *this;
    int s = this->col * this->row;
    for(int i=0; i<s; i++) this->value[i] += MTX1.value[i];
    return *this;
};
template <typename T> MATRIX<T> MATRIX<T>::operator -= (MATRIX<T> const &MTX1){
    if(this->col != MTX1.col || this->row != MTX1.row) return *this;
    int s = this->col * this->row;
    for(int i=0; i<s; i++) this->value[i] -= MTX1.value[i];
    return *this;
};
template <typename T> MATRIX<T> MATRIX<T>::operator *= (MATRIX<T> const &MTX1){
    *this = *this * MTX1;
    return *this;
};

template <typename T> MATRIX<T> MATRIX<T>::operator = (double* const NUM1){
    int s = this->col * this->row;
    for(int i=0; i<s; i++) this->value[i] = NUM1[i];
    return *this;
};
template <typename T> MATRIX<T> MATRIX<T>::operator = (std::initializer_list<double> NUM1){
    this->value = NUM1;
    return *this;
};
// template <typename T> MATRIX MATRIX<T>::operator = (std::initializer_list<std::std::initializer_list<std::initializer_list<double>> NUM1);<double>> NUM1){
//     this->value = NUM1;
//     return *this;
// };

template <typename T> MATRIX<T> MATRIX<T>::operator + (double const &NUM1){
    MATRIX<T> res(this->row, this->col);
    int s = this->col * this->row;
    if(this->row != this->col) for(int i=0; i<s; i++) res.value[i] = this->value[i] + NUM1;
    else for(int i=0; i<s; i++) res.value[i] = this->value[i] + (i%(this->row+1) ? 0 : NUM1);
    return res;
};
template <typename T> MATRIX<T> MATRIX<T>::operator - (double const &NUM1){
    MATRIX<T> res(this->row, this->col);
    int s = this->col * this->row;
    if(this->row != this->col) for(int i=0; i<s; i++) res.value[i] = this->value[i] - NUM1;
    else for(int i=0; i<s; i+=this->row+1) res.value[i] = this->value[i] - (i%(this->row+1) ? 0 : NUM1);
    return res;
};
template <typename T> MATRIX<T> MATRIX<T>::operator * (double const &NUM1){
    MATRIX<T> res(this->row, this->col);
    int s = this->col * this->row;
    for(int i=0; i<s; i++) res.value[i] = this->value[i] * NUM1;
    return res;
};
template <typename T> MATRIX<T> MATRIX<T>::operator / (double const &NUM1){
    MATRIX<T> res(this->row, this->col);
    int s = this->col * this->row;
    for(int i=0; i<s; i++) res.value[i] = this->value[i] / NUM1;
    return res;
};
template <typename T> MATRIX<T> MATRIX<T>::operator += (double const &NUM1){
    int s = this->col * this->row;
    if(this->row != this->col) for(int i=0; i<s; i++) this->value[i] += NUM1;
    else for(int i=0; i<s; i+=this->row+1) this->value[i] += NUM1;
    return *this;
};
template <typename T> MATRIX<T> MATRIX<T>::operator -= (double const &NUM1){
    int s = this->col * this->row;
    if(this->row != this->col) for(int i=0; i<s; i++) this->value[i] -= NUM1;
    else for(int i=0; i<s; i+=this->row+1) this->value[i] -= NUM1;
    return *this;
};
template <typename T> MATRIX<T> MATRIX<T>::operator *= (double const &NUM1){
    int s = this->col * this->row;
    for(int i=0; i<s; i++) this->value[i] *= NUM1;
    return *this;
};
template <typename T> MATRIX<T> MATRIX<T>::operator /= (double const &NUM1){
    int s = this->col * this->row;
    for(int i=0; i<s; i++) this->value[i] /= NUM1;
    return *this;
};







template <typename T> MATRIX<T> transposed(MATRIX<T> MTX1){
    MATRIX res(MTX1.row, MTX1.col);
    for(int i=0; i<MTX1.row; i++){ // 2
        for(int j=0; j<MTX1.col; j++){ // 3
            res.value[j*MTX1.row+i] = MTX1.value[i*MTX1.col+j];
        }
    }
    return res;
};
template <typename T> MATRIX<T> unitMTX(int order){
    int s = order * order;
    MATRIX<T> res(order, order);
    for(int i=0; i<s; i+=order+1){
        res.value[i] = 1;
    }
    return res;
};
template <typename T> MATRIX<T> rotateMTX_d(double deg){
    MATRIX<T> res(2, 2);
    double theta = deg * M_PI / 180.0;
    res.value[0] = cos(theta);
    res.value[1] = -sin(theta);
    res.value[2] = sin(theta);
    res.value[3] = cos(theta);
    return res;
};
template <typename T> MATRIX<T> rotateMTX_r(double rad){
    MATRIX<T> res(2, 2);
    res.value[0] = cos(rad);
    res.value[1] = -sin(rad);
    res.value[2] = sin(rad);
    res.value[3] = cos(rad);
    return res;
};
