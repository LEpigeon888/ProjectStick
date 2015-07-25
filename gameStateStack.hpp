#ifndef GAMESTATESTACK_HPP
#define GAMESTATESTACK_HPP

#include <list>
#include <memory>
#include <SFML/Graphics.hpp>

#include "gameState.hpp"

/*
Classe: gameStateStackClass
Parents: Aucun.
Abstraite: Non.
Utilité: C'est la classe qui contient toutes les gameState qui sont actuellement active et qui s'occupera d'apeller update et draw de la dernière
gameState appelé quand la classe principale du jeu le lui demandera.
*/

class gameStateStackClass
{
public:
    gameStateStackClass();
	void set(gameStateClass* state);
	void add(gameStateClass* state);
	void pop();
	void update(sf::RenderWindow& window);
	void draw(sf::RenderWindow& window);
    void oldUpdate(sf::RenderWindow& window);
    void oldDraw(sf::RenderWindow& window);
	bool getChange();
	bool getPreviouslyChange();
	void setChange(bool newChange);
private:
	std::list<std::unique_ptr<gameStateClass>> stateList;
	bool change;
	bool previouslyChange;
};

#endif
