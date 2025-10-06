#ifndef GRAPH_H
#define GRAPH_H
#include "node.h"
#include <set>
#include <fstream>
#include <sstream>
#include <iostream>

class Exception {};

class Graph {
private:
	set<Node*> nodes;

	Node* findNode(const string& name) const;
public:
	using iterator = set<Node*>::const_iterator;

	Graph() = default;
	explicit Graph(const string& file_name);
	~Graph();

	void addNode(Node* node);
	void removeNode(Node* node);
	void addEdge(Node* begin, Node* end, int weight);
	void removeEdge(Node* begin, Node* end);
	
	size_t size() const;
	iterator begin() const;
	iterator end() const;
};

#endif // !_GRAPH_H
