#include <vector>
#include <string>

#ifndef LIB_SUDOKU_H_
#define LIB_SUDOKU_H_

class SUDOKU{
    private:
        char map[9][9];
    public:

        SUDOKU();
        SUDOKU(char map[9][9]);
        SUDOKU(std::initializer_list<std::initializer_list<char>> map);

        SUDOKU solve();
        void print();
}

class NONOGRAM{
    private:
        std::vector<char> map;
        int width, height;
        std::vector<std::vector<char>> hintV, hintH;

    public:

        NONOGRAM(int width, int height, std::vector<std::vector<char>> hintV, std::vector<std::vector<char>> hintH);

        NONOGRAM solve();
        void print();
}
#endif