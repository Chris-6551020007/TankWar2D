#pragma once
#include <SFML/Graphics.hpp>

enum class ItemType {
    Heal,
    RapidFire,
    BigBullet,
    Shield
};

class Item {
private:
    sf::CircleShape shape;
    ItemType type;

public:
    Item(sf::Vector2f position, ItemType itemType);

    void draw(sf::RenderWindow& window);

    sf::FloatRect getBounds() const;
    ItemType getType() const;
};