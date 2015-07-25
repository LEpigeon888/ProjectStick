#include "endLevelState.hpp"
#include "playState.hpp"
#include "mainMenuState.hpp"
#include "global.hpp"

endLevelClass::endLevelClass(bool victory, std::string newNextLevel)
{
    nextLevel = newNextLevel;
    leave = false;

    if(victory == true)
    {
        baseMenu.addButton(0, 0, "Continuer");
    }
    else
    {
        baseMenu.addButton(0, 0, "Rejouer");
    }

    baseMenu.setLastButttonClickable(std::bind(&endLevelClass::pressPlay, this));
    baseMenu.centerXLastButton();
    baseMenu.addButton(0, 0, "Retourner au menu principal");
    baseMenu.setLastButttonClickable(std::bind(&endLevelClass::pressMainMenu, this));
    baseMenu.centerXLastButton();
    baseMenu.addButton(0, 0, "Quitter");
    baseMenu.setLastButttonClickable(std::bind(&endLevelClass::pressLeave, this));
    baseMenu.centerXLastButton();
    baseMenu.centerYAllButton(20);
    if(victory == true)
    {
        baseMenu.addButton(15, 15, "Bravo !!!");
    }
    else
    {
        baseMenu.addButton(15, 15, "Perdu...");
    }
}

void endLevelClass::update(sf::RenderWindow& window)
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
}

void endLevelClass::draw(sf::RenderWindow& window)
{
    window.setView(window.getDefaultView());
    window.clear(sf::Color::White);
    baseMenu.draw(window);
}

void endLevelClass::pressPlay()
{
    global::activeGameStateStack->set(new playStateClass(nextLevel));
}

void endLevelClass::pressMainMenu()
{
    global::activeGameStateStack->set(new mainMenuStateClass());
}

void endLevelClass::pressLeave()
{
    leave = true;
}
