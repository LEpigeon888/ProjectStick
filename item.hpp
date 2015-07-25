#ifndef ITEM_HPP
#define ITEM_HPP

#include <unordered_map>
#include <SFML/Graphics.hpp>

/*
Classe: itemClass
Parents: Aucun.
Abstraite: Non.
Utilité: Elle représente un objet dans le jeu, elle contient toutes les différentes informations (type, nom, nombre) ainsi
que le sprite qui sera affiché.
*/

enum typeOfItem {WEAPON, BULLET, USEABLE_ITEM};

struct itemTypeInfo
{
    typeOfItem type;
    std::string nameOfItem;
};

struct itemInfo
{
    sf::Vector2f sizeOfItem;
    sf::Color colorOfItem;
    bool stackableItem;
    int maxStack;
};

inline bool operator<(const itemTypeInfo &firstItem, const itemTypeInfo &secondItem)
{
    return (firstItem.type < secondItem.type || (firstItem.type == secondItem.type && firstItem.nameOfItem < secondItem.nameOfItem));
}

class itemClass
{
public:
    itemClass();
    itemClass(typeOfItem newType, std::string newName, int newNumber);
    static void initializeItem();
    void update();
    void draw(sf::RenderWindow& window);
    int applyVerticalMove();
    void hasEnterInCollide();
    bool contains(int x, int y);
    void moveItem(int x, int y);
    void turnItem();
    int addNumberOfItem(int newNumber);
    bool isStackableItem();
    sf::Vector2f getPosition();
    sf::FloatRect getCollideBox();
    itemTypeInfo getTypeOfItem();
    int getNumberOfItem();
    bool getIsTurn();
    void setTextPositionWithThisNumber(int number = -1);
    void setPosition(int x, int y);
    void setPositionOfBG(int x, int y);
    void setAlphaColor(int newAlphaColor);
    void setNumberOfItem(int newNumber);
    void setIsOnTheGround(bool newValue);
private:
    static std::map<itemTypeInfo, itemInfo> listOfTypeOfItem;
    sf::RectangleShape sprite;
    sf::Text numberOfItemText;
    sf::Vector2f originalSize;
    int verticalVelocity;
    int weight;
    bool isTurn;
    itemTypeInfo infoForItem;
    int numberOfItem;
    bool stackableItem;
    int maxStack;
    bool isOnTheGround;
};

#endif
