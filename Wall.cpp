#include "Wall.h"

static sf::Texture normalWallTexture;
static sf::Texture breakableWallTexture;
static bool wallTexturesLoaded = false;

Wall::Wall(float x, float y, float width, float height, bool canBreak) {
    if (wallTexturesLoaded == false) {
        normalWallTexture.loadFromFile("assets/wall.png");
        breakableWallTexture.loadFromFile("assets/breakable_wall.png");
        wallTexturesLoaded = true;
    }

    breakable = canBreak;

    shape.setSize({ width, height });
    shape.setPosition({ x, y });

    if (breakable == true) {
        if (breakableWallTexture.getSize().x > 0) {
            shape.setTexture(&breakableWallTexture);
            shape.setFillColor(sf::Color::White);
        }
        else {
            shape.setFillColor(sf::Color(150, 90, 40));
        }
    }
    else {
        if (normalWallTexture.getSize().x > 0) {
            shape.setTexture(&normalWallTexture);
            shape.setFillColor(sf::Color::White);
        }
        else {
            shape.setFillColor(sf::Color(100, 100, 100));
        }
    }
}

void Wall::draw(sf::RenderWindow& window) {
    window.draw(shape);
}

sf::FloatRect Wall::getBounds() const {
    return shape.getGlobalBounds();
}

bool Wall::isBreakable() const {
    return breakable;
}