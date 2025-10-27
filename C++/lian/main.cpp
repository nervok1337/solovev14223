#include "lian.h"

int main(int argc, char* argv[]) { 
    if (argc < 2) {
        cerr << "Введите имя файла\n";
        return -1;
    }

    string filename = argv[1];
    Map algMap(filename);

    if (algMap.width == 0 || algMap.height == 0) {
        cout << "Ошибка загрузки карты" << '\n';
        return -2;
    }

    //Cell* start = algMap.getCell(165,305);
    //Cell* end = algMap.getCell(1287,689);
    Cell* start = algMap.getCell(165,305);
    Cell* end = algMap.getCell(1287,689);


    cout << "Map size: " << algMap.width << "x" << algMap.height << "\n";    
    cout << "Start: " << start->x << "," << start->y << "  obstacle=" << start->obstacle << "\n";
    cout << "End:   " << end->x   << "," << end->y   << "  obstacle=" << end->obstacle << "\n";
    
    if (!start || !end || start->obstacle || end->obstacle) {
        cout << "Старт или цель недоступны!" << '\n';
        return -3;
    }
    LIAN lian(algMap,start,end, 50.0f, 20.0f);
    //LIAN lian(algMap, start, end, 50.0f, 45.0f);
    vector<Cell*> path = lian.findPath();

    if (!path.empty()) {
        cout << "Путь найден! Длина: " << path.size() << "\n";
        for (Cell* c : path)
            cout << "(" << c->y << "," << c->x << "), ";
        cout << "\n";
    }
    else
        cout << "Путь не найден" << '\n';

    return 0;
}

