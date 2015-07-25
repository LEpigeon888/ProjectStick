#include "pauseMenuState.hpp"
#include "mainMenuState.hpp"

pauseMenuStateClass::pauseMenuStateClass()
{
    leave = false;

    baseMenu.addButton(0, 0, "Retourner au menu principal");
    baseMenu.setLastButttonClickable(std::bind(&pauseMenuStateClass::pressMainMenu, this));
    baseMenu.centerXLastButton();
    baseMenu.addButton(0, 0, "Retourner en jeu");
    baseMenu.setLastButttonClickable(std::bind(&pauseMenuStateClass::pressBackInGame, this));
    baseMenu.centerXLastButton();
    baseMenu.addButton(0, 0, "Quitter");
    baseMenu.setLastButttonClickable(std::bind(&pauseMenuStateClass::pressLeave, this));
    baseMenu.centerXLastButton();
    baseMenu.centerYAllButton(20);
    baseMenu.addButton(15, 15, "Pause");
}

void pauseMenuStateClass::update(sf::RenderWindow& window)
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
            if(event.key.code == sf::Keyboard::Escape)
            {
                pressBackInGame();
            }
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

void pauseMenuStateClass::draw(sf::RenderWindow& window)
{
    window.setView(window.getDefaultView());
    window.clear(sf::Color::White);
    baseMenu.draw(window);
}

void pauseMenuStateClass::pressMainMenu()
{
    global::activeGameStateStack->set(new mainMenuStateClass());
}

void pauseMenuStateClass::pressBackInGame()
{
    global::activeGameStateStack->pop();
}

void pauseMenuStateClass::pressLeave()
{
    leave = true;
}

