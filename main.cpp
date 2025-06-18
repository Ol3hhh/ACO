#include <SFML/Graphics.hpp>
#include "Menu.hpp"
#include <iostream>

int main() {
    sf::RenderWindow window(sf::VideoMode(600, 420), "ACO Menu");
    sf::Font font;

    if (!font.loadFromFile("../assets/arial.ttf")) {
        std::cerr << "Font load error\n";
        return 1;
    }

    Menu menu(font);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseMoved) {
                sf::Vector2f mouse(sf::Mouse::getPosition(window));
                menu.handleHover(mouse);
            }

            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2f mouse(sf::Mouse::getPosition(window));
                MenuOption choice = menu.handleClick(mouse);

                if (choice == MenuOption::Sequential)
                    std::cout << "Sekwencyjny wybrany\n";
                else if (choice == MenuOption::Parallel)
                    std::cout << "Wielowatkowy wybrany\n";
            }
        }

        window.clear(sf::Color(30, 30, 30)); 
        menu.draw(window);
        window.display();
    }

    return 0;
}
