#include "witnessStick.hpp"

witnessStickClass::witnessStickClass()
{
    sprite.setFillColor(sf::Color(255, 176, 0));
    sprite.setSize(sf::Vector2f(40, 80));

    life = 20;
}

void witnessStickClass::draw(sf::RenderWindow& window)
{
    window.draw(sprite);
}

int witnessStickClass::applyHorizontalMove()
{
    sprite.move(horizontalVelocity, 0);

    return horizontalVelocity;
}

int witnessStickClass::applyVerticalMove()
{
    sprite.move(0, verticalVelocity);

    return verticalVelocity;
}

bool witnessStickClass::damaged(int valueOfDamage)
{
    life -= valueOfDamage;

    return (life <= 0);
}

void witnessStickClass::hasEnterInCollide(direction dir)
{
    if(dir == UP || dir == DOWN)
    {
        verticalVelocity = 0;
    }
}

void witnessStickClass::lookAt(int x, int y, int cameraX, int cameraY)
{
    (void)x;
    (void)y;
    (void)cameraX;
    (void)cameraY;

    return;
}

bool witnessStickClass::hasThisItem(typeOfItem itemType, std::string nameOfItem)
{
    (void)itemType;
    (void)nameOfItem;

    return false;
}

bool witnessStickClass::getIsHidden()
{
    return false;
}

void witnessStickClass::setPosition(int x, int y)
{
    sprite.setPosition(x, y);
}

void witnessStickClass::setIsHidden(bool newVal)
{
    (void)newVal;

    return;
}
