#ifndef PAUSEMENUSTATE_HPP
#define PAUSEMENUSTATE_HPP

#include "gameState.hpp"
#include "baseMenu.hpp"
#include "global.hpp"

/*
Classe: pauseMenuStateClass
Parents: gameStateClass
Abstraite: Non.
Utilité: C'est une classe qui représente le menu pause du jeu, elle permet de retourner au menu ou de continuer de jouer.
*/

class pauseMenuStateClass : public gameStateClass
{
public:
    pauseMenuStateClass();
    void update(sf::RenderWindow& window);
    void draw(sf::RenderWindow& window);
    void pressMainMenu();
    void pressBackInGame();
    void pressLeave();
private:
    baseMenuClass baseMenu;
    bool leave;
};

#endif
