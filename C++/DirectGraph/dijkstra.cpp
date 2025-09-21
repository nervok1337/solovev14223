#include "dijkstra.h"


MarkedNode::MarkedNode(Node* anode, int amark, Node* aprev) : node(anode), mark(amark), prev(aprev) {}

MarkedNode PriorityQueue::pop() {
    MarkedNode mn = nodes.back();
    nodes.pop_back();

    return mn;
}

void PriorityQueue::push(Node* node, int mark, Node* prev) {
    MarkedNode mn(node, mark, prev);
    auto it = nodes.begin();

    while (it != nodes.end() && mark < it->mark)
        ++it;
    if (it == nodes.end()) 
        nodes.push_back(mn);
    else 
        nodes.insert(it, mn);
}

bool PriorityQueue::empty() const {
    return nodes.empty();
}
Way::Way() : length(-1) {}

Dijkstra::Dijkstra(const Graph& agraph) : graph(agraph) {}

static Way unroll(const map<Node*, MarkedNode>& visited, Node* begin, Node* curr) {
    Way way;
    way.length = visited.at(curr).mark;

    while (curr != begin) {
        way.nodes.push_back(curr);
        curr = visited.at(curr).prev;
    }

    way.nodes.push_back(begin);
    return way;
}

Way Dijkstra::shortestWay(Node* begin, Node* end) {
    PriorityQueue pq;
    pq.push(begin, 0, nullptr);
    
    map<Node*, MarkedNode> visited;

    while (!pq.empty()) {
        MarkedNode next = pq.pop();
        if (visited.find(next.node) != visited.end())
            continue;

        visited[next.node] = next;

        if (end == next.node)
            return unroll(visited, begin, end);

        for (auto it = next.node->nb_begin(); it != next.node->nb_end(); ++it) {
            Node* neighbour = it->first;
            int weight = it->second + next.mark;

            if (visited.find(neighbour) == visited.end())
                pq.push(neighbour, weight, next.node);
        }
    }
    return Way();
}
