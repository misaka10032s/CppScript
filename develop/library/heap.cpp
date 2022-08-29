#include <cstring>
#include <vector>
#include <map>
using namespace std;

struct NODE {
    NODE(string key, int weight):
        key(key),
        weight(weight),
        children{nullptr, nullptr},
        childrenNum{0, 0},
        parent(nullptr)
        {}

    string key;
    int weight;
    NODE* children[2];
    int childrenNum[2];
    NODE *parent;
};

class HEAP{
    private:
        NODE rootHeap = *(new NODE("", 0));
        bool htype; // 0 = min, 1 = max
        int minmaxMultiply;

        int getChildrenLength(struct NODE checkingNode){
            int tmpHeapChildrenLength = 0;
            for(int i=0; i<2; i++){
                if(checkingNode.children[i] != nullptr && checkingNode.children[i]->key != "") tmpHeapChildrenLength++;
            }
            return tmpHeapChildrenLength;
        }

    public:
        HEAP(bool type = 0){
            htype = type;
            minmaxMultiply = type ? -1 : 1;
        }
        
        void push(string key, int weight){
            if(key == "") return;
            NODE nowVertexHeap = *(new NODE(key, weight));

            if(rootHeap.key == ""){
                rootHeap = *(new NODE(key, weight));
            }
            else{
                NODE* tmpHeap = &rootHeap;
                int chn[2], insertIdx, iTmp;
                string sTmp;

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

                while(tmpHeap->parent != nullptr && tmpHeap->weight * minmaxMultiply < tmpHeap->parent->weight * minmaxMultiply){
                    sTmp = tmpHeap->key;
                    tmpHeap->key = tmpHeap->parent->key;
                    tmpHeap->parent->key = sTmp;

                    iTmp = tmpHeap->weight;
                    tmpHeap->weight = tmpHeap->parent->weight;
                    tmpHeap->parent->weight = iTmp;
                    
                    tmpHeap = tmpHeap->parent;
                }
            }
        }

        NODE pop(){
            NODE rtn = *(new NODE(rootHeap.key, rootHeap.weight));
            if(rootHeap.key == "") return rtn;

            NODE* tmpHeap = &rootHeap;
            int extremeV, extremeIdx;

            while(getChildrenLength(*tmpHeap)){
                extremeV = 2147483647 * minmaxMultiply;
                for(int i=0; i<2; i++){
                    if(tmpHeap->children[i] != nullptr && tmpHeap->children[i]->weight * minmaxMultiply < extremeV * minmaxMultiply){
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

            return rtn;
        }

        NODE get(){
            return *(new NODE(rootHeap.key, rootHeap.weight));
        }

        void list(NODE *tmpHeap){
            if(tmpHeap == nullptr) tmpHeap = &rootHeap;
            cout<<"@ "<<tmpHeap->key<<"\n";
            for(int i=0; i<2; i++){
                if(tmpHeap->children[i] != nullptr) list(tmpHeap->children[i]);
            }
        }
};


class SMMH{
    private:
        NODE rootHeap = *(new NODE("", 0));
        bool htype; // 0 = min, 1 = max
        int minmaxMultiply;

        int getChildrenLength(struct NODE checkingNode){
            int tmpHeapChildrenLength = 0;
            for(int i=0; i<2; i++){
                if(checkingNode.children[i] != nullptr && checkingNode.children[i]->key != "") tmpHeapChildrenLength++;
            }
            return tmpHeapChildrenLength;
        }

        NODE* getValue(NODE *nodeList[2], int index){
            index = index ? 1 : 0;
            if(nodeList[index] == nullptr) return nodeList[!index];
            return nodeList[index];
        }

    public:
        SMMH(bool type = 0){
            htype = type;
            minmaxMultiply = type ? -1 : 1;
        }

        void push(string key, int weight){
            if(key == "") return;

            NODE* tmpHeap = &rootHeap;
            int chn[2], insertIdx, iTmp;
            string sTmp;

            while(getChildrenLength(*tmpHeap) == 2){
                if(weight < tmpHeap->children[0]->weight){
                    insertIdx = 0;
                    iTmp = tmpHeap->children[0]->weight;
                    tmpHeap->children[0]->weight = weight;
                    weight = iTmp;

                    sTmp = tmpHeap->children[0]->key;
                    tmpHeap->children[0]->key = key;
                    key = sTmp;
                }
                else if(weight < tmpHeap->children[1]->weight){
                    insertIdx = tmpHeap->childrenNum[0] <= tmpHeap->childrenNum[1] ? 0 : 1;
                }
                else{
                    insertIdx = 1;
                    iTmp = tmpHeap->children[1]->weight;
                    tmpHeap->children[1]->weight = weight;
                    weight = iTmp;

                    sTmp = tmpHeap->children[1]->key;
                    tmpHeap->children[1]->key = key;
                    key = sTmp;
                }
                tmpHeap->childrenNum[insertIdx]++;
                tmpHeap = tmpHeap->children[insertIdx];
            }

            for(int i=0; i<2; i++){
                if(tmpHeap->children[i] == nullptr){
                    tmpHeap->children[i] = new NODE(key, weight);
                    tmpHeap->childrenNum[i]++;
                    break;
                }
            }

            if(getChildrenLength(*tmpHeap) == 2){
                if(tmpHeap->children[0]->weight > tmpHeap->children[1]->weight){
                    iTmp = tmpHeap->children[0]->weight;
                    tmpHeap->children[0]->weight = tmpHeap->children[1]->weight;
                    tmpHeap->children[1]->weight = iTmp;

                    sTmp = tmpHeap->children[0]->key;
                    tmpHeap->children[0]->key = tmpHeap->children[1]->key;
                    tmpHeap->children[1]->key = sTmp;
                }
            }
        }

        NODE pop(int popIdx){
            if(!getChildrenLength(rootHeap)) return *(new NODE("", 0));
            popIdx = popIdx ? 1 : 0;
            int minmaxMultiply = popIdx ? -1 : 1;

            NODE *tmpHeap = &rootHeap, *leftNode, *rightNode, *leftNodeChild, *rightNodeChild, *tmpNodeChild;
            NODE rtn = *getValue(rootHeap.children, popIdx);
            int chn[2], insertIdx, iTmp;
            string sTmp;

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

        NODE popMin(){
            return pop(0);
        }

        NODE popMax(){
            return pop(1);
        }

        NODE getMin(){
            if(!getChildrenLength(rootHeap)) return *(new NODE("", 0));
            return *(new NODE(rootHeap.children[0]->key, rootHeap.children[0]->weight));
        }

        NODE getMax(){
            if(!getChildrenLength(rootHeap)) return *(new NODE("", 0));
            NODE rtn = *getValue(rootHeap.children, 1);
            return *(new NODE(rtn.key, rtn.weight));
        }

        void list(NODE *tmpHeap){
            if(tmpHeap == nullptr) tmpHeap = &rootHeap;
            cout<<"@ "<<tmpHeap->key<<"\n";
            for(int i=0; i<2; i++){
                if(tmpHeap->children[i] != nullptr) list(tmpHeap->children[i]);
            }
        }
};