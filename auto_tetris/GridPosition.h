#pragma once
#ifndef GRIDPOSITION_CLASS
#define GRIDPOSITION_CLASS

/*
* Àà£º 20 * 10 µÄÕ¤À¸×ø±ê
*/
class GridPosition
{
public:
	int x;
	int y;

	GridPosition();
	GridPosition(int , int );
	~GridPosition();
	GridPosition & operator = (const GridPosition &);
	
};

#endif // !GRIDPOSITION_CLASS



