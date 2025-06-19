#include <SFML/Graphics.hpp>
#include "Menu.hpp"
#include "Graph.hpp"
#include "GraphRender.hpp"
#include "PathRender.hpp"
#include "ACO.hpp"
#include "Constants.hpp"
#include <iostream>
#include <cmath>
#include <random>
#include <chrono>

int main() {
    const int N = 150;
    Graph graph(N, 0.5f);  

    std::vector<sf::Vector2f> positions;
    unsigned int seed = 1; 
    std::mt19937 rng(seed);
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

                    if (choice == MenuOption::Sequential || choice == MenuOption::Parallel) {
                        showGraph = true;

                  
                        int numAnts = 50;
                        int iterations = 100;
                        float alpha = 1.0f;
                        float beta = 5.0f;
                        float evaporation = 0.5f;
                        float Q = 100.0f;

                        ACO aco(graph, numAnts, alpha, beta, evaporation, Q);

                       
                        auto start = std::chrono::high_resolution_clock::now();

                        if (choice == MenuOption::Sequential) {
                            std::cout << "Tryb sekwencyjny...\n";
                            aco.runSequential(iterations);
                        } else {
                            std::cout << "Tryb wielowątkowy...\n";
                            unsigned int threads = std::thread::hardware_concurrency();
                            if (threads == 0) threads = 4;
                            aco.runParallel(iterations, threads);
                        }

                        auto end = std::chrono::high_resolution_clock::now();
                        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

                        std::cout << "Długość najlepszej trasy: " << aco.getBestPathLength() << "\n";
                        std::cout << "Czas wykonania: " << duration.count() << " ms\n";

                      
                        pathRenderer.setPath(aco.getBestPath());
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
