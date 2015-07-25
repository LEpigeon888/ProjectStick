#include <fstream>

#include "playState.hpp"
#include "showInventoryState.hpp"
#include "editLevelState.hpp"
#include "radarState.hpp"
#include "endLevelState.hpp"
#include "pauseMenuState.hpp"
#include "global.hpp"

playStateClass::playStateClass(std::string newLevelName)
{
    currentLevelName = newLevelName;
    gamePlayed.setLevel(currentLevelName);
}

void playStateClass::update(sf::RenderWindow& window)
{
    sf::Event event;

    while(window.pollEvent(event))
    {
        if(event.type == sf::Event::Closed)
        {
            window.close();
            return;
        }
        else if(event.type == sf::Event::KeyPressed)
        {
            if(event.key.code == sf::Keyboard::Space)
            {
                gamePlayed.jumpPlayer();
            }
            else if(event.key.code == sf::Keyboard::I)
            {
                global::activeGameStateStack->add(new showInventoryStateClass(gamePlayed.getPlayerInventory(), gamePlayed.getPlayerFootPosition()));
                return;
            }
            else if(event.key.code == sf::Keyboard::R)
            {
                gamePlayed.playerReload();
            }
            else if(event.key.code == sf::Keyboard::Z)
            {
                gamePlayed.playerUseSomething();
            }
            else if(event.key.code == sf::Keyboard::E)
            {
                std::list<itemClass>::iterator itemToTake;
                int floorNumber = gamePlayed.playerTakeItem(itemToTake);

                if(floorNumber >= 0)
                {
                    global::activeGameStateStack->add(new showInventoryStateClass(gamePlayed.getPlayerInventory(), gamePlayed.getPlayerFootPosition(), floorNumber, itemToTake));
                    return;
                }
            }
            else if(event.key.code == sf::Keyboard::Tab)
            {
                global::activeGameStateStack->add(new radarStateClass(gamePlayed.getListOfFloor(), gamePlayed.getPlayer(), gamePlayed.getSizeOfLevel(), currentLevelName));
                return;
            }
            else if(event.key.code == sf::Keyboard::Escape)
            {
                global::activeGameStateStack->add(new pauseMenuStateClass());
                return;
            }
            else if(event.key.code == sf::Keyboard::Num1)
            {
                gamePlayed.selectWeapon(1);
            }
            else if(event.key.code == sf::Keyboard::Num2)
            {
                gamePlayed.selectWeapon(2);
            }
            else if(event.key.code == sf::Keyboard::Num3)
            {
                gamePlayed.selectWeapon(3);
            }
            else if(event.key.code == sf::Keyboard::B)
            {
                window.setFramerateLimit(60);
            }
            else if(event.key.code == sf::Keyboard::N)
            {
                window.setFramerateLimit(0);
            }
            else if(event.key.code == sf::Keyboard::V)
            {
                window.setVerticalSyncEnabled(true);
            }
            else if(event.key.code == sf::Keyboard::C)
            {
                window.setVerticalSyncEnabled(false);
            }
        }
        else if(event.type == sf::Event::MouseButtonPressed)
        {
            if(event.mouseButton.button == sf::Mouse::Left)
            {
                gamePlayed.playerShoot(false);
            }
        }
    }

    if(!((sf::Keyboard::isKeyPressed(sf::Keyboard::Q) == true && sf::Keyboard::isKeyPressed(sf::Keyboard::D) == false) || (sf::Keyboard::isKeyPressed(sf::Keyboard::Q) == false && sf::Keyboard::isKeyPressed(sf::Keyboard::D) == true)))
    {
        gamePlayed.movePlayerTo(NOWHERE);
    }
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
    {
        gamePlayed.movePlayerTo(LEFT);
    }
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        gamePlayed.movePlayerTo(RIGHT);
    }

    if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        gamePlayed.playerShoot(true);
    }

    gamePlayed.update();

    if(gamePlayed.playerHasLost() == true)
    {
        global::activeGameStateStack->set(new endLevelClass(false, currentLevelName));
        return;
    }
    else if(gamePlayed.playerHasWon() == true)
    {
        saveInfoAndGoToNextLevel(gamePlayed.getNextLevel());
        return;
    }

    gamePlayed.playerLookAt(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);
}

void playStateClass::draw(sf::RenderWindow& window)
{
    window.clear(sf::Color::Black);
    gamePlayed.draw(window);
}

void playStateClass::saveInfoAndGoToNextLevel(std::string nextLevel)
{
    std::ofstream file;
    file.open("save/saveinfo.txt");

    file << nextLevel;

    file.close();

    global::activeGameStateStack->set(new endLevelClass(true, nextLevel));
}
