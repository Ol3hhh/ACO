#include <SFML/Graphics.hpp>
#include "Menu.hpp"
#include "Graph.hpp"
#include "GraphRender.hpp"
#include "PathRender.hpp"
#include "ACO.hpp"
#include "ACO_GPU.hpp"
#include "Constants.hpp"
#include <iostream>
#include <random>
#include <chrono>
#include <thread>
#include <iomanip>

void runTestComparison(const Graph& graph) {
    int numAnts = 50;
    int iterations = 100;
    float alpha = 1.0f;
    float beta = 5.0f;
    float evaporation = 0.5f;
    float Q = 100.0f;

    std::cout << "\n=== AUTO TEST porównawczy ===\n";

    auto runAndMeasure = [&](const std::string& name, auto&& algoFunc) {
        auto start = std::chrono::high_resolution_clock::now();
        auto [path, length] = algoFunc();
        auto end = std::chrono::high_resolution_clock::now();
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

        std::cout << std::fixed << std::setprecision(2);
        std::cout << name << " - długość: " << length << ", czas: " << ms << " ms\n";
    };

    runAndMeasure("SEKWENCYJNY", [&]() {
        ACO aco(graph, numAnts, alpha, beta, evaporation, Q);
        aco.runSequential(iterations);
        return std::make_pair(aco.getBestPath(), aco.getBestPathLength());
    });

    runAndMeasure("WIELOWĄTKOWY", [&]() {
        ACO aco(graph, numAnts, alpha, beta, evaporation, Q);
        unsigned int threads = std::thread::hardware_concurrency();
        if (threads == 0) threads = 4;
        aco.runParallel(iterations, threads);
        return std::make_pair(aco.getBestPath(), aco.getBestPathLength());
    });

    runAndMeasure("GPU (CUDA)", [&]() {
        try {
            ACO_GPU acoGPU(graph, numAnts, alpha, beta, evaporation, Q);
            acoGPU.run(iterations);
            return std::make_pair(acoGPU.getBestPath(), acoGPU.getBestPathLength());
        } catch (const std::exception& e) {
            std::cerr << "Błąd CUDA: " << e.what() << std::endl;
            return std::make_pair(std::vector<int>(), -1.0f);
        }
    });

    std::cout << "==============================\n";
}


int main() {
    const int N = 50;
    Graph graph(N, 1);

    std::vector<sf::Vector2f> positions;
    std::mt19937 rng(1);
    std::uniform_real_distribution<float> distX(50, SIZE - 50);
    std::uniform_real_distribution<float> distY(50, SIZE - 50);
    for (int i = 0; i < N; ++i)
        positions.emplace_back(distX(rng), distY(rng));

    sf::RenderWindow window(sf::VideoMode(SIZE, SIZE), "ACO - Graf");

    sf::Font font;
    if (!font.loadFromFile("assets/arial.ttf")) {
        std::cerr << "Font load error\n";
        return 1;
    }

    Menu menu(font);
    GraphRenderer renderer(graph, positions);
    PathRenderer pathRenderer(positions);

    bool showGraph = false;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (!showGraph) {
                if (event.type == sf::Event::MouseMoved) {
                    sf::Vector2f mouse(sf::Mouse::getPosition(window));
                    menu.handleHover(mouse);
                }

                if (event.type == sf::Event::MouseButtonPressed) {
                    sf::Vector2f mouse(sf::Mouse::getPosition(window));
                    MenuOption choice = menu.handleClick(mouse);

                    if (choice == MenuOption::Sequential || choice == MenuOption::Parallel || choice == MenuOption::GPU) {
                        showGraph = true;

                        int numAnts = 50;
                        int iterations = 100;
                        float alpha = 1.0f;
                        float beta = 5.0f;
                        float evaporation = 0.5f;
                        float Q = 100.0f;

                        auto start = std::chrono::high_resolution_clock::now();

                        if (choice == MenuOption::Sequential) {
                            std::cout << "Tryb sekwencyjny...\n";
                            ACO aco(graph, numAnts, alpha, beta, evaporation, Q);
                            aco.runSequential(iterations);
                            pathRenderer.setPath(aco.getBestPath());
                            std::cout << "Długość najlepszej trasy: " << aco.getBestPathLength() << "\n";
                        } else if (choice == MenuOption::Parallel) {
                            std::cout << "Tryb wielowątkowy...\n";
                            ACO aco(graph, numAnts, alpha, beta, evaporation, Q);
                            unsigned int threads = std::thread::hardware_concurrency();
                            if (threads == 0) threads = 4;
                            aco.runParallel(iterations, threads);
                            pathRenderer.setPath(aco.getBestPath());
                            std::cout << "Długość najlepszej trasy: " << aco.getBestPathLength() << "\n";
                        } else if (choice == MenuOption::GPU) {
                            std::cout << "Tryb GPU (CUDA)...\n";
                            try {
                                ACO_GPU acoGPU(graph, numAnts, alpha, beta, evaporation, Q);
                                acoGPU.run(iterations);
                                pathRenderer.setPath(acoGPU.getBestPath());
                                std::cout << "Długość najlepszej trasy: " << acoGPU.getBestPathLength() << "\n";
                            } catch (const std::exception& e) {
                                std::cerr << "Błąd CUDA: " << e.what() << std::endl;
                            }
                        }

                        auto end = std::chrono::high_resolution_clock::now();
                        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
                        std::cout << "Czas wykonania: " << duration.count() << " ms\n";

                    } else if (choice == MenuOption::AutoTest) {
                        runTestComparison(graph);
                    }
                }
            }
        }

        window.clear(sf::Color(30, 30, 30));
        if (showGraph) {
            renderer.draw(window);
            pathRenderer.draw(window);
        } else {
            menu.draw(window);
        }
        window.display();
    }

    return 0;
}
