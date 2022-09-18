#include <string>
#include <iostream>
#include <cmath>
#include <functional>

#include "heap.h"
using namespace std;

// ######################## struct NODE ########################
NODE::NODE(){
    this->key = "";
    this->weight = 0;
    for(int i=0; i<2; i++){
        this->children[i] = nullptr;
        this->childrenNum[i] = 0;
    }
    this->parent = nullptr;
}
NODE::NODE(std::string key, int weight){
    this->key = key;
    this->weight = weight;
    for(int i=0; i<2; i++){
        this->children[i] = nullptr;
        this->childrenNum[i] = 0;
    }
    this->parent = nullptr;
}

// NODE operation methods
void swapNODE(NODE *N0, NODE *N1){
    int iTmp;
    std::string sTmp;

    iTmp = N0->weight;
    N0->weight = N1->weight;
    N1->weight = iTmp;

    sTmp = N0->key;
    N0->key = N1->key;
    N1->key = sTmp;
}

int getChildrenLength(NODE checkingNode){
    int tmpHeapChildrenLength = 0;
    for(int i=0; i<2; i++){
        if(checkingNode.children[i] != nullptr && checkingNode.children[i]->key != "") tmpHeapChildrenLength++;
    }
    return tmpHeapChildrenLength;
}


// ######################## class HEAP ########################
HEAP::HEAP(bool type){
    this->htype = type;
    this->minmaxMultiply = type ? -1 : 1;
}
HEAP::HEAP(){
    this->htype = 0;
    this->minmaxMultiply = 1;
}
HEAP::~HEAP(){
    if(this->rootHeap != nullptr) delete this->rootHeap;
}

void HEAP::push(int key, int weight){
    this->push(std::to_string(key), weight);
}

void HEAP::push(std::string key, int weight){
    if(key == "") return;

    if(this->rootHeap->key == ""){
        this->rootHeap = new NODE(key, weight);
    }
    else{
        NODE* tmpHeap = this->rootHeap;
        int chn[2], insertIdx;

        while(getChildrenLength(*tmpHeap) == 2){
            chn[0] = tmpHeap->childrenNum[0];
            chn[1] = tmpHeap->childrenNum[1];
            insertIdx = chn[0] <= chn[1] ? 0 : 1;
            tmpHeap->childrenNum[insertIdx]++;
            tmpHeap = tmpHeap->children[insertIdx];
        }

        for(int i=0; i<2; i++){
            if(tmpHeap->children[i] == nullptr){
                tmpHeap->children[i] = new NODE(key, weight);
                tmpHeap->children[i]->parent = tmpHeap;
                tmpHeap->childrenNum[i]++;
                insertIdx = i;
                break;
            }
        }
        tmpHeap = tmpHeap->children[insertIdx];

        while(tmpHeap->parent != nullptr && tmpHeap->weight * this->minmaxMultiply < tmpHeap->parent->weight * this->minmaxMultiply){
            swapNODE(tmpHeap, tmpHeap->parent);
            tmpHeap = tmpHeap->parent;
        }
    }
}

NODE HEAP::pop(){
    NODE rtn = *(new NODE(this->rootHeap->key, this->rootHeap->weight));
    if(this->rootHeap->key == "") return rtn;

    NODE* tmpHeap = this->rootHeap;
    int extremeV, extremeIdx = -1;

    while(getChildrenLength(*tmpHeap)){
        extremeV = 2147483647 * this->minmaxMultiply;
        for(int i=0; i<2; i++){
            if(tmpHeap->children[i] != nullptr && tmpHeap->children[i]->weight * this->minmaxMultiply < extremeV * this->minmaxMultiply){
                extremeV = tmpHeap->children[i]->weight;
                extremeIdx = i;
            }
        }

        tmpHeap->key = tmpHeap->children[extremeIdx]->key;
        tmpHeap->weight = tmpHeap->children[extremeIdx]->weight;
        tmpHeap->childrenNum[extremeIdx]--;

        tmpHeap = tmpHeap->children[extremeIdx];
    }

    if(tmpHeap->parent != nullptr) {
        free(tmpHeap->parent->children[extremeIdx]);
        tmpHeap->parent->children[extremeIdx] = nullptr;
    }

    if(extremeIdx == -1){
        tmpHeap->key = "";
        tmpHeap->weight = 0;
    }

    return rtn;
}
NODE HEAP::get(){
    return *(new NODE(this->rootHeap->key, this->rootHeap->weight));
}

void HEAP::list(NODE *tmpHeap){
    if(tmpHeap == nullptr) tmpHeap = this->rootHeap;
    cout<<"@ "<<tmpHeap->key<<"\n";
    for(int i=0; i<2; i++){
        if(tmpHeap->children[i] != nullptr) list(tmpHeap->children[i]);
    }
}


// ######################## class SMMH ########################
SMMH::SMMH(){}
SMMH::~SMMH(){
    if(this->rootHeap != nullptr) delete this->rootHeap;
}

NODE* SMMH::getValue(NODE *nodeList[2], int index){
    index = index ? 1 : 0;
    if(nodeList[index] == nullptr) return nodeList[!index];
    return nodeList[index];
}

void SMMH::push(int key, int weight){
    this->push(std::to_string(key), weight);
}

void SMMH::push(std::string key, int weight){
    if(key == "") return;

    NODE* tmpHeap = this->rootHeap;
    int chn[2], insertIdx;

    while(getChildrenLength(*tmpHeap) == 2){
        chn[0] = tmpHeap->childrenNum[0];
        chn[1] = tmpHeap->childrenNum[1];
        insertIdx = chn[0] <= chn[1] ? 0 : 1;
        tmpHeap->childrenNum[insertIdx]++;
        tmpHeap = tmpHeap->children[insertIdx];
    }

    for(int i=0; i<2; i++){
        if(tmpHeap->children[i] == nullptr){
            tmpHeap->children[i] = new NODE(key, weight);
            tmpHeap->children[i]->parent = tmpHeap;
            tmpHeap->childrenNum[i]++;
            insertIdx = i;
            break;
        }
    }
    tmpHeap = tmpHeap->children[insertIdx];

    if(getChildrenLength(*tmpHeap->parent) == 2 && tmpHeap->weight * pow(-1, insertIdx) > tmpHeap->parent->children[insertIdx?0:1]->weight * pow(-1, insertIdx)){
        swapNODE(tmpHeap->parent->children[0], tmpHeap->parent->children[1]);
        tmpHeap = tmpHeap->parent->children[insertIdx?0:1];
    }

    while(tmpHeap->parent->parent != nullptr && (tmpHeap->weight < tmpHeap->parent->parent->children[0]->weight || (tmpHeap->parent->parent->children[1] != nullptr && tmpHeap->weight > tmpHeap->parent->parent->children[1]->weight))){
        if(tmpHeap->weight < tmpHeap->parent->parent->children[0]->weight){
            swapNODE(tmpHeap, tmpHeap->parent->parent->children[0]);
            tmpHeap = tmpHeap->parent->parent->children[0];
        }
        else{
            swapNODE(tmpHeap, tmpHeap->parent->parent->children[1]);
            tmpHeap = tmpHeap->parent->parent->children[1];
        }
    }
}

NODE SMMH::pop(int popIdx){
    if(!getChildrenLength(*rootHeap)) return *(new NODE("", 0));
    popIdx = popIdx ? 1 : 0;
    int minmaxMultiply = popIdx ? -1 : 1;

    NODE *tmpHeap = rootHeap, *leftNode, *rightNode, *leftNodeChild, *rightNodeChild, *tmpNodeChild;
    NODE rtn = *getValue(rootHeap->children, popIdx);

    rtn = *(new NODE(rtn.key, rtn.weight));
    leftNode = tmpHeap->children[0];
    rightNode = tmpHeap->children[1];

    while((leftNode && getChildrenLength(*leftNode)) || (rightNode && getChildrenLength(*rightNode))){
        leftNodeChild = getValue(leftNode->children, popIdx);
        rightNodeChild = getValue(rightNode->children, popIdx);
        tmpNodeChild =  getValue(tmpHeap->children, popIdx);

        if(!leftNodeChild || (rightNodeChild && minmaxMultiply * leftNodeChild->weight >= minmaxMultiply * rightNodeChild->weight)){
            tmpNodeChild->key = rightNodeChild->key;
            tmpNodeChild->weight = rightNodeChild->weight;
            tmpHeap->childrenNum[1]--;
            tmpHeap = rightNode;
        }

        else if(!rightNodeChild || (leftNodeChild && minmaxMultiply * leftNodeChild->weight < minmaxMultiply * rightNodeChild->weight)){
            tmpNodeChild->key = leftNodeChild->key;
            tmpNodeChild->weight = leftNodeChild->weight;
            tmpHeap->childrenNum[0]--;
            tmpHeap = leftNode;
        }

        leftNode = tmpHeap->children[0];
        rightNode = tmpHeap->children[1];
    }

    free(tmpHeap->children[popIdx]);
    tmpHeap->children[popIdx] = nullptr;
    tmpHeap->childrenNum[popIdx] = 0;

    if(!popIdx && getChildrenLength(*tmpHeap) == 1){
        tmpHeap->children[0] = tmpHeap->children[1];
        tmpHeap->childrenNum[0] = tmpHeap->childrenNum[1];

        tmpHeap->children[1] = nullptr;
        tmpHeap->childrenNum[1] = 0;
    }

    return rtn;
}

NODE SMMH::popMin(){
    return this->pop(0);
}

NODE SMMH::popMax(){
    return this->pop(1);
}

NODE SMMH::getMin(){
    if(!getChildrenLength(*rootHeap)) return *(new NODE("", 0));
    return *(new NODE(rootHeap->children[0]->key, rootHeap->children[0]->weight));
}

NODE SMMH::getMax(){
    if(!getChildrenLength(*rootHeap)) return *(new NODE("", 0));
    NODE rtn = *getValue(rootHeap->children, 1);
    return *(new NODE(rtn.key, rtn.weight));
}

void SMMH::list(NODE *tmpHeap){
    if(tmpHeap == nullptr) tmpHeap = rootHeap;
    std::cout<<"@ "<<tmpHeap->key<<"\n";
    for(int i=0; i<2; i++){
        if(tmpHeap->children[i] != nullptr) list(tmpHeap->children[i]);
    }
}