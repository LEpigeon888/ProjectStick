#include <cmath>
#include "weapon.hpp"
#include "soundManager.hpp"
#include "global.hpp"

std::map<std::string, weaponInfo> weaponClass::listOfTypeOfWeapon;

weaponClass::weaponClass()
{

}

weaponClass::weaponClass(std::string newTypeOfWeapon)
{
    weaponInfo newWeapon = listOfTypeOfWeapon[newTypeOfWeapon];
    sprite.setFillColor(newWeapon.color);
    sprite.setSize(newWeapon.sizeOfWeapon);
    sprite.setOrigin(newWeapon.origin);
    numberOfBulletWhenShot = newWeapon.numberOfBulletWhenShot;
    isFullWeapon = newWeapon.fullWeapon;
    outOfWeapon = newWeapon.outOfWeapon;
    sizeOfMagazine = newWeapon.sizeOfMagazine;
    numberOfBulletInMagazine = sizeOfMagazine;
    timeForReload = newWeapon.timeForReload;
    timeBetweenShot = newWeapon.timeBetweenShot;
    angleOfBullet = newWeapon.angleOfBullet;

    typeOfWeapon = newTypeOfWeapon;

    timerBetweenShot.restart();
}

void weaponClass::initializeWeapon()
{
    weaponInfo newWeapon;
    newWeapon.color = sf::Color::Transparent;
    newWeapon.sizeOfWeapon = sf::Vector2f(0, 0);
    newWeapon.origin = sf::Vector2f(0, 0);
    newWeapon.numberOfBulletWhenShot = 0;
    newWeapon.fullWeapon = false;
    newWeapon.outOfWeapon = sf::Vector2f(0, 0);
    newWeapon.sizeOfMagazine = 0;
    newWeapon.timeForReload = 0;
    newWeapon.timeBetweenShot = 0;
    newWeapon.angleOfBullet = 0;

    listOfTypeOfWeapon["NO_WEAPON"] = newWeapon;

    newWeapon.color = sf::Color::Magenta;
    newWeapon.sizeOfWeapon = sf::Vector2f(40, 16);
    newWeapon.origin = sf::Vector2f(5, 8);
    newWeapon.numberOfBulletWhenShot = 1;
    newWeapon.fullWeapon = false;
    newWeapon.outOfWeapon = sf::Vector2f(35, 8);
    newWeapon.sizeOfMagazine = 10;
    newWeapon.timeForReload = 1;
    newWeapon.timeBetweenShot = 0.35;
    newWeapon.angleOfBullet = 0;

    listOfTypeOfWeapon["BASIC_WEAPON"] = newWeapon;

    newWeapon.color = sf::Color::Green;
    newWeapon.sizeOfWeapon = sf::Vector2f(50, 12);
    newWeapon.origin = sf::Vector2f(5, 6);
    newWeapon.numberOfBulletWhenShot = 1;
    newWeapon.fullWeapon = true;
    newWeapon.outOfWeapon = sf::Vector2f(47, 6);
    newWeapon.sizeOfMagazine = 1000;
    newWeapon.timeForReload = 2;
    newWeapon.timeBetweenShot = 0.01;
    newWeapon.angleOfBullet = 0;

    listOfTypeOfWeapon["FULL_WEAPON"] = newWeapon;

    newWeapon.color = sf::Color(128, 30, 0);
    newWeapon.sizeOfWeapon = sf::Vector2f(30, 16);
    newWeapon.origin = sf::Vector2f(5, 8);
    newWeapon.numberOfBulletWhenShot = 5;
    newWeapon.fullWeapon = false;
    newWeapon.outOfWeapon = sf::Vector2f(25, 8);
    newWeapon.sizeOfMagazine = 20;
    newWeapon.timeForReload = 1.5;
    newWeapon.timeBetweenShot = 0.5;
    newWeapon.angleOfBullet = 5;

    listOfTypeOfWeapon["SHOTGUN_WEAPON"] = newWeapon;
}

void weaponClass::draw(sf::RenderWindow& window)
{
    window.draw(sprite);
}

bulletClass weaponClass::shoot(int numberOfThisBullet, bool isAgainstPlayer)
{
    double angleForThisBullet = angle;
    sf::Vector2f velocityOfThisBullet = velocityOfBullet;
    bulletClass bullet(typeOfWeapon, isAgainstPlayer);
    bullet.setPosition(sprite.getTransform().transformPoint(outOfWeapon).x, sprite.getTransform().transformPoint(outOfWeapon).y);

    if(numberOfThisBullet > 0)
    {
        angleForThisBullet += (((numberOfThisBullet % 2) * 2) - 1) * angleOfBullet * ((numberOfThisBullet + 1) / 2);
        velocityOfThisBullet.x = cos(angleForThisBullet * PI / 180);
        velocityOfThisBullet.y = sin(angleForThisBullet * PI / 180);
    }

    bullet.setRotation(angleForThisBullet);
    bullet.setVelocity(velocityOfThisBullet.x, velocityOfThisBullet.y);

    soundManagerClass::playThisSound("piou.ogg");

    numberOfBulletInMagazine -= 1;
    timerBetweenShot.restart();

    return bullet;
}

bool weaponClass::canShot()
{
    return (numberOfBulletInMagazine >= numberOfBulletWhenShot) && (timerBetweenShot.getElapsedTime().asSeconds() >= timeBetweenShot);
}

void weaponClass::reload(int number)
{
    numberOfBulletInMagazine += number;
}

sf::Vector2i weaponClass::getOutOfWeapon()
{
    return sf::Vector2i(sprite.getTransform().transformPoint(outOfWeapon).x, sprite.getTransform().transformPoint(outOfWeapon).y);
}

std::string weaponClass::getTypeOfWeapon()
{
    return typeOfWeapon;
}

int weaponClass::getNumberOfBulletWhenShot()
{
    return numberOfBulletWhenShot;
}

int weaponClass::getNumberOfBulletInMagazine()
{
    return numberOfBulletInMagazine;
}

int weaponClass::getSizeOfMagazine()
{
    return sizeOfMagazine;
}

float weaponClass::getTimeForReload()
{
    return timeForReload;
}

bool weaponClass::getIsFullWeapon()
{
    return isFullWeapon;
}

void weaponClass::setPosition(int x, int y)
{
    sprite.setPosition(x, y);
}

void weaponClass::setLookAt(int x, int y, int cameraX, int cameraY)
{
    double areaX;
    double areaY;

    areaX = fabs(sprite.getPosition().x - cameraX - x);
    areaY = fabs(sprite.getPosition().y - cameraY - y);

    if(areaX == 0 && areaY == 0)
    {
        return;
    }

    angle = atan(areaY / areaX) * 180 / PI;

    if(sprite.getPosition().x - cameraX <= x && sprite.getPosition().y - cameraY >= y)
    {
        angle = -angle;
    }
    else if(sprite.getPosition().x - cameraX >= x && sprite.getPosition().y - cameraY >= y)
    {
        angle -= 180;
    }
    else if(sprite.getPosition().x - cameraX >= x && sprite.getPosition().y - cameraY <= y)
    {
        angle -= 180;
        angle = -angle;
    }

    sprite.setRotation(angle);

    velocityOfBullet.x = cos(angle * PI / 180);
    velocityOfBullet.y = sin(angle * PI / 180);
}

void weaponClass::setRotation(int newAngle)
{
    sprite.setRotation(newAngle);
}
