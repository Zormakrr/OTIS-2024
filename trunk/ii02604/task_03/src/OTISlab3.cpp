#include <iostream>
#include <vector>
#include <list>
#include <queue>
#include <unordered_set>

using namespace std;

class GraphStructure {
private:
    struct Node {
        int identifier;
        list<int> connections;
    };

    vector<Node> nodes;

    // Проверка на наличие Эйлерова цикла
    bool isEulerian() const {
        for (const auto& node : nodes) {
            if (node.connections.size() % 2 != 0) {
                return false;
            }
        }
        return true;
    }

    // Рекурсивная функция для поиска Гамильтонова цикла
    bool findHamiltonianCycle(int currentNode, int depth, vector<int>& path, vector<bool>& visitedNodes) {
        if (depth == nodes.size()) {
            return path.front() == path.back();
        }

        for (int adjacent : nodes[currentNode].connections) {
            if (!visitedNodes[adjacent]) {
                visitedNodes[adjacent] = true;
                path[depth] = adjacent;
                if (findHamiltonianCycle(adjacent, depth + 1, path, visitedNodes)) {
                    return true;
                }
                visitedNodes[adjacent] = false;
            }
        }
        return false;
    }

public:
    // Добавление нового узла
    void addNode(int identifier) {
        nodes.push_back({ identifier, {} });
    }

    // Создание рёбер (неориентированный граф)
    void createEdge(int start, int end) {
        nodes[start].connections.push_back(end);
        nodes[end].connections.push_back(start);
    }

    // Отображение графа
    void showGraph() const {
        for (const auto& node : nodes) {
            cout << "Node " << node.identifier << ": ";
            for (int adjacent : node.connections) {
                cout << adjacent << " ";
            }
            cout << endl;
        }
    }

    // Получение Эйлерова цикла
    vector<int> getEulerianCycle() {
        vector<int> cycle;
        if (!isEulerian()) return cycle;

        vector<bool> visited(nodes.size(), false);
        list<int> traversalStack;
        traversalStack.push_back(0);

        while (!traversalStack.empty()) {
            int current = traversalStack.back();
            if (!nodes[current].connections.empty()) {
                int next = nodes[current].connections.front();
                traversalStack.push_back(next);
                nodes[current].connections.remove(next);
                nodes[next].connections.remove(current);
            }
            else {
                cycle.push_back(current);
                traversalStack.pop_back();
            }
        }
        return cycle;
    }

    // Получение Гамильтонова цикла
    vector<int> getHamiltonianCycle() {
        vector<int> cycle(nodes.size(), -1);
        vector<bool> visited(nodes.size(), false);
        visited[0] = true;
        cycle[0] = 0;

        if (findHamiltonianCycle(0, 1, cycle, visited)) {
            return cycle;
        }
        return {};
    }

    // Построение остовного дерева (используем BFS)
    GraphStructure constructSpanningTree() const {
        GraphStructure spanningTree;
        for (int i = 0; i < nodes.size(); ++i) {
            spanningTree.addNode(i);
        }

        vector<bool> visited(nodes.size(), false);
        visited[0] = true;
        queue<int> nodeQueue;
        nodeQueue.push(0);

        while (!nodeQueue.empty()) {
            int current = nodeQueue.front();
            nodeQueue.pop();

            for (int adjacent : nodes[current].connections) {
                if (!visited[adjacent]) {
                    visited[adjacent] = true;
                    spanningTree.createEdge(current, adjacent);
                    nodeQueue.push(adjacent);
                }
            }
        }
        return spanningTree;
    }
};

int main() {
    GraphStructure graph;

    // Добавляем узлы
    graph.addNode(0);
    graph.addNode(1);
    graph.addNode(2);
    graph.addNode(3);
    graph.addNode(4);

    // Добавляем рёбра
    graph.createEdge(0, 1);
    graph.createEdge(1, 2);
    graph.createEdge(2, 3);
    graph.createEdge(3, 4);
    graph.createEdge(4, 0);

    // Отображаем граф
    cout << "Graph representation:\n";
    graph.showGraph();

    // Проверка на Эйлеров цикл
    vector<int> eulerCycle = graph.getEulerianCycle();
    if (!eulerCycle.empty()) {
        cout << "Eulerian cycle: ";
        for (int node : eulerCycle) {
            cout << node << " ";
        }
        cout << endl;
    }
    else {
        cout << "Eulerian cycle not found.\n";
    }

    // Проверка на Гамильтонов цикл
    vector<int> hamiltonCycle = graph.getHamiltonianCycle();
    if (!hamiltonCycle.empty()) {
        cout << "Hamiltonian cycle: ";
        for (int node : hamiltonCycle) {
            cout << node << " ";
        }
        cout << endl;
    }
    else {
        cout << "Hamiltonian cycle not found.\n";
    }

    // Построение остовного дерева
    GraphStructure spanningTree = graph.constructSpanningTree();
    cout << "Spanning Tree representation:\n";
    spanningTree.showGraph();

    return 0;
}

