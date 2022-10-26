#include <string>
#include <iostream>
#include <vector>
#include <functional>

#include "systemOS.h"
#include "heap.h"

#ifndef LIB_SORTING_H_
#define LIB_SORTING_H_

template <typename T> bool increasing(T i0, T i1);
template <typename T> bool decreasing(T i0, T i1);

template <typename T> void quick_sort(T* start, int num, bool (*compare)(T i0, T i1));
template <typename T> void merge_sort(T* start, int num, bool (*compare)(T i0, T i1));
template <typename T> void heap_sort(T* start, int num, bool (*compare)(T i0, T i1));

// std::sort();

#endif