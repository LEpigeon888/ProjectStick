#include "item.hpp"
#include "utilities.hpp"
#include "global.hpp"

std::map<itemTypeInfo, itemInfo> itemClass::listOfTypeOfItem;

itemClass::itemClass()
{
    numberOfItem = 0;
}

itemClass::itemClass(typeOfItem newType, std::string newName, int newNumber)
{
    itemInfo newItem;

    infoForItem.type = newType;
    infoForItem.nameOfItem = newName;

    newItem = listOfTypeOfItem[infoForItem];

    sprite.setFillColor(newItem.colorOfItem);
    sprite.setSize(newItem.sizeOfItem);
    numberOfItem = newNumber;
    stackableItem = newItem.stackableItem;
    maxStack = newItem.maxStack;
    isTurn = false;
    isOnTheGround = false;
    verticalVelocity = 0;
    weight = 5;
    originalSize = newItem.sizeOfItem;

    numberOfItemText.setColor(sf::Color::White);
    numberOfItemText.setFont(global::font);
    numberOfItemText.setCharacterSize(10);
    numberOfItemText.setOrigin(0, 0);

    setTextPositionWithThisNumber(numberOfItem);
}

void itemClass::initializeItem()
{
    itemInfo newItem;
    itemTypeInfo newItemType;

    newItemType.type = WEAPON;
    newItemType.nameOfItem = "BASIC_WEAPON";
    newItem.sizeOfItem = sf::Vector2f(2 * 40, 5 * 40);
    newItem.colorOfItem = sf::Color::Magenta;
    newItem.stackableItem = false;
    newItem.maxStack = 1;

    listOfTypeOfItem[newItemType] = newItem;

    newItemType.type = BULLET;
    newItemType.nameOfItem = "BASIC_WEAPON";
    newItem.sizeOfItem = sf::Vector2f(2 * 40, 2 * 40);
    newItem.colorOfItem = sf::Color::Red;
    newItem.stackableItem = true;
    newItem.maxStack = 100;

    listOfTypeOfItem[newItemType] = newItem;

    newItemType.type = WEAPON;
    newItemType.nameOfItem = "FULL_WEAPON";
    newItem.sizeOfItem = sf::Vector2f(1 * 40, 7 * 40);
    newItem.colorOfItem = sf::Color::Green;
    newItem.stackableItem = false;
    newItem.maxStack = 1;

    listOfTypeOfItem[newItemType] = newItem;

    newItemType.type = BULLET;
    newItemType.nameOfItem = "FULL_WEAPON";
    newItem.sizeOfItem = sf::Vector2f(2 * 40, 2 * 40);
    newItem.colorOfItem = sf::Color::Cyan;
    newItem.stackableItem = true;
    newItem.maxStack = 10000;

    listOfTypeOfItem[newItemType] = newItem;

    newItemType.type = WEAPON;
    newItemType.nameOfItem = "SHOTGUN_WEAPON";
    newItem.sizeOfItem = sf::Vector2f(2 * 40, 4 * 40);
    newItem.colorOfItem = sf::Color(128, 30, 0);
    newItem.stackableItem = false;
    newItem.maxStack = 1;

    listOfTypeOfItem[newItemType] = newItem;

    newItemType.type = BULLET;
    newItemType.nameOfItem = "SHOTGUN_WEAPON";
    newItem.sizeOfItem = sf::Vector2f(2 * 40, 2 * 40);
    newItem.colorOfItem = sf::Color(174, 0, 255);
    newItem.stackableItem = true;
    newItem.maxStack = 80;

    listOfTypeOfItem[newItemType] = newItem;

    newItemType.type = USEABLE_ITEM;
    newItemType.nameOfItem = "GOLD_KEY";
    newItem.sizeOfItem = sf::Vector2f(1 * 40, 2 * 40);
    newItem.colorOfItem = sf::Color::Yellow;
    newItem.stackableItem = false;
    newItem.maxStack = 1;

    listOfTypeOfItem[newItemType] = newItem;

    newItemType.type = USEABLE_ITEM;
    newItemType.nameOfItem = "NOTHING";
    newItem.sizeOfItem = sf::Vector2f(0, 0);
    newItem.colorOfItem = sf::Color::Transparent;
    newItem.stackableItem = false;
    newItem.maxStack = 0;

    listOfTypeOfItem[newItemType] = newItem;
}

void itemClass::update()
{
    verticalVelocity += weight;
}

void itemClass::draw(sf::RenderWindow& window)
{
    window.draw(sprite);

    if(stackableItem == true && isOnTheGround == false)
    {
        window.draw(numberOfItemText);
    }
}

int itemClass::applyVerticalMove()
{
    sprite.move(0, verticalVelocity);

    return verticalVelocity;
}

void itemClass::hasEnterInCollide()
{
    verticalVelocity = 0;
}

bool itemClass::contains(int x, int y)
{
    return sprite.getGlobalBounds().contains(x, y);
}

void itemClass::moveItem(int x, int y)
{
    sprite.move(x, y);
    setTextPositionWithThisNumber();
}

void itemClass::turnItem()
{
    if(isTurn == false)
    {
        sprite.setOrigin(0, sprite.getSize().y);
        sprite.rotate(90);
        isTurn = true;
    }
    else
    {
        sprite.setOrigin(0, 0);
        sprite.rotate(-90);
        isTurn = false;
    }

    setTextPositionWithThisNumber();
}

int itemClass::addNumberOfItem(int newNumber)
{
    if(numberOfItem + newNumber <= maxStack)
    {
        numberOfItem += newNumber;
        newNumber = 0;
    }
    else
    {
        newNumber = numberOfItem + newNumber - maxStack;
        numberOfItem = maxStack;
    }

    setTextPositionWithThisNumber(numberOfItem);

    return newNumber;
}

bool itemClass::isStackableItem()
{
    return stackableItem;
}

sf::Vector2f itemClass::getPosition()
{
    return sprite.getPosition();
}

sf::FloatRect itemClass::getCollideBox()
{
    return sprite.getGlobalBounds();
}

itemTypeInfo itemClass::getTypeOfItem()
{
    return infoForItem;
}

int itemClass::getNumberOfItem()
{
    return numberOfItem;
}

bool itemClass::getIsTurn()
{
    return isTurn;
}

void itemClass::setTextPositionWithThisNumber(int number)
{
    if(number != -1)
    {
        numberOfItemText.setString(utilitiesClass::intToString(number));
    }

    numberOfItemText.setPosition(sprite.getGlobalBounds().left + sprite.getGlobalBounds().width - numberOfItemText.getGlobalBounds().width - 2,
                                 sprite.getGlobalBounds().top + sprite.getGlobalBounds().height - numberOfItemText.getGlobalBounds().height - 2);
}

void itemClass::setPosition(int x, int y)
{
    sprite.setPosition(x, y);
    setTextPositionWithThisNumber();
}

void itemClass::setAlphaColor(int newAlphaColor)
{
    sprite.setFillColor(sf::Color(sprite.getFillColor().r, sprite.getFillColor().g, sprite.getFillColor().b, newAlphaColor));
    numberOfItemText.setColor(sf::Color(numberOfItemText.getColor().r, numberOfItemText.getColor().g, numberOfItemText.getColor().b, newAlphaColor));
}

void itemClass::setNumberOfItem(int newNumber)
{
    if(newNumber <= maxStack)
    {
        numberOfItem = newNumber;
    }
    else
    {
        numberOfItem = maxStack;
    }

    setTextPositionWithThisNumber(numberOfItem);
}

void itemClass::setIsOnTheGround(bool newValue)
{
    if(newValue != isOnTheGround)
    {
        isOnTheGround = newValue;

        if(isOnTheGround == true)
        {
            if(isTurn == true)
            {
                turnItem();
            }
            sprite.setSize(sf::Vector2f(sprite.getSize().x / 3, sprite.getSize().y / 3));
        }
        else
        {
            sprite.setSize(originalSize);
        }
    }
}
