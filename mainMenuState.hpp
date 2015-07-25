#ifndef MAINMENUSTATE_HPP
#define MAINMENUSTATE_HPP

#include <array>

#include "gameState.hpp"
#include "baseMenu.hpp"
#include "global.hpp"

/*
Classe: mainMenuStateClass
Parents: gameStateClass
Abstraite: Non.
Utilité: C'est une classe qui représente le menu principal du jeu, elle permet d'accéder à la phase de jeu et à l'éditeur de niveau.
*/

class mainMenuStateClass : public gameStateClass
{
public:
    mainMenuStateClass();
    void update(sf::RenderWindow& window);
    void draw(sf::RenderWindow& window);
    bool loadTheSave();
    void pressTuto();
    void pressPlay();
    void pressLoad();
    void pressEditLevel();
    void pressLeave();
private:
    std::string nameOfSaveLevel;
    baseMenuClass baseMenu;
    std::array<sf::Keyboard::Key, NUMBER_OF_KEY> secret;
    sf::Clock keyPressTime;
    sf::Color clearColor;
    int offset;
    bool leave;
};

#endif

