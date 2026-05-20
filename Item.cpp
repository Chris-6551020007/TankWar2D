#include "Item.h"

Item::Item(sf::Vector2f position, ItemType itemType) {
    type = itemType;

    shape.setRadius(12.f);
    shape.setOrigin({ 12.f, 12.f });
    shape.setPosition(position);

    if (type == ItemType::Heal) {
        shape.setFillColor(sf::Color::Green);
    }
    else if (type == ItemType::RapidFire) {
        shape.setFillColor(sf::Color::Cyan);
    }
    else if (type == ItemType::BigBullet) {
        shape.setFillColor(sf::Color::Magenta);
    }
    else if (type == ItemType::Shield) {
        shape.setFillColor(sf::Color::Blue);
    }

    shape.setOutlineThickness(2.f);
    shape.setOutlineColor(sf::Color::White);
}

void Item::draw(sf::RenderWindow& window) {
    window.draw(shape);
}

sf::FloatRect Item::getBounds() const {
    return shape.getGlobalBounds();
}

ItemType Item::getType() const {
    return type;
}