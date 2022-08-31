#include <string>

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

        void push(std::string key, int weight);
        NODE pop(int popIdx);
        NODE popMin();
        NODE popMax();
        NODE getMin();
        NODE getMax();
        void list(NODE *tmpHeap);
};

#endif