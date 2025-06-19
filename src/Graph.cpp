#include "Graph.hpp"
#include <queue>
#include <set>
#include <algorithm>
#include <cmath>

Graph::Graph(int numVertices, double fillFactor)
    : n(numVertices), matrix(n * n, 0.0), rng(std::random_device{}()) {
    if (n < 2) throw std::invalid_argument("Za mało wierzchołków");
    if (fillFactor <= 0.0 || fillFactor > 1.0) throw std::invalid_argument("fillFactor musi być z zakresu (0, 1]");

    generate(fillFactor);
}

double Graph::getWeight(int i, int j) const {
    return matrix[i * n + j];
}

void Graph::setWeight(int i, int j, double value) {
    matrix[i * n + j] = value;
}

int Graph::size() const {
    return n;
}

const std::vector<double>& Graph::data() const {
    return matrix;
}

void Graph::generate(double fillFactor) {
    std::uniform_real_distribution<double> dist(1.0, 100.0);
    ensureConnectivity();

    int totalEdges = n * (n - 1); 
    int requiredEdges = static_cast<int>(totalEdges * fillFactor);

    int currentEdges = 0;
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            if (i != j && getWeight(i, j) > 0)
                ++currentEdges;

    std::uniform_int_distribution<int> indexDist(0, n - 1);
    while (currentEdges < requiredEdges) {
        int i = indexDist(rng);
        int j = indexDist(rng);
        if (i != j && getWeight(i, j) == 0) {
            double w = dist(rng);
            setWeight(i, j, w);
            ++currentEdges;
        }
    }
}

void Graph::ensureConnectivity() {
    std::uniform_real_distribution<double> dist(1.0, 100.0);
    std::vector<int> nodes(n);
    for (int i = 0; i < n; ++i) nodes[i] = i;
    std::shuffle(nodes.begin(), nodes.end(), rng);

    for (int i = 1; i < n; ++i) {
        int from = nodes[i - 1];
        int to = nodes[i];
        double weight = dist(rng);
        setWeight(from, to, weight);
        setWeight(to, from, weight); 
    }
}
