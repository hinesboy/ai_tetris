#pragma once
#ifndef TETRIS_CLASS
#define TETRIS_CLASS

#include"GridPosition.h"
/*
* 类： 俄罗斯方块图形
*/
class GridPosition;

class Tetris
{
public:
	GridPosition positions[4];
	int color;
	int key; // 关键点下标,为负数不可以旋转

	Tetris();
	// 采用可变参数构造图形，传入9个BYTE类型参数，表示4组坐标与关键点下标
	Tetris(int key, ...);
	~Tetris();
	// 析构
	Tetris & operator = (const Tetris & tetris);
	// 移动所有x
	void changeX(int step);
	// 移动所有y
	void changeY(int step);
	// 获得图形的最大最小X，Y
	void getMaxMin(int & minX, int & maxX, int & minY, int & maxY);
	// 获取最左下角的元素下标
	int getLeftBottom();
	// 顺时针旋转90度, 根据边界范围旋转完成后自行调整图形位置
	void rotate(int min_x, int max_x, int min_y, int max_y);
	void rotate();
	// 向下移动
	void down();
	// 向左移动
	void left();
	// 向右移动
	void right();

	// 赋随机值
	void random();
	// 根据下标获取画刷
	friend HBRUSH getHbrush(int);
};
#endif // !TETRIS_CLASS

