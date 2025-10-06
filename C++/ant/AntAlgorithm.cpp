#include "AntAlgorithm.h"

Ant::Ant(double a, double b, double r) : alpha(a), beta(b), rho(r) {}

AntColony::AntColony(Graph* g, int ants, int iterations, double Q_)
	: graph(g), numAnts(ants), maxIterations(iterations), Q(Q_), rng(random_device{}()) {}

double AntColony::computePathLength(const vector<Node*>& path) const {
    	double length = 0.0;
    	for (size_t i = 0; i + 1 < path.size(); ++i) {
        	Node* a = path[i];
        	Node* b = path[i + 1];
        	length += a->getWeight(b);
    	}
    	return length;
}

Node* AntColony::chooseNextNode(Node* current, const unordered_map<Node*, bool>& visited, const Ant& ant) {
	vector<Node*> choices;
	vector<double> probabilities;
	for (auto it = current->nb_begin(); it != current->nb_end(); ++it) {
		Node* neigh = it->first;
        	if (visited.at(neigh))
			continue;

        	double tau = current->getSubWeight(neigh);
        	double eta = 1.0 / it->second.first;
        	choices.push_back(neigh);
        	probabilities.push_back(pow(tau, ant.alpha) * pow(eta, ant.beta));
    	}

    	if (choices.empty())
		return nullptr;

    	double sum = accumulate(probabilities.begin(), probabilities.end(), 0.0);
    	uniform_real_distribution<double> dist(0.0, sum);
    	double pick = dist(rng);
    	double cumulative = 0.0;

    	for (size_t i = 0; i < choices.size(); ++i) {
        	cumulative += probabilities[i];
        if (cumulative >= pick)
        	return choices[i];
    	}

    	return choices.back();
}

void AntColony::run() {
    	vector<Node*> nodes(graph->begin(), graph->end());
    	double bestLength = INFINITY;
    	vector<Node*> bestPath;

	for (int iter = 0; iter < maxIterations; ++iter) {
        	vector<Ant> ants;
        	for (int i = 0; i < numAnts; ++i)
            		ants.emplace_back(1.0, 2.0, 0.1);

		for (Ant& ant : ants) {
			vector<Node*> path;
			unordered_map<Node*, bool> visited;
			for (auto n : nodes)
				visited[n] = false;

			Node* start = nodes[rng() % nodes.size()];
			path.push_back(start);
			visited[start] = true;

			while (path.size() < nodes.size()) {
				Node* next = chooseNextNode(path.back(), visited, ant);
				if (!next)
					break;
				path.push_back(next);
				visited[next] = true;
			}

			Node* last = path.back();
			double w = last->getWeight(start);
			if (w > 0)                      
				path.push_back(start);

			double length = computePathLength(path);

			if (length < bestLength && path.size() == nodes.size() + 1) {
				bestLength = length;
				bestPath = path;
			}

			double delta = Q / length;
			for (size_t i = 0; i + 1 < path.size(); ++i) {
				Node* a = path[i];
				Node* b = path[i + 1];
				double oldPheromone = a->getSubWeight(b);
				a->setSubWeight(b, oldPheromone * (1 - ant.rho) + delta);
			}
		}
	}

	cout << "Best path length: " << bestLength << "\nPath: ";
	for (auto n : bestPath)
		cout << n->getName() << " \n";
}

