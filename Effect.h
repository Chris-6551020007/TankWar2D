#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

enum class EffectType {
    MuzzleFlash,
    BulletImpact,
    ExplosionEnemy,
    ExplosionPlayer
};

class Effect {
private:
    std::vector<sf::CircleShape> particles;
    std::vector<sf::Vector2f> velocities;

    sf::Clock lifeClock;
    float duration;

public:
    Effect(sf::Vector2f position, EffectType type, sf::Vector2f direction = { 0.f, 0.f });

    void update();
    void draw(sf::RenderWindow& window);
    bool isAlive() const;
};

