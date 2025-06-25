#pragma once

#include "Graph.hpp"
#include <vector>
#include <random>

class Ant {
public:
    Ant(int numVertices);

    void reset(int startVertex);
    void visitVertex(int vertex, float distance);

    void buildTour(const Graph& graph, const std::vector<double>& pheromones,
                   float alpha, float beta, std::mt19937& rng);

    float computePathLength(const Graph& graph);
    const std::vector<int>& getPath() const;
    float getPathLength() const;

private:
    int chooseNextVertex(const Graph& graph, const std::vector<double>& pheromones,
                         float alpha, float beta, std::mt19937& rng);

    int numVertices;
    int currentVertex;
    float pathLength;
    std::vector<int> path;
    std::vector<bool> visited;
};
