#ifndef LEVEL_HPP
#define LEVEL_HPP

#include <list>
#include <vector>
#include <SFML/System.hpp>

#include "stickman.hpp"
#include "enemyStick.hpp"
#include "witnessStick.hpp"
#include "playerStick.hpp"

/*
Classe: levelClass
Parents: Aucun.
Abstraite: Non.
Utilité: C'est une classe qui contient tout les éléments chargé depuis le fichier du niveau, elle gère aussi les collisions et le chargement
de niveau.
*/

struct wall
{
    sf::Vector2i firstPoint;
    sf::Vector2i secondPoint;
    std::string typeOfWall;
    wall(sf::Vector2i newFirstPoint, sf::Vector2i newSecondPoint) : firstPoint(newFirstPoint), secondPoint(newSecondPoint) {}
    wall() {}
};

struct door
{
    sf::RectangleShape sprite;
    std::list<wall> listOfWall;
    std::string nameOfKey;
    door() {sprite.setFillColor(sf::Color(133, 74, 0));}
};

struct switchFloorStruct
{
    int idOfSecondSwitch;
    int numberOfFloorOfSecondSwitch;
    std::string nameOfKey;
    sf::FloatRect switchArea;
};

struct switchDoorFloorStruct
{
    int idOfSecondSwitch;
    int numberOfFloorOfSecondSwitch;
    std::string nameOfKey;
    direction dirOfSwitchWall;
    wall switchWall;
};

struct chest
{
    sf::FloatRect chestArea;
    typeOfItem itemType;
    std::string nameOfItem;
    int numberOfItem;
    bool chestIsUsed;
    chest() {chestIsUsed = false;}
};

struct floorStruct
{
    std::list<wall> horizontalWall;
    std::list<wall> verticalWall;
    std::list<door> listOfDoor;
    std::vector<switchFloorStruct> listOfSwitch;
    std::vector<switchDoorFloorStruct> listOfSwitchDoor;
    std::list<sf::FloatRect> listOfHideZone;
    std::list<enemyStickClass> listOfEnemy;
    std::list<witnessStickClass> listOfWitness;
    std::list<chest> listOfChest;
    std::list<itemClass> listOfItem;
    sf::Vector2f alarm;
    sf::RectangleShape floorArea;
    sf::Sprite floorBackground;
    int minimumYPosForCamera;
    int maximumYPosForCamera;
    floorStruct() : alarm(-1, -1) {floorArea.setFillColor(sf::Color(0, 0, 0, 235));}
};

class levelClass
{
public:
    bool update(stickmanClass& stickman);
    void drawFloorBackground(sf::RenderWindow& window, sf::Vector2f viewPosition, sf::Vector2f viewSize);
    void drawEnemy(sf::RenderWindow& window, stickmanClass& stickman);
    void drawItem(sf::RenderWindow& window, stickmanClass& stickman);
    void drawFloorShadow(sf::RenderWindow& window, stickmanClass& stickman);
    void checkStickmanCollideWithWall(stickmanClass& stickman, bool horizontalMove, int distanceMove);
    bool checkPointCollideWithWall(sf::Vector2i point, sf::Vector2i oldPoint, int damageOfBullet = 0, bool checkWithEnemyToo = false, bool checkWithPlayerToo = false);
    void checkItemCollideWithWall(itemClass& item, int distanceMove);
    bool pointCollideWithTheseWall(sf::Vector2i point, sf::Vector2i oldPoint, std::list<wall>& thisListOfWall, long& shortestDistance, bool needSwitch = false);
    bool pointCollideWithThisBox(sf::Vector2i point, sf::Vector2i oldPoint, sf::FloatRect thisBox, long& shortestDistance);
    sf::Vector2i checkBoxCollideWithWall(sf::FloatRect collideBox, bool horizontalMove, int distanceMove, bool isStickman);
    sf::Vector2i boxCollideWithTheseWall(sf::FloatRect collideBox, std::list<wall>& thisListOfWall, int distanceMove, bool isStickman, bool needSwitch = false);
    bool stickmanUseSwitch(stickmanClass& thisStickman);
    bool stickmanUseHideZone(stickmanClass& thisStickman);
    bool stickmanUseChest(stickmanClass& thisStickman);
    int stickmanTakeItem(stickmanClass& thisStickman, std::list<itemClass>::iterator& thisItem);
    void stickmanUseSwitchDoor(stickmanClass& thisStickman, int distanceMove, floorStruct& currentFloor);
    void addBulletToList(bulletClass thisBullet);
    void clearListOfBullet();
    void addItemAtThisPoint(itemClass& newItem, sf::Vector2f point);
    void soundAlarm();
    std::list<itemInInventoryStruct>& getListOfItemForThisLevel();
    std::vector<floorStruct>* getListOfFloor();
    std::list<bulletClass>& getListOfBulletNeedAdd();
    sf::Vector2i getLimitOfLevel();
    sf::Vector2i getInitalPosition();
    bool getAlarmIsRinging();
    bool getAllWitnessAreDead();
    int getMinimumYPosForCamera(sf::FloatRect thisCollideBox);
    int getMaximumYPosForCamera(sf::FloatRect thisCollideBox);
    int getFloorForThisPoint(sf::Vector2f point);
    std::string getNextLevel();
    void setPlayerPointer(playerStickClass* newPointer);
    void loadLevel(std::string nameOfLevel);
private:
    sf::Vector2i limitOfLevel;
    std::list<wall> horizontalLevelWall;
    std::list<wall> verticalLevelWall;
    std::list<bulletClass> listOfBulletNeedAdd;
    std::list<itemInInventoryStruct> listOfItemForThisLevel;
    sf::Vector2i initialPosition;
    std::vector<floorStruct> listOfFloor;
    sf::Texture backgroudTexture;
    playerStickClass* playerStick;
    std::string nextLevel;
    bool alarmIsRinging;
};

#endif
