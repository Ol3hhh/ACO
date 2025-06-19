#include "ACO.hpp"


ACO::ACO(const Graph& graph, int numAnts, float alpha, float beta, float evaporation, float Q)
    : graph(graph), numVertices(graph.size()), numAnts(numAnts),
      alpha(alpha), beta(beta), evaporation(evaporation), Q(Q),
      pheromones(numVertices * numVertices, 1.0), 
      bestLength(std::numeric_limits<float>::max()) {

    ants.reserve(numAnts);
    for (int i = 0; i < numAnts; ++i)
        ants.emplace_back(numVertices);
}

void ACO::runSequential(int iterations) {
    for (int iter = 0; iter < iterations; ++iter) {
        resetAnts();

        for (auto& ant : ants) {
            int start = rand() % numVertices;
            ant.reset(start);
            ant.buildTour(graph, pheromones, alpha, beta);

            float length = ant.getPathLength();
            if (length < bestLength) {
                bestLength = length;
                bestPath = ant.getPath();
            }
        }

        updatePheromones();
    }
}

void ACO::resetAnts() {
    for (auto& ant : ants) {
        ant.reset(0);  
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

        for (size_t i = 0; i < path.size() - 1; ++i) {
            int from = path[i];
            int to = path[i + 1];
            double delta = Q / length;

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
 

void ACO::runParallel(int iterations, int numThreads) {
    for (int iter = 0; iter < iterations; ++iter) {
        std::mutex pheromoneMutex;
        std::mutex bestPathMutex;

        std::vector<std::thread> threads;

        int antsPerThread = numAnts / numThreads;

        for (int t = 0; t < numThreads; ++t) {
            threads.emplace_back([&, t]() {
                int start = t * antsPerThread;
                int end = (t == numThreads - 1) ? numAnts : start + antsPerThread;

                for (int i = start; i < end; ++i) {
                    int startV = rand() % numVertices;
                    ants[i].reset(startV);
                    ants[i].buildTour(graph, pheromones, alpha, beta);

                    float len = ants[i].getPathLength();
                    {
                        std::lock_guard<std::mutex> lock(bestPathMutex);
                        if (len < bestLength) {
                            bestLength = len;
                            bestPath = ants[i].getPath();
                        }
                    }
                }
            });
        }

        for (auto& t : threads) t.join();

        updatePheromones();  
    }
}
