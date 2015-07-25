#ifndef ENDLEVELSTATE_HPP
#define ENDLEVELSTATE_HPP

#include "gameState.hpp"
#include "baseMenu.hpp"

#include <SFML/Graphics.hpp>

/*
Classe: endLevelStateClass
Parents: gameStateClass
Abstraite: Non.
Utilit�: C'est une gameState qui s'occupe d'afficher l'�cran de fin, avec un message diff�rent selon si le joueur a perdu ou
gagn� le niveau, elle s'occupe aussi de changer de niveau apr�s que l'�cran de fin soit quitt�.
*/

class endLevelClass : public gameStateClass
{
public:
    endLevelClass(bool victory, std::string newNextLevel);
    void update(sf::RenderWindow& window);
    void draw(sf::RenderWindow& window);
    void pressPlay();
    void pressMainMenu();
    void pressLeave();
private:
    std::string nextLevel;
    baseMenuClass baseMenu;
    bool leave;
};

#endif
