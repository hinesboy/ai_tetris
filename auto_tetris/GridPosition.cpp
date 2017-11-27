#include "stdafx.h"
#include "GridPosition.h"


GridPosition::GridPosition()
{
}
GridPosition::GridPosition(int x, int y) :x(x), y(y)
{
}

GridPosition::~GridPosition()
{
}
GridPosition & GridPosition::operator = (const GridPosition & ps)
{
	x = ps.x;
	y = ps.y;
	return *this;
}