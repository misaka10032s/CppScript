#include <vector>
#include <string>

#include "systemOS.h"
#include "sudoku.h"





SUDOKU::SUDOKU(){
    for(int i=0; i<9; i++) for(int j=0; j<9;j++) this->map[i][j]=0;
};
SUDOKU::SUDOKU(char map[9][9]){
    for(int i=0; i<9; i++) {
        for(int j=0; j<9;j++) {
            if(map[i][j]<0 || map[i][j]>9) this->map[i][j] = 0;
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

SUDOKU SUDOKU::solve(){
    for(int i=0; i<9; i++) for(int j=0; j<9;j++) if(this->map[i][j]<0 || this->map[i][j]>9) return *this;
};
void SUDOKU::print(){
    for(int i=0; i<9; i++){
        for(int j=0; j<9;j++){
            std::cout << this->map[i][j] ;
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