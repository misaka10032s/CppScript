#include <string>
#include <iostream>
#include <vector>
#include <functional>
#include <math>

#include "systemOS.h"
#include "heap.h"

template <typename T> bool increasing(T i0, T i1){
    return i0 < i1;
};
template <typename T> bool decreasing(T i0, T i1){
    return i0 > i1;
};

template <typename T> void quick_sort(T* start, int num, bool (*compare)(T i0, T i1)){
    if(num <= 1) return;
    T datum = *start, tmp;
    int last = 0;
    for(int i=1; i<num - last; i++){
        if(!compare(datum, *(start + i))){
            last++;
            tmp = *(start + i);
            *(start + i) = *(start + num - last);
            *(start + num - last) = tmp;
        }
    }
    tmp = *(start + num - last);
    *(start + num - last) = *start;
    *start = tmp;

    quick_sort<T>(start, num - last - 1, compare);
    quick_sort<T>(start + num - last + 1, last - 1, compare);
};

// (0,1) (2,3) (4,5) (6,7)
// (01,23) (45,67)
// (0123, 4567)

template <typename T> void merge_sort(T* start, int num, bool (*compare)(T i0, T i1)){
    int range = ceil(log2(num)), iterA, iterB, loopTime, compareTme;
    T *mirror = (T*)malloc(sizeof(T) * num), *tmp;
    for(int i=0; i<range; i++){
        loopTime = num/i/2;
        for(int j=0; j<loopTime; j++){
            iterA = j<<i;
            iterB = (j+1)<<i;
            compareTme = 1<<(i+1);
            for(int k=0; k<compareTme; k++){
                if(compare(*(start + iterA), *(start + iterB))){
                    *(mirror + k) = *(start + iterA);
                    iterA++;
                }
                else{
                    *(mirror + k) = *(start + iterB);
                    iterB++;
                }
            }
        }
        tmp = start;
        start = mirror;
        mirror = tmp;
    }
    if(range%2) for(int i=0; i<num; i++) *(start + i) = *(mirror + i);
    free(range%2 ? start : mirror);
};
template <typename T> void heap_sort(T* start, int num, bool (*compare)(T i0, T i1)){
    listHEAP<T> LH(compare);
    for(int i=0; i<num; i++) LH.push(*(start + i));
    for(int i=0; i<num; i++) *(start + i) = LH.pop();
};