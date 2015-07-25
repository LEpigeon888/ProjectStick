#include <fstream>

#include "mainMenuState.hpp"
#include "playState.hpp"
#include "editLevelState.hpp"

mainMenuStateClass::mainMenuStateClass()
{
    nameOfSaveLevel = "tuto1";
    clearColor = sf::Color::White;
    leave = false;

    offset = 0;
    secret[0] = sf::Keyboard::Up;
    secret[1] = sf::Keyboard::Up;
    secret[2] = sf::Keyboard::Down;
    secret[3] = sf::Keyboard::Down;
    secret[4] = sf::Keyboard::Left;
    secret[5] = sf::Keyboard::Right;
    secret[6] = sf::Keyboard::Left;
    secret[7] = sf::Keyboard::Right;
    secret[8] = sf::Keyboard::B;
    secret[9] = sf::Keyboard::A;

    baseMenu.addButton(0, 0, "Commencer le tutoriel");
    baseMenu.setLastButttonClickable(std::bind(&mainMenuStateClass::pressTuto, this));
    baseMenu.centerXLastButton();
    baseMenu.addButton(0, 0, "Nouvelle partie");
    baseMenu.setLastButttonClickable(std::bind(&mainMenuStateClass::pressPlay, this));
    baseMenu.centerXLastButton();
    baseMenu.addButton(0, 0, "Charger la sauvegarde");
    baseMenu.setLastButttonClickable(std::bind(&mainMenuStateClass::pressLoad, this));
    baseMenu.centerXLastButton();
    baseMenu.addButton(0, 0, "Editeur de niveau");
    baseMenu.setLastButttonClickable(std::bind(&mainMenuStateClass::pressEditLevel, this));
    baseMenu.centerXLastButton();
    baseMenu.addButton(0, 0, "Quitter");
    baseMenu.setLastButttonClickable(std::bind(&mainMenuStateClass::pressLeave, this));
    baseMenu.centerXLastButton();
    baseMenu.centerYAllButton(20);
    baseMenu.addButton(15, 15, "Project : Stick");

    if(global::easterEggEnable == true)
    {
        baseMenu.addButton(15, 15, "HYPER MODE ACTIF");
        baseMenu.reversePositionOfLastButton(true, true);
    }

    loadTheSave();
}

void mainMenuStateClass::update(sf::RenderWindow& window)
{
    sf::Event event;

    while(window.pollEvent(event))
    {
        if(event.type == sf::Event::Closed)
        {
            window.close();
            return;
        }
        else if(event.type == sf::Event::MouseButtonPressed)
        {
            if(event.mouseButton.button == sf::Mouse::Left)
            {
                baseMenu.playerClickHere(event.mouseButton.x, event.mouseButton.y);
            }
        }
        else if(event.type == sf::Event::MouseMoved)
        {
            baseMenu.playerMoveMouseHere(event.mouseMove.x, event.mouseMove.y);
        }
        else if(event.type == sf::Event::KeyPressed)
        {
            if(secret[offset] != event.key.code)
            {
                offset = 0;
            }
            if(secret[offset] == event.key.code)
            {
                ++offset;
                if(offset == NUMBER_OF_KEY)
                {
                    global::easterEggEnable = !global::easterEggEnable;

                    if(global::easterEggEnable == true)
                    {
                        baseMenu.addButton(15, 15, "HYPER MODE ACTIF");
                        baseMenu.reversePositionOfLastButton(true, true);
                    }
                    else
                    {
                        baseMenu.removeLastButton();
                    }

                    offset = 0;
                }
            }
            keyPressTime.restart();
        }

        if(global::activeGameStateStack->getChange() == true)
        {
            return;
        }
    }

    if(leave == true)
    {
        window.close();
        return;
    }

    if(keyPressTime.getElapsedTime().asSeconds() > TIME_FOR_KEY_PRESS)
    {
        offset = 0;
        keyPressTime.restart();
    }

    if(global::easterLaunchEgg == true)
    {
        switch(std::rand() % 7)
        {
            case 0:
            {
                clearColor = sf::Color::Yellow;
                break;
            }
            case 1:
            {
                clearColor = sf::Color::Yellow;
                break;
            }
            case 2:
            {
                clearColor = sf::Color::Blue;
                break;
            }
            case 3:
            {
                clearColor = sf::Color::Cyan;
                break;
            }
            case 4:
            {
                clearColor = sf::Color::Green;
                break;
            }
            case 5:
            {
                clearColor = sf::Color::Magenta;
                break;
            }
            case 6:
            {
                clearColor = sf::Color::Red;
                break;
            }
            default:
            {
                clearColor = sf::Color::White;
                break;
            }
        }
    }
}

void mainMenuStateClass::draw(sf::RenderWindow& window)
{
    window.setView(window.getDefaultView());
    window.clear(clearColor);

    baseMenu.draw(window);
}

bool mainMenuStateClass::loadTheSave()
{
    std::string currentLine;
    std::ifstream file;
    file.open("save/saveinfo.txt");

    if(file.is_open() == true)
    {
        std::getline(file, currentLine);
        nameOfSaveLevel = currentLine;

        return true;
    }

    return false;
}

void mainMenuStateClass::pressTuto()
{
    global::activeGameStateStack->set(new playStateClass("tuto1"));
}

void mainMenuStateClass::pressPlay()
{
    global::activeGameStateStack->set(new playStateClass("level1"));
}

void mainMenuStateClass::pressLoad()
{
    global::activeGameStateStack->set(new playStateClass(nameOfSaveLevel));
}

void mainMenuStateClass::pressEditLevel()
{
    global::activeGameStateStack->set(new editLevelStateClass("level1"));
}

void mainMenuStateClass::pressLeave()
{
    leave = true;
}
