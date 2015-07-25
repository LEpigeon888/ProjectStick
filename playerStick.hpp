#ifndef PLAYERSICK_HPP
#define PLAYERSICK_HPP

#include "stickman.hpp"
#include "inventory.hpp"

/*
Classe: playerStickClass
Parents: stickmanClass
Abstraite: Non.
Utilité: C'est une classe qui représente le joueur, elle offre des fonction permettant l'intéraction avec lui (se déplacer, sauter, tirer, etc).
*/

struct lineStruct
{
    sf::Vector2i firstPoint;
    sf::Vector2i secondPoint;
};

class playerStickClass : public stickmanClass
{
public:
    playerStickClass();
    bool update();
    void draw(sf::RenderWindow& window);
    int applyHorizontalMove();
    int applyVerticalMove();
    bool damaged(int valueOfDamage);
    void hasEnterInCollide(direction dir);
    void lookAt(int x, int y, int cameraX, int cameraY);
    bool hasThisItem(typeOfItem itemType, std::string nameOfItem);
    void jump();
    void moveTo(direction newDir);
    std::list<bulletClass> shoot(bool isFull);
    void reloadNeeded();
    void reload();
    void selectWeapon(int number);
    void checkIfWeaponIsInInventory();
    int getNumberOfBulletInMagazine();
    int getNumberOfBulletInInentory();
    inventoryClass* getInventory();
    lineStruct getLineOfShot();
    int getLife();
    bool getIsHidden();
    void setPosition(int x, int y);
    void setIsHidden(bool newVal);
private:
    sf::Clock reloadTimer;
    bool isInReload;
    bool isHidden;
    bool needToCheckWeapon;
    inventoryClass inventory;
    std::vector<weaponClass> listOfWeapon;
    int speedJump;
    int currentWeapon;
    int totalNumberOfJump;
    int currentNumberOfJump;
};

#endif
