#ifndef SUDOKU_H
#define SUDOKU_H

#define GRIDNUM 9
#include<QList>
#include<QSet>
#include<QtGlobal>
#include<QTime>
#include"enum.h"
#include<iostream>
using namespace std;
class Sudoku
{
public:
    Sudoku();
    Sudoku(int m[GRIDNUM][GRIDNUM]);
    ~Sudoku();
     //判断给某行某列赋值是否符合规则
    bool check(int row, int column, int number);
    //回溯求解
    bool backTrace(int row, int column);
    //获取数独矩阵
    void getMatrix(int a_matrix[GRIDNUM][GRIDNUM]);
    //生成一个包含1-9的长度为9的数组
    QSet<int> createRandomLineArray();
    //随机生成一个标准数独
    void createRandomSudokuArray(int matrix[GRIDNUM][GRIDNUM]);
    //随机对数独矩阵中元素置零，生成游戏矩阵
    void createRandomSudokuGame(int matrix[GRIDNUM][GRIDNUM],Difficulty difficulty=easy);
private:
    int matrix[GRIDNUM][GRIDNUM];
};

#endif // SUDOKU_H
