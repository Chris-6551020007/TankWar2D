#include "Tank.h"
#include <cmath>

Tank::Tank(float x, float y) {
    body.setSize({ 50.f, 35.f });//kích thướt tank
    body.setFillColor(sf::Color::Green);//maaufxe tank
    body.setOrigin({ 25.f, 17.5f });//gốc xoay
    body.setPosition({ x, y });//vị trí ban đầu

    if (bodyTexture.loadFromFile("assets/player_body.png")) {
        body.setTexture(&bodyTexture);
        body.setFillColor(sf::Color::White);
    }
    else {
        body.setFillColor(sf::Color::Green);
    }

    //nòng súng
    barrel.setSize({ 40.f, 25.f });
    barrel.setFillColor(sf::Color::Yellow);
    barrel.setOrigin({ 8.f, 12.f });
    barrel.setPosition({ x, y });

    if (barrelTexture.loadFromFile("assets/player_barrel.png")) {
        barrel.setTexture(&barrelTexture);
        barrel.setFillColor(sf::Color::White);
    }
    else {
        barrel.setFillColor(sf::Color::Blue);
    }
    //kí hiệu hướng
    directionMark.setRadius(8.f);
    directionMark.setPointCount(3);
    directionMark.setFillColor(sf::Color::Red);
    directionMark.setOrigin({ 8.f, 8.f });

    speed = 2.f;
    direction = { 1.f, 0.f };  
    aimDirection = { 1.f, 0.f };

    moveDirection = { 1.f, 0.f };
    lastInputDirection = { 1.f, 0.f };

    sameDirectionPressCount = 0;

    oldW = false;
    oldA = false;
    oldS = false;
    oldD = false;

    currentAngle = 0.f;
    targetAngle = 0.f;
    rotateSpeed = 5.f;

    updateDirectionMark();
}


void Tank::update() {
    bool nowW = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W);
    bool nowA = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A);
    bool nowS = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S);
    bool nowD = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D);

    bool newPress = false;

    if (nowW == true && oldW == false) {
        newPress = true;
    }

    if (nowA == true && oldA == false) {
        newPress = true;
    }

    if (nowS == true && oldS == false) {
        newPress = true;
    }

    if (nowD == true && oldD == false) {
        newPress = true;
    }

    sf::Vector2f inputDirection = getKeyboardDirection();

    if (inputDirection.x != 0.f || inputDirection.y != 0.f) {
        if (newPress == true) {
            if (inputDirection == lastInputDirection) {
                sameDirectionPressCount++;
            }
            else {
                sameDirectionPressCount = 1;
                lastInputDirection = inputDirection;
            }
        }

        if (sameDirectionPressCount <= 1) {
            moveDirection = normalizeVector(direction + inputDirection);
        }
        else {
            moveDirection = inputDirection;
        }

        if (moveDirection.x != 0.f || moveDirection.y != 0.f) {
            direction = moveDirection;
        }

        body.move(moveDirection * speed);
        updateBarrelTransform();

        targetAngle = getAngleFromDirection(moveDirection);
    }

    rotateSmoothly();

    oldW = nowW;
    oldA = nowA;
    oldS = nowS;
    oldD = nowD;

    updateDirectionMark();
}

void Tank::draw(sf::RenderWindow& window) {
    window.draw(body);//vẽ tank
    window.draw(barrel);
}

