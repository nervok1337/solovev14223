#include "graph.h"
#include "AntAlgorithm.h"
#include <iostream>

int main() {
        Graph g("1000.txt");

        AntColony aco(&g, 10, 50);
        aco.run();
}

