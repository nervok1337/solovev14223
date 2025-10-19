#include "map.h"

//============CELL============
float Cell::distanceTo(const Cell& other) const {
    return sqrt((x - other.x)*(x - other.x) + (y - other.y)*(y - other.y));
}

bool Cell::operator==(const Cell& other) const {
    return x == other.x && y == other.y;
}

//============MAP=============
Map::Map(const string& filename) {
    ifstream file(filename);
    string line;
    int y = 0;

    if (!file.is_open()){
        cout << "Не удалось считать файл" << '\n';
        return;
    }

    while (getline(file,line)) {
        vector<Cell> row;
        int cellX = 0;
        for (char ch : line) {
            if (ch != '0' && ch != '1') continue;
            bool obstacle = (ch == '1');
            row.push_back(Cell{cellX, y, obstacle});
            cellX++;
        }

        if (!row.empty()){
            grid.push_back(row);
            y++;
        }
    }

    height = grid.size();
    width = (height > 0 ? grid[0].size() : 0);

}

Cell* Map::getCell(int x, int y) {
    if (y < 0 || y >= height || x < 0 || x >= width)
        return nullptr;
    return &grid[y][x];
}

bool Map::isFree(int x, int y) const {
    if (y < 0 || y >= height || x < 0 || x >= width)
        return false;
    return !grid[y][x].obstacle;
}
const vector<vector<Cell>>& Map::getGrid() const {
    return grid;
}
