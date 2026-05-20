#pragma once
#include <SFML/Graphics.hpp>
class Wall {
private:
	sf::RectangleShape shape;
    bool breakable;//tường có thể phá hủy
public:
    Wall(float x, float y, float width, float height, bool canBreak);

    void draw(sf::RenderWindow& window);

    sf::FloatRect getBounds() const;
    bool isBreakable() const;//tường có thể phá
};