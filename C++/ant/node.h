#ifndef NODE_H
#define NODE_H
#include <string>
#include <unordered_map>
#include <map>
using namespace std;

class Node {
private:
    string name;
    unordered_map<Node*, pair<int, double>> neighbours; // вес и под-вес

    void addNeighbour(Node* neighbour, int weight);
    void removeNeighbour(Node* neighbour);

public:
    explicit Node(const std::string& name);
    ~Node() = default;

    using iterator = unordered_map<Node*, pair<int, double>>::const_iterator;
    iterator nb_begin() const;
    iterator nb_end() const;

    const string& getName() const;
    double getWeight(Node* neighbour) const;
    double getSubWeight(Node* neighbour) const;
    void setSubWeight(Node* neighbour, double subWeight);

    friend class Graph;
};

#endif // !_NODE_H
