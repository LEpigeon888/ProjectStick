#include "playerStick.hpp"
#include "global.hpp"

playerStickClass::playerStickClass()
{
    weight = 2;
    speed = 5;
    life = 500;
    speedJump = -25;
    totalNumberOfJump = 2;
    currentNumberOfJump = totalNumberOfJump;
    sprite.setSize(sf::Vector2f(40, 80));
    sprite.setFillColor(sf::Color::Blue);
    listOfWeapon.push_back(weaponClass("NO_WEAPON"));
    listOfWeapon.push_back(weaponClass("BASIC_WEAPON"));
    listOfWeapon.push_back(weaponClass("FULL_WEAPON"));
    listOfWeapon.push_back(weaponClass("SHOTGUN_WEAPON"));
    currentWeapon = 1;
    isInReload = false;
    isHidden = false;
    needToCheckWeapon = false;
    checkIfWeaponIsInInventory();
}

bool playerStickClass::update()
{
    bool updateOfGameNeeded = false;

    verticalVelocity += weight;

    if(isInReload == true && reloadTimer.getElapsedTime().asSeconds() >= listOfWeapon[currentWeapon].getTimeForReload())
    {
        reload();
        isInReload = false;
        updateOfGameNeeded = true;
    }

    if(needToCheckWeapon == true)
    {
        checkIfWeaponIsInInventory();
    }

    return updateOfGameNeeded;
}

void playerStickClass::draw(sf::RenderWindow& window)
{
    window.draw(sprite);
    if(isHidden == false)
    {
        listOfWeapon[currentWeapon].draw(window);
    }
}

int playerStickClass::applyHorizontalMove()
{
    sprite.move(horizontalVelocity, 0);
    listOfWeapon[currentWeapon].setPosition(sprite.getGlobalBounds().left + (sprite.getGlobalBounds().width / 2), sprite.getGlobalBounds().top + (sprite.getGlobalBounds().height / 2));

    return horizontalVelocity;
}

int playerStickClass::applyVerticalMove()
{
    sprite.move(0, verticalVelocity);
    listOfWeapon[currentWeapon].setPosition(sprite.getGlobalBounds().left + (sprite.getGlobalBounds().width / 2), sprite.getGlobalBounds().top + (sprite.getGlobalBounds().height / 2));

    return verticalVelocity;
}

bool playerStickClass::damaged(int valueOfDamage)
{
    if(global::easterEggEnable == true)
    {
        valueOfDamage /= 2;
    }

    life -= valueOfDamage;

    if(life <= 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void playerStickClass::hasEnterInCollide(direction dir)
{
    if(dir == UP || dir == DOWN)
    {
        verticalVelocity = 0;
        if(dir == DOWN)
        {
            currentNumberOfJump = totalNumberOfJump;
        }
    }
}

void playerStickClass::lookAt(int x, int y, int cameraX, int cameraY)
{
    listOfWeapon[currentWeapon].setLookAt(x, y, cameraX, cameraY);
}

bool playerStickClass::hasThisItem(typeOfItem itemType, std::string nameOfItem)
{
    return (inventory.getNumberOfThisItem(itemType, nameOfItem) > 0);
}

void playerStickClass::jump()
{
    if(currentNumberOfJump > 0)
    {
        verticalVelocity = speedJump;
        --currentNumberOfJump;
    }
}

void playerStickClass::moveTo(direction newDir)
{
    if(newDir == LEFT)
    {
        horizontalVelocity = -speed;
    }
    else if(newDir == RIGHT)
    {
        horizontalVelocity = speed;
    }
    else
    {
        horizontalVelocity = 0;
    }
}

std::list<bulletClass> playerStickClass::shoot(bool isFull)
{
    std::list<bulletClass> listOfBullet;

    if(listOfWeapon[currentWeapon].getNumberOfBulletWhenShot() > 0)
    {
        if(listOfWeapon[currentWeapon].getIsFullWeapon() == isFull && isInReload == false)
        {
            if(listOfWeapon[currentWeapon].canShot() == true)
            {
                for(int i = 0; i < listOfWeapon[currentWeapon].getNumberOfBulletWhenShot(); ++i)
                {
                    listOfBullet.push_back(listOfWeapon[currentWeapon].shoot(i));
                }
            }
        }
    }

    return std::move(listOfBullet);
}

void playerStickClass::reloadNeeded()
{
    if(isInReload == false && listOfWeapon[currentWeapon].getNumberOfBulletInMagazine() < listOfWeapon[currentWeapon].getSizeOfMagazine())
    {
        isInReload = true;
        reloadTimer.restart();
    }
}

void playerStickClass::reload()
{
    int bulletNeedLoaded = listOfWeapon[currentWeapon].getSizeOfMagazine() - listOfWeapon[currentWeapon].getNumberOfBulletInMagazine();
    int bulletNotLoaded = inventory.removeThisItem(BULLET, listOfWeapon[currentWeapon].getTypeOfWeapon(), bulletNeedLoaded);
    listOfWeapon[currentWeapon].reload(bulletNeedLoaded - bulletNotLoaded);
}

void playerStickClass::selectWeapon(int number)
{
    if(static_cast<unsigned int>(number) < listOfWeapon.size())
    {
        if(inventory.getNumberOfThisItem(WEAPON, listOfWeapon[number].getTypeOfWeapon()) > 0)
        {
            currentWeapon = number;
            isInReload = false;
        }
    }
}

void playerStickClass::checkIfWeaponIsInInventory()
{
    if(inventory.getNumberOfThisItem(WEAPON, listOfWeapon[currentWeapon].getTypeOfWeapon()) == 0)
    {
        currentWeapon = 0;
        isInReload = false;
    }
}

int playerStickClass::getNumberOfBulletInMagazine()
{
    return listOfWeapon[currentWeapon].getNumberOfBulletInMagazine();
}

int playerStickClass::getNumberOfBulletInInentory()
{
    return inventory.getNumberOfThisItem(BULLET, listOfWeapon[currentWeapon].getTypeOfWeapon());
}

inventoryClass* playerStickClass::getInventory()
{
    needToCheckWeapon = true;
    return &inventory;
}

lineStruct playerStickClass::getLineOfShot()
{
    lineStruct lineOfShot;
    lineOfShot.firstPoint = sf::Vector2i(sprite.getGlobalBounds().left + (sprite.getGlobalBounds().width / 2), sprite.getGlobalBounds().top + (sprite.getGlobalBounds().height / 2));
    lineOfShot.secondPoint = listOfWeapon[currentWeapon].getOutOfWeapon();

    return lineOfShot;
}

int playerStickClass::getLife()
{
    return life;
}

bool playerStickClass::getIsHidden()
{
    return isHidden;
}

void playerStickClass::setPosition(int x, int y)
{
    sprite.setPosition(x, y);
    listOfWeapon[currentWeapon].setPosition(sprite.getGlobalBounds().left + (sprite.getGlobalBounds().width / 2), sprite.getGlobalBounds().top + (sprite.getGlobalBounds().height / 2));
}

void playerStickClass::setIsHidden(bool newVal)
{
    isHidden = newVal;

    if(isHidden == true)
    {
        sprite.setFillColor(sf::Color(sprite.getFillColor().r, sprite.getFillColor().g, sprite.getFillColor().b, 100));

        if(verticalVelocity < 0)
        {
            verticalVelocity = 0;
        }
    }
    else
    {
        sprite.setFillColor(sf::Color(sprite.getFillColor().r, sprite.getFillColor().g, sprite.getFillColor().b, 255));
    }
}
