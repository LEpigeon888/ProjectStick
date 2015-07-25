#ifndef BULLET_HPP
#define BULLET_HPP

#include <SFML/Graphics.hpp>

/*
Classe: bulletClass
Parents: Aucun.
Abstraite: Non.
Utilité: Elle représente une balle dans le niveau, elle contient toutes les différentes informations (dégats, vitesse, etc) ainsi
que le sprite qui sera affiché.
*/

struct bulletInfo
{
    sf::Color color;
    sf::Vector2f sizeOfBullet;
    int speed;
    int damageValue;
};

class bulletClass
{
public:
    bulletClass(std::string typeOfBullet, bool againstPlayer);
    static void initializeBullet();
    void draw(sf::RenderWindow& window);
    void applyMove();
    sf::Vector2f getVelocity();
    int getDamageValue();
    sf::FloatRect getCollideBox();
    sf::Vector2i getCenter();
    sf::Vector2i getOldCenter();
    bool getIsAgainstPlayer();
    void setPosition(int x, int y);
    void setRotation(double angle);
    void setVelocity(float x, float y);
private:
    static std::map<std::string, bulletInfo> listOfTypeOfBullet;
    sf::RectangleShape sprite;
    sf::Vector2i oldCenter;
    sf::Vector2f velocity;
    int speed;
    int damageValue;
    bool isAgainstPlayer;
};

#endif
