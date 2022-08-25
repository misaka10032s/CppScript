#include <cstring>
#include <vector>
#include <map>
using namespace std;

struct node {
    string key;
    int weight;
    struct node* children[2];
    int childrenNum[2];
    struct node* parent;
};

class minHeap{
    private:
        struct node *rootHeap = {
            (string)NULL,
            0,
            {nullptr, nullptr},
            {0, 0},
            nullptr,
        };
        bool type; // 0 = min, 1 = max
        int minmaxMultiply;

        int getChildrenLength(struct node checkingNode){
            int tmpHeapChildrenLength = 2;
            for(int i=0; i<2; i++){
                if(checkingNode.children[i] == nullptr) tmpHeapChildrenLength--;
            }
            return tmpHeapChildrenLength;
        }

    public:
        minHeap(bool type = 0){
            minmaxMultiply = type ? 1 : -1;
        }
        
        struct node push(string key, int weight){
            struct node* nowVertexHeap = {
                key,
                weight,
                {nullptr, nullptr},
                {0, 0},
                nullptr,
            };
            if((*rootHeap).key == (string)NULL){
                rootHeap = nowVertexHeap;
            }
            else{
                struct node *tmpHeap = rootHeap;
                int chn[2], insertIdx, iTmp;
                string sTmp;

                while(getChildrenLength(*tmpHeap) == 2){
                    chn[0] = (*tmpHeap).childrenNum[0];
                    chn[1] = (*tmpHeap).childrenNum[1];
                    insertIdx = chn[0] <= chn[1] ? 0 : 1;
                    chn[insertIdx]++;
                    tmpHeap = (*tmpHeap).children[insertIdx];
                }

                for(int i=0; i<2; i++){
                    if((*tmpHeap).children[i] == nullptr){
                        (*tmpHeap).children[i] = nowVertexHeap;
                        (*tmpHeap).childrenNum[i]++;
                        (*nowVertexHeap).parent = tmpHeap;
                        break;
                    }
                }

                while((*nowVertexHeap).parent != nullptr && (*nowVertexHeap).weight * minmaxMultiply < (*(*nowVertexHeap).parent).weight * minmaxMultiply){
                    sTmp = (*nowVertexHeap).key;
                    (*nowVertexHeap).key = (*(*nowVertexHeap).parent).key;
                    (*(*nowVertexHeap).parent).key = sTmp;

                    iTmp = (*nowVertexHeap).weight;
                    (*nowVertexHeap).weight = (*(*nowVertexHeap).parent).weight;
                    (*(*nowVertexHeap).parent).weight = iTmp;
                    
                    nowVertexHeap = (*nowVertexHeap).parent;
                }
            }
        }

        struct node pop(string key, int weight){

        }

        struct node get(string key, int weight){
            struct node *rtn = rootHeap;
            return *rtn;
        }

};
class SMMH{
    
};