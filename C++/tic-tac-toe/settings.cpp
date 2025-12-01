#include "settings.h"

Config::Config(string configname) {
    ifstream file(configname);

    if (!file.is_open()){
        cout << "Не удалось считать файл" << '\n';
        return;
    }
    file >> size;
    file >> winLength;
    file >> depth;
    file >> time;
}

