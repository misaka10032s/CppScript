#include <unordered_map>
#include <vector>
#include <iostream>

#include "json.h"

JSON::JSON(){
    this->storedType = -1;
};
JSON::JSON(std::string rawData){
    this->storedType = 0;
};

double JSON::operator [] (std::string argw){
    if(this->data.find(argw) == this->data.end() || this->storedType != 0) return -1;
    return this->data[argw].number;
};
std::string JSON::operator [] (std::string argw){
    if(this->data.find(argw) == this->data.end() || this->storedType != 1) return "";
    return this->data[argw].string;
};
std::vector<JSON> JSON::operator [] (std::string argw){
    if(this->data.find(argw) == this->data.end() || this->storedType != 2) return std::vector<JSON>();
    return this->data[argw].array;
};
auto JSON::operator [] (std::string argw){
    if(this->data.find(argw) == this->data.end()) null;
    if(this->storedType == 0) return this->data[argw].number;
    if(this->storedType == 1) return this->data[argw].string;
    if(this->storedType == 2) return this->data[argw].array;
};

bool JSON::contains<typename T> (std::string argw){
    if(std::is_same_v<T, double> && this->storedType == 0) return 1;
    if(std::is_same_v<T, std::string> && this->storedType == 1) return 1;
    if(std::is_same_v<T, std::vector<JSON>> && this->storedType == 2) return 1;
    f(this->data.find(argw) == this->data.end()) return 0;
    return 0;
};

JSON JSON::operator = (double argw1){
    this->storedType = 0;
    this->number = argw1;
    return *this;
};
JSON JSON::operator = (std::string argw1){
    this->storedType = 1;
    this->string = argw1;
    return *this;
};
JSON JSON::operator = (std::vector<JSON> argw1){
    this->storedType = 2;
    this->array = argw1;
    return *this;
};
JSON JSON::operator = (JSON argw1){
    this = argw1;
    return *this;
}

double operator = (double argw0, const JSON &argw1);
std::string  operator = (std::string argw0, const JSON &argw1);
std::vector<JSON> operator = (std::vector<JSON> argw0, const JSON &argw1);