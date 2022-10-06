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
#include "library/number3.h"
#include "library/picture.h"
#include "library/systemFnc.h"

using namespace std;

inline int getDigz(BIGNUM const &NUM0){
    int dig0 = 0;
    for(int i=NUM0.value.size()-1; i>=0; i--){
        if(NUM0.value[i] != 0){
            return DIGITNUM * i + floor(log10(NUM0.value[i]));
        }
    }
    return dig0;
}
inline int degreez(BIGNUM const &NUM0){
    int dig0 = 0;
    for(int i=NUM0.value.size()-1; i>=0; i--){
        if(NUM0.value[i] != 0){
            return DIGITNUM * i + floor(log10(NUM0.value[i])) - NUM0.point;
        }
    }
    
    return dig0;
};
int firstDigit2(BIGNUM const &NUM0){
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
int pow10Listz[10] = {
    1, 10, 100, 1000, 10000, 
    100000, 1000000, 10000000, 100000000, 1000000000
};
inline int quick_pow10z(int n){
    return pow10Listz[n];
}
bool compareAbsz(BIGNUM const &NUM0, BIGNUM const &NUM1){
    if(NUM0.isInf) return 0;

    int dig0 = getDigz(NUM0), dig1 = getDigz(NUM1), tmp;
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
    modNUM1 = quick_pow10z(abs(pointDiff));
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
void newNUM(){
    BIGNUM xs;
}
void newNUM(int sz){
    BIGNUM xs;
    xs.value.resize(sz);
}


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
    PICTURE MS(200, 300, "MapleStory"), effect;
    MS.screenShot(50, 50).display();
    MS.twoValue(effect, 128).display();
    MS.subPic(effect, RANGE(70, 150, 20, 160)).display();
    MS.GaussBlur(effect, 2).display();
    MS.twist(effect, 100, 150, 50).display();
    MS.blackhole(effect, 100, 150, 20, 2).display();
    MS.negative(effect).display();
    MS.RGBtoYUV(effect, 'Y').display();
    MS.RGBtoYUV(effect, 'U').display();
    MS.RGBtoYUV(effect, 'V').display();
    MS.RGBtoYUV(effect, 'Z').display();
    MS.GammaTransform(effect, 1.5).display();
    MS.sharp(effect, 2).display();
    MS.imgFloor(effect, 64).display();
    MS.rotateD(effect, 100, 150, 45, RANGE(50, 150, 50, 250)).display();
    MS.rotateQ(effect, 1).display();
    MS.rotateQ(effect, 2).display();
    MS.rotateQ(effect, 3).display(); // error
    MS.rotateQ(effect, 4).display();
    MS.ripple(effect, 100, 150, 10).display();
    MS.copyPaste(effect, RANGE(50, 150, 50, 250), 60, 70).display();

    SYS MYSYS("MapleStory");
    while(1){
        MYSYS.wait(50);
    }
    
    // int x=1, y=1;
    // int* a[2] = {&x, &y};
    // test(a)[0] *= 7;
    // cout<<(*a)[0]<<", "<<(*a)[1]<<"\n";

    // STACK<int> mySTACK(10);
    // for(int i=0; i<10; i++) mySTACK.push(i+1);
    // while(mySTACK.canPop) std::cout<<mySTACK.pop()<<", "; std::cout<<"\n";

    // STACK<int> mySTACK(10);
    // mySTACK.push(5);
    // mySTACK.push(8);
    // mySTACK.push(6);
    // mySTACK.push(9);
    // std::cout<<mySTACK.pop()<<", "; 
    // std::cout<<mySTACK.pop()<<", "; 
    // std::cout<<mySTACK.pop()<<", "; 
    // mySTACK.push(1);
    // mySTACK.push(2);
    // std::cout<<mySTACK.pop()<<", "; 
    // std::cout<<mySTACK.pop()<<", "; 
    // std::cout<<mySTACK.pop()<<", "; 
    // std::cout<<"\n";

    // QUEUE<int> myQUEUE(10);
    // myQUEUE.push(5);
    // myQUEUE.push(8);
    // myQUEUE.push(6);
    // myQUEUE.push(9);
    // std::cout<<myQUEUE.pop()<<", "; 
    // std::cout<<myQUEUE.pop()<<", "; 
    // std::cout<<myQUEUE.pop()<<", "; 
    // myQUEUE.push(1);
    // myQUEUE.push(2);
    // std::cout<<myQUEUE.pop()<<", "; 
    // std::cout<<myQUEUE.pop()<<", "; 
    // std::cout<<myQUEUE.pop()<<", "; 
    // std::cout<<"\n";


    // COMPLEX C0(9,8), C1(1,-9);
    // std::cout<<"C+C "<<C0 + C1<<"\n";
    // std::cout<<"C-C "<<C0 - C1<<"\n";
    // std::cout<<"C*C "<<C0 * C1<<"\n";
    // std::cout<<"C/C "<<C0 / C1<<"\n";

    // HEAPHEAPHEAPHEAPHEAPHEAPHEAPHEAPHEAPHEAPHEAPHEAPHEAPHEAPHEAPHEAPHEAP
    // HEAP myHeap(1);
    // // SMMH myHeap;
    // myHeap.push("A", 5);
    // myHeap.push("B", 8);
    // myHeap.push("C", 4);
    // myHeap.push("D", 10);
    // myHeap.push("E", 6);
    // myHeap.push("F", 7);
    // myHeap.push("G", 5);
    // // myHeap.list(nullptr);cout<<"------------------\n";

    // // CAGEFBD
    // for(int i=0; i<8; i++){
    //     cout<< myHeap.pop().key<<"\n";
    //     // cout<< myHeap.popMin().key<<"\n";
    // }

    // for(int i=0; i<10000000; i++){ // memory check ok;
    //     myHeap.push("A", 5);
    //     myHeap.push("B", 8);
    //     myHeap.push("C", 4);
    //     myHeap.push("D", 10);
    //     myHeap.push("E", 6);
    //     myHeap.push("F", 7);
    //     myHeap.push("G", 5);
    //     for(int j=0; j<7; j++) myHeap.pop();
    // }

    // listHEAP<int> mylistHEAP(&cpInt);
    // for(int i=0; i<7; i++) cout<< "Empty Pop" << mylistHEAP.pop()<<"\n";
    // mylistHEAP.push(5);
    // mylistHEAP.push(8);
    // mylistHEAP.push(4);
    // mylistHEAP.push(10);
    // mylistHEAP.push(6);
    // mylistHEAP.push(7);
    // mylistHEAP.push(5);
    // for(int i=0; i<7; i++) cout<< mylistHEAP.pop()<<"\n";

    // for(int i=0; i<10000000; i++){ // memory check ok;
    //     mylistHEAP.push(5);
    //     mylistHEAP.push(8);
    //     mylistHEAP.push(4);
    //     mylistHEAP.push(10);
    //     mylistHEAP.push(6);
    //     mylistHEAP.push(7);
    //     mylistHEAP.push(5);
    //     for(int j=0; j<7; j++) mylistHEAP.pop();
    // }



    // std::vector<EDGE> links;
    // string vtx[21][2] = {{"a", "b"}, {"a", "c"}, {"a", "d"}, {"b", "c"}, {"b", "f"}, {"b", "g"}, {"c", "d"}, {"c", "e"}, {"c", "f"}, {"d", "e"}, {"d", "i"}, {"e", "i"}, {"e", "f"}, {"f", "i"}, {"f", "h"}, {"f", "g"}, {"g", "h"}, {"g", "j"}, {"h", "i"}, {"h", "j"}, {"i", "j"}};
    // int vtxW[21] = {9,3,6,9,8,18,4,2,9,2,9,9,8,7,9,10,3,4,5,1,4};
    // for(int i=0; i<21; i++) links.push_back(*(new EDGE(vtx[i][0], vtx[i][1], vtxW[i])));
    // cout<<MST_P(links).cost<<"\n";
    // cout<<MST_K(links).cost<<"\n";

    // for(int i=0; i<10000000; i++){ // memory check ok;
    //     MST_P(links);
    //     MST_K(links);
    // }

    // std::cout << "Float Range" << std::numeric_limits<float>::min() << " / " << std::numeric_limits<float>::max() << '\n';

    // BIGNUM X = 100.7, Y = (std::string)"456000", Z = (std::string)"123456789";
    // std::cout << (-8)%7 <<"\n";
    // std::cout << X <<"\n";
    // std::cout << Y <<"\n";
    // X *= Y;
    // std::cout << X <<"\n";

    // std::cout << (Y>>3) <<"\n";
    // Y >>= 3;
    // std::cout << Y <<"\n";
    // std::cout << (Z<<3) <<"\n";
    // Z <<= 3;
    // std::cout << Z <<"\n";

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
    std::cout << (BIGNUM)"437526789.678" * (BIGNUM)"871678.67525" <<"\n"; // 381382772412904.4140695
    std::cout << (BIGNUM)"7868645375337.8167168" * (BIGNUM)"91898092870358973.509840958732"  <<"\n"; // 723113503466715323511603733380.6303096690085310976
    std::cout << (BIGNUM)"10000000,000000000,000000000" * (BIGNUM)"0.0000000001" <<"\n";
    std::cout << "---------------------------------------------------------" <<"\n";
    std::cout << (BIGNUM)"123456" / (BIGNUM)"987" <<"\n"; // 125.08206686930092
    std::cout << (BIGNUM)"100000" / (BIGNUM)"8" <<"\n"; // 12500
    std::cout << (BIGNUM)"123456789101112" / (BIGNUM)"0.0000000001" <<"\n"; // 1234567891011120000000000
    std::cout << (BIGNUM)"18446744073709551616" / (BIGNUM)"548894" <<"\n"; // 33607115533617.69597772976203055599077417
    std::cout << (BIGNUM)"1844674407370955.1616" / (BIGNUM)"5488.94" <<"\n"; // 336071155336.1769597772976203055599077417
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


BIGNUM BNUM0 = (BIGNUM)"89498504469840568940468468045.123", BNUM1 = (BIGNUM)"18446744173709551616.1", BNUM2 = (BIGNUM)"89498504469840568940468468045.2", resBNUM;
// std::cout<<"?:"<<BNUM0 / BNUM1<<"\n"; // 4851723622.718992244523586499562
MATRIX<int> M(2,2,{1,2,3,4});
std::cout<<M<<"\n";

std::cout<<"0?:"<<50 + BNUM0<<"\n";
BNUM0.plus(BNUM1, BNUM0).minus(BNUM1, BNUM0);
std::cout<<"1?:"<<BNUM0<<"\n";

BNUM0.multiply(BNUM1, BNUM0).divideBy(BNUM1, BNUM0);;
std::cout<<"2?:"<<BNUM0<<", "<<BNUM0.point<<"\n";
// BNUM0.divideBy(BNUM1, resBNUM);
// std::cout<<"?:"<<resBNUM<<"\n";
// std::cout<<"?:"<<BNUM0<<", "<<BNUM1<<"\n";
BNUM0.modulus(BNUM1, resBNUM);
std::cout<<"0--:"<<BNUM0<<", "<<BNUM1<<", "<<resBNUM<<", "<<BNUM0.point<<", "<<BNUM1.point<<", "<<resBNUM.point<<", "<<"\n";
    BNUM0.divideBy(BNUM1, resBNUM);
std::cout<<"1--:"<<BNUM0<<", "<<BNUM1<<", "<<resBNUM<<", "<<BNUM0.point<<", "<<BNUM1.point<<", "<<resBNUM.point<<", "<<"\n";
    BNUM0.divideBy(BNUM1, resBNUM);
std::cout<<"2--:"<<BNUM0<<", "<<BNUM1<<", "<<resBNUM<<", "<<BNUM0.point<<", "<<BNUM1.point<<", "<<resBNUM.point<<", "<<"\n";

// for(int i=0; i<10000000; i++) {
//     BNUM0+BNUM1;
// }

auto st = std::chrono::high_resolution_clock::now();
for(int i=0; i<10000000; i++) {
    // BNUM0.plus(BNUM1, resBNUM).minus(BNUM1, resBNUM);
}
auto ed = std::chrono::high_resolution_clock::now();
auto duration = std::chrono::duration_cast<std::chrono::microseconds>(ed - st);
std::cout<<"a:"<<duration.count()<<"\n";


st = std::chrono::high_resolution_clock::now();
for(int i=0; i<10000000; i++) {
    BNUM0+BNUM1;
}
ed = std::chrono::high_resolution_clock::now();
duration = std::chrono::duration_cast<std::chrono::microseconds>(ed - st);
std::cout<<"b:"<<duration.count()<<"\n";


st = std::chrono::high_resolution_clock::now();
for(int i=0; i<10000000; i++) {
    
}
ed = std::chrono::high_resolution_clock::now();
duration = std::chrono::duration_cast<std::chrono::microseconds>(ed - st);
std::cout<<"c:"<<duration.count()<<"\n";


st = std::chrono::high_resolution_clock::now();
for(int i=0; i<10000000; i++) {
    
}
ed = std::chrono::high_resolution_clock::now();
duration = std::chrono::duration_cast<std::chrono::microseconds>(ed - st);
std::cout<<"d:"<<duration.count()<<"\n";

// // ------------------------------------------------------------------------
// st = std::chrono::high_resolution_clock::now();
// for(int i=0; i<10000000; i++) {
//     BNUM0 > BNUM1;
// }
// ed = std::chrono::high_resolution_clock::now();
// duration = std::chrono::duration_cast<std::chrono::microseconds>(ed - st);
// std::cout<<">:"<<duration.count()<<"\n";


// st = std::chrono::high_resolution_clock::now();
// for(int i=0; i<10000000; i++) {
//     BNUM0 > BNUM2;
// }
// ed = std::chrono::high_resolution_clock::now();
// duration = std::chrono::duration_cast<std::chrono::microseconds>(ed - st);
// std::cout<<">:"<<duration.count()<<"\n";


// st = std::chrono::high_resolution_clock::now();
// for(int i=0; i<10000000; i++) {
//     BNUM0 == BNUM1;
// }
// ed = std::chrono::high_resolution_clock::now();
// duration = std::chrono::duration_cast<std::chrono::microseconds>(ed - st);
// std::cout<<"==:"<<duration.count()<<"\n";



st = std::chrono::high_resolution_clock::now();
for(int i=0; i<10000000; i++) {
    BNUM0.plus(BNUM1, resBNUM);
}
ed = std::chrono::high_resolution_clock::now();
duration = std::chrono::duration_cast<std::chrono::microseconds>(ed - st);
std::cout<<"+:"<<duration.count()<<"\n";


st = std::chrono::high_resolution_clock::now();
for(int i=0; i<10000000; i++){
    BNUM0.minus(BNUM1, resBNUM);
}
ed = std::chrono::high_resolution_clock::now();
duration = std::chrono::duration_cast<std::chrono::microseconds>(ed - st);
std::cout<<"-:"<<duration.count()<<"\n";


st = std::chrono::high_resolution_clock::now();
for(int i=0; i<10000000; i++) {
    BNUM0.multiply(BNUM1, resBNUM);
}
ed = std::chrono::high_resolution_clock::now();
duration = std::chrono::duration_cast<std::chrono::microseconds>(ed - st);
std::cout<<"*:"<<duration.count()<<"\n";


st = std::chrono::high_resolution_clock::now();
for(int i=0; i<100000; i++) {
    BNUM0.divideBy(BNUM1, resBNUM);
}
ed = std::chrono::high_resolution_clock::now();
duration = std::chrono::duration_cast<std::chrono::microseconds>(ed - st);
std::cout<<"/:"<<duration.count()<<"\n";


st = std::chrono::high_resolution_clock::now();
for(int i=0; i<100000; i++){
    BNUM0.modulus(BNUM1, resBNUM);
}
ed = std::chrono::high_resolution_clock::now();
duration = std::chrono::duration_cast<std::chrono::microseconds>(ed - st);
std::cout<<"%:"<<duration.count()<<"\n";


    // PAUSE
    return 0;
}