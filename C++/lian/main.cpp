#include "lian.h"
#include "settings.h"

int main() {
    string configname;

    cout << "Введите имя конфиг файла: ";
    cin >> configname;

    Config config(configname);
    Map algMap(config.filename);

    if (algMap.width == 0 || algMap.height == 0) {
        cout << "Ошибка загрузки карты" << '\n';
        return -2;
    }
    
    Cell* start = algMap.getCell(config.startX,config.startY);
    Cell* end = algMap.getCell(config.endX,config.endY);


    cout << "Размер карты: " << algMap.width << "x" << algMap.height << "\n";    
    cout << "Начало: " << start->x << "," << start->y << "  доступность=" << start->obstacle << "\n";
    cout << "Конец:   " << end->x   << "," << end->y   << "  доступность=" << end->obstacle << "\n";
    
    if (!start || !end || start->obstacle || end->obstacle) {
        cout << "Старт или цель недоступны!" << '\n';
        return -3;
    }

    LIAN lian(algMap,start,end, config.step, config.angle);
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

