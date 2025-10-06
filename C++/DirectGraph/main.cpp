#include "graph.h"
#include "dijkstra.h"

#include <chrono>

using namespace std::chrono;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Введите имя файла\n";
        return 1;
    }

    string filename = argv[1];

    Graph g(filename);

    string startName, endName;
    cout << "Введите стартовую вершину: ";
    cin >> startName;
    cout << "Введите конечную вершину: ";
    cin >> endName;

    Node* start = nullptr;
    Node* end = nullptr;

    for (auto node : g) {
        if (node->getName() == startName) 
            start = node;
        if (node->getName() == endName) 
            end = node;
    }

    if (!start || !end) {
        cerr << "Введены некорректные вершины!\n";
        return 1;
    }

    auto t1 = high_resolution_clock::now();

    Dijkstra dijkstra(g);
    Way way = dijkstra.shortestWay(start, end);

    auto t2 = high_resolution_clock::now();
    duration<double> duration = t2 - t1;

    if (way.length == -1)
        cout << "Путь не найден.\n";
    
    else {
        cout << "Длина пути: " << way.length << '\n';

        cout << "Путь: ";
        for (auto it = way.nodes.rbegin(); it != way.nodes.rend(); ++it) {
            cout << (*it)->getName();
            if (it + 1 != way.nodes.rend())
                cout << " -> ";
        }
        cout << '\n';
    }

    cout << "Время работы алгоритма: " << duration.count() << " секунд\n";

    return 0;
}
