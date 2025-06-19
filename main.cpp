#include <SFML/Graphics.hpp>
#include "Menu.hpp"
#include "Graph.hpp"
#include "GraphRender.hpp"
#include "Constants.hpp"
#include <iostream>
#include <cmath>
#include <random>

int main() {
    const int N = 20;
    Graph graph(N, 0.5);

    std::vector<sf::Vector2f> positions;
    std::mt19937 rng(std::random_device{}());
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

                    if (choice == MenuOption::Sequential) {
                        std::cout << "Sekwencyjny wybrany\n";
                        showGraph = true;
                    } else if (choice == MenuOption::Parallel) {
                        std::cout << "Wielowatkowy wybrany\n";
                        showGraph = true;
                    }
                }
            }
        }

        window.clear(sf::Color(30, 30, 30));

        if (showGraph) {
            renderer.draw(window);
        } else {
            menu.draw(window);
        }

        window.display();
    }

    return 0;
}
