#include "PathRender.hpp"

PathRenderer::PathRenderer(const std::vector<sf::Vector2f>& positions)
    : positions(positions) {}

void PathRenderer::setPath(const std::vector<int>& path) {
    this->path = path;
}

void PathRenderer::draw(sf::RenderWindow& window) const {
    if (path.size() < 2) return;

    for (size_t i = 0; i < path.size() - 1; ++i) {
        sf::Vertex line[] = {
            sf::Vertex(positions[path[i]], sf::Color::Yellow),
            sf::Vertex(positions[path[i + 1]], sf::Color::Yellow)
        };
        window.draw(line, 2, sf::Lines);
    }
}
