#include "ACO_GPU.hpp"
#include <cuda_runtime.h>
#include <curand_kernel.h>
#include <algorithm>
#include <limits>
#include <iostream>
#include <stdexcept>
#include <cassert>

#define MAX_VERTICES 64
#define MAX_ANTS     1024
#define INF_WEIGHT   1e6f  

__global__ void setup_kernel(curandState *state, unsigned long seed, int n) {
    int id = threadIdx.x + blockIdx.x * blockDim.x;
    if (id < n)
        curand_init(seed, id, 0, &state[id]);
}

__global__ void aco_kernel(
    int numVertices,
    int numAnts,
    const float* graph,         // n x n
    float* pheromones,          // n x n
    float alpha,
    float beta,
    float Q,
    int* outPaths,              // numAnts x (numVertices+1)
    float* outLengths,
    curandState* states)
{
    int antId = blockIdx.x * blockDim.x + threadIdx.x;
    if (antId >= numAnts) return;

    int visited[MAX_VERTICES] = {0};
    int path[MAX_VERTICES + 1];

    curandState localState = states[antId];
    int start = curand(&localState) % numVertices;
    int current = start;
    path[0] = current;
    visited[current] = 1;
    float length = 0.0f;

    for (int step = 1; step < numVertices; ++step) {
        float probs[MAX_VERTICES] = {0};
        float sumProb = 0.0f;
        for (int j = 0; j < numVertices; ++j) {
            if (!visited[j]) {
                float weight = graph[current * numVertices + j];
                if (weight < 1e-5f)
                    continue;
                float tau = powf(pheromones[current * numVertices + j], alpha);
                float eta = powf(1.0f / weight, beta);
                probs[j] = tau * eta;
                sumProb += probs[j];
            }
        }
        float pick = curand_uniform(&localState) * sumProb;
        float acc = 0.0f;
        int next = -1;
        for (int j = 0; j < numVertices; ++j) {
            if (!visited[j] && probs[j] > 0) {
                acc += probs[j];
                if (acc >= pick) {
                    next = j;
                    break;
                }
            }
        }
        if (next == -1) {
            for (int j = 0; j < numVertices; ++j)
                if (!visited[j] && graph[current * numVertices + j] > 1e-5f) {
                    next = j; break;
                }
        }
        if (next == -1) {
            for (int fill = step; fill <= numVertices; ++fill) path[fill] = current;
            length += INF_WEIGHT * (numVertices - step + 1);
            break;
        }

        path[step] = next;
        visited[next] = 1;
        length += graph[current * numVertices + next];
        current = next;
    }
    float retWeight = graph[current * numVertices + start];
    if (retWeight > 1e-5f) {
        path[numVertices] = start;
        length += retWeight;
    } else {
        path[numVertices] = current;
        length += INF_WEIGHT;
    }


    for (int i = 0; i <= numVertices; ++i)
        outPaths[antId * (numVertices + 1) + i] = path[i];
    outLengths[antId] = length;
    states[antId] = localState;
}

inline void CUDA_ASSERT(cudaError_t code, const char* file, int line, const char* msg = "") {
    if (code != cudaSuccess) {
        std::cerr << "CUDA error at " << file << ":" << line << " - " << cudaGetErrorString(code) << " " << msg << std::endl;
        throw std::runtime_error("CUDA error");
    }
}
#define CUDA_ASSERT(val) CUDA_ASSERT((val), __FILE__, __LINE__)


ACO_GPU::ACO_GPU(const Graph& graph, int numAnts, float alpha, float beta, float evaporation, float Q)
    : numAnts(numAnts), alpha(alpha), beta(beta), evaporation(evaporation), Q(Q), bestLength(std::numeric_limits<float>::max())
{
    numVertices = graph.size();
    assert(numVertices <= MAX_VERTICES && "Zwiększ MAX_VERTICES!");
    assert(numAnts <= MAX_ANTS && "Zwiększ MAX_ANTS!");

    // Konwertuj double -> float (CUDA)
    const std::vector<double>& src = graph.data();
    hostGraphMatrix.resize(numVertices * numVertices);
    for (int i = 0; i < numVertices * numVertices; ++i)
        hostGraphMatrix[i] = static_cast<float>(src[i]);
    bestPath.clear();
}

