#include "Ant.hpp"
#include <cmath>
#include <limits>

Ant::Ant(int numVertices)
    : numVertices(numVertices), pathLength(0.0f) {
    path.reserve(numVertices + 1);
    visited.resize(numVertices, false);
}

void Ant::reset(int startVertex) {
    path.clear();
    std::fill(visited.begin(), visited.end(), false);
    currentVertex = startVertex;
    visited[startVertex] = true;
    path.push_back(startVertex);
    pathLength = 0.0f;
}

void Ant::visitVertex(int vertex, float distance) {
    path.push_back(vertex);
    visited[vertex] = true;
    pathLength += distance;
    currentVertex = vertex;
}

int Ant::chooseNextVertex(const Graph& graph, const std::vector<double>& pheromones,
                          float alpha, float beta, std::mt19937& rng) {
    std::vector<float> probabilities(numVertices, 0.0f);
    float sum = 0.0f;

    for (int j = 0; j < numVertices; ++j) {
        if (!visited[j]) {
            double tau = pheromones[currentVertex * numVertices + j];
            double eta = 1.0 / graph.getWeight(currentVertex, j);
            float value = static_cast<float>(std::pow(tau, alpha) * std::pow(eta, beta));
            probabilities[j] = value;
            sum += value;
        }
    }

    if (sum == 0.0f) {
        for (int j = 0; j < numVertices; ++j)
            if (!visited[j]) return j;
        return -1;
    }

    for (float& p : probabilities) {
        p /= sum;
    }

    std::uniform_real_distribution<float> dist(0.0f, 1.0f);
    float r = dist(rng);
    float acc = 0.0f;

    for (int j = 0; j < numVertices; ++j) {
        if (!visited[j]) {
            acc += probabilities[j];
            if (r <= acc)
                return j;
        }
    }

    for (int j = 0; j < numVertices; ++j)
        if (!visited[j]) return j;

    return -1;
}

void Ant::buildTour(const Graph& graph, const std::vector<double>& pheromones,
                    float alpha, float beta, std::mt19937& rng) {
    for (int i = 1; i < numVertices; ++i) {
        int next = chooseNextVertex(graph, pheromones, alpha, beta, rng);
        float dist = graph.getWeight(currentVertex, next);
        visitVertex(next, dist);
    }

    float back = graph.getWeight(currentVertex, path[0]);
    visitVertex(path[0], back);
}

float Ant::computePathLength(const Graph& graph) {
    float total = 0.0f;
    for (size_t i = 0; i < path.size() - 1; ++i) {
        total += graph.getWeight(path[i], path[i + 1]);
    }
    return total;
}

const std::vector<int>& Ant::getPath() const {
    return path;
}

float Ant::getPathLength() const {
    return pathLength;
}
