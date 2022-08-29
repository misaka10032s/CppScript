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

#include "library/heap.cpp"

using namespace std;

int* test(int *i[2]){
    cout<<(*i)[0]<<", "<<(*i)[1]<<"\n";
    (*i)[0]++;
    (*i)[1] *= 10;
    cout<<(*i)[0]<<", "<<(*i)[1]<<"\n";
    return *i;
}

int main(){
    int x=1, y=1;
    int* a[2] = {&x, &y};
    test(a)[0] *= 7;
    cout<<(*a)[0]<<", "<<(*a)[1]<<"\n";

    // HEAP myHeap(1);
    SMMH myHeap(1);
    myHeap.push((string)"A", 5);
    myHeap.push((string)"B", 8);
    myHeap.push((string)"C", 4);
    myHeap.push((string)"D", 10);
    myHeap.push((string)"E", 6);
    myHeap.push((string)"F", 7);
    myHeap.push((string)"G", 5);

    myHeap.list(nullptr);cout<<"------------------\n";

    // CAGEFBD
    for(int i=0; i<7; i++){
        cout<< myHeap.popMin().key<<"\n";
    }
    // system("pause");
    return 0;
}