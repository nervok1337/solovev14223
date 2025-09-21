#include "node.h"

Node::Node(const string& name) : name(name) {}

const string& Node::getName() const {
	return name;
}

Node::iterator Node::nb_begin() const {
	return neighbours.cbegin();
}
Node::iterator Node::nb_end() const {
	return neighbours.cend();
}

void Node::addNeighbour(Node* neighbour, int weight) {
    neighbours[neighbour] = weight;
}

void Node::removeNeighbour(Node* neighbour) {
    neighbours.erase(neighbour);
}
