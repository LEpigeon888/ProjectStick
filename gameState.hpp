#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

#include <SFML/Graphics.hpp>

/*
Classe: gameStateClass
Parents: Aucun.
Abstraite: Oui.
Utilité: C'est la classe de base pour toutes les autres gameState, une gameState et la phase du jeu (menu, éditeur de niveau, radar, etc),
ses fonction de base sont update et draw, la premère met à jour le gameState et la deuxième qui dessine à l'écran ce dont il a besoin.
*/

class gameStateStackClass;

class gameStateClass
{
public:
	virtual ~gameStateClass() {};
	virtual void update(sf::RenderWindow& window) = 0;
	virtual void draw(sf::RenderWindow& window) = 0;
};

#endif
