#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include "graph.h"
#include <vector>

struct MarkedNode {
    Node* node;
    Node* prev;
    int mark;

    MarkedNode(Node* anode = nullptr,
               int amark = 0,
               Node* aprev = nullptr);
};

class PriorityQueue {
private:
    vector<MarkedNode> nodes;
public:
    bool empty() const;
    MarkedNode pop();
    void push(Node* node, int mark, Node* prev);
};


struct Way {
    vector<Node*> nodes;
    int length;
    Way();
};

class Dijkstra {
private:
    const Graph& graph;
public:
    explicit Dijkstra(const Graph& agraph);
    Way shortestWay(Node* begin, Node* end);
};

#endif // DIJKSTRA_H
