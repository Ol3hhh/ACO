#include "ACO.hpp"
#include <thread>
#include <algorithm>
#include <limits>

ACO::ACO(const Graph& graph, int numAnts, float alpha, float beta, float evaporation, float Q)
    : graph(graph), numVertices(graph.size()), numAnts(numAnts),
      alpha(alpha), beta(beta), evaporation(evaporation), Q(Q),
      pheromones(numVertices * numVertices, 1.0),
      bestLength(std::numeric_limits<float>::max()),
      rng(std::random_device{}()) {

    ants.reserve(numAnts);
    for (int i = 0; i < numAnts; ++i)
        ants.emplace_back(numVertices);
}

void ACO::resetAnts() {
    std::uniform_int_distribution<int> dist(0, numVertices - 1);
    for (auto& ant : ants) {
        int start = dist(rng);
        ant.reset(start);
    }
}

void ACO::runSequential(int iterations) {
    std::uniform_int_distribution<int> dist(0, numVertices - 1);

    for (int iter = 0; iter < iterations; ++iter) {
        resetAnts();

        for (auto& ant : ants) {
            ant.buildTour(graph, pheromones, alpha, beta, rng);

            float length = ant.getPathLength();
            if (length < bestLength) {
                bestLength = length;
                bestPath = ant.getPath();
            }
        }

        updatePheromones();
    }
}

void ACO::runParallel(int iterations, int numThreads) {
    for (int iter = 0; iter < iterations; ++iter) {
        std::mutex bestMutex;
        std::vector<std::thread> threads;

        int antsPerThread = numAnts / numThreads;

        for (int t = 0; t < numThreads; ++t) {
            threads.emplace_back([&, t]() {
                std::mt19937 localRng(std::random_device{}());
                std::uniform_int_distribution<int> dist(0, numVertices - 1);

                int start = t * antsPerThread;
                int end = (t == numThreads - 1) ? numAnts : start + antsPerThread;

                for (int i = start; i < end; ++i) {
                    ants[i].reset(dist(localRng));
                    ants[i].buildTour(graph, pheromones, alpha, beta, localRng);

                    float length = ants[i].getPathLength();
                    std::lock_guard<std::mutex> lock(bestMutex);
                    if (length < bestLength) {
                        bestLength = length;
                        bestPath = ants[i].getPath();
                    }
                }
            });
        }

        for (auto& t : threads) t.join();

        updatePheromones();
    }
}

void ACO::updatePheromones() {
    for (double& p : pheromones) {
        p *= (1.0 - evaporation);
        if (p < 0.0001) p = 0.0001;
    }

    for (const auto& ant : ants) {
        const auto& path = ant.getPath();
        float length = ant.getPathLength();
        double delta = Q / length;

        for (size_t i = 0; i < path.size() - 1; ++i) {
            int from = path[i];
            int to = path[i + 1];
            pheromones[from * numVertices + to] += delta;
            pheromones[to * numVertices + from] += delta;
        }
    }
}

const std::vector<int>& ACO::getBestPath() const {
    return bestPath;
}

float ACO::getBestPathLength() const {
    return bestLength;
}
