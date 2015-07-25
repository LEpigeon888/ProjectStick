#ifndef EDITLEVEL_HPP
#define EDITLEVEL_HPP

#include <SFML/Graphics.hpp>
#include <list>

#include "gameState.hpp"

/*
Classe: editLevelStateClass
Parents: gameStateClass
Abstraite: Non.
Utilité: C'est une gameState qui gère l'éditeur de niveau, elle permet de charger un niveau et de le sauvegarder, ainsi
que d'y ajouter tout les éléments composant un niveau présent dans le jeu.
*/

enum editMode {ADD_FLOOR, ADD_WALL, PLACE_PLAYER, NO_MODE};

struct smartRectangle
{
    sf::Vector2i origin;
    sf::RectangleShape rect;
};

struct editFloorStruct
{
    sf::RectangleShape floor;
    std::list<std::pair<sf::RectangleShape, std::string>> listOfWall;
    std::list<std::string> untestedLines;
};

class editLevelStateClass : public gameStateClass
{
public:
    editLevelStateClass() : editLevelStateClass("level1") {};
    editLevelStateClass(std::string levelName);
    void update(sf::RenderWindow& window);
    void draw(sf::RenderWindow& window);
    void returnPressed();
    void backspacePressed();
    void deletePressed();
    void leftClick(int x, int y);
    void rightClick(int x, int y);
    void addFloor();
    void addWallToCurrentFloor();
    void moveView(int x, int y);
    void moveAllLevel(int x, int y);
    void zoomView(int delta);
    void setAddFloorMode();
    void setAddWallMode(std::string newTypeOfNewWall);
    void setNoMode();
    void nameOfLevelTextHasChanged();
    void updateNameOfLevelText();
    void loadListOfLevel();
    void loadThisLevel(std::string levelName);
    void saveLevel();
    void loadLevel();
    int readFirstNumber(std::string& thisString);
private:
    sf::View view;
    sf::Vector2i limitOfLevel;
    sf::Sprite backgroundSprite;
    sf::Texture backgroudTexture;
    sf::RectangleShape sprite;
    std::vector<editFloorStruct> listOfFloor;
    std::list<std::string> untestedGlobalLines;
    std::string typeOfNewWall;
    std::string nameOfLevel;
    smartRectangle tempoRect;
    int currentFloor;
    int speedView;
    int speedFactor;
    double zoomLevel;
    bool drawAllFloor;
    editMode currentMode;
    std::vector<std::string> listOfLevel;
    int currentIndexOfLevel;
    sf::Text nameOfLevelText;
    int tmpIndexOfLevel;
    bool chooseNameOfLevel;
    bool nameOfLevelTextNeedHide;
    sf::Clock clockForNameOfLevel;
    double timeBeforeHideText;
};

#endif
