#pragma once
#include <SFML/Graphics.hpp>
#include <cstdlib>

class Enemy {
private:
    sf::RectangleShape body;
    sf::RectangleShape barrel;

    //sf::Texture bodyTexture;
    //sf::Texture barrelTexture;

    int health;
    int maxHealth;

    sf::RectangleShape healthBarBack;
    sf::RectangleShape healthBarFront;

    float patrolSpeed;//tốc độ tuần tra
    float chaseSpeed;//tốc độ đuổi theo
    sf::Vector2f direction;
    //di chuyển
    sf::Clock changeDirectionClock;

    float detectionRadius;//Bán kính phát hiện tank
    bool canSeePlayer;//biến Phát hiện tank
    sf::Vector2f aimDirection;//Hướng từ tank tới enemy

public:
    Enemy(float x, float y);

    void update(sf::Vector2f playerPosition, bool hasLineOfSight);
    void updateAimToPlayer(sf::Vector2f playerPosition, bool hasLineOfSight);

    void draw(sf::RenderWindow& window);

    sf::FloatRect getBounds() const;
    void takeDamage(int damage);
    bool isDead() const;
    sf::Vector2f getPosition() const;
    void setPosition(sf::Vector2f position);
    void reverseDirection();

    sf::Vector2f getDirection() const;
    sf::Vector2f getBulletSpawnPosition() const;
    void randomDirection();//di chuyển
    //thay đổi hướng di chuyển
    void setDirection(sf::Vector2f newDirection);
    void updateRotation();

    bool isPlayerInRange() const;
    sf::Vector2f getAimDirection() const;

    void chooseNewDirectionAfterCollision();
    void chooseDirection(sf::Vector2f newDirection);
};
