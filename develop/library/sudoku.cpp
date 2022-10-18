#include <vector>
#include <string>

#include "systemOS.h"
#include "sudoku.h"


int quick_pow2L[] = {1,2,4,8,16,32,64,128,256,512,1024,2048,4096,8192,16384,32768,65536,131072,262144,524288,1048576,2097152,4194304,8388608,16777216,33554432,67108864,134217728,268435456,536870912,1073741824};
inline int quick_pow2(int x){
    if(x>=0 && x<31) return quick_pow2L[x];
    else return -1;
}

SUDOKU::SUDOKU(){
    for(int i=0; i<9; i++) for(int j=0; j<9;j++) this->map[i][j] = 0;
};
SUDOKU::SUDOKU(char map[9][9]){
    for(int i=0; i<9; i++) {
        for(int j=0; j<9;j++) {
            if(map[i][j]<1 || map[i][j]>9) this->map[i][j] = 0;
            else this->map[i][j] = map[i][j];
        }
    }
};
SUDOKU::SUDOKU(std::initializer_list<std::initializer_list<char>> map){
    for(int i=0; i<std::min((int)map.size(), 9); i++) {
        for(int j=0; j<std::min((int)(map.begin()+i)->size(), 9);j++) {
            if(*((map.begin()+i)->begin()+j)<0 || *((map.begin()+i)->begin()+j)>9) this->map[i][j] = 0;
            else this->map[i][j] = *((map.begin()+i)->begin()+j);
        }
    }
};

int SUDOKU::write(int &map[9][9], int &numberLeft[9][9]){
    int updateN = 0, filledNum = 0, tmp;
    for(int i=0; i<9; i++) {
        for(int j=0; j<9;j++){
            if(this->numberLeft[i][j] == 0){
                return -2;
            }
            if(this->map[i][j] != 0){
                filledNum++;
                continue;
            }

            tmp = numberLeft[i][j];
            for(int l=0; l<9; l++){
                if(this->map[i][l] != 0){
                    numberLeft[i][j] &= (quick_pow2(10) - 2 - quick_pow2(this->map[i][l]));
                }
                if(this->map[l][j] != 0){
                    numberLeft[i][j] &= (quick_pow2(10) - 2 - quick_pow2(this->map[l][j]));
                }
                if(this->map[(i/3)*3 + l/3][(j/3)*3 + l%3] != 0){
                    numberLeft[i][j] &= (quick_pow2(10) - 2 - quick_pow2(this->map[(i/3)*3 + l/3][(j/3)*3 + l%3]));
                }
            }
            if(tmp != numberLeft[i][j]) updateN++;

            for(int k=1; k<10; k++){
                if(numberLeft[i][j] == quick_pow2(k)){
                    this->map[i][j] = k;
                    updateN++;
                    filledNum++;
                    break;
                }
            }
        }
    }

    if(filledNum == 81) return -1;
    return updateN;
}
bool SUDOKU::suppose(int map[9][9], int numberLeft[9][9]){
    int res, si = rand()%9, sj = rand()%9, origNF[9][9], origMap[9][9];
    do{
        sj = (sj+1)%9;
        if(!sj) si = (si+1)%9;
    }
    while(map[si][sj]);

    origNF = numberLeft[si][sj];
    for(int i=0; i<9; i++) {
        for(int j=0; j<9;j++) {
            origMap[i][j] = map[i][j];
            origNF[i][j] = numberLeft[i][i];
        }
    }

    for(int sv=1; sv<10; sv++){
        if(!(numberLeft[si][sj] & quick_pow2(sv))) continue;
        for(int i=0; i<9; i++) {
            for(int j=0; j<9;j++) {
                map[i][j] = origMap[i][j];
                numberLeft[i][j] = origNF[i][i];
            }
        }
        map[si][sj] = sv;
        while(1){
            res = this->write(map, numberLeft);
            if(!res) this->suppose(map, numberLeft);
            else if(res == -1){
                for(int i=0; i<9; i++) for(int j=0; j<9;j++) this->map[i][j] = map[i][j];
                this->status = 1;
                return 1;
            }
            else if(res == -2) break;
        };
    }
    this->status = 2;
    return 0;
}

SUDOKU SUDOKU::solve(){
    for(int i=0; i<9; i++) for(int j=0; j<9;j++) if(this->map[i][j]<0 || this->map[i][j]>9) return *this;
    int numberLeft[9][9] = {0}, res;
    for(int i=0; i<9; i++) {
        for(int j=0; j<9;j++) {
            if(this->map[i][j] == 0) numberLeft[i][j] = quick_pow2(10) - 2;
            else numberLeft[i][j] = quick_pow2(this->map[i][j]);
        }
    }
    while(1){
        res = this->write(this->map, numberLeft);
        if(!res) this->suppose(this->map, numberLeft);
        else if(res < 0) {
            this->status = -res;
            break;
        }
    };
    return *this;
};
void SUDOKU::print(){
    switch(this->status){
        case 1:
            std::cout << "找到一組解\n";
            break;
        case 2:
            std::cout << "無解\n";
            break;
    }
    for(int i=0; i<9; i++){
        for(int j=0; j<9;j++){
            std::cout << this->map[i][j] << "\t";
        }
        std::cout << "\n";
    }
};





NONOGRAM::NONOGRAM(int width, int height, std::vector<std::vector<char>> hintV, std::vector<std::vector<char>> hintH){};

NONOGRAM NONOGRAM::solve(){
    if((int)this->hintV.size() > this->width || (int)this->hintH.size() > this->height) return *this;
};
void NONOGRAM::print(){
    for(int j=0; j<this->height;j++){
        for(int i=0; i<this->width; i++){
            std::cout << this->map[i + j*this->width];
        }
        std::cout << "\n";
    }
};