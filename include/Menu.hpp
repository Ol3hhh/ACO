#pragma once
#include <SFML/Graphics.hpp>

enum class MenuOption {
    None,
    Sequential,
    Parallel
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

    sf::Color baseColor;
    sf::Color hoverColor;
};
