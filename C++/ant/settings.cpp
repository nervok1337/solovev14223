#include "settings.h"

Config::Config(string configname) {
    ifstream file(configname);

    if (!file.is_open()){
        cout << "Не удалось считать файл" << '\n';
        return;
    }
    getline(file, filename);
    file >> countAnts;
    file >> maxIter;  
    file >> iterToStop;
    file >> epsilon;
    file >> alpha >> beta >> rho;
    file >> Q;
}

