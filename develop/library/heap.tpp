#include <functional>
#include <iostream>

// ######################## class listHEAP ########################

template <typename T> listHEAP<T>::listHEAP(bool (*cp)(T i0, T i1)){
    // std::cout<<(typeid(T) == typeid(int))<<"\n";
    this->compare = std::bind(cp, std::placeholders::_1, std::placeholders::_2);
};
template <typename T> listHEAP<T>::~listHEAP(){
}

template <typename T> int listHEAP<T>::getChild(int idx, bool lr){
    return std::min((idx+1)*2 - lr, (int)this->_listheap.size());
};
template <typename T> int listHEAP<T>::getParent(int idx){
    return (idx+1)/2 - 1;
};

template <typename T> void listHEAP<T>::push(T item){
    int chIdx = this->_listheap.size(), parIdx;
    T tmpEle;

    this->_listheap.push_back(item);
    parIdx = this->getParent(chIdx);

    while(parIdx >= 0){
        if(this->compare(_listheap[chIdx], _listheap[parIdx])){ // if _listheap[chIdx] < _listheap[parIdx] return 1 =>> min heap
            tmpEle = this->_listheap[chIdx];
            this->_listheap[chIdx] = this->_listheap[parIdx];
            this->_listheap[parIdx] = tmpEle;
        }
        else{
            break;
        }
        chIdx = parIdx;
        parIdx = this->getParent(chIdx);
    }
};

template <typename T> T listHEAP<T>::pop(){
    T rtn, tmpEle;
    int parIdx = 0, ch0, ch1, chIdx;

    if(this->_listheap.size()) rtn = this->_listheap[0];
    else return rtn;

    tmpEle = this->_listheap[0];
    this->_listheap[0] = this->_listheap[_listheap.size()-1];
    this->_listheap[_listheap.size()-1] = tmpEle;
    _listheap.pop_back();

    while(this->getChild(parIdx, 1) < this->_listheap.size()){
        ch0 = this->getChild(parIdx, 0);
        ch1 = this->getChild(parIdx, 1);

        if(ch0 >= _listheap.size()) ch0 = ch1;
        chIdx = this->compare(_listheap[ch0], _listheap[ch1]) ? ch0 : ch1;

        if(this->compare(_listheap[parIdx], _listheap[chIdx])) break;
        
        tmpEle = this->_listheap[chIdx];
        this->_listheap[chIdx] = this->_listheap[parIdx];
        this->_listheap[parIdx] = tmpEle;

        parIdx = chIdx;
    }
    return rtn;
};

template <typename T> void listHEAP<T>::list(){
    for(int i=0; i<_listheap.size(); i++){
        std::cout<<_listheap[i]<<",\n";
    }
};






template <typename T> STACK<T>::STACK(int num){
    this->idx = 0;
    this->_STACK.resize(num);
};

template <typename T> T STACK<T>::pop(){
    T rtn;
    if(this->idx > 0) {
        this->idx--;
        return this->_STACK[this->idx];
    }
    else return rtn;
};
template <typename T> bool STACK<T>::push(T item){
    if(this->idx < this->_STACK.size()) {
        this->_STACK[this->idx] = item;
        this->idx++;
        return 0;
    }
    else return 1;
};
template <typename T> void STACK<T>::list(){
    for(int i=0; i<this->idx; i++){
        std::cout<<_STACK[i]<<",\n";
    }
};





template <typename T> QUEUE<T>::QUEUE(int num, bool type){
    this->_popIdx = 0;
    this->_pushIdx = 0;
    this->_type = type;
    this->_QUEUE.resize(num);
};
template <typename T> QUEUE<T>::QUEUE(int num){
    this->_popIdx = 0;
    this->_pushIdx = 0;
    this->_type = 0;
    this->_QUEUE.resize(num);
};

template <typename T> T QUEUE<T>::pop(){
    T rtn;
    int length = this->_QUEUE.size();
    if(this->_popIdx != this->_pushIdx) {
        this->_popIdx = (length + this->_popIdx - 1) % length;
        return this->_QUEUE[this->idx];
    }
    else return rtn;
};
template <typename T> bool QUEUE<T>::push(T item){
    int length = this->_QUEUE.size(), next = (length + this->_pushIdx + 1) % length;
    if(next != this->_popIdx || this->type) {
        this->_QUEUE[this->idx] = item;
        this->_pushIdx = next;
        return 0;
    }
    else return 1;
};
template <typename T> void QUEUE<T>::list(){
    int length = this->_QUEUE.size();
    for(int i=0; i<this->idx; i++){
        std::cout<<_QUEUE[(this->_popIdx + i) % length]<<",\n";
    }
};