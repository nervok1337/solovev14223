#ifndef ANT_ALGORITHM_H
#define ANT_ALGORITHM_H

#include "graph.h"
#include <vector>
#include <unordered_map>
#include <random>
#include <numeric>
#include <iostream>
#include <cmath>

using namespace std;

struct Ant {
	double alpha;
    	double beta;
    	double rho;

    	Ant(double a, double b, double r);
};

class AntColony {
private:
    	Graph* graph;
    	int numAnts;
	int maxIterations;
    	double Q;
    	mt19937 rng;

    	double computePathLength(const vector<Node*>& path) const;
    	Node* chooseNextNode(Node* current, const unordered_map<Node*, bool>& visited, const Ant& ant);

public:
    	AntColony(Graph* g, int ants, int iterations, double Q_ = 20.0);
    	void run();
};

#endif // ANT_ALGORITHM_H
