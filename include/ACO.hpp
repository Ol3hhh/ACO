#pragma once
#include "Graph.hpp"
#include "Ant.hpp"
#include <vector>
#include <random>
#include <mutex>

class ACO {
public:
    ACO(const Graph& graph, int numAnts, float alpha, float beta, float evaporation, float Q);

    void runSequential(int iterations);
    void runParallel(int iterations, int numThreads);

    const std::vector<int>& getBestPath() const;
    float getBestPathLength() const;

private:
    void resetAnts();
    void updatePheromones();

    const Graph& graph;
    int numVertices;
    int numAnts;
    float alpha, beta, evaporation, Q;

    std::vector<double> pheromones;
    std::vector<Ant> ants;

    std::vector<int> bestPath;
    float bestLength;

    std::mt19937 rng;
};

