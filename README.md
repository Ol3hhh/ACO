# Ant Colony Optimization: Sequential vs Multithreaded vs GPU

This project implements the Ant Colony Optimization (ACO) algorithm in C++ to solve the classic Travelling Salesman Problem (TSP). The goal is to compare the performance of different implementations: a basic sequential version, a multithreaded CPU version, and a GPU-accelerated version.

ACO is a metaheuristic inspired by the behavior of real ants searching for the shortest path. It uses artificial pheromone trails, probabilistic decisions, and iterative optimization to efficiently explore solutions in combinatorial problems like TSP.

## Features
- C++ implementation of Ant Colony Optimization
- Solves the Travelling Salesman Problem (TSP)
- Three implementations:
  - Sequential (single-threaded)
  - Multithreaded (using `std::thread`)
  - GPU version (CUDA-style logic)
- Modular code structure (headers and source separated)
- CMake build system
- Performance benchmarking and comparison

## Requirements
- C++17 or newer
- CMake 3.10+
- SFML
- CUDA-compatible GPU
- CUDA toolkit

## Build & Run
```bash
git clone https://github.com/Ol3hhh/ACO.git
cd ACO
mkdir build
cd build
cmake ..
make
./sfml_game
