#include <fstream>
#include <dirent.h>

#include "editLevelState.hpp"
#include "playState.hpp"
#include "mainMenuState.hpp"
#include "utilities.hpp"
#include "global.hpp"

editLevelStateClass::editLevelStateClass(std::string levelName)
{
    view.setSize(WIDTH_SCREEN, HEIGHT_SCREEN);
    view.setCenter(WIDTH_SCREEN / 2, HEIGHT_SCREEN / 2);

    sprite.setPosition(-100, -100);
    sprite.setSize(sf::Vector2f(40, 80));
    sprite.setFillColor(sf::Color::Blue);

    speedView = 10;
    speedFactor = 1;
    limitOfLevel = sf::Vector2i(WIDTH_SCREEN, HEIGHT_SCREEN);
    currentMode = NO_MODE;
    currentFloor = -1;
    zoomLevel = 1;
    drawAllFloor = false;

    nameOfLevelText.setFont(global::font);
    nameOfLevelText.setCharacterSize(80);
    nameOfLevelText.setColor(sf::Color::Blue);
    chooseNameOfLevel = false;
    nameOfLevelTextNeedHide = false;
    timeBeforeHideText = 1.5;

    loadThisLevel(levelName);
    loadListOfLevel();
}

void editLevelStateClass::update(sf::RenderWindow& window)
{
    sf::Event event;

    while(window.pollEvent(event))
    {
        if(event.type == sf::Event::Closed)
        {
            window.close();
        }
        else if(event.type == sf::Event::KeyPressed)
        {
            switch(event.key.code)
            {
                case sf::Keyboard::Escape:
                {
                    setNoMode();
                    break;
                }
                case sf::Keyboard::A:
                {
                    drawAllFloor = !drawAllFloor;
                    break;
                }
                case sf::Keyboard::F:
                {
                    setAddFloorMode();
                    break;
                }
                case sf::Keyboard::W:
                {
                    setAddWallMode("WALL");
                    break;
                }
                case sf::Keyboard::X:
                {
                    setAddWallMode("STICKMAN_WALL");
                    break;
                }
                case sf::Keyboard::P:
                {
                    currentMode = PLACE_PLAYER;
                    break;
                }
                case sf::Keyboard::Return:
                {
                    returnPressed();
                    break;
                }
                case sf::Keyboard::BackSpace:
                {
                    backspacePressed();
                    break;
                }
                case sf::Keyboard::Delete:
                {
                    deletePressed();
                    break;
                }
                case sf::Keyboard::LShift:
                {
                    speedFactor = 5;
                    break;
                }
                case sf::Keyboard::O:
                {
                    saveLevel();
                    break;
                }
                case sf::Keyboard::I:
                {
                    loadLevel();
                    break;
                }
                case sf::Keyboard::H:
                {
                    moveAllLevel(-1, 0);
                    break;
                }
                case sf::Keyboard::J:
                {
                    moveAllLevel(0, 1);
                    break;
                }
                case sf::Keyboard::K:
                {
                    moveAllLevel(0, -1);
                    break;
                }
                case sf::Keyboard::L:
                {
                    moveAllLevel(1, 0);
                    break;
                }
                case sf::Keyboard::F10:
                {
                    global::activeGameStateStack->set(new mainMenuStateClass());
                    return;
                }
                case sf::Keyboard::Add:
                {
                    if(static_cast<unsigned int>(currentFloor - 1) < listOfFloor.size())
                    {
                        ++currentFloor;
                    }
                    break;
                }
                case sf::Keyboard::Subtract:
                {
                    if(currentFloor > 0)
                    {
                        --currentFloor;
                    }
                    break;
                }
                case sf::Keyboard::B:
                {
                    if(chooseNameOfLevel == false)
                    {
                        chooseNameOfLevel = true;
                        tmpIndexOfLevel = currentIndexOfLevel;
                    }
                    else
                    {
                        --tmpIndexOfLevel;
                        if(tmpIndexOfLevel < 0)
                        {
                            tmpIndexOfLevel = listOfLevel.size() - 1;
                        }
                    }
                    nameOfLevelTextHasChanged();
                    break;
                }
                case sf::Keyboard::N:
                {
                    if(chooseNameOfLevel == false)
                    {
                        chooseNameOfLevel = true;
                        tmpIndexOfLevel = currentIndexOfLevel;
                    }
                    else
                    {
                        ++tmpIndexOfLevel;
                        if(static_cast<unsigned int>(tmpIndexOfLevel) >= listOfLevel.size())
                        {
                            tmpIndexOfLevel = 0;
                        }
                    }
                    nameOfLevelTextHasChanged();
                    break;
                }
                case sf::Keyboard::M:
                {
                    if(chooseNameOfLevel == true)
                    {
                        chooseNameOfLevel = false;
                        currentIndexOfLevel = tmpIndexOfLevel;
                        loadThisLevel(listOfLevel[currentIndexOfLevel]);
                    }
                    break;
                }
                default:
                {
                    break;
                }
            }
        }
        else if(event.type == sf::Event::KeyReleased)
        {
            if(event.key.code == sf::Keyboard::LShift)
            {
                speedFactor = 1;
            }
        }
        else if(event.type == sf::Event::MouseButtonPressed)
        {
            if(event.mouseButton.button == sf::Mouse::Left)
            {
                leftClick(view.getCenter().x - ((WIDTH_SCREEN / 2) * zoomLevel) + event.mouseButton.x * zoomLevel,
                          view.getCenter().y - ((HEIGHT_SCREEN / 2) * zoomLevel) + event.mouseButton.y * zoomLevel);
            }
            else if(event.mouseButton.button == sf::Mouse::Right)
            {
                rightClick(view.getCenter().x - ((WIDTH_SCREEN / 2) * zoomLevel) + event.mouseButton.x * zoomLevel,
                          view.getCenter().y - ((HEIGHT_SCREEN / 2) * zoomLevel) + event.mouseButton.y * zoomLevel);
            }
        }
        else if(event.type == sf::Event::MouseWheelMoved)
        {
            zoomView(event.mouseWheel.delta);
        }

        if(event.type == sf::Event::TextEntered)
        {
            char character = static_cast<char>(event.text.unicode);

            if(character >= 48 && character <= 57 && currentMode == NO_MODE)
            {
                if(character - 48 < static_cast<char>(listOfFloor.size()))
                {
                    currentFloor = character - 48;
                }
            }
        }
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
    {
        moveView(-speedView * zoomLevel - 1, 0);
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        moveView(speedView * zoomLevel + 1, 0);
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
    {
        moveView(0, -speedView * zoomLevel - 1);
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        moveView(0, speedView * zoomLevel + 1);
    }

    if(chooseNameOfLevel == true)
    {
        updateNameOfLevelText();
    }
}

void editLevelStateClass::draw(sf::RenderWindow& window)
{
    window.setView(view);
    window.clear(sf::Color::Black);
    window.draw(backgroundSprite);

    if(drawAllFloor == true)
    {
        for(editFloorStruct& thisFloor : listOfFloor)
        {
            window.draw(thisFloor.floor);
        }
    }

    if(currentFloor != -1)
    {
        window.draw(listOfFloor[currentFloor].floor);

        for(std::pair<sf::RectangleShape, std::string>& thisWall : listOfFloor[currentFloor].listOfWall)
        {
            window.draw(thisWall.first);
        }
    }
    if(currentMode != NO_MODE)
    {
        window.draw(tempoRect.rect);
    }

    window.draw(sprite);

    if(chooseNameOfLevel == true)
    {
        window.setView(window.getDefaultView());
        window.draw(nameOfLevelText);
    }
}

void editLevelStateClass::returnPressed()
{
    if(currentMode == ADD_FLOOR)
    {
        addFloor();
    }
    else if(currentMode == ADD_WALL)
    {
        addWallToCurrentFloor();
    }
    else if(currentMode == PLACE_PLAYER)
    {
        setNoMode();
    }
}

void editLevelStateClass::backspacePressed()
{
    if(currentMode == ADD_FLOOR || currentMode == ADD_WALL)
    {
        tempoRect.origin.x = -1;
        tempoRect.rect.setPosition(-1, -1);
        tempoRect.rect.setSize(sf::Vector2f(0, 0));
    }
}

void editLevelStateClass::deletePressed()
{
    if(currentMode == ADD_FLOOR)
    {
        if(listOfFloor.size() > 0)
        {
            listOfFloor.pop_back();
        }
    }
}

void editLevelStateClass::leftClick(int x, int y)
{
    int sizeX = 0;
    int sizeY = 0;
    if(x >= 0 && y >= 0)
    {
        if(currentMode == ADD_FLOOR)
        {
            if(tempoRect.origin.x == -1)
            {
                tempoRect.origin = sf::Vector2i(x, y);
                tempoRect.rect.setPosition(x, y);
                tempoRect.rect.setSize(sf::Vector2f(1, 1));
            }
            else
            {
                sizeX = fabs(x - tempoRect.origin.x) + 1;
                sizeY = fabs(y - tempoRect.origin.y) + 1;

                if(x < tempoRect.origin.x)
                {
                    tempoRect.rect.setPosition(x, tempoRect.rect.getPosition().y);
                }
                else
                {
                    tempoRect.rect.setPosition(tempoRect.origin.x, tempoRect.rect.getPosition().y);
                }

                if(y < tempoRect.origin.y)
                {
                    tempoRect.rect.setPosition(tempoRect.rect.getPosition().x, y);
                }
                else
                {
                    tempoRect.rect.setPosition(tempoRect.rect.getPosition().x, tempoRect.origin.y);
                }

                tempoRect.rect.setSize(sf::Vector2f(sizeX, sizeY));
            }
        }
        else if(currentMode == ADD_WALL)
        {
            if(listOfFloor[currentFloor].floor.getGlobalBounds().contains(x, y) == true)
            {
                if(tempoRect.origin.x == -1)
                {
                    tempoRect.origin = sf::Vector2i(x, y);
                    tempoRect.rect.setPosition(x, y);
                    tempoRect.rect.setSize(sf::Vector2f(1, 1));
                }
                else
                {
                    sizeX = fabs(x - tempoRect.origin.x);
                    sizeY = fabs(y - tempoRect.origin.y);

                    if(sizeX > sizeY)
                    {
                        sizeY = 1;
                        sizeX += 1;
                        if(x < tempoRect.origin.x)
                        {
                            tempoRect.rect.setPosition(x, tempoRect.origin.y);
                        }
                        else
                        {
                            tempoRect.rect.setPosition(tempoRect.origin.x, tempoRect.origin.y);
                        }
                    }
                    else
                    {
                        sizeX = 1;
                        sizeY += 1;
                        if(y < tempoRect.origin.y)
                        {
                            tempoRect.rect.setPosition(tempoRect.origin.x, y);
                        }
                        else
                        {
                            tempoRect.rect.setPosition(tempoRect.origin.x, tempoRect.origin.y);
                        }
                    }

                    tempoRect.rect.setSize(sf::Vector2f(sizeX, sizeY));
                }
            }
        }
        else if(currentMode == PLACE_PLAYER)
        {
            sprite.setPosition(x, y);
        }
    }
}

void editLevelStateClass::rightClick(int x, int y)
{
    if(currentMode == ADD_WALL)
    {
        for(auto wallIte = listOfFloor[currentFloor].listOfWall.begin(); wallIte != listOfFloor[currentFloor].listOfWall.end(); )
        {
            if(wallIte->first.getGlobalBounds().contains(x, y) == true)
            {
                listOfFloor[currentFloor].listOfWall.erase(wallIte++);
                continue;
            }
            ++wallIte;
        }
    }
}

void editLevelStateClass::addFloor()
{
    if(tempoRect.origin.x != -1)
    {
        listOfFloor.push_back(editFloorStruct());
        listOfFloor.back().floor = tempoRect.rect;
        listOfFloor.back().floor.setFillColor(sf::Color(0, 255, 0, 50));
        currentMode = NO_MODE;
        currentFloor = listOfFloor.size() - 1;
    }
}

void editLevelStateClass::addWallToCurrentFloor()
{
    if(tempoRect.origin.x != -1)
    {
        listOfFloor[currentFloor].listOfWall.push_back(std::pair<sf::RectangleShape, std::string>());
        listOfFloor[currentFloor].listOfWall.back().first = tempoRect.rect;
        listOfFloor[currentFloor].listOfWall.back().first.setFillColor(sf::Color(tempoRect.rect.getFillColor().r, tempoRect.rect.getFillColor().g, tempoRect.rect.getFillColor().b, 50));
        listOfFloor[currentFloor].listOfWall.back().second = typeOfNewWall;

        currentMode = NO_MODE;
    }
}

void editLevelStateClass::moveView(int x, int y)
{
    view.move(x * speedFactor, y * speedFactor);

    if(view.getCenter().x < (WIDTH_SCREEN / 2) * zoomLevel)
    {
        view.setCenter((WIDTH_SCREEN / 2) * zoomLevel, view.getCenter().y);
    }
    else if(view.getCenter().x > limitOfLevel.x - ((WIDTH_SCREEN / 2) * zoomLevel))
    {
        view.setCenter(limitOfLevel.x - ((WIDTH_SCREEN / 2) * zoomLevel), view.getCenter().y);
    }

    if(view.getCenter().y < (HEIGHT_SCREEN / 2) * zoomLevel)
    {
        view.setCenter(view.getCenter().x, (HEIGHT_SCREEN / 2) * zoomLevel);
    }
    else if(view.getCenter().y > limitOfLevel.y - ((HEIGHT_SCREEN / 2) * zoomLevel))
    {
        view.setCenter(view.getCenter().x, limitOfLevel.y - ((HEIGHT_SCREEN / 2) * zoomLevel));
    }
}

void editLevelStateClass::moveAllLevel(int x, int y)
{
    x *= speedFactor;
    y *= speedFactor;

    for(editFloorStruct& thisFloor : listOfFloor)
    {
        thisFloor.floor.move(x, y);

        for(std::pair<sf::RectangleShape, std::string>& thisWall : thisFloor.listOfWall)
        {
            thisWall.first.move(x, y);
        }
    }

    if(sprite.getPosition().x > -100)
    {
        sprite.move(x, y);
    }
}

void editLevelStateClass::zoomView(int delta)
{
    float newZoom = 1;

    if(delta > 0)
    {
        newZoom = (1 / static_cast<double>(delta * 1.5));
    }
    else
    {
        newZoom = (-delta * 1.5);
    }

    zoomLevel *= newZoom;
    view.zoom(newZoom);

    if(view.getSize().x > WIDTH_SCREEN)
    {
        view.setSize(WIDTH_SCREEN, HEIGHT_SCREEN);
        zoomLevel = 1;
    }

    moveView(0, 0);
}

void editLevelStateClass::setAddFloorMode()
{
    if(currentMode == NO_MODE)
    {
        currentMode = ADD_FLOOR;
        currentFloor = -1;

        tempoRect.origin.x = -1;
        tempoRect.rect.setPosition(-1, -1);
        tempoRect.rect.setSize(sf::Vector2f(0, 0));
        tempoRect.rect.setFillColor(sf::Color(0, 255, 0, 100));
    }
}

void editLevelStateClass::setNoMode()
{
    currentMode = NO_MODE;
}

void editLevelStateClass::setAddWallMode(std::string newTypeOfNewWall)
{
    if(currentMode == NO_MODE && currentFloor != -1)
    {
        currentMode = ADD_WALL;
        typeOfNewWall = newTypeOfNewWall;

        tempoRect.origin.x = -1;
        tempoRect.rect.setPosition(-1, -1);
        tempoRect.rect.setSize(sf::Vector2f(0, 0));

        if(typeOfNewWall == "WALL")
        {
            tempoRect.rect.setFillColor(sf::Color(255, 0, 0, 100));
        }
        else if(typeOfNewWall == "STICKMAN_WALL")
        {
            tempoRect.rect.setFillColor(sf::Color(255, 128, 0, 100));
        }
    }
}

void editLevelStateClass::nameOfLevelTextHasChanged()
{
    nameOfLevelText.setString(listOfLevel[tmpIndexOfLevel]);
    nameOfLevelText.setOrigin(0, static_cast<int>(nameOfLevelText.getLocalBounds().top));
    nameOfLevelText.setPosition((WIDTH_SCREEN / 2) - (nameOfLevelText.getGlobalBounds().width / 2), (HEIGHT_SCREEN / 2) - (nameOfLevelText.getGlobalBounds().height / 2));
    nameOfLevelText.setColor(sf::Color(nameOfLevelText.getColor().r, nameOfLevelText.getColor().g, nameOfLevelText.getColor().b, 255));
    clockForNameOfLevel.restart();
    nameOfLevelTextNeedHide = false;
}

void editLevelStateClass::updateNameOfLevelText()
{
    if(nameOfLevelTextNeedHide == false)
    {
        if(clockForNameOfLevel.getElapsedTime().asSeconds() >= timeBeforeHideText)
        {
            nameOfLevelTextNeedHide = true;
        }
    }
    else
    {
        nameOfLevelText.setColor(sf::Color(nameOfLevelText.getColor().r, nameOfLevelText.getColor().g, nameOfLevelText.getColor().b, nameOfLevelText.getColor().a - 2));

        if(nameOfLevelText.getColor().a <= 5)
        {
            chooseNameOfLevel = false;
        }
    }
}

void editLevelStateClass::loadListOfLevel()
{
    DIR *directory;
    struct dirent *file;
    if((directory = opendir("level/")) != NULL)
    {
        while((file = readdir(directory)) != NULL)
        {
            if(std::string(file->d_name) != "." && std::string(file->d_name) != "..")
            {
                listOfLevel.push_back(file->d_name);

                if(file->d_name == nameOfLevel)
                {
                    currentIndexOfLevel = listOfLevel.size() - 1;
                }
            }
        }
        closedir(directory);
    }
}

void editLevelStateClass::loadThisLevel(std::string levelName)
{
    currentMode = NO_MODE;
    currentFloor = -1;
    zoomLevel = 1;
    drawAllFloor = false;
    view.setSize(WIDTH_SCREEN, HEIGHT_SCREEN);
    view.setCenter(WIDTH_SCREEN / 2, HEIGHT_SCREEN / 2);

    nameOfLevel = levelName;
    loadLevel();
}

void editLevelStateClass::saveLevel()
{
    std::ofstream file;
    file.open("level/" + nameOfLevel + "/levelinfo.txt");

    file << "SIZE_OF_LEVEL " << limitOfLevel.x << " " << limitOfLevel.y << std::endl;

    file << "PLAYER_POSITION " << sprite.getPosition().x << " " << sprite.getPosition().y << std::endl;

    for(editFloorStruct& thisFloor : listOfFloor)
    {
        file << "NEW_FLOOR " << thisFloor.floor.getPosition().x << " " << thisFloor.floor.getPosition().y << " "
         << thisFloor.floor.getSize().x << " " << thisFloor.floor.getSize().y << std::endl;

        for(std::pair<sf::RectangleShape, std::string>& thisWall : thisFloor.listOfWall)
        {
            file << "NEW_" << thisWall.second << " " << thisWall.first.getPosition().x << " " << thisWall.first.getPosition().y << " "
             << (thisWall.first.getPosition().x + thisWall.first.getSize().x - 1) << " " << (thisWall.first.getPosition().y + thisWall.first.getSize().y - 1) << std::endl;
        }

        for(std::string& thisLine : thisFloor.untestedLines)
        {
            file << thisLine << std::endl;
        }

        file << "END_OF_NEW_FLOOR" << std::endl;
    }

    for(std::string& thisLine : untestedGlobalLines)
    {
        file << thisLine << std::endl;
    }

    file.close();
}

void editLevelStateClass::loadLevel()
{
    size_t spacePos;
    std::string currentLine;
    std::string currentType;
    std::string firstWordOfLine;
    std::vector<int> listOfNumber;
    std::ifstream file;
    file.open("level/" + nameOfLevel + "/levelinfo.txt");

    backgroudTexture.loadFromFile("level/" + nameOfLevel + "/background.png");
    backgroundSprite.setTexture(backgroudTexture);
    backgroundSprite.setTextureRect(sf::IntRect(0, 0, backgroudTexture.getSize().x, backgroudTexture.getSize().y));
    backgroundSprite.setPosition(0, 0);

    listOfFloor.clear();
    untestedGlobalLines.clear();

    while(std::getline(file, currentLine))
    {
        spacePos = currentLine.find(' ');
        firstWordOfLine = currentLine.substr(0, spacePos);

        if(spacePos == std::string::npos)
        {
            currentLine.erase(0);
        }
        else
        {
            currentLine.erase(0, spacePos + 1);
        }

        if(currentType == "NEW_FLOOR")
        {
            if(firstWordOfLine == "NEW_WALL" || firstWordOfLine == "NEW_STICKMAN_WALL")
            {
                listOfNumber = utilitiesClass::readNumber(currentLine, 4);

                listOfFloor.back().listOfWall.push_back(std::pair<sf::RectangleShape, std::string>());
                listOfFloor.back().listOfWall.back().first.setPosition(listOfNumber[0], listOfNumber[1]);
                listOfFloor.back().listOfWall.back().first.setSize(sf::Vector2f(listOfNumber[2] - listOfNumber[0] + 1, listOfNumber[3] - listOfNumber[1] + 1));

                if(firstWordOfLine == "NEW_WALL")
                {
                    listOfFloor.back().listOfWall.back().first.setFillColor(sf::Color(255, 0, 0, 50));
                    listOfFloor.back().listOfWall.back().second = "WALL";
                }
                else if(firstWordOfLine == "NEW_STICKMAN_WALL")
                {
                    listOfFloor.back().listOfWall.back().first.setFillColor(sf::Color(255, 128, 0, 50));
                    listOfFloor.back().listOfWall.back().second = "STICKMAN_WALL";
                }
            }
            else if(firstWordOfLine == "END_OF_NEW_FLOOR")
            {
                currentType.clear();
            }
            else
            {
                listOfFloor.back().untestedLines.push_back(firstWordOfLine + " " + currentLine);
            }
        }
        else
        {
            if(firstWordOfLine == "SIZE_OF_LEVEL")
            {
                limitOfLevel.x = utilitiesClass::stringToInt(utilitiesClass::readFirstString(currentLine));
                limitOfLevel.y = utilitiesClass::stringToInt(utilitiesClass::readFirstString(currentLine));
            }
            else if(firstWordOfLine == "PLAYER_POSITION")
            {
                int posX = utilitiesClass::stringToInt(utilitiesClass::readFirstString(currentLine));
                int posY = utilitiesClass::stringToInt(utilitiesClass::readFirstString(currentLine));

                sprite.setPosition(posX, posY);
            }
            else if(firstWordOfLine == "NEW_FLOOR")
            {
                listOfNumber = utilitiesClass::readNumber(currentLine, 4);

                listOfFloor.push_back(editFloorStruct());
                listOfFloor.back().floor.setPosition(listOfNumber[0], listOfNumber[1]);
                listOfFloor.back().floor.setSize(sf::Vector2f(listOfNumber[2], listOfNumber[3]));
                listOfFloor.back().floor.setFillColor(sf::Color(0, 255, 0, 50));

                currentType = "NEW_FLOOR";
            }
            else
            {
                untestedGlobalLines.push_back(firstWordOfLine + " " + currentLine);
            }
        }
    }

    file.close();
}
