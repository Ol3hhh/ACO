#pragma once
#include <vector>
#include <random>
#include <stdexcept>

class Graph {
public:
    Graph(int numVertices, double fillFactor); 

    double getWeight(int i, int j) const;
    void setWeight(int i, int j, double value);

    int size() const;

    const std::vector<double>& data() const;

private:
    int n;
    std::vector<double> matrix; 
    void generate(double fillFactor);
    void ensureConnectivity();

    std::mt19937 rng;
};
