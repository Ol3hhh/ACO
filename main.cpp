#include <SFML/Graphics.hpp>

int main() {
    // Tworzymy okno o wymiarach 800x600
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Example");

    // Ustawiamy limit klatek na sekundę (opcjonalnie)
    window.setFramerateLimit(60);

    // Tworzymy kształt — czerwone koło
    sf::CircleShape circle(100);  // promień 100
    circle.setFillColor(sf::Color::Red);
    circle.setPosition(350, 250); // środek ekranu (w przybliżeniu)

    // Pętla główna programu
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            // Zamknięcie okna
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Czyszczenie okna (kolor tła: niebieski)
        window.clear(sf::Color::Blue);

        // Rysujemy obiekty
        window.draw(circle);

        // Wyświetlamy to, co zostało narysowane
        window.display();
    }

    return 0;
}
