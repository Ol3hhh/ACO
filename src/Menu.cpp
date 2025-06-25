#include "Menu.hpp"
#include "Constants.hpp"

Menu::Menu(sf::Font& font) {
    baseColor = sf::Color(50, 50, 200);
    hoverColor = sf::Color(100, 100, 250);

    float centerX = SIZE / 2.0f;

    // sequential
    btnSequential.setSize({240, 70});
    btnSequential.setPosition(centerX - 120, (SIZE / 2) - 140);
    btnSequential.setFillColor(baseColor);
    btnSequential.setOutlineThickness(3);
    btnSequential.setOutlineColor(sf::Color::White);

    txtSequential.setFont(font);
    txtSequential.setString("Sequential");
    txtSequential.setCharacterSize(24);
    txtSequential.setFillColor(sf::Color::White);

    sf::FloatRect bounds1 = txtSequential.getLocalBounds();
    txtSequential.setOrigin(bounds1.left + bounds1.width / 2.0f, bounds1.top + bounds1.height / 2.0f);
    txtSequential.setPosition(
        btnSequential.getPosition().x + btnSequential.getSize().x / 2.0f,
        btnSequential.getPosition().y + btnSequential.getSize().y / 2.0f
    );

    // CPU
    btnParallel.setSize({240, 70});
    btnParallel.setPosition(centerX - 120, (SIZE / 2) - 40);
    btnParallel.setFillColor(baseColor);
    btnParallel.setOutlineThickness(3);
    btnParallel.setOutlineColor(sf::Color::White);

    txtParallel.setFont(font);
    txtParallel.setString("CPU");
    txtParallel.setCharacterSize(24);
    txtParallel.setFillColor(sf::Color::White);

    sf::FloatRect bounds2 = txtParallel.getLocalBounds();
    txtParallel.setOrigin(bounds2.left + bounds2.width / 2.0f, bounds2.top + bounds2.height / 2.0f);
    txtParallel.setPosition(
        btnParallel.getPosition().x + btnParallel.getSize().x / 2.0f,
        btnParallel.getPosition().y + btnParallel.getSize().y / 2.0f
    );

    // GPU
    btnGPU.setSize({240, 70});
    btnGPU.setPosition(centerX - 120, (SIZE / 2) + 60);
    btnGPU.setFillColor(baseColor);
    btnGPU.setOutlineThickness(3);
    btnGPU.setOutlineColor(sf::Color::White);

    txtGPU.setFont(font);
    txtGPU.setString("GPU");
    txtGPU.setCharacterSize(24);
    txtGPU.setFillColor(sf::Color::White);

    sf::FloatRect bounds3 = txtGPU.getLocalBounds();
    txtGPU.setOrigin(bounds3.left + bounds3.width / 2.0f, bounds3.top + bounds3.height / 2.0f);
    txtGPU.setPosition(
        btnGPU.getPosition().x + btnGPU.getSize().x / 2.0f,
        btnGPU.getPosition().y + btnGPU.getSize().y / 2.0f
    );

    // Auto Test 
    btnAutoTest.setSize({240, 70});
    btnAutoTest.setPosition(centerX - 120, (SIZE / 2) + 160);
    btnAutoTest.setFillColor(baseColor);
    btnAutoTest.setOutlineThickness(3);
    btnAutoTest.setOutlineColor(sf::Color::White);

    txtAutoTest.setFont(font);
    txtAutoTest.setString("AUTO TEST");
    txtAutoTest.setCharacterSize(24);
    txtAutoTest.setFillColor(sf::Color::White);

    sf::FloatRect bounds4 = txtAutoTest.getLocalBounds();
    txtAutoTest.setOrigin(bounds4.left + bounds4.width / 2.0f, bounds4.top + bounds4.height / 2.0f);
    txtAutoTest.setPosition(
        btnAutoTest.getPosition().x + btnAutoTest.getSize().x / 2.0f,
        btnAutoTest.getPosition().y + btnAutoTest.getSize().y / 2.0f
    );


        // Benchmark
    btnBenchmark.setSize({240, 70});
    btnBenchmark.setPosition(centerX - 120, (SIZE / 2) + 260);
    btnBenchmark.setFillColor(baseColor);
    btnBenchmark.setOutlineThickness(3);
    btnBenchmark.setOutlineColor(sf::Color::White);

    txtBenchmark.setFont(font);
    txtBenchmark.setString("BENCHMARK");
    txtBenchmark.setCharacterSize(24);
    txtBenchmark.setFillColor(sf::Color::White);

    sf::FloatRect bounds5 = txtBenchmark.getLocalBounds();
    txtBenchmark.setOrigin(bounds5.left + bounds5.width / 2.0f, bounds5.top + bounds5.height / 2.0f);
    txtBenchmark.setPosition(
        btnBenchmark.getPosition().x + btnBenchmark.getSize().x / 2.0f,
        btnBenchmark.getPosition().y + btnBenchmark.getSize().y / 2.0f
    );

}

void Menu::draw(sf::RenderWindow& window) {
    window.draw(btnSequential);
    window.draw(txtSequential);
    window.draw(btnParallel);
    window.draw(txtParallel);
    window.draw(btnGPU);
    window.draw(txtGPU);
    window.draw(btnAutoTest);
    window.draw(txtAutoTest);
    window.draw(btnBenchmark);
    window.draw(txtBenchmark);

}

MenuOption Menu::handleClick(const sf::Vector2f& mousePos) {
    if (btnSequential.getGlobalBounds().contains(mousePos))
        return MenuOption::Sequential;
    if (btnParallel.getGlobalBounds().contains(mousePos))
        return MenuOption::Parallel;
    if (btnGPU.getGlobalBounds().contains(mousePos))
        return MenuOption::GPU;
    if (btnAutoTest.getGlobalBounds().contains(mousePos))
        return MenuOption::AutoTest;
    if (btnBenchmark.getGlobalBounds().contains(mousePos))
        return MenuOption::Benchmark;

    return MenuOption::None;
}

void Menu::handleHover(const sf::Vector2f& mousePos) {
    btnSequential.setFillColor(
        btnSequential.getGlobalBounds().contains(mousePos) ? hoverColor : baseColor);
    btnParallel.setFillColor(
        btnParallel.getGlobalBounds().contains(mousePos) ? hoverColor : baseColor);
    btnGPU.setFillColor(
        btnGPU.getGlobalBounds().contains(mousePos) ? hoverColor : baseColor);
    btnAutoTest.setFillColor(
        btnAutoTest.getGlobalBounds().contains(mousePos) ? hoverColor : baseColor);
    btnBenchmark.setFillColor(
        btnBenchmark.getGlobalBounds().contains(mousePos) ? hoverColor : baseColor);

}
