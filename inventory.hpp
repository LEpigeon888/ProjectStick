#ifndef INVENTORY_HPP
#define INVENTORY_HPP

#include <list>

#include "item.hpp"

/*
Classe: inventoryClass
Parents: Aucun.
Abstraite: Non.
Utilité: C'est une classe qui contient tout l'inventaire du joueur, ainsi qui plusieurs fonction utilitaires pour rechercher des objets
dans l'inventaire, les déplacer, les supprimer, etc.
*/

struct itemInInventoryStruct
{
    typeOfItem type;
    std::string name;
    int number;
    bool isTurn;
    int posX;
    int posY;
};

class inventoryClass
{
public:
    inventoryClass();
    void draw(sf::RenderWindow& window);
    void leftClick(int x, int y);
    void escapPressed();
    void returnPressed();
    void moveSelectedItem(int x, int y);
    void turnSelectedItem();
    void autoPlaceSelectedItem();
    void placeTmpItem();
    void cancelTmpItem();
    void addThisItem(itemInInventoryStruct newItem);
    void clearInventory();
    int removeThisItem(typeOfItem type, std::string name, int number);
    int getNumberOfThisItem(typeOfItem type, std::string name);
    bool getItemIsSelected();
    itemClass& getItemAndRemoveItFromInventory();
    bool getItemSelectedCanBeDeleted();
    bool getItemSelectedIsPlaced();
    bool getItemSelectedIsCanceled();
    std::list<itemInInventoryStruct> getListOfItemInInventory();
    void setPositionOfBGInventory(int x, int y);
    void setThisItemHasTmpItem(itemClass& item);
    void setItemSelectedCanBeDeleted(bool newValue);
private:
    std::list<itemClass> listOfItem;
    sf::Vector2i positionOfBGInventory;
    sf::Vector2i sizeOfInventory;
    int sizeOfCase;
    itemClass tmpItem;
    bool itemIsSelected;
    bool itemSelectedIsTurn;
    bool itemSelectedCanBeDeleted;
    bool itemSelectedIsPlaced;
    bool itemSelectedIsCanceled;
    sf::Vector2f oldPositionOfItemSelected;
};

#endif
