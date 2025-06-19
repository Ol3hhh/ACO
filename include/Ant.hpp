#pragma once
#include <cmath>
#include <random>
#include <cstdlib>
#include <vector>
#include "Graph.hpp"

class Ant {
public:
    Ant(int numVertices);

    void reset(int startVertex);
    void visitVertex(int vertex, float distance);
    int chooseNextVertex(const Graph& graph, const std::vector<double>& pheromones, float alpha, float beta);
    void buildTour(const Graph& graph, const std::vector<double>& pheromones, float alpha, float beta);

    float computePathLength(const Graph& graph);
    const std::vector<int>& getPath() const;
    float getPathLength() const;

private:
    int numVertices;
    int currentVertex;
    std::vector<int> path;
    std::vector<bool> visited;
    float pathLength;
};
