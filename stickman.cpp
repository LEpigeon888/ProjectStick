#include "stickman.hpp"

stickmanClass::stickmanClass()
{
    verticalVelocity = 0;
    horizontalVelocity = 0;
}

sf::FloatRect stickmanClass::getCollideBox()
{
    return sprite.getGlobalBounds();
}

sf::FloatRect stickmanClass::getSpriteBox()
{
    return sprite.getGlobalBounds();
}

sf::Vector2f stickmanClass::getFootPosition()
{
    sf::Vector2f footPosition = sprite.getPosition();

    footPosition.x += sprite.getSize().x / 2;
    footPosition.y += sprite.getSize().y;

    return footPosition;
}
