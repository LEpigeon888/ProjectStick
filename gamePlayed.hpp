#ifndef GAMEPLAYED_HPP
#define GAMEPLAYED_HPP

#include <SFML/Graphics.hpp>

#include "level.hpp"
#include "playerStick.hpp"
#include "inventory.hpp"

/*
Classe: gamePlayedClass
Parents: Aucun.
Abstraite: Non.
Utilité: C'est la principale classe de la phase active du jeu, elle gère les balles et sert d'interface entre la classe qui récupèrera les entrés
et le joueur, elle s'occupe aussi d'appeler la fonction de mise à jour du niveau, des ennemis, et de ce qu'il reste.
*/

class gamePlayedClass
{
public:
    gamePlayedClass();
    void update();
    void draw(sf::RenderWindow& window);
    void updateAmmoText();
    void updateAlarmTimeText();
    void jumpPlayer();
    void movePlayerTo(direction newDir);
    void playerLookAt(int x, int y);
    void playerShoot(bool isFull);
    void playerReload();
    void playerUseSomething();
    int playerTakeItem(std::list<itemClass>::iterator& thisItem);
    void selectWeapon(int number);
    bool playerHasLost();
    bool playerHasWon();
    void checkStickmenCollideWithBullet(bool moveBullet);
    void startShake();
    void manageShake();
    void shakeCamera();
    inventoryClass* getPlayerInventory();
    std::vector<floorStruct>* getListOfFloor();
    playerStickClass* getPlayer();
    sf::Vector2f getPlayerFootPosition();
    sf::Vector2i getSizeOfLevel();
    std::string getNextLevel();
    void setCameraToStickman(stickmanClass& stickman);
    void setLevel(std::string levelName);
private:
    sf::View view;
    sf::Vector2f speedView;
    levelClass level;
    playerStickClass playerStick;
    std::list<bulletClass> listOfBullet;
    bool alarmIsRinging;
    bool allWitnessAreDead;
    bool needToUpdateHud;
    int minimumYPosForCamera;
    int maximumYPosForCamera;
    sf::RectangleShape lifeBar;
    sf::RectangleShape alertAlarm;
    sf::Text ammoText;
    sf::Clock shakeTimer;
    bool isInShake;
    sf::Text alarmTimeText;
    sf::Clock alarmTimer;
    sf::Clock pauseTimer;
    unsigned int timeUntilLose;
    unsigned int pauseTime;
    bool timeIsOver;
};

#endif
