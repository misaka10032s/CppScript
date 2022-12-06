#include <string>
#include <iostream>
#include <unordered_map>
#include <vector>

#include "systemOS.h"

#ifndef LIB_JSON_H_
#define LIB_JSON_H_

class JSON{
    public:
        double number;
        std::string string;
        std::vector<JSON> array;
        char storedType; // 0=number, 1=string, 2=array

        std::unordered_map<std::string, JSON> data;

        JSON();
        JSON(std::string rawData);

        double operator [] (std::string argw);
        std::string JSON::operator [] (std::string argw);
        std::vector<JSON> JSON::operator [] (std::string argw);
        auto JSON::operator [] (std::string argw);

        bool contains<typename T> (std::string argw);

        JSON operator = (double argw1);
        JSON operator = (std::string argw1);
        JSON operator = (std::vector<JSON> argw1);

        JSON operator = (JSON argw1);

        friend double operator = (double argw0, const JSON &argw1);
        friend std::string  operator = (std::string argw0, const JSON &argw1);
        friend std::vector<JSON> operator = (std::vector<JSON> argw0, const JSON &argw1);
}

#endif