#pragma once
#include <SFML/Graphics.hpp>
#include "Constants.hpp"

enum class MenuOption {
    None,
    Sequential,
    Parallel,
    GPU,
    AutoTest,
    Benchmark
};

class Menu {
public:
    Menu(sf::Font& font);

    void draw(sf::RenderWindow& window);
    MenuOption handleClick(const sf::Vector2f& mousePos);
    void handleHover(const sf::Vector2f& mousePos);

private:
    sf::RectangleShape btnSequential;
    sf::Text txtSequential;

    sf::RectangleShape btnParallel;
    sf::Text txtParallel;

    sf::RectangleShape btnGPU;
    sf::Text txtGPU;

    sf::RectangleShape btnAutoTest;
    sf::Text txtAutoTest;

    sf::RectangleShape btnBenchmark;
    sf::Text txtBenchmark;

    sf::Color baseColor;
    sf::Color hoverColor;
};
