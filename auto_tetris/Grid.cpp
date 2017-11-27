#include "stdafx.h"
#include "Grid.h"

Grid::Grid() {};
Grid::Grid(int row, int col)
{
	init(row,col);
};
VOID Grid::init(int row, int col)
{
	grid.clear();
	for (int i = 0; i < row; i++)
	{
		ROWSPAN row(col);
		grid.push_back(row);
	}
}
Grid::~Grid() {};
// 重载下标运算
ROWSPAN & Grid::operator [](int index)
{
	return grid[index];
}
// 根据图形位置判断当前位置是否被占用
BOOL Grid::isExist(Tetris tetris)
{
	for (int i = 0; i < 4; i++)
	{
		// 节点已经被占用
		if (grid[tetris.positions[i].x][tetris.positions[i].y] > 0)
		{
			return true;
		}
	}
	return false;
}
// 将当前图形坐标添加进网格
void Grid::addCurTerisToGrid(Tetris tetris)
{
	for (int i = 0; i < 4; i++)
	{
		grid[tetris.positions[i].x][tetris.positions[i].y] = tetris.color;
	}
}
int Grid::lineStatus(int i )
{
	ROWSPAN::iterator it = grid[i].begin();
	while (it < grid[i].end() - 1)
	{
		// 相邻两个值一个为0 一个大于零，即一个有节点一个无节点
		if (!(*it * *(it + 1)) && *it != *(it + 1))
		{
			return 0;
		}
		// 整行值相等
		if (it == grid[i].end() - 2)
		{
			// 都有节点
			if (*it > 0)
			{
				return 1;
			}
			// 都无节点
			return -1;
		}
		it++;
	}
}
int Grid::clearLine()
{
	int  status , clearline = 0;
	ROWSPAN row(10);
	int size = grid.size();
	for (int i = size -1; i >= 0 ; i--)
	{
		status = lineStatus(i);
		if (status == 0)
		{
			continue;
		}
		if (status == 1)
		{
			// 满
			grid.erase(grid.begin() + i); // 删除当前行
			grid.insert(grid.begin(),row);
			clearline++;
			i++; // 仍然检查此行
		}
		else 
		{
			// 空
			break;
		}
	}
	return clearline;
}

Grid & Grid::operator=(const Grid & row)
{
	grid = row.grid;
	return *this;
}