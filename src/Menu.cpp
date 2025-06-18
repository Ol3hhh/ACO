#include "Menu.hpp"

Menu::Menu(sf::Font& font) {
    baseColor = sf::Color(50, 50, 200);
    hoverColor = sf::Color(100, 100, 250);

    btnSequential.setSize({240, 70});
    btnSequential.setPosition(180, 120);
    btnSequential.setFillColor(baseColor);
    btnSequential.setOutlineThickness(3);
    btnSequential.setOutlineColor(sf::Color::White);

    txtSequential.setFont(font);
    txtSequential.setString("Tryb sekwencyjny");
    txtSequential.setCharacterSize(24);
    txtSequential.setFillColor(sf::Color::White);
    txtSequential.setPosition(200, 140);

    //////////////

    btnParallel.setSize({240, 70});
    btnParallel.setPosition(180, 220);
    btnParallel.setFillColor(baseColor);
    btnParallel.setOutlineThickness(3);
    btnParallel.setOutlineColor(sf::Color::White);

    txtParallel.setFont(font);
    txtParallel.setString("Tryb wielowatkowy");
    txtParallel.setCharacterSize(24);
    txtParallel.setFillColor(sf::Color::White);
    txtParallel.setPosition(185, 240);
}

void Menu::draw(sf::RenderWindow& window) {
    window.draw(btnSequential);
    window.draw(txtSequential);
    window.draw(btnParallel);
    window.draw(txtParallel);
}

MenuOption Menu::handleClick(const sf::Vector2f& mousePos) {
    if (btnSequential.getGlobalBounds().contains(mousePos))
        return MenuOption::Sequential;
    if (btnParallel.getGlobalBounds().contains(mousePos))
        return MenuOption::Parallel;
    return MenuOption::None;
}

void Menu::handleHover(const sf::Vector2f& mousePos) {
    btnSequential.setFillColor(
        btnSequential.getGlobalBounds().contains(mousePos) ? hoverColor : baseColor);
    btnParallel.setFillColor(
        btnParallel.getGlobalBounds().contains(mousePos) ? hoverColor : baseColor);
}
