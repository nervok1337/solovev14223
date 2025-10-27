#include "graph.h"
#include "AntAlgorithm.h"
#include <iostream>

int main(int argc, char* argv[]) { 
    if (argc < 2) {
        cerr << "Введите имя файла\n";
        return 1;
    }

    string filename = argv[1];
    Graph g(filename);

    AntColony aco(&g, 10, 50);
    aco.run();

    return 0;
}

