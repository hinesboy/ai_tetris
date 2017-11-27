#pragma once
#ifndef GRID_CLASS
#define GRID_CLASS

#include"Tetris.h"
#include <vector>
using std::vector;
class Tetris;
// 网格类
typedef vector<BYTE> ROWSPAN; 
typedef vector<ROWSPAN> GRID;
class Grid
{
public:
	GRID grid;

	Grid();
	Grid(int row, int col) ;
	~Grid();
	// 重载下标运算
	ROWSPAN & operator [](int index);
	Grid & operator = (const Grid &);
	// 根据图形位置判断当前位置是否被占用
	BOOL isExist(Tetris tetris);
	// 将当前图形坐标添加进网格
	void addCurTerisToGrid(Tetris tetris);
	// 消除行 ， return 行数
	int clearLine();
	// 判断该行状态, -1：空 0：混合 1：满
	int lineStatus(int);
	// 初始化
	void init(int ,int);
};

#endif // !GRID_CLASS

