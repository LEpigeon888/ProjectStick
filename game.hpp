#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>

#include "gameStateStack.hpp"

/*
Classe: gameClass
Parents Aucun.
Abstraite: Non.
Utilit�: C'est une classe qui s'occupe d'appeler les fonctions update et draw (dans cet ordre) de la derni�re gameState se trouvant
dans le gameStateStack et d'appeler les diff�rente fonction d'initialisation dont le programme a besoin.
*/

class gameClass
{
public:
	gameClass();
	void run();
private:
    gameStateStackClass gameStateStack;
    sf::RenderWindow window;
};

#endif
