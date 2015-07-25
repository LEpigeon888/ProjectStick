#ifndef PLAYSTATE_HPP
#define PLAYSTATE_HPP

#include "gameState.hpp"
#include "gamePlayed.hpp"

/*
Classe: playStateClass
Parents: gameStateClass
Abstraite: Non.
Utilit�: C'est une classe qui g�re l'int�raction entre entre le joueur et la phase de jeu active.
*/

class playStateClass : public gameStateClass
{
public:
    playStateClass(std::string newLevelName);
    void update(sf::RenderWindow& window);
    void draw(sf::RenderWindow& window);
    void saveInfoAndGoToNextLevel(std::string nextLevel);
private:
    gamePlayedClass gamePlayed;
    std::string currentLevelName;
};

#endif
