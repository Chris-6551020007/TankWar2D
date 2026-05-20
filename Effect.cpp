#include "Effect.h"
#include <cmath>
#include <cstdlib>

namespace {
    float randomFloat(float min, float max) {
        float t = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
        return min + t * (max - min);
    }
}

Effect::Effect(sf::Vector2f position, EffectType type, sf::Vector2f direction) {
    duration = 0.3f;

    int particleCount = 10;
    float minSpeed = 1.f;
    float maxSpeed = 3.f;
    float minRadius = 2.f;
    float maxRadius = 4.f;

    sf::Color baseColor = sf::Color::White;

    if (type == EffectType::MuzzleFlash) {
        duration = 0.12f;
        particleCount = 8;
        minSpeed = 1.5f;
        maxSpeed = 3.5f;
        minRadius = 2.f;
        maxRadius = 4.f;
        baseColor = sf::Color(255, 220, 80);
    }
    else if (type == EffectType::BulletImpact) {
        duration = 0.18f;
        particleCount = 10;
        minSpeed = 1.0f;
        maxSpeed = 3.0f;
        minRadius = 2.f;
        maxRadius = 4.f;
        baseColor = sf::Color(255, 170, 70);
    }
    else if (type == EffectType::ExplosionEnemy) {
        duration = 0.45f;
        particleCount = 20;
        minSpeed = 1.5f;
        maxSpeed = 4.5f;
        minRadius = 3.f;
        maxRadius = 6.f;
        baseColor = sf::Color(255, 120, 40);
    }
    else if (type == EffectType::ExplosionPlayer) {
        duration = 0.6f;
        particleCount = 28;
        minSpeed = 2.0f;
        maxSpeed = 5.5f;
        minRadius = 3.f;
        maxRadius = 7.f;
        baseColor = sf::Color(255, 200, 50);
    }

    float baseAngle = 0.f;

    if (direction.x != 0.f || direction.y != 0.f) {
        baseAngle = std::atan2(direction.y, direction.x);
    }

    for (int i = 0; i < particleCount; i++) {
        float radius = randomFloat(minRadius, maxRadius);

        sf::CircleShape particle;
        particle.setRadius(radius);
        particle.setOrigin({ radius, radius });
        particle.setPosition(position);

        sf::Color color = baseColor;

        if (type == EffectType::ExplosionEnemy) {
            if (i % 3 == 0) {
                color = sf::Color(255, 220, 120);
            }
            else if (i % 3 == 1) {
                color = sf::Color(255, 120, 40);
            }
            else {
                color = sf::Color(180, 40, 40);
            }
        }
        else if (type == EffectType::ExplosionPlayer) {
            if (i % 3 == 0) {
                color = sf::Color(255, 240, 130);
            }
            else if (i % 3 == 1) {
                color = sf::Color(255, 170, 50);
            }
            else {
                color = sf::Color(220, 70, 30);
            }
        }
        else if (type == EffectType::BulletImpact) {
            if (i % 2 == 0) {
                color = sf::Color(255, 230, 160);
            }
            else {
                color = sf::Color(255, 150, 60);
            }
        }

        particle.setFillColor(color);
        particles.push_back(particle);

        float angle = 0.f;

        if (type == EffectType::MuzzleFlash) {
            angle = baseAngle + randomFloat(-0.5f, 0.5f);
        }
        else {
            angle = randomFloat(0.f, 6.283185f);
        }

        float speed = randomFloat(minSpeed, maxSpeed);

        sf::Vector2f velocity;
        velocity.x = std::cos(angle) * speed;
        velocity.y = std::sin(angle) * speed;

        velocities.push_back(velocity);
    }
}

void Effect::update() {
    float elapsed = lifeClock.getElapsedTime().asSeconds();
    float progress = elapsed / duration;

    if (progress > 1.f) {
        progress = 1.f;
    }

    int alpha = static_cast<int>(255.f * (1.f - progress));

    for (int i = 0; i < particles.size(); i++) {
        particles[i].move(velocities[i]);

        velocities[i].x *= 0.94f;
        velocities[i].y *= 0.94f;

        sf::Color color = particles[i].getFillColor();
        color.a = static_cast<std::uint8_t>(alpha);
        particles[i].setFillColor(color);
    }
}

void Effect::draw(sf::RenderWindow& window) {
    for (int i = 0; i < particles.size(); i++) {
        window.draw(particles[i]);
    }
}

bool Effect::isAlive() const {
    return lifeClock.getElapsedTime().asSeconds() < duration;
}