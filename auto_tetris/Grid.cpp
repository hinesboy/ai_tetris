#include <vector>
#include "Grid.h"

// 假设 ROWSPAN 是 std::vector<int>
typedef std::vector<int> ROWSPAN;

class Grid {
private:
    std::vector<ROWSPAN> grid;

public:
    Grid() {}
    Grid(int row, int col) : grid(row, ROWSPAN(col, 0)) {}
    void init(int row, int col) {
        grid = std::vector<ROWSPAN>(row, ROWSPAN(col, 0));
    }
    ROWSPAN& operator[](int index) {
        return grid[index];
    }
    bool isExist(const Tetris& tetris) {
        for (int i = 0; i < 4; i++) {
            if (tetris.positions[i].x >= 0 && tetris.positions[i].y >= 0 &&
                tetris.positions[i].x < grid.size() && tetris.positions[i].y < grid[0].size() &&
                grid[tetris.positions[i].x][tetris.positions[i].y] > 0) {
                return true;
            }
        }
        return false;
    }
    void addCurTerisToGrid(const Tetris& tetris) {
        for (int i = 0; i < 4; i++) {
            if (tetris.positions[i].x >= 0 && tetris.positions[i].y >= 0 &&
                tetris.positions[i].x < grid.size() && tetris.positions[i].y < grid[0].size()) {
                grid[tetris.positions[i].x][tetris.positions[i].y] = tetris.color;
            }
        }
    }
    int lineStatus(int i) {
        bool isFull = true;
        for (int cell : grid[i]) {
            if (cell == 0) {
                isFull = false;
                break;
            }
        }
        return isFull ? 1 : 0;
    }
    int clearLine() {
        int clearline = 0;
        for (int i = grid.size() - 1; i >= 0; i--) {
            int status = lineStatus(i);
            if (status == 1) {
                grid.erase(grid.begin() + i);
                grid.insert(grid.begin(), ROWSPAN(grid[0].size(), 0));
                clearline++;
                i++; // 检查新插入的行
            }
        }
        return clearline;
    }
    Grid& operator=(const Grid& other) {
        if (this != &other) {
            grid = other.grid;
        }
        return *this;
    }
    ~Grid() {}
};
