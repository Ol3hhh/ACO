#include "GraphRender.hpp"

GraphRenderer::GraphRenderer(const Graph& g, const std::vector<sf::Vector2f>& pos)
    : graph(g), positions(pos) {}

void GraphRenderer::draw(sf::RenderWindow& window) const {
    int n = graph.size();


    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            double w = graph.getWeight(i, j);
            if (w > 0) {
                sf::Vertex line[] = {
                    sf::Vertex(positions[i], sf::Color(150, 150, 150)),
                    sf::Vertex(positions[j], sf::Color(150, 150, 150))
                };
                window.draw(line, 2, sf::Lines);
            }
        }
    }

    for (int i = 0; i < n; ++i) {
        sf::CircleShape node(6);
        node.setFillColor(sf::Color::Red);
        node.setOrigin(6, 6); 
        node.setPosition(positions[i]);
        window.draw(node);
    }
}
