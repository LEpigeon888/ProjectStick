#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

#include <SFML/Graphics.hpp>

/*
Classe: gameStateClass
Parents: Aucun.
Abstraite: Oui.
Utilit�: C'est la classe de base pour toutes les autres gameState, une gameState et la phase du jeu (menu, �diteur de niveau, radar, etc),
ses fonction de base sont update et draw, la prem�re met � jour le gameState et la deuxi�me qui dessine � l'�cran ce dont il a besoin.
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
