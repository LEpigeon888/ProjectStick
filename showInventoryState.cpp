#include "showInventoryState.hpp"
#include "global.hpp"

showInventoryStateClass::showInventoryStateClass(inventoryClass* newInventory, sf::Vector2f newPositionOfFootOfPlayer, int newFloorNumber, std::list<itemClass>::iterator newItemToTake)
{
    backgroundTexture.loadFromFile("resource/image/bginventory.png");
    backgroundSprite.setTexture(backgroundTexture);

    backgroundSprite.setOrigin(backgroundTexture.getSize().x / 2, backgroundTexture.getSize().y / 2);
    backgroundSprite.setPosition(WIDTH_SCREEN / 2, HEIGHT_SCREEN / 2);

    positionOfFootOfPlayer = newPositionOfFootOfPlayer;
    inventory = newInventory;
    inventory->setPositionOfBGInventory(backgroundSprite.getGlobalBounds().left + 1, backgroundSprite.getGlobalBounds().top + 1);

    floorNumber = newFloorNumber;

    if(floorNumber != -1)
    {
        itemIsTakingFromTheGround = true;
        itemTryToBeAutoPlaced = true;
        itemToTake = newItemToTake;
        inventory->setThisItemHasTmpItem(*itemToTake);
        inventory->setItemSelectedCanBeDeleted(false);
        inventory->autoPlaceSelectedItem();
    }
    else
    {
        itemIsTakingFromTheGround = false;
        itemTryToBeAutoPlaced = false;
    }
}

void showInventoryStateClass::update(sf::RenderWindow& window)
{
    sf::Event event;

    if(itemTryToBeAutoPlaced == true)
    {
        if(inventory->getItemSelectedIsPlaced() == true)
        {
            global::activeLevel->getListOfFloor()->at(floorNumber).listOfItem.erase(itemToTake);
            inventory->escapPressed();
            global::activeGameStateStack->pop();
            return;
        }

        itemTryToBeAutoPlaced = false;
    }

    while(window.pollEvent(event))
    {
        if(event.type == sf::Event::Closed)
        {
            window.close();
        }
        else if(event.type == sf::Event::KeyPressed)
        {
            if(event.key.code == sf::Keyboard::Escape)
            {
                inventory->escapPressed();
            }
            else if(event.key.code == sf::Keyboard::Return)
            {
                inventory->returnPressed();
            }
            else if(event.key.code == sf::Keyboard::Delete)
            {
                if(inventory->getItemSelectedCanBeDeleted() == true)
                {
                    global::activeLevel->addItemAtThisPoint(inventory->getItemAndRemoveItFromInventory(), positionOfFootOfPlayer);
                }
            }
            else if(event.key.code == sf::Keyboard::T)
            {
                inventory->turnSelectedItem();
            }
            else if(event.key.code == sf::Keyboard::P)
            {
                inventory->autoPlaceSelectedItem();
            }
            else if(event.key.code == sf::Keyboard::Z)
            {
                inventory->moveSelectedItem(0, -1);
            }
            else if(event.key.code == sf::Keyboard::Q)
            {
                inventory->moveSelectedItem(-1, 0);
            }
            else if(event.key.code == sf::Keyboard::S)
            {
                inventory->moveSelectedItem(0, 1);
            }
            else if(event.key.code == sf::Keyboard::D)
            {
                inventory->moveSelectedItem(1, 0);
            }
            else if(event.key.code == sf::Keyboard::I)
            {
                inventory->escapPressed();
                global::activeGameStateStack->pop();
                return;
            }
        }
        else if(event.type == sf::Event::MouseButtonPressed)
        {
            if(event.mouseButton.button == sf::Mouse::Left)
            {
                inventory->leftClick(event.mouseButton.x, event.mouseButton.y);
            }
        }

        if(itemIsTakingFromTheGround == true)
        {
            if(inventory->getItemSelectedIsPlaced() == true)
            {
                global::activeLevel->getListOfFloor()->at(floorNumber).listOfItem.erase(itemToTake);
                itemIsTakingFromTheGround = false;
            }
            else if(inventory->getItemSelectedIsCanceled() == true)
            {
                itemIsTakingFromTheGround = false;
            }
        }
    }
}

void showInventoryStateClass::draw(sf::RenderWindow& window)
{
    window.setView(window.getDefaultView());
    window.clear(sf::Color::White);
    window.draw(backgroundSprite);
    inventory->draw(window);
}
