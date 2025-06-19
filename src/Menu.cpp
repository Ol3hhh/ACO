#include "Menu.hpp"

Menu::Menu(sf::Font& font) {
    baseColor = sf::Color(50, 50, 200);
    hoverColor = sf::Color(100, 100, 250);

    btnSequential.setSize({240, 70});
    btnSequential.setPosition((SIZE / 2) - 120, (SIZE / 2) - 100);
    btnSequential.setFillColor(baseColor);
    btnSequential.setOutlineThickness(3);
    btnSequential.setOutlineColor(sf::Color::White);

    txtSequential.setFont(font);
    txtSequential.setString("Tryb sekwencyjny");
    txtSequential.setCharacterSize(24);
    txtSequential.setFillColor(sf::Color::White);

    sf::FloatRect bounds1 = txtSequential.getLocalBounds();
    txtSequential.setOrigin(bounds1.left + bounds1.width / 2.0f, bounds1.top + bounds1.height / 2.0f);
    txtSequential.setPosition(
        btnSequential.getPosition().x + btnSequential.getSize().x / 2.0f,
        btnSequential.getPosition().y + btnSequential.getSize().y / 2.0f
    );

    btnParallel.setSize({240, 70});
    btnParallel.setPosition((SIZE / 2) - 120, (SIZE / 2) + 20);
    btnParallel.setFillColor(baseColor);
    btnParallel.setOutlineThickness(3);
    btnParallel.setOutlineColor(sf::Color::White);

    txtParallel.setFont(font);
    txtParallel.setString("Tryb wielowatkowy");
    txtParallel.setCharacterSize(24);
    txtParallel.setFillColor(sf::Color::White);

    sf::FloatRect bounds2 = txtParallel.getLocalBounds();
    txtParallel.setOrigin(bounds2.left + bounds2.width / 2.0f, bounds2.top + bounds2.height / 2.0f);
    txtParallel.setPosition(
        btnParallel.getPosition().x + btnParallel.getSize().x / 2.0f,
        btnParallel.getPosition().y + btnParallel.getSize().y / 2.0f
    );
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
