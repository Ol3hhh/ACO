#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class PathRenderer {
public:
    PathRenderer(const std::vector<sf::Vector2f>& positions);

    void setPath(const std::vector<int>& path);
    void draw(sf::RenderWindow& window) const;

private:
    std::vector<sf::Vector2f> positions;
    std::vector<int> path;
};
