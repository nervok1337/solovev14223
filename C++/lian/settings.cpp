#include "settings.h"

Config::Config(string configname) {
    ifstream file(configname);

    if (!file.is_open()){
        cout << "Не удалось считать файл" << '\n';
        return;
    }
    getline(file, filename);
    file >> startX >> startY;
    file >> endX >> endY;  
    file >> step >> angle;
}

