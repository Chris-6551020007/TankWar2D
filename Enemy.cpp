#include "Enemy.h"

static sf::Texture enemyBodyTexture;
static sf::Texture enemyBarrelTexture;
static bool enemyTextureLoaded = false;

Enemy::Enemy(float x, float y) {
    if (enemyTextureLoaded == false) {
        enemyBodyTexture.loadFromFile("assets/enemy_body.png");
        enemyBarrelTexture.loadFromFile("assets/enemy_barrel.png");
        enemyTextureLoaded = true;
    }

    body.setSize({ 50.f, 35.f });
    body.setOrigin({ 25.f, 17.5f });
    body.setPosition({ x, y });

    if (enemyBodyTexture.getSize().x > 0) {
        body.setTexture(&enemyBodyTexture);
        body.setFillColor(sf::Color::White);
    }
    else {
        body.setFillColor(sf::Color::Red);
    }

    barrel.setSize({ 30.f, 8.f });
    barrel.setOrigin({ 0.f, 4.f });
    barrel.setPosition({ x, y });

    if (enemyBarrelTexture.getSize().x > 0) {
        barrel.setTexture(&enemyBarrelTexture);
        barrel.setFillColor(sf::Color::White);
    }
    else {
        barrel.setFillColor(sf::Color::Yellow);
    }

    health = 3;
    maxHealth = 3;

    healthBarBack.setSize({ 50.f, 6.f });
    healthBarBack.setFillColor(sf::Color::Red);

    healthBarFront.setSize({ 50.f, 6.f });
    healthBarFront.setFillColor(sf::Color::Green);

    patrolSpeed = 2.f;
    chaseSpeed = 1.f;

    randomDirection();

    detectionRadius = 450.f;
    canSeePlayer = false;
    aimDirection = { -1.f, 0.f };
}

void Enemy::update(sf::Vector2f playerPosition, bool hasLineOfSight) {
    updateAimToPlayer(playerPosition, hasLineOfSight);

    if (canSeePlayer == true) {
        body.move(aimDirection * chaseSpeed);
    }
    else {
        if (changeDirectionClock.getElapsedTime().asSeconds() >= 2.f) {
            randomDirection();
            changeDirectionClock.restart();
        }

        body.move(direction * patrolSpeed);
    }

    barrel.setPosition(body.getPosition());

    sf::Vector2f position = body.getPosition();

    healthBarBack.setPosition({ position.x - 25.f, position.y - 40.f });
    healthBarFront.setPosition({ position.x - 25.f, position.y - 40.f });
}

void Enemy::draw(sf::RenderWindow& window) {
    window.draw(body); 
    window.draw(barrel);
    window.draw(healthBarBack);
    window.draw(healthBarFront);
}

sf::FloatRect Enemy::getBounds() const {
    return body.getGlobalBounds();
}

void Enemy::takeDamage(int damage) {
    health -= damage;

    if (health < 0) {
        health = 0;
    }

    float healthPercent = static_cast<float>(health) / maxHealth;
    healthBarFront.setSize({ 50.f * healthPercent, 6.f });
}

bool Enemy::isDead() const {
    return health <= 0;
}
sf::Vector2f Enemy::getPosition() const {
    return body.getPosition();
}

void Enemy::setPosition(sf::Vector2f position) {
    body.setPosition(position);
    barrel.setPosition(position);
    updateRotation();
}

void Enemy::reverseDirection() {
    randomDirection();
}

sf::Vector2f Enemy::getDirection() const {
    return direction;
}
//Bắn đạn từ hướng của nòng
sf::Vector2f Enemy::getBulletSpawnPosition() const {
    return body.getPosition() + aimDirection * 35.f;
}
void Enemy::randomDirection() {
    int randomNumber = rand() % 4;

    if (randomNumber == 0) {
        direction = { 1.f, 0.f };
    }
    else if (randomNumber == 1) {
        direction = { -1.f, 0.f };
    }
    else if (randomNumber == 2) {
        direction = { 0.f, 1.f };
    }
    else if (randomNumber == 3) {
        direction = { 0.f, -1.f };
    }

    float angleRadians = std::atan2(direction.y, direction.x);
    float angleDegrees = angleRadians * 180.f / 3.14159265f;

    body.setRotation(sf::degrees(angleDegrees));
    barrel.setRotation(sf::degrees(angleDegrees));
}
void Enemy::setDirection(sf::Vector2f newDirection) {
    direction = newDirection;
    updateRotation();
}
void Enemy::updateRotation() {
    sf::Angle angle = sf::degrees(0.f);

    if (direction.x == 1.f) {
        angle = sf::degrees(0.f);      // sang phải
    }
    else if (direction.x == -1.f) {
        angle = sf::degrees(180.f);    // sang trái
    }
    else if (direction.y == 1.f) {
        angle = sf::degrees(90.f);     // xuống
    }
    else if (direction.y == -1.f) {
        angle = sf::degrees(-90.f);    // lên
    }

    body.setRotation(angle);
    barrel.setRotation(angle);

    // đảm bảo nòng luôn nằm đúng tâm thân
    barrel.setPosition(body.getPosition());
}

void Enemy::updateAimToPlayer(sf::Vector2f playerPosition, bool hasLineOfSight) {
    sf::Vector2f enemyPosition = body.getPosition();

    sf::Vector2f toPlayer = playerPosition - enemyPosition;

    float distance = std::sqrt(
        toPlayer.x * toPlayer.x +
        toPlayer.y * toPlayer.y
    );

    if (distance <= detectionRadius && hasLineOfSight == true) {
        canSeePlayer = true;

        if (distance != 0.f) {
            aimDirection = toPlayer / distance;
        }

        float angleRadians = std::atan2(aimDirection.y, aimDirection.x);
        float angleDegrees = angleRadians * 180.f / 3.14159265f;

        body.setRotation(sf::degrees(angleDegrees));
        barrel.setRotation(sf::degrees(angleDegrees));
    }
    else {
        canSeePlayer = false;

        float angleRadians = std::atan2(direction.y, direction.x);
        float angleDegrees = angleRadians * 180.f / 3.14159265f;

        body.setRotation(sf::degrees(angleDegrees));
        barrel.setRotation(sf::degrees(angleDegrees));
    }
}
//Trạng thái thấy or không
bool Enemy::isPlayerInRange() const {
    return canSeePlayer;
}
//hướng
sf::Vector2f Enemy::getAimDirection() const {
    return aimDirection;
}

void Enemy::chooseNewDirectionAfterCollision() {
    int randomNumber = rand() % 4;

    if (randomNumber == 0) {
        direction = { 1.f, 0.f };
    }
    else if (randomNumber == 1) {
        direction = { -1.f, 0.f };
    }
    else if (randomNumber == 2) {
        direction = { 0.f, 1.f };
    }
    else {
        direction = { 0.f, -1.f };
    }

    updateRotation();
}
void Enemy::chooseDirection(sf::Vector2f newDirection) {
    direction = newDirection;
    updateRotation();
}