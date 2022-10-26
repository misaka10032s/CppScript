#include <string>
#include <iostream>
#include <vector>
#include <functional>

#include "systemOS.h"
#include "heap.h"

template <typename T> bool increasing(T i0, T i1){
    return i0 < i1;
};
template <typename T> bool decreasing(T i0, T i1){
    return i0 > i1;
};

template <typename T> void quick_sort(T* start, int num, bool (*compare)(T i0, T i1)){
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
template <typename T> void merge_sort(T* start, int num, bool (*compare)(T i0, T i1)){};
template <typename T> void heap_sort(T* start, int num, bool (*compare)(T i0, T i1)){};