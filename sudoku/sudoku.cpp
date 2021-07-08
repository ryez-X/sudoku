#include "sudoku.h"

Sudoku::Sudoku()
{
    int array[GRIDNUM][GRIDNUM]={{9, 7, 8, 3, 1, 2, 6, 4, 5},
                                 {3, 1, 2, 6, 4, 5, 9, 7, 8},
                                 {6, 4, 5, 9, 7, 8, 3, 1, 2},
                                 {7, 8, 9, 1, 2, 3, 4, 5, 6},
                                 {1, 2, 3, 4, 5, 6, 7, 8, 9},
                                 {4, 5, 6, 7, 8, 9, 1, 2, 3},
                                 {8, 9, 7, 2, 3, 1, 5, 6, 4},
                                 {2, 3, 1, 5, 6, 4, 8, 9, 7},
                                 {5, 6, 4, 8, 9, 7, 2, 3, 1}};
    for(int i=0;i<GRIDNUM;i++)
        for(int j=0;j<GRIDNUM;j++)
            matrix[i][j]=array[i][j];
}
Sudoku::Sudoku(int m[GRIDNUM][GRIDNUM])
{
    for(int i=0;i<GRIDNUM;i++)
        for(int j=0;j<GRIDNUM;j++)
            matrix[i][j]=m[i][j];
}

Sudoku::~Sudoku()
{

}

bool Sudoku::check(int row, int column, int number)
{
    //@brief 判断给某行某列赋值是否符合规则
    //判断该行该列是否有重复数字
    for (int k = 0; k < GRIDNUM; k++)
    {
        if (matrix[row][k] == number || matrix[k][column] == number)
            return false;
    }
    //判断小九宫格是否有重复
    int tmpRow = row/3 *3;
    int tmpColumne = column/3*3;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (matrix[tmpRow+i][tmpColumne+j] == number)
            {
                return false;
            }
        }
    }
    return true;
}

bool Sudoku::backTrace(int row, int column)
{
    //@backTrace 回溯求解
    if (row == 8 && column == GRIDNUM)
    {
        //成功求解
        return true;
    }

    //已经到了列末尾了，还没到行尾，就换行
    if (column == GRIDNUM)
    {
        row++;
        column = 0;
    }

    //如果i行j列是空格，那么才进入给空格填值的逻辑
    if (matrix[row][column] == 0)
    {
        for (int key = 1; key <= GRIDNUM; key++)
        {
            //判断给i行j列放1-GRIDNUM中的任意一个数是否能满足规则
            if (check(row, column, key))
            {
                //将该值赋给该空格，然后进入下一个空格
                matrix[row][column] = key;
                if (backTrace(row, column + 1))return true;

            }
        }
        //初始化该空格
        matrix[row][column] = 0;
        //返回上一层继续求解
        return false;
    }
    else
    {
        //如果该位置已经有值了，就进入下一个空格进行计算
        return backTrace(row, column + 1);
    }
}

void Sudoku::getMatrix(int a_matrix[GRIDNUM][GRIDNUM])
{
    //获取数独矩阵
    for(int i=0;i<GRIDNUM;i++)
        for(int j=0;j<GRIDNUM;j++)
            a_matrix[i][j]=matrix[i][j];
}

QSet<int> Sudoku::createRandomLineArray()
{
    //生成一个包含1-9的长度为9的数组
    QSet<int>lineArray;
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    while (lineArray.size()<9)
    {
        lineArray.insert(qrand()%9+1);
    }
    return lineArray;
}

void Sudoku::createRandomSudokuArray(int array[GRIDNUM][GRIDNUM])
{
    //随机生成一个标准数独
    QList<int>lineArray=createRandomLineArray().toList();
    for (int i = 0; i < GRIDNUM; i++)
    {
        for (int j = 0; j < GRIDNUM; j++)
        {
            int index=lineArray.indexOf(matrix[i][j]);
            matrix[i][j] = lineArray.at((index + 1) % GRIDNUM);
            array[i][j]=matrix[i][j];
        }
    }
}

void Sudoku::createRandomSudokuGame(int array[GRIDNUM][GRIDNUM],Difficulty difficulty)
{
    //随机对数独矩阵中元素置零，生成游戏矩阵
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    int zeroNum=0;
    switch (difficulty)
    {
    case easy:
        zeroNum=qrand()%7+10; //10-16
        break;
    case normal:
        zeroNum=qrand()%7+30;  //30-36
        break;
    case hard:
        zeroNum=qrand()%7+56; //56-62
        break;
    }
    QList<int>zeroSet;
    int k=0,n;
    while (k++<zeroNum)
    {
        n=qrand()%81;
        if(!zeroSet.contains(n))
        {
            array[n/9][n%9]=0;
            zeroSet.append(n);
        }
    }

}
