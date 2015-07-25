#ifndef SHOWINVENTORYSTATE_HPP
#define SHOWINVENTORYSTATE_HPP

#include "gameState.hpp"
#include "inventory.hpp"

/*
Classe: showInventoryStateClass
Parents: gameStateClass
Abstraite: Non.
Utilité: C'est une classe qui permet de voir l'inventaire, ainsi que d'intéragire avec (déplacer les objets, les tourner, les jeter, etc).
*/

class showInventoryStateClass : public gameStateClass
{
public:
    showInventoryStateClass(inventoryClass* newInventory, sf::Vector2f newPositionOfFootOfPlayer, int newFloorNumber = -1, std::list<itemClass>::iterator newItemToTake = std::list<itemClass>::iterator());
    void update(sf::RenderWindow& window);
    void draw(sf::RenderWindow& window);
private:
    sf::Sprite backgroundSprite;
    sf::Texture backgroundTexture;
    bool itemIsTakingFromTheGround;
    bool itemTryToBeAutoPlaced;
    int floorNumber;
    std::list<itemClass>::iterator itemToTake;
    inventoryClass* inventory;
    sf::Vector2f positionOfFootOfPlayer;
};

#endif
