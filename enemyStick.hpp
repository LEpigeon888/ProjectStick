#ifndef ENEMYSTICK_HPP
#define ENEMYSTICK_HPP

#include "stickman.hpp"
#include "item.hpp"

/*
Classe: enemyStickClass
Parents: stickmanClass
Abstraite: Non.
Utilité: C'est une classe réprensentant les ennemis dans le jeu, sa principale différence par rapport aux autres classe de stickman
c'est qu'elle possède une IA.
*/

enum iaMode {PATROL_MODE, SCOPE_MODE, WAITING_SCOPE_MODE, SOUND_ALARM_MODE};
enum directionOfView {LEFT_VIEW, RIGHT_VIEW};
enum seeMode {CANT_SEE, CAN_SEE, INSTANT_SEE};

class enemyStickClass : public stickmanClass
{
public:
    enemyStickClass();
    bool update(stickmanClass& stickman);
    void draw(sf::RenderWindow& window);
    void useIA(stickmanClass& stickman);
    int applyHorizontalMove();
    int applyVerticalMove();
    bool damaged(int valueOfDamage);
    void hasEnterInCollide(direction dir);
    void lookAt(int x, int y, int cameraX, int cameraY);
    bool hasThisItem(typeOfItem itemType, std::string nameOfItem);
    seeMode seeThisPoint(sf::Vector2i centerOfEnemy, sf::Vector2i thisPoint);
    void checkSeeStickman(stickmanClass& stickman);
    void alarmIsRinging();
    bool getIsHidden();
    itemClass& getItemLoot();
    void setPatrolMode();
    void setWaitingScopeMode();
    void setScopeMode();
    void setSoundAlarmMode();
    void setPosition(int x, int y);
    void setPatrolPoints(sf::Vector2f firstPoint, sf::Vector2f secondPoint);
    void setWaitForPatrolTime(float time);
    void setDistanceOfViewInfo(int newDistanceOfView, int newDistanceOfViewAfterScope, int newDistanceWithInstantReactionTime);
    void setFrameForCheck(int newFrameForCheck);
    void setIdFloorOfEnemy(int newIdFloor);
    void setReactionTime(float time);
    void setIAType(std::string newIAType);
    void setLife(int newLife);
    void setIsHidden(bool newVal);
    void setItemLoot(typeOfItem newType, std::string newName, int newNumber);
    void setWeapon(std::string newWeapon);
private:
    sf::Vector2f firstPointPatrol;
    sf::Vector2f secondPointPatrol;
    sf::Clock patrolWaitTimer;
    float waitTimeForPatrol;
    bool isWaitingForPatrol;
    weaponClass weapon;
    float reactionTime;
    float currentReactionTime;
    int distanceOfView;
    int currentDistanceOfView;
    int distanceOfViewAfterScope;
    int distanceWithInstantReactionTime;
    iaMode enemyIAMode;
    directionOfView enemyViewDirection;
    sf::Clock waitingTimer;
    int currentFrame;
    int frameForCheck;
    int maxNumberOfFrame;
    int idFloorOfEnemy;
    sf::Clock reloadTimer;
    std::string iaType;
    bool isInReload;
    bool needToSoundAlarm;
    bool waitForSoundAlarm;
    bool isInAlert;
    int positionOfAlarm;
    itemClass itemLoot;
};

#endif
