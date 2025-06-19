#pragma once

#include "Graph.hpp"
#include "Ant.hpp"
#include <vector>
#include <limits>
#include <cstdlib>
#include <thread>
#include <mutex>
class ACO {
public:
    ACO(const Graph& graph, int numAnts, float alpha, float beta, float evaporation, float Q);

    void runSequential(int iterations);

    const std::vector<int>& getBestPath() const;
    float getBestPathLength() const;
    void runParallel(int iterations, int numThreads);
private:
    void resetAnts();
    void updatePheromones();

    const Graph& graph;
    int numVertices;
    int numAnts;
    float alpha, beta;
    float evaporation;
    float Q;

    std::vector<Ant> ants;
    std::vector<double> pheromones;  
    std::vector<int> bestPath;
    float bestLength;
};
