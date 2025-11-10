#include "graph.h"
#include "AntAlgorithm.h"
#include "settings.h"
#include <iostream>

int main(int argc, char* argv[]) { 
    if (argc < 2) {
        cerr << "Введите имя конфигурационного файла\n";
        return 1;
    }

    Config config(argv[1]);
    Graph g(config.filename);

    AntColony aco(&g, config.countAnts, config.maxIter, config.iterToStop, config.Q);
    aco.run(config.alpha,config.beta,config.rho);

    return 0;
}

