#pragma once
#include <SFML/Graphics.hpp>

class Tank {
private:
    sf::RectangleShape body;
    sf::RectangleShape barrel;//nòng súng

    sf::Texture bodyTexture;
    sf::Texture barrelTexture;

    float speed;
    sf::Vector2f direction;//lưu hướng xe quay
    sf::Vector2f aimDirection;//hướng nòng súng
    sf::CircleShape directionMark;//kí hiệu hướng

    //xoay nòng súng
    float currentAngle;
    float targetAngle;
    float rotateSpeed;//tốc dộ xoay


    sf::Vector2f moveDirection;
    sf::Vector2f lastInputDirection;

    int sameDirectionPressCount;

    bool oldW;
    bool oldA;
    bool oldS;
    bool oldD;
    void updateDirectionMark();

public:
    Tank(float x, float y);

    void update();
    void draw(sf::RenderWindow& window);

    sf::Vector2f getPosition() const;
    sf::Vector2f getDirection() const;
    sf::Vector2f getBulletSpawnPosition() const;
    void setPosition(sf::Vector2f position);//
    sf::FloatRect getBounds() const;//va chạm

    void updateAim(sf::Vector2f mouseWorld);
    void keepInMap(float mapWidth, float mapHeight);
    sf::Vector2f getAimDirection() const;
    void aimAt(sf::Vector2f target);
    //xoay
    void rotateSmoothly();

    sf::Vector2f getKeyboardDirection();
    sf::Vector2f normalizeVector(sf::Vector2f v);
    float getAngleFromDirection(sf::Vector2f dir);
    void updateBarrelTransform();

};