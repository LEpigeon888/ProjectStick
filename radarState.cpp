#include "radarState.hpp"
#include "global.hpp"

sf::Clock radarStateClass::scannerTimer;

radarStateClass::radarStateClass(std::vector<floorStruct>* newListOfFloor, playerStickClass* newPlayerStick, sf::Vector2i newSizeOfLevel, std::string nameOfLevel)
{
    mapTexture.loadFromFile("level/" + nameOfLevel + "/map.png");
    mapSprite.setTexture(mapTexture);
    mapSprite.setPosition(0, 0);

    scannerSprite.setSize(sf::Vector2f(20, HEIGHT_SCREEN));
    scannerSprite.setFillColor(sf::Color::Green);

    listOfFloor = newListOfFloor;
    playerStick = newPlayerStick;
    sizeOfLevel = newSizeOfLevel;

    numberOfWaitFrame = 2;
    currentFrame = 0;
    maxThickness = 8;
    maxRadius = 6;
    inScan = false;
    timeBetweenScan = 5;
    speedScan = 20;
    sizeOfLevel.x = global::activeLevel->getLimitOfLevel().x;
    sizeOfLevel.y = global::activeLevel->getLimitOfLevel().y;
}

void radarStateClass::update(sf::RenderWindow& window)
{
    sf::Event event;

    while(window.pollEvent(event))
    {
        if(event.type == sf::Event::Closed)
        {
            window.close();
            return;
        }
        else if(event.type == sf::Event::KeyPressed)
        {
            if(event.key.code == sf::Keyboard::Tab)
            {
                stopScan();
                global::activeGameStateStack->pop();
                return;
            }
        }
    }

    ++currentFrame;

    if(currentFrame > numberOfWaitFrame)
    {
        currentFrame = 0;
    }

    if(currentFrame == 0)
    {
        for(auto circleIte = listOfCircle.begin(); circleIte != listOfCircle.end(); )
        {
            if(circleIte->getOutlineThickness() < maxThickness)
            {
                circleIte->setOutlineThickness(circleIte->getOutlineThickness() + 1);

                if(circleIte->getOutlineThickness() >= maxThickness)
                {
                    circleIte->setFillColor(sf::Color::Transparent);
                }
            }
            else
            {
                circleIte->setRadius(circleIte->getRadius() + 0.1);
            }

            if(circleIte->getRadius() > maxRadius)
            {
                listOfCircle.erase(circleIte++);
                continue;
            }

            circleIte->setOrigin(circleIte->getRadius(), circleIte->getRadius());

            ++circleIte;
        }
    }

    if(scannerTimer.getElapsedTime().asSeconds() > timeBetweenScan && inScan == false)
    {
        startScan();
    }

    if(inScan == true)
    {
        moveScannerAndScan();
    }

    global::activeGameStateStack->oldUpdate(window);
}

void radarStateClass::draw(sf::RenderWindow& window)
{
    window.setView(window.getDefaultView());
    window.draw(mapSprite);

    for(sf::CircleShape& thisCircle : listOfCircle)
    {
        window.draw(thisCircle);
    }

    if(inScan == true)
    {
        window.draw(scannerSprite);
    }
}

void radarStateClass::startScan()
{
    scannerSprite.setPosition(0, 0);
    listOfFuturCircle.clear();

    for(floorStruct& thisFloor : *listOfFloor)
    {
        for(enemyStickClass& thisEnemy : thisFloor.listOfEnemy)
        {
            listOfFuturCircle.push_back(sf::Vector2f((thisEnemy.getCollideBox().left + (thisEnemy.getCollideBox().width / 2)) * (WIDTH_SCREEN / static_cast<double>(sizeOfLevel.x)),
                                                      (thisEnemy.getCollideBox().top + (thisEnemy.getCollideBox().height / 2)) * (HEIGHT_SCREEN / static_cast<double>(sizeOfLevel.y))));
        }
        for(witnessStickClass& thisWitness : thisFloor.listOfWitness)
        {
            listOfFuturCircle.push_back(sf::Vector2f((thisWitness.getCollideBox().left + (thisWitness.getCollideBox().width / 2)) * (WIDTH_SCREEN / static_cast<double>(sizeOfLevel.x)),
                                                      (thisWitness.getCollideBox().top + (thisWitness.getCollideBox().height / 2)) * (HEIGHT_SCREEN / static_cast<double>(sizeOfLevel.y))));
        }
    }

    listOfFuturCircle.push_back(sf::Vector2f((playerStick->getCollideBox().left + (playerStick->getCollideBox().width / 2)) * (WIDTH_SCREEN / static_cast<double>(sizeOfLevel.x)),
                                              (playerStick->getCollideBox().top + (playerStick->getCollideBox().height / 2)) * (HEIGHT_SCREEN / static_cast<double>(sizeOfLevel.y))));

    inScan = true;
}

void radarStateClass::stopScan()
{
    if(inScan == true)
    {
        scannerTimer.restart();
    }
}

void radarStateClass::moveScannerAndScan()
{
    scannerSprite.move(speedScan, 0);

    for(auto futurCircleIte = listOfFuturCircle.begin(); futurCircleIte != listOfFuturCircle.end(); )
    {
        if(futurCircleIte->x < scannerSprite.getPosition().x + scannerSprite.getSize().x)
        {
            listOfCircle.push_back(sf::CircleShape());
            listOfCircle.back().setRadius(1);
            listOfCircle.back().setPosition(futurCircleIte->x, futurCircleIte->y);
            listOfCircle.back().setOutlineThickness(0);
            listOfCircle.back().setOutlineColor(sf::Color::White);
            listOfCircle.back().setFillColor(sf::Color::White);

            listOfFuturCircle.erase(futurCircleIte++);
            continue;
        }
        ++futurCircleIte;
    }

    if(scannerSprite.getPosition().x >= WIDTH_SCREEN)
    {
        inScan = false;
        scannerTimer.restart();
    }
}
