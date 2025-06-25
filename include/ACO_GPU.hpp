#pragma once
#include "Graph.hpp"
#include <vector>

class ACO_GPU {
public:
    ACO_GPU(const Graph& graph, int numAnts, float alpha, float beta, float evaporation, float Q);
    void run(int iterations, unsigned long seed = 1);


    const std::vector<int>& getBestPath() const;
    float getBestPathLength() const;

private:
    int numVertices;
    int numAnts;
    float alpha, beta, evaporation, Q;

    std::vector<float> hostGraphMatrix; 
    std::vector<int> bestPath;
    float bestLength;
};
