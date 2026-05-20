#include "Bullet.h"
#include <cmath>

static sf::Texture bulletTexture;
static bool bulletTextureLoaded = false;

Bullet::Bullet(
    sf::Vector2f startPosition,
    sf::Vector2f shootDirection,
    float bulletSpeed,
    float radius,
    int bulletDamage
) {
    if (bulletTextureLoaded == false) {
        bulletTexture.loadFromFile("assets/bullet.png");
        bulletTextureLoaded = true;
    }

    shape.setRadius(radius);
    shape.setOrigin({ radius, radius });
    shape.setPosition(startPosition);

    if (bulletTexture.getSize().x > 0) {
        shape.setTexture(&bulletTexture);
        shape.setFillColor(sf::Color::White);
    }
    else {
        shape.setFillColor(sf::Color::Red);
    }

    damage = bulletDamage;

    float length = std::sqrt(
        shootDirection.x * shootDirection.x +
        shootDirection.y * shootDirection.y
    );

    if (length != 0.f) {
        direction = shootDirection / length;
    }
    else {
        direction = { 1.f, 0.f };
    }

    speed = bulletSpeed;
    active = true;
}

void Bullet::update() {
    shape.move(direction * speed);
}

void Bullet::draw(sf::RenderWindow& window) {
    if (active == true) {
        window.draw(shape);
    }
}

bool Bullet::isActive() const {
    return active;
}

void Bullet::deactivate() {
    active = false;
}

sf::FloatRect Bullet::getBounds() const {
    return shape.getGlobalBounds();
}

sf::Vector2f Bullet::getPosition() const {
    return shape.getPosition();
}

int Bullet::getDamage() const {
    return damage;
}