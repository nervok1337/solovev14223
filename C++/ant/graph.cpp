#include "graph.h"

Node* Graph::findNode(const string& name) const {
	for (Node* node : nodes) {
		if (node->getName() == name)
			return node;
	}
	return nullptr;
}

Graph::Graph(const string& file_name) {
	ifstream file(file_name);
	if (!file.is_open())
		throw Exception();

	string line;
    getline(file, line);
	while (getline(file, line)){
		istringstream in(line);
		string from, to;
        int weight;
		in >> from >> to >> weight;
		if (!in)
			continue;

		Node* fromNode = findNode(from);
		if (!fromNode) {
			fromNode = new Node(from);
			addNode(fromNode);
		}
		Node* toNode = findNode(to);
		if (!toNode) {
			toNode = new Node(to);
			addNode(toNode);
		}

		addEdge(fromNode, toNode, weight);
	}
}
Graph::~Graph() {
	for (auto node : nodes)
		delete node;
    nodes.clear();
}

void Graph::addNode(Node* node) {
	nodes.insert(node);
}
void Graph::removeNode(Node* node) {
	if (!node) return;

	for (auto neighbourNode : nodes)
		neighbourNode->removeNeighbour(node);
        
	nodes.erase(node);
	delete node;
}


void Graph::addEdge(Node* begin, Node* end, int weight) {
	if (!begin || !end) return;
	if (nodes.find(begin) == nodes.end()) return;
	if (nodes.find(end) == nodes.end()) return;

	begin->addNeighbour(end, weight);
//	end->addNeighbour(begin, weight);
}
void Graph::removeEdge(Node* begin, Node* end) {
	if (!begin || !end) return;

	begin->removeNeighbour(end);
//	end->removeNeighbour(begin);
}

size_t Graph::size() const{
	return nodes.size();
}
Graph::iterator Graph::begin() const {
	return nodes.cbegin();
}
Graph::iterator Graph::end() const {
	return nodes.cend();
}