//Lấy vị trí
sf::Vector2f Tank::getPosition() const {
    return body.getPosition();
}
//lấy hướng
sf::Vector2f Tank::getDirection() const {
    return direction;
}
//đạn bắn ra từ nòng
sf::Vector2f Tank::getBulletSpawnPosition() const {
    return body.getPosition() + aimDirection * 26.f;
}
//Đặt lại vị trí cũ cho tank khi va chạm
void Tank::setPosition(sf::Vector2f position) {
    body.setPosition(position);
    updateBarrelTransform();
    updateDirectionMark();
}
//Lấy vùng va chạm
sf::FloatRect Tank::getBounds() const {
    return body.getGlobalBounds();
}
//chuẩn hóa vecto thành 1 rồi tính hướng quay của nòng súng
void Tank::updateAim(sf::Vector2f mouseWorld) {
    sf::Vector2f tankPosition = body.getPosition();

    sf::Vector2f dir = mouseWorld - tankPosition;

    float length = std::sqrt(dir.x * dir.x + dir.y * dir.y);

    if (length != 0.f) {
        aimDirection = dir / length;
    }

    updateBarrelTransform();
}
void Tank::keepInMap(float mapWidth, float mapHeight) {
    sf::Vector2f position = body.getPosition();

    float wallSize = 20.f;//tường biên dày 20

    float halfW = body.getSize().x / 2.f;
    float halfH = body.getSize().y / 2.f;

    if (position.x < wallSize + halfW) {
        position.x = wallSize + halfW;
    }

    if (position.x > mapWidth - wallSize - halfW) {
        position.x = mapWidth - wallSize - halfW;
    }

    if (position.y < wallSize + halfH) {
        position.y = wallSize + halfH;
    }

    if (position.y > mapHeight - wallSize - halfH) {
        position.y = mapHeight - wallSize - halfH;
    }

    body.setPosition(position);
    updateBarrelTransform();
    updateDirectionMark();
}

//lấy hướng bắn hiện tại nửa nòng súng
sf::Vector2f Tank::getAimDirection() const {
    return aimDirection;
}
//xoay tank mượt
void Tank::rotateSmoothly() {
    float difference = targetAngle - currentAngle;

    if (difference > 180.f) {
        difference -= 360.f;
    }

    if (difference < -180.f) {
        difference += 360.f;
    }

    if (std::abs(difference) < rotateSpeed) {
        currentAngle = targetAngle;
    }
    else {
        if (difference > 0.f) {
            currentAngle += rotateSpeed;
        }
        else {
            currentAngle -= rotateSpeed;
        }
    }

    if (currentAngle < 0.f) {
        currentAngle += 360.f;
    }

    if (currentAngle >= 360.f) {
        currentAngle -= 360.f;
    }

    body.setRotation(sf::degrees(currentAngle));
}
//chuẩn hóa vecto
sf::Vector2f Tank::normalizeVector(sf::Vector2f v) {
    float length = std::sqrt(v.x * v.x + v.y * v.y);

    if (length == 0.f) {
        return { 0.f, 0.f };
    }

    return v / length;
}
//lấy hướng bàn phím
sf::Vector2f Tank::getKeyboardDirection() {
    sf::Vector2f inputDirection = { 0.f, 0.f };

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
        inputDirection.y -= 1.f;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
        inputDirection.y += 1.f;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
        inputDirection.x -= 1.f;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
        inputDirection.x += 1.f;
    }

    return normalizeVector(inputDirection);
}

//tính góc từ hướng
float Tank::getAngleFromDirection(sf::Vector2f dir) {
    return std::atan2(dir.y, dir.x) * 180.f / 3.14159f;
}

void Tank::updateDirectionMark() {
    sf::Vector2f tankPosition = body.getPosition();

    float angle = body.getRotation().asDegrees();
    float radian = angle * 3.14159f / 180.f;

    sf::Vector2f frontDirection;
    frontDirection.x = std::cos(radian);
    frontDirection.y = std::sin(radian);

    // Đặt tam giác ở phía trước thân tank
    sf::Vector2f markPosition = tankPosition + frontDirection * 18.f;

    directionMark.setPosition(markPosition);

    // +90 để mũi tam giác quay đúng theo hướng trước của tank
    directionMark.setRotation(sf::degrees(angle + 90.f));
}
void Tank::updateBarrelTransform() {
    float angle = std::atan2(aimDirection.y, aimDirection.x) * 180.f / 3.14159f;

    sf::Vector2f barrelPosition = body.getPosition() + aimDirection * 6.f;

    barrel.setPosition(barrelPosition);
    barrel.setRotation(sf::degrees(angle));
}