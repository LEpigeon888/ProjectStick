#include <utility>

#include "inventory.hpp"

inventoryClass::inventoryClass()
{
    itemIsSelected = false;
    itemSelectedIsTurn = false;
    itemSelectedCanBeDeleted = true;
    itemSelectedIsPlaced = false;
    itemSelectedIsCanceled = false;
    positionOfBGInventory.x = 0;
    positionOfBGInventory.y = 0;
    sizeOfInventory.x = 20;
    sizeOfInventory.y = 10;
    sizeOfCase = 40;
}

void inventoryClass::draw(sf::RenderWindow& window)
{
    for(itemClass& thisItem : listOfItem)
    {
        thisItem.draw(window);
    }

    if(itemIsSelected == true)
    {
        tmpItem.draw(window);
    }
}

void inventoryClass::leftClick(int x, int y)
{
    if(itemIsSelected == false)
    {
        for(auto itemIte = listOfItem.begin(); itemIte != listOfItem.end(); )
        {
            if(itemIte->contains(x, y) == true)
            {
                tmpItem = std::move(*itemIte);
                tmpItem.setAlphaColor(100);
                itemIsSelected = true;
                itemSelectedIsTurn = false;
                oldPositionOfItemSelected = tmpItem.getPosition();

                listOfItem.erase(itemIte++);
                break;
            }
            ++itemIte;
        }
    }
}

void inventoryClass::escapPressed()
{
    if(itemIsSelected == true)
    {
        cancelTmpItem();
    }
}

void inventoryClass::returnPressed()
{
    bool cantPlaceHere = false;
    if(itemIsSelected)
    {
        for(itemClass& thisItem : listOfItem)
        {
            if((static_cast<int>(thisItem.getCollideBox().top) < static_cast<int>(tmpItem.getCollideBox().top + tmpItem.getCollideBox().height)) &&
               (static_cast<int>(thisItem.getCollideBox().top + thisItem.getCollideBox().height) > static_cast<int>(tmpItem.getCollideBox().top)) &&
               (static_cast<int>(thisItem.getCollideBox().left) < static_cast<int>(tmpItem.getCollideBox().left + tmpItem.getCollideBox().width)) &&
               ((static_cast<int>(thisItem.getCollideBox().left) + thisItem.getCollideBox().width) > static_cast<int>(tmpItem.getCollideBox().left)))
            {

                if(thisItem.getTypeOfItem().type == tmpItem.getTypeOfItem().type && thisItem.getTypeOfItem().nameOfItem == tmpItem.getTypeOfItem().nameOfItem && thisItem.isStackableItem() == true)
                {
                    int restOfItem = thisItem.addNumberOfItem(tmpItem.getNumberOfItem());

                    if(restOfItem == 0)
                    {
                        tmpItem.setPosition(-1, -1);
                        placeTmpItem();
                    }
                    else
                    {
                        tmpItem.setNumberOfItem(restOfItem);
                    }

                    return;
                }
                else
                {
                    cantPlaceHere = true;
                }
            }
        }

        if(cantPlaceHere == false)
        {
            placeTmpItem();
        }
    }
}

void inventoryClass::moveSelectedItem(int x, int y)
{
    if(itemIsSelected == true)
    {
        tmpItem.moveItem(x * sizeOfCase, y * sizeOfCase);

        if(tmpItem.getCollideBox().left < positionOfBGInventory.x)
        {
            tmpItem.setPosition(positionOfBGInventory.x, tmpItem.getPosition().y);
        }
        else if(tmpItem.getCollideBox().left + tmpItem.getCollideBox().width > positionOfBGInventory.x + (sizeOfInventory.x * sizeOfCase))
        {
            tmpItem.setPosition(positionOfBGInventory.x + (sizeOfInventory.x * sizeOfCase) - tmpItem.getCollideBox().width, tmpItem.getPosition().y);
        }

        if(tmpItem.getCollideBox().top < positionOfBGInventory.y)
        {
            tmpItem.setPosition(tmpItem.getPosition().x, positionOfBGInventory.y);
        }
        else if(tmpItem.getCollideBox().top + tmpItem.getCollideBox().height > positionOfBGInventory.y + (sizeOfInventory.y * sizeOfCase))
        {
            tmpItem.setPosition(tmpItem.getPosition().x, positionOfBGInventory.y + (sizeOfInventory.y * sizeOfCase) - tmpItem.getCollideBox().height);
        }
    }
}

void inventoryClass::turnSelectedItem()
{
    if(itemIsSelected == true)
    {
        tmpItem.turnItem();
        itemSelectedIsTurn = !itemSelectedIsTurn;
        moveSelectedItem(0, 0);
    }
}

void inventoryClass::autoPlaceSelectedItem()
{
    bool canPlaceItem = true;
    sf::Vector2f oldPos = tmpItem.getPosition();

    if(itemIsSelected)
    {
        for(itemClass& thisItem : listOfItem)
        {
            if(thisItem.getTypeOfItem().type == tmpItem.getTypeOfItem().type && thisItem.getTypeOfItem().nameOfItem == tmpItem.getTypeOfItem().nameOfItem && thisItem.isStackableItem() == true)
            {
                int restOfItem = thisItem.addNumberOfItem(tmpItem.getNumberOfItem());

                if(restOfItem == 0)
                {
                    tmpItem.setPosition(-1, -1);
                    placeTmpItem();
                    return;
                }
                else
                {
                    tmpItem.setNumberOfItem(restOfItem);
                }
            }
        }

        tmpItem.setPosition(positionOfBGInventory.x, positionOfBGInventory.y);

        while(tmpItem.getCollideBox().top + tmpItem.getCollideBox().height <= positionOfBGInventory.y + (sizeOfInventory.y * sizeOfCase))
        {
            canPlaceItem = true;
            for(itemClass& thisItem : listOfItem)
            {
                if(thisItem.getCollideBox().intersects(tmpItem.getCollideBox()) == true)
                {
                    canPlaceItem = false;
                    tmpItem.setPosition(thisItem.getCollideBox().left + thisItem.getCollideBox().width, tmpItem.getPosition().y);

                    if(tmpItem.getCollideBox().left + tmpItem.getCollideBox().width > positionOfBGInventory.x + (sizeOfInventory.x * sizeOfCase))
                    {
                        tmpItem.setPosition(positionOfBGInventory.x, tmpItem.getPosition().y + sizeOfCase);
                    }

                    break;
                }
            }

            if(canPlaceItem == true)
            {
                placeTmpItem();
                return;
            }
        }

        tmpItem.setPosition(oldPos.x, oldPos.y);
    }
}

void inventoryClass::placeTmpItem()
{
    if(tmpItem.getPosition().x != -1)
    {
        tmpItem.setAlphaColor(255);
        listOfItem.push_back(std::move(tmpItem));
    }

    itemIsSelected = false;
    itemSelectedCanBeDeleted = true;
    itemSelectedIsPlaced = true;
}

void inventoryClass::cancelTmpItem()
{
    if(itemSelectedIsTurn == true)
    {
        tmpItem.turnItem();
    }

    if(oldPositionOfItemSelected.x != -1)
    {
        tmpItem.setPosition(oldPositionOfItemSelected.x, oldPositionOfItemSelected.y);
        tmpItem.setAlphaColor(255);
        listOfItem.push_back(std::move(tmpItem));
    }

    itemIsSelected = false;
    itemSelectedIsCanceled = true;
}

void inventoryClass::addThisItem(itemInInventoryStruct newItem)
{
    listOfItem.push_back(itemClass(newItem.type, newItem.name, newItem.number));

    if(newItem.isTurn == true)
    {
        listOfItem.back().turnItem();
    }

    listOfItem.back().setPosition(positionOfBGInventory.x + (sizeOfCase * newItem.posX), positionOfBGInventory.y + (sizeOfCase * newItem.posY));
}

void inventoryClass::clearInventory()
{
    cancelTmpItem();
    listOfItem.clear();
}

int inventoryClass::removeThisItem(typeOfItem type, std::string name, int number)
{
    for(auto itemIte = listOfItem.begin(); itemIte != listOfItem.end(); )
    {
        if(itemIte->getTypeOfItem().type == type && itemIte->getTypeOfItem().nameOfItem == name)
        {
            if(itemIte->getNumberOfItem() <= number)
            {
                number -= itemIte->getNumberOfItem();
                listOfItem.erase(itemIte++);
            }
            else
            {
                itemIte->setNumberOfItem(itemIte->getNumberOfItem() - number);
                number = 0;
                break;
            }
        }
        ++itemIte;
    }

    return number;
}

int inventoryClass::getNumberOfThisItem(typeOfItem type, std::string name)
{
    int numberOfItem = 0;

    for(itemClass& thisItem : listOfItem)
    {
        if(thisItem.getTypeOfItem().type == type && thisItem.getTypeOfItem().nameOfItem == name)
        {
            numberOfItem += thisItem.getNumberOfItem();
        }
    }

    return numberOfItem;
}

bool inventoryClass::getItemIsSelected()
{
    return itemIsSelected;
}

itemClass& inventoryClass::getItemAndRemoveItFromInventory()
{
    if(itemSelectedIsTurn == true)
    {
        tmpItem.turnItem();
    }

    tmpItem.setAlphaColor(255);
    itemIsSelected = false;

    return tmpItem;
}

bool inventoryClass::getItemSelectedCanBeDeleted()
{
    return itemSelectedCanBeDeleted;
}

bool inventoryClass::getItemSelectedIsPlaced()
{
    bool tmpValue = itemSelectedIsPlaced;

    itemSelectedIsPlaced = false;

    return tmpValue;
}

bool inventoryClass::getItemSelectedIsCanceled()
{
    bool tmpValue = itemSelectedIsCanceled;

    itemSelectedIsCanceled = false;

    return tmpValue;
}

std::list<itemInInventoryStruct> inventoryClass::getListOfItemInInventory()
{
    std::list<itemInInventoryStruct> listOfItemInInventory;
    itemInInventoryStruct newItemForListing;

    for(itemClass& thisItem : listOfItem)
    {
        newItemForListing.type = thisItem.getTypeOfItem().type;
        newItemForListing.name = thisItem.getTypeOfItem().nameOfItem;
        newItemForListing.number = thisItem.getNumberOfItem();
        newItemForListing.isTurn = thisItem.getIsTurn();
        newItemForListing.posX = (thisItem.getPosition().x - positionOfBGInventory.x) / sizeOfCase;
        newItemForListing.posY = (thisItem.getPosition().y - positionOfBGInventory.y) / sizeOfCase;

        listOfItemInInventory.push_back(newItemForListing);
    }

    return listOfItemInInventory;
}

void inventoryClass::setPositionOfBGInventory(int x, int y)
{
    for(itemClass& thisItem : listOfItem)
    {
        thisItem.moveItem(x - positionOfBGInventory.x, y - positionOfBGInventory.y);
    }

    positionOfBGInventory.x = x;
    positionOfBGInventory.y = y;
}

void inventoryClass::setThisItemHasTmpItem(itemClass& item)
{
    oldPositionOfItemSelected = sf::Vector2f(-1, -1);
    tmpItem = item;

    tmpItem.setIsOnTheGround(false);
    tmpItem.setPosition(positionOfBGInventory.x, positionOfBGInventory.y);
    tmpItem.setAlphaColor(100);

    itemIsSelected = true;
    itemSelectedIsTurn = false;
}

void inventoryClass::setItemSelectedCanBeDeleted(bool newValue)
{
    itemSelectedCanBeDeleted = newValue;
}
