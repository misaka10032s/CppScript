#include <string>
#include <iostream>
#include <vector>
#include <functional>

#ifndef LIB_HEAP_H_
#define LIB_HEAP_H_

struct NODE {
    NODE(std::string key, int weight);
    NODE();

    std::string key;
    int weight;
    NODE* children[2];
    int childrenNum[2];
    NODE* parent;
};

void swapNODE(NODE *N0, NODE *N1);
int getChildrenLength(NODE checkingNode);

class HEAP{
    private:
        bool htype; // 0 = min, 1 = max
        int minmaxMultiply;
        NODE *rootHeap = new NODE("", 0);

    public:
        HEAP(bool type);
        HEAP();
        ~HEAP();

        void push(int key, int weight);
        void push(std::string key, int weight);
        NODE pop();
        NODE get();
        void list(NODE *tmpHeap);
};


class SMMH{
    private:
        NODE* getValue(NODE *nodeList[2], int index);
        NODE *rootHeap = new NODE("", 0);

    public:
        SMMH();
        ~SMMH();

        void push(int, int weight);
        void push(std::string key, int weight);
        NODE pop(int popIdx);
        NODE popMin();
        NODE popMax();
        NODE getMin();
        NODE getMax();
        void list(NODE *tmpHeap);
};

template <typename T> class listHEAP{
    private:
        std::vector<T> _listheap;
        int getChild(int idx, bool lr);
        int getParent(int idx);
        std::function<bool(int, int)> compare;
        // bool *compare(int i0, int i1);

    public:
        listHEAP(bool (*cp)(T i0, T i1));
        ~listHEAP();
        void push(T item);
        T pop();
        void list();
};

template <typename T> class STACK{
    private:
        int _idx;
        std::vector<T> _STACK;
    public:
        STACK(int num);

        bool canPop;

        T pop();
        bool push(T item);
        void reset();
        void list();
};


template <typename T> class QUEUE{
    private:
        bool _type;
        int _popIdx, _pushIdx;
        std::vector<T> _QUEUE;
    public:
        QUEUE(int num, bool type);
        QUEUE(int num);

        bool canPop;

        T pop();
        bool push(T item);
        void reset();
        void list();
};

#include "heap.tpp"
#endif