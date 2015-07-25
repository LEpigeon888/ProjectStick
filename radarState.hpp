#ifndef RADAR_HPP
#define RADAR_HPP

#include <list>
#include <SFML/Graphics.hpp>

#include "gameState.hpp"
#include "enemyStick.hpp"
#include "playerStick.hpp"
#include "level.hpp"

/*
Classe: radarStateClass
Parents: gameStateClass
Abstraite: Non.
Utilité: C'est une classe qui gère l'affichage du radar.
*/

class radarStateClass : public gameStateClass
{
public:
	radarStateClass(std::vector<floorStruct>* newListOfFloor, playerStickClass* newPlayerStick, sf::Vector2i newSizeOfLevel, std::string nameOfLevel);
	void update(sf::RenderWindow& window);
	void draw(sf::RenderWindow& window);
	void startScan();
	void stopScan();
	void moveScannerAndScan();
private:
    sf::Texture mapTexture;
    sf::Sprite mapSprite;
    std::list<sf::CircleShape> listOfCircle;
    std::vector<floorStruct>* listOfFloor;
    std::list<sf::Vector2f> listOfFuturCircle;
    playerStickClass* playerStick;
    sf::Vector2i sizeOfLevel;
    int maxSizeOfCircle;
    int numberOfWaitFrame;
    int currentFrame;
    int maxThickness;
    int maxRadius;
    float timeBetweenScan;
    int speedScan;
    bool inScan;
    sf::RectangleShape scannerSprite;
    static sf::Clock scannerTimer;
};

#endif
