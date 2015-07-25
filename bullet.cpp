#include "bullet.hpp"

std::map<std::string, bulletInfo> bulletClass::listOfTypeOfBullet;

bulletClass::bulletClass(std::string typeOfBullet, bool againstPlayer)
{
    bulletInfo newBullet = listOfTypeOfBullet[typeOfBullet];
    sprite.setSize(newBullet.sizeOfBullet);
    sprite.setFillColor(newBullet.color);
    sprite.setOrigin(newBullet.sizeOfBullet.x / 2, newBullet.sizeOfBullet.y / 2);
    speed = newBullet.speed;
    damageValue = newBullet.damageValue;
    isAgainstPlayer = againstPlayer;
}

void bulletClass::initializeBullet()
{
    bulletInfo newBullet;
    newBullet.color = sf::Color::Red;
    newBullet.sizeOfBullet.x = 5;
    newBullet.sizeOfBullet.y = 5;
    newBullet.speed = 30;
    newBullet.damageValue = 10;

    listOfTypeOfBullet["BASIC_WEAPON"] = newBullet;

    newBullet.color = sf::Color::Cyan;
    newBullet.sizeOfBullet.x = 3;
    newBullet.sizeOfBullet.y = 3;
    newBullet.speed = 15;
    newBullet.damageValue = 1;

    listOfTypeOfBullet["FULL_WEAPON"] = newBullet;

    newBullet.color = sf::Color(174, 0, 255);
    newBullet.sizeOfBullet.x = 6;
    newBullet.sizeOfBullet.y = 6;
    newBullet.speed = 20;
    newBullet.damageValue = 10;

    listOfTypeOfBullet["SHOTGUN_WEAPON"] = newBullet;
}

void bulletClass::draw(sf::RenderWindow& window)
{
    window.draw(sprite);
}

void bulletClass::applyMove()
{
    oldCenter = getCenter();
    sprite.move(velocity);
}

sf::Vector2f bulletClass::getVelocity()
{
    return velocity;
}

int bulletClass::getDamageValue()
{
    return damageValue;
}

sf::FloatRect bulletClass::getCollideBox()
{
    return sprite.getGlobalBounds();
}

sf::Vector2i bulletClass::getCenter()
{
    sf::Vector2i center;
    center.x = sprite.getGlobalBounds().left + (sprite.getGlobalBounds().width / 2);
    center.y = sprite.getGlobalBounds().top +(sprite.getGlobalBounds().height / 2);

    return center;
}

sf::Vector2i bulletClass::getOldCenter()
{
    return oldCenter;
}

bool bulletClass::getIsAgainstPlayer()
{
    return isAgainstPlayer;
}

void bulletClass::setPosition(int x, int y)
{
    oldCenter.x = x;
    oldCenter.y = y;
    sprite.setPosition(x, y);
}

void bulletClass::setRotation(double angle)
{
    sprite.setRotation(angle);
}

void bulletClass::setVelocity(float x, float y)
{
    velocity.x = x * speed;
    velocity.y = y * speed;
}
