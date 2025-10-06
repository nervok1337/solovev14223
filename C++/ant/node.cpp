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
    neighbours[neighbour] = {weight, 1.0};
}

void Node::removeNeighbour(Node* neighbour) {
    neighbours.erase(neighbour);
}

double Node::getWeight(Node* neighbour) const {
    auto it = neighbours.find(neighbour);
    if (it != neighbours.end())
        return it->second.first;
    return -1;
}

double Node::getSubWeight(Node* neighbour) const {
    auto it = neighbours.find(neighbour);
    if (it != neighbours.end())
        return it->second.second;
    return 0.0;
}

void Node::setSubWeight(Node* neighbour, double subWeight) {
    auto it = neighbours.find(neighbour);
    if (it != neighbours.end())
        it->second.second = subWeight;
}

