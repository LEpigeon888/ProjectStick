#ifndef WITNESSSTICK_HPP
#define WITNESSSTICK_HPP

#include "stickman.hpp"

/*
Classe: witnessStickClass
Parents: stickmanClass
Utilité: C'est une classe qui représente un témoin.
*/

class witnessStickClass : public stickmanClass
{
public:
    witnessStickClass();
    void draw(sf::RenderWindow& window);
    int applyHorizontalMove();
    int applyVerticalMove();
    bool damaged(int valueOfDamage);
    void hasEnterInCollide(direction dir);
    void lookAt(int x, int y, int cameraX, int cameraY);
    bool hasThisItem(typeOfItem itemType, std::string nameOfItem);
    bool getIsHidden();
    void setPosition(int x, int y);
    void setIsHidden(bool newVal);
private:
};

#endif
