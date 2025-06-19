#pragma once
#include <SFML/Graphics.hpp>
#include "Graph.hpp"

class GraphRenderer {
public:
    GraphRenderer(const Graph& graph, const std::vector<sf::Vector2f>& positions);
    void draw(sf::RenderWindow& window) const;

private:
    const Graph& graph;
    std::vector<sf::Vector2f> positions;
};
