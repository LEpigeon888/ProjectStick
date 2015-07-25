#ifndef BASEMENU_HPP
#define BASEMENU_HPP

#include <SFML/Graphics.hpp>
#include <functional>
#include <list>

/*
Classe: baseMenuClass
Parents: Aucun.
Abstraite: Non.
Utilité: C'est une classe qui sera instancié dans chaque classe de menu du jeu, c'est une base de menu.
*/

struct buttonStruct
{
    sf::Text buttonText;
    std::function<void(void)> slot;
    bool isAClickableButton;
};

class baseMenuClass
{
public:
    void draw(sf::RenderWindow& window);
    void addButton(int x, int y, std::string text);
    void setLastButttonClickable(std::function<void(void)> newSlot);
    void centerXLastButton();
    void centerYAllButton(int spaceBetweenButton);
    void reversePositionOfLastButton(bool inXAxe, bool inYAxe);
    void removeLastButton();
    void playerClickHere(int x, int y);
    void playerMoveMouseHere(int x, int y);
private:
    std::list<buttonStruct> listOfButton;
};

#endif
