// #define SYS_LINUX
#define SYS_WINDOWS
#include "library/systemS.h"

#include <iostream>
#define _USE_MATH_DEFINES
#include <cmath> 
#include <map>
// #include <windows.h>
#include <ctime>

#include <cstring>
#include <vector>

#include <string>
#include <sstream>
#include <chrono>

#include "library/heap.h"
#include "library/graph.h"
#include "library/number2.h"

using namespace std;

int* test(int *i[2]){
    cout<<(*i)[0]<<", "<<(*i)[1]<<"\n";
    (*i)[0]++;
    (*i)[1] *= 10;
    cout<<(*i)[0]<<", "<<(*i)[1]<<"\n";
    return *i;
}

bool cpInt(int a, int b){
    return a<b;
}

int main(){
    int x=1, y=1;
    int* a[2] = {&x, &y};
    test(a)[0] *= 7;
    cout<<(*a)[0]<<", "<<(*a)[1]<<"\n";

    HEAP myHeap(1);
    // SMMH myHeap;
    myHeap.push("A", 5);
    myHeap.push("B", 8);
    myHeap.push("C", 4);
    myHeap.push("D", 10);
    myHeap.push("E", 6);
    myHeap.push("F", 7);
    myHeap.push("G", 5);

    // myHeap.list(nullptr);cout<<"------------------\n";

    // CAGEFBD
    for(int i=0; i<8; i++){
        cout<< myHeap.pop().key<<"\n";
        // cout<< myHeap.popMin().key<<"\n";
    }

    listHEAP<int> mylistHEAP(&cpInt);
    mylistHEAP.push(5);
    mylistHEAP.push(8);
    mylistHEAP.push(4);
    mylistHEAP.push(10);
    mylistHEAP.push(6);
    mylistHEAP.push(7);
    mylistHEAP.push(5);
    for(int i=0; i<7; i++) cout<< mylistHEAP.pop()<<"\n";

    std::vector<EDGE> links;
    string vtx[21][2] = {{"a", "b"}, {"a", "c"}, {"a", "d"}, {"b", "c"}, {"b", "f"}, {"b", "g"}, {"c", "d"}, {"c", "e"}, {"c", "f"}, {"d", "e"}, {"d", "i"}, {"e", "i"}, {"e", "f"}, {"f", "i"}, {"f", "h"}, {"f", "g"}, {"g", "h"}, {"g", "j"}, {"h", "i"}, {"h", "j"}, {"i", "j"}};
    int vtxW[21] = {9,3,6,9,8,18,4,2,9,2,9,9,8,7,9,10,3,4,5,1,4};
    for(int i=0; i<21; i++) links.push_back(*(new EDGE(vtx[i][0], vtx[i][1], vtxW[i])));
    cout<<MST_P(links).cost<<"\n";
    cout<<MST_K(links).cost<<"\n";

    std::cout << "Float Range" << std::numeric_limits<float>::min() << " / " << std::numeric_limits<float>::max() << '\n';

    BIGNUM X = 100.7, Y = (std::string)"456000", Z = (std::string)"123456789";
    // std::cout << (-8)%7 <<"\n";
    // std::cout << X <<"\n";
    // std::cout << Y <<"\n";
    // X *= Y;
    // std::cout << X <<"\n";
    std::cout << (Y>>3) <<"\n";
    Y >>= 3;
    std::cout << Y <<"\n";
    std::cout << (Z<<3) <<"\n";
    Z <<= 3;
    std::cout << Z <<"\n";
    std::cout << ((BIGNUM)1.5) <<"\n";
    std::cout << ((BIGNUM)4000 > 4000) <<"\n"; // 0
    std::cout << ((BIGNUM)0 == 0) <<"\n"; // 1
    std::cout << ((BIGNUM)0 == 0) <<"\n"; // 1
    std::cout << ((BIGNUM)0 == 1) <<"\n"; // 0
    std::cout << ((BIGNUM)8 >= 0) <<"\n"; // 1
    std::cout << ((BIGNUM)8 <= 1.5) <<"\n"; // 0
    std::cout << ((BIGNUM)"5649825757867898" <= (BIGNUM)"9829562782678167816849846") <<"\n"; // 1
    std::cout << ((BIGNUM)"5649825,757867898" > (BIGNUM)"9829562,782678167,816849846") <<"\n"; // 0
    std::cout << "---------------------------------------------------------" <<"\n";
    std::cout << (BIGNUM)0.0054658885 <<"\n";
    std::cout << (BIGNUM)54658885 <<"\n";
    std::cout << (BIGNUM)"0.0054658885" <<"\n";
    std::cout << (BIGNUM)"0.000000000000000054658885" <<"\n"; // 16個0 0.0000000000000000000000000+ digitShift
    std::cout << (BIGNUM)"546588850000000000000000" <<"\n"; // 16個0
    std::cout << (BIGNUM)"0054658885.0054658885" <<"\n";
    std::cout << (BIGNUM)"0.0054658885000" <<"\n";
    std::cout << (BIGNUM)"0.54658885000" <<"\n";
    std::cout << (BIGNUM)"0054658885" <<"\n";
    std::cout << (BIGNUM)"0.5" <<"\n";
    std::cout << (BIGNUM)"10000000,000000000,000000000" <<"\n";
    std::cout << "---------------------------------------------------------" <<"\n";
    std::cout << ((BIGNUM)"0.5"<<1) <<"\n";
    std::cout << ((BIGNUM)"0.5"<<10) <<"\n";
    std::cout << ((BIGNUM)"0.0054658885"<<1) <<"\n";
    std::cout << ((BIGNUM)"0.0054658885"<<10) <<"\n";
    std::cout << ((BIGNUM)"50"<<1) <<"\n";
    std::cout << ((BIGNUM)"50"<<10) <<"\n";
    std::cout << ((BIGNUM)"123456789"<<5) <<"\n";
    std::cout << ((BIGNUM)"18446744173709551616"<<3) <<"\n";
    std::cout << ((BIGNUM)"18446744173709551616"<<10) <<"\n";
    std::cout << ((BIGNUM)"18446744173709551616"<<20) <<"\n";
    std::cout << ((BIGNUM)"0.18446744173709551616"<<20) <<"\n";
    std::cout << "---------------------------------------------------------" <<"\n";
    std::cout << ((BIGNUM)"0.5">>1) <<"\n";
    std::cout << ((BIGNUM)"0.5">>10) <<"\n";
    std::cout << ((BIGNUM)"0.0054658885">>1) <<"\n";
    std::cout << ((BIGNUM)"0.0054658885">>10) <<"\n";
    std::cout << ((BIGNUM)"50">>1) <<"\n";
    std::cout << ((BIGNUM)"50">>10) <<"\n";
    std::cout << ((BIGNUM)"123456789">>5) <<"\n";
    std::cout << ((BIGNUM)"18446744173709551616">>3) <<"\n";
    std::cout << ((BIGNUM)"18446744173709551616">>10) <<"\n";
    std::cout << ((BIGNUM)"18446744173709551616">>20) <<"\n";
    std::cout << ((BIGNUM)"0.18446744173709551616">>20) <<"\n";
    std::cout << "---------------------------------------------------------" <<"\n";
    std::cout << (BIGNUM)"18446744073709650360.1158" <<"\n"; // 18446744073709650360.1158
    std::cout << (BIGNUM)"98744.1158" + (BIGNUM)"18446744073709551616" <<"\n"; // 18446744073709650360.1158
    std::cout << (BIGNUM)"18446744073709551616" + (BIGNUM)"98744.1158" <<"\n"; // 18446744073709650360.1158
    std::cout << (BIGNUM)"-18446744073709551616" + (BIGNUM)"98744.1158" <<"\n"; // -18446744073709452871.8842
    std::cout << (BIGNUM)"18446744073709551616" + (BIGNUM)"-98744.1158" <<"\n";
    std::cout << (BIGNUM)"-18446744073709551616" + (BIGNUM)"-98744.1158" <<"\n";
    std::cout << "---------------------------------------------------------" <<"\n";
    std::cout << (BIGNUM)"18446744073709551616" - (BIGNUM)"98744.1158" <<"\n";
    std::cout << (BIGNUM)"-18446744073709551616" - (BIGNUM)"98744.1158" <<"\n";
    std::cout << (BIGNUM)"18446744073709551616" - (BIGNUM)"-98744.1158" <<"\n";
    std::cout << (BIGNUM)"-18446744073709551616" - (BIGNUM)"-98744.1158" <<"\n";
    std::cout << "---------------------------------------------------------" <<"\n";
    std::cout << (BIGNUM)"9999999999999999999999999999999999999999999999" * (BIGNUM)"9999999999999999999999999999999999999999999999" <<"\n";
    std::cout << (BIGNUM)"123456789101112" * (BIGNUM)"0.01" <<"\n"; // 1234567891011.12
    std::cout << (BIGNUM)"123456789101112" * (BIGNUM)"0.0000000001" <<"\n"; // 12345.6789101112
    std::cout << (BIGNUM)"10000000,000000000,000000000"<<"\n";
    std::cout << (BIGNUM)"0.0000000001" <<"\n";
    std::cout << (BIGNUM)"10000000,000000000,000000000" * (BIGNUM)"0.0000000001" <<"\n";
    std::cout << "---------------------------------------------------------" <<"\n";
    std::cout << (BIGNUM)"123456" / (BIGNUM)"987" <<"\n"; // 125.08206686930092
    std::cout << (BIGNUM)"100000" / (BIGNUM)"8" <<"\n"; // 12500
    std::cout << (BIGNUM)"123456789101112" / (BIGNUM)"0.0000000001" <<"\n"; // 1234567891011120000000000
    std::cout << (BIGNUM)"18446744073709551616" / (BIGNUM)"548894" <<"\n"; // 33607115533617.69597772976203055599077417
    std::cout << "---------------------------------------------------------" <<"\n";
    std::cout << (BIGNUM)"987654321" % (BIGNUM)"159" <<"\n"; // 63
    std::cout << (BIGNUM)"18446744073709551616" % (BIGNUM)"548894" <<"\n"; // 382018
    std::cout << (BIGNUM)"18446744073709551616" % (BIGNUM)"98744.1158" <<"\n"; // 98524.0954
    std::cout << "---------------------------------------------------------" <<"\n";
    std::cout << (BIGNUM)1.023 <<"\n";
    std::cout << (BIGNUM)1.45 <<"\n";
    std::cout << ((BIGNUM)0 == 0) <<"\n";
    std::cout << ((BIGNUM)0 == 1) <<"\n";
    std::cout << ((BIGNUM)8 >= 0) <<"\n";
    std::cout << ((BIGNUM)8 <= 1.5) <<"\n";
    std::cout << (BIGNUM)1234560789 + (BIGNUM)2076543210 <<"\n";
    std::cout << (BIGNUM)1234560789 * (BIGNUM)2076543210 <<"\n"; // 2563618823730192690
    std::cout << (BIGNUM)1234560789.654654 * (BIGNUM)2076543210.123123 <<"\n"; // 2563618825241612836.
    std::cout << (BIGNUM)"-1234560789.654654" * (BIGNUM)"2076543210.123123"<<"\n";
    std::cout << (BIGNUM)"1234560789.654654" * (BIGNUM)"2076543210.123123" <<"\n";
    std::cout << (BIGNUM)1234560789.654654 <<"\n";
    std::cout << (BIGNUM)2076543210.123123 <<"\n";
    std::cout << (BIGNUM)"0.0000000001" <<"\n";
    std::cout << "---------------------------------------------------------" <<"\n";
    std::cout << (BIGNUM)"9876543210" - (BIGNUM)"1239012" <<"\n"; // 9875304198
    std::cout << (BIGNUM)"9876543210" - (BIGNUM)"123456789012" <<"\n"; // -113580245802
    std::cout << "---------------------------------------------------------" <<"\n";
    std::cout << (BIGNUM)"-123.456" + (BIGNUM)"98744.1158" <<"\n"; // 98620.6598
    std::cout << "---------------------------------------------------------" <<"\n";


BIGNUM BNUM0 = (BIGNUM)"18446744173709551616.1", BNUM1 = (BIGNUM)"98198298.1";


auto st = std::chrono::high_resolution_clock::now();
for(int i=0; i<10000000; i++) {
    BNUM0 + BNUM1;
}
auto ed = std::chrono::high_resolution_clock::now();
auto duration = std::chrono::duration_cast<std::chrono::microseconds>(ed - st);
std::cout<<"c:"<<duration.count()<<"\n";


st = std::chrono::high_resolution_clock::now();
for(int i=0; i<10000000; i++){
    BNUM0 - BNUM1;
}
ed = std::chrono::high_resolution_clock::now();
duration = std::chrono::duration_cast<std::chrono::microseconds>(ed - st);
std::cout<<"c:"<<duration.count()<<"\n";


st = std::chrono::high_resolution_clock::now();
for(int i=0; i<10000000; i++) {
    BNUM0 * BNUM1;
}
ed = std::chrono::high_resolution_clock::now();
duration = std::chrono::duration_cast<std::chrono::microseconds>(ed - st);
std::cout<<"c:"<<duration.count()<<"\n";


st = std::chrono::high_resolution_clock::now();
for(int i=0; i<100000; i++) {
    BNUM0 / BNUM1;
}
ed = std::chrono::high_resolution_clock::now();
duration = std::chrono::duration_cast<std::chrono::microseconds>(ed - st);
std::cout<<"c:"<<duration.count()<<"\n";


st = std::chrono::high_resolution_clock::now();
for(int i=0; i<100000; i++){
    BNUM0 % BNUM1;
}
ed = std::chrono::high_resolution_clock::now();
duration = std::chrono::duration_cast<std::chrono::microseconds>(ed - st);
std::cout<<"c:"<<duration.count()<<"\n";

    return 0;
}