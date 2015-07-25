#ifndef STICKMAN_HPP
#define STICKMAN_HPP

#include <list>
#include <SFML/Graphics.hpp>

#include "weapon.hpp"
#include "item.hpp"

/*
Classe: stickmanClass
Parents: Aucun.
Abstraite: Oui.
Utilité: C'est une classe de base pour tout les différents stickmen (le joueur, les ennemis et le témoins).
*/

enum direction {UP = 0, RIGHT, DOWN, LEFT, NOWHERE};

class stickmanClass
{
public:
    stickmanClass();
    virtual ~stickmanClass() {}
    virtual void draw(sf::RenderWindow& window) = 0;
    virtual int applyHorizontalMove() = 0;
    virtual int applyVerticalMove() = 0;
    virtual bool damaged(int valueOfDamage) = 0;
    virtual void hasEnterInCollide(direction dir) = 0;
    virtual void lookAt(int x, int y, int cameraX, int cameraY) = 0;
    virtual bool hasThisItem(typeOfItem itemType, std::string nameOfItem) = 0;
    virtual sf::FloatRect getCollideBox();
    virtual sf::FloatRect getSpriteBox();
    virtual sf::Vector2f getFootPosition();
    virtual bool getIsHidden() = 0;
    virtual void setPosition(int x, int y) = 0;
    virtual void setIsHidden(bool newVal) = 0;
protected:
    sf::RectangleShape sprite;
    int horizontalVelocity;
    int verticalVelocity;
    int weight;
    int speed;
    int life;
};

#endif