void ACO_GPU::run(int iterations) {
    float* d_graph = nullptr;
    float* d_pheromones = nullptr;
    int* d_paths = nullptr;
    float* d_lengths = nullptr;
    curandState* d_states = nullptr;

    size_t matSize = numVertices * numVertices * sizeof(float);
    size_t pathSize = numAnts * (numVertices + 1) * sizeof(int);
    size_t lenSize = numAnts * sizeof(float);

    CUDA_ASSERT(cudaMalloc(&d_graph, matSize));
    CUDA_ASSERT(cudaMalloc(&d_pheromones, matSize));
    CUDA_ASSERT(cudaMalloc(&d_paths, pathSize));
    CUDA_ASSERT(cudaMalloc(&d_lengths, lenSize));
    CUDA_ASSERT(cudaMalloc(&d_states, numAnts * sizeof(curandState)));

    CUDA_ASSERT(cudaMemcpy(d_graph, hostGraphMatrix.data(), matSize, cudaMemcpyHostToDevice));
    std::vector<float> hostPheromones(numVertices * numVertices, 1.0f);
    CUDA_ASSERT(cudaMemcpy(d_pheromones, hostPheromones.data(), matSize, cudaMemcpyHostToDevice));

    setup_kernel<<<(numAnts+127)/128, 128>>>(d_states, 12345, numAnts);
    cudaDeviceSynchronize();

    std::vector<int> h_paths(numAnts * (numVertices + 1));
    std::vector<float> h_lengths(numAnts);

    for (int iter = 0; iter < iterations; ++iter) {
        aco_kernel<<<(numAnts+127)/128, 128>>>(numVertices, numAnts, d_graph, d_pheromones,
                                               alpha, beta, Q, d_paths, d_lengths, d_states);
        cudaDeviceSynchronize();

        CUDA_ASSERT(cudaMemcpy(h_paths.data(), d_paths, pathSize, cudaMemcpyDeviceToHost));
        CUDA_ASSERT(cudaMemcpy(h_lengths.data(), d_lengths, lenSize, cudaMemcpyDeviceToHost));

        for (int i = 0; i < numAnts; ++i) {
            if (h_lengths[i] < bestLength) {
                bestLength = h_lengths[i];
                bestPath.assign(h_paths.begin() + i*(numVertices+1), h_paths.begin() + (i+1)*(numVertices+1));
            }
        }

        // Aktualizacja feromonów na CPU
        for (auto& p : hostPheromones) p *= (1.0f - evaporation);
        for (int i = 0; i < numAnts; ++i) {
            float len = h_lengths[i];
            if (len < INF_WEIGHT * 0.9f) { // ignoruj niepełne cykle!
                float delta = Q / (len > 0.001f ? len : 0.001f);
                for (int j = 0; j < numVertices; ++j) {
                    int from = h_paths[i*(numVertices+1) + j];
                    int to   = h_paths[i*(numVertices+1) + j + 1];
                    hostPheromones[from * numVertices + to] += delta;
                    hostPheromones[to   * numVertices + from] += delta; // symetria
                }
            }
        }
        for (auto& p : hostPheromones) if (p < 0.0001f) p = 0.0001f;

        CUDA_ASSERT(cudaMemcpy(d_pheromones, hostPheromones.data(), matSize, cudaMemcpyHostToDevice));
    }

    cudaFree(d_graph); cudaFree(d_pheromones); cudaFree(d_paths); cudaFree(d_lengths); cudaFree(d_states);
}

const std::vector<int>& ACO_GPU::getBestPath() const {
    return bestPath;
}

float ACO_GPU::getBestPathLength() const {
    return bestLength;
}
