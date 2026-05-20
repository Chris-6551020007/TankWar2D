#pragma once
#include <SFML/Graphics.hpp>

class Bullet {
private:
    sf::CircleShape shape;
    sf::Vector2f direction;
    float speed;
    bool active;
    int damage;

public:
    Bullet(
        sf::Vector2f startPosition,
        sf::Vector2f shootDirection,
        float bulletSpeed = 5.f,
        float radius = 7.f,
        int bulletDamage = 1
    );

    void update();
    void draw(sf::RenderWindow& window);

    bool isActive() const;
    void deactivate();

    sf::FloatRect getBounds() const;
    sf::Vector2f getPosition() const;

    int getDamage() const;
};