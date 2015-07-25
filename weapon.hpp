#ifndef WEAPON_HPP
#define WEAPON_HPP

#include <SFML/Graphics.hpp>

#include "bullet.hpp"

/*
Classe: weaponClass
Parents: Aucun.
Abstraite: Non.
Utilité: Elle représente une arme dans le jeu, elle contient toutes les différentes informations (nom, taille du chargeur, etc) ainsi
que le sprite qui sera affiché.
*/

struct weaponInfo
{
    sf::Color color;
    sf::Vector2f sizeOfWeapon;
    sf::Vector2f origin;
    int numberOfBulletWhenShot;
    bool fullWeapon;
    sf::Vector2f outOfWeapon;
    int sizeOfMagazine;
    float timeForReload;
    float timeBetweenShot;
    int angleOfBullet;
};

class weaponClass
{
public:
    weaponClass();
    weaponClass(std::string newTypeOfWeapon);
    static void initializeWeapon();
    void draw(sf::RenderWindow& window);
    bulletClass shoot(int numberOfThisBullet, bool isAgainstPlayer = false);
    bool canShot();
    void reload(int number);
    sf::Vector2i getOutOfWeapon();
    std::string getTypeOfWeapon();
    int getNumberOfBulletWhenShot();
    int getNumberOfBulletInMagazine();
    int getSizeOfMagazine();
    float getTimeForReload();
    bool getIsFullWeapon();
    void setPosition(int x, int y);
    void setLookAt(int x, int y, int cameraX, int cameraY);
    void setRotation(int newAngle);
private:
    static std::map<std::string, weaponInfo> listOfTypeOfWeapon;
    sf::Clock timerBetweenShot;
    sf::RectangleShape sprite;
    sf::Vector2f velocityOfBullet;
    std::string typeOfWeapon;
    sf::Vector2f outOfWeapon;
    int numberOfBulletWhenShot;
    bool isFullWeapon;
    int sizeOfMagazine;
    int numberOfBulletInMagazine;
    float timeForReload;
    float timeBetweenShot;
    int angleOfBullet;
    double angle;
};

#endif
