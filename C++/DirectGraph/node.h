#ifndef NODE_H
#define NODE_H
#include <string>
#include <map>
using namespace std;

class Node {
private:
	string name;
	map<Node*, int> neighbours;

	void addNeighbour(Node* neighbour, int weight);
	void removeNeighbour(Node* neighbour);
public:
	explicit Node(const std::string& name);
	~Node() = default;

	const string& getName() const;

	using iterator = map<Node*, int>::const_iterator;
	iterator nb_begin() const;
	iterator nb_end() const;
	friend class Graph;
};

#endif // !_NODE_H