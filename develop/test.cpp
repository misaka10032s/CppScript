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

#include "library/heap.h"
#include "library/graph.h"

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

    // system("pause");
    return 0;
}