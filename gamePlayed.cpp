#include <fstream>

#include "gamePlayed.hpp"
#include "utilities.hpp"
#include "global.hpp"

gamePlayedClass::gamePlayedClass()
{
    view.setSize(WIDTH_SCREEN, HEIGHT_SCREEN);
    view.setCenter(WIDTH_SCREEN / 2, HEIGHT_SCREEN / 2);
    speedView.x = 6;
    speedView.y = 20;

    ammoText.setFont(global::font);
    ammoText.setCharacterSize(20);
    ammoText.setColor(sf::Color::Red);
    ammoText.setPosition(10, 25);

    alarmTimeText.setFont(global::font);
    alarmTimeText.setCharacterSize(30);
    alarmTimeText.setColor(sf::Color::Red);
    alarmTimeText.setOrigin(0, static_cast<int>(alarmTimeText.getLocalBounds().top));

    lifeBar.setFillColor(sf::Color::Green);
    lifeBar.setPosition(10, 10);
    lifeBar.setSize(sf::Vector2f(playerStick.getLife(), 10));

    alertAlarm.setPosition(4, 4);
    alertAlarm.setSize(sf::Vector2f(WIDTH_SCREEN - 8, HEIGHT_SCREEN - 8));
    alertAlarm.setFillColor(sf::Color::Transparent);
    alertAlarm.setOutlineColor(sf::Color::Red);
    alertAlarm.setOutlineThickness(4);

    global::activeLevel = &level;
    level.setPlayerPointer(&playerStick);
    timeUntilLose = 30000;
    pauseTime = 0;
    alarmIsRinging = false;
    allWitnessAreDead = false;
    needToUpdateHud = false;
    isInShake = false;
    timeIsOver = false;

    updateAmmoText();
}

void gamePlayedClass::update()
{
    if(alarmIsRinging == true && global::activeGameStateStack->getPreviouslyChange() == true)
    {
        pauseTime += pauseTimer.getElapsedTime().asMilliseconds();
    }

    level.checkStickmanCollideWithWall(playerStick, true, playerStick.applyHorizontalMove());
    level.checkStickmanCollideWithWall(playerStick, false, playerStick.applyVerticalMove());

    if(level.update(playerStick) == true)
    {
        for(bulletClass& bullet : level.getListOfBulletNeedAdd())
        {
            listOfBullet.push_back(bullet);
        }

        level.clearListOfBullet();
    }

    checkStickmenCollideWithBullet(false);
    checkStickmenCollideWithBullet(true);

    if(alarmIsRinging == false)
    {
        alarmIsRinging = level.getAlarmIsRinging();

        if(alarmIsRinging == true)
        {
            alarmTimer.restart();
        }
    }

    if(playerStick.update() == true || needToUpdateHud == true)
    {
        updateAmmoText();
    }

    setCameraToStickman(playerStick);

    if(isInShake == true)
    {
        manageShake();
    }

    if(alarmIsRinging == true)
    {
        updateAlarmTimeText();

        if((alarmTimer.getElapsedTime().asMilliseconds() - pauseTime) > timeUntilLose)
        {
            timeIsOver = true;
        }
    }

    pauseTimer.restart();
}

void gamePlayedClass::draw(sf::RenderWindow& window)
{
    window.setView(view);
    level.drawFloorBackground(window, sf::Vector2f(view.getCenter().x - (WIDTH_SCREEN / 2), view.getCenter().y - (HEIGHT_SCREEN / 2)), sf::Vector2f(WIDTH_SCREEN, HEIGHT_SCREEN));

    for(bulletClass& bullet : listOfBullet)
    {
        bullet.draw(window);
    }

    if(playerStick.getIsHidden() == true)
    {
        playerStick.draw(window);
    }

    level.drawEnemy(window, playerStick);

    if(playerStick.getIsHidden() == false)
    {
        playerStick.draw(window);
    }

    level.drawItem(window, playerStick);
    level.drawFloorShadow(window, playerStick);

    window.setView(window.getDefaultView());
    window.draw(lifeBar);
    window.draw(ammoText);

    if(alarmIsRinging == true)
    {
        window.draw(alertAlarm);
        window.draw(alarmTimeText);
    }
}

void gamePlayedClass::updateAmmoText()
{
    std::string newText;

    newText += utilitiesClass::intToString(playerStick.getNumberOfBulletInMagazine());
    newText += " / ";
    newText += utilitiesClass::intToString(playerStick.getNumberOfBulletInInentory());

    ammoText.setString(newText);
}

void gamePlayedClass::updateAlarmTimeText()
{
    unsigned int timeToShow = timeUntilLose - alarmTimer.getElapsedTime().asMilliseconds() + pauseTime;
    std::string newText;

    newText += utilitiesClass::intToString(timeToShow / 60000);
    newText += ":";

    if(((timeToShow % 60000) / 1000) < 10)
    {
        newText += "0";
    }

    newText += utilitiesClass::intToString((timeToShow % 60000) / 1000);
    newText += ":";

    if((((timeToShow % 60000) % 1000) / 10) < 10)
    {
        newText += "0";
    }

    newText += utilitiesClass::intToString(((timeToShow % 60000) % 1000) / 10);

    alarmTimeText.setString(newText);
    alarmTimeText.setOrigin(0, static_cast<int>(alarmTimeText.getLocalBounds().top));
    alarmTimeText.setPosition((WIDTH_SCREEN / 2) - (alarmTimeText.getGlobalBounds().width / 2), 10);
}

void gamePlayedClass::jumpPlayer()
{
    if(playerStick.getIsHidden() == false)
    {
        playerStick.jump();
    }
}

void gamePlayedClass::movePlayerTo(direction newDir)
{
    if(playerStick.getIsHidden() == false)
    {
        playerStick.moveTo(newDir);
    }
}

void gamePlayedClass::playerLookAt(int x, int y)
{
    playerStick.lookAt(x - (view.getViewport().left * WIDTH_SCREEN), y - (view.getViewport().top * HEIGHT_SCREEN), view.getCenter().x - (WIDTH_SCREEN / 2), view.getCenter().y - (HEIGHT_SCREEN / 2));
}

void gamePlayedClass::playerShoot(bool isFull)
{
    if(playerStick.getIsHidden() == false)
    {
        lineStruct lineOfShot = playerStick.getLineOfShot();

        if(level.checkPointCollideWithWall(lineOfShot.secondPoint, lineOfShot.firstPoint) == false)
        {
            for(bulletClass& bullet : playerStick.shoot(isFull))
            {
                listOfBullet.push_back(bullet);
            }
        }

        updateAmmoText();
    }
}

void gamePlayedClass::playerReload()
{
    playerStick.reloadNeeded();
    updateAmmoText();
}

void gamePlayedClass::playerUseSomething()
{
    if(level.stickmanUseSwitch(playerStick) == true)
    {
        minimumYPosForCamera = level.getMinimumYPosForCamera(playerStick.getSpriteBox());
        maximumYPosForCamera = level.getMaximumYPosForCamera(playerStick.getSpriteBox());
    }
    else if(level.stickmanUseHideZone(playerStick) == true)
    {
        playerStick.moveTo(NOWHERE);
    }
    else if(level.stickmanUseChest(playerStick) == true)
    {
        //vide
    }
}

int gamePlayedClass::playerTakeItem(std::list<itemClass>::iterator& thisItem)
{
    return level.stickmanTakeItem(playerStick, thisItem);
}

void gamePlayedClass::selectWeapon(int number)
{
    playerStick.selectWeapon(number);
    updateAmmoText();
}

bool gamePlayedClass::playerHasLost()
{
    return ((playerStick.getLife() <= 0) || (timeIsOver == true));
}

bool gamePlayedClass::playerHasWon()
{
    return allWitnessAreDead;
}

void gamePlayedClass::checkStickmenCollideWithBullet(bool moveBullet)
{
    for(auto bulletIte = listOfBullet.begin(); bulletIte != listOfBullet.end(); )
    {
        if(moveBullet == true)
        {
            bulletIte->applyMove();
        }

        if(bulletIte->getIsAgainstPlayer() == false)
        {
            if(level.checkPointCollideWithWall(bulletIte->getCenter(), bulletIte->getOldCenter(), bulletIte->getDamageValue(), true) == true)
            {
                if(global::easterEggEnable == true)
                {
                    startShake();
                }

                allWitnessAreDead = level.getAllWitnessAreDead();
                listOfBullet.erase(bulletIte++);
                continue;
            }
        }
        else
        {
            if(level.checkPointCollideWithWall(bulletIte->getCenter(), bulletIte->getOldCenter(), bulletIte->getDamageValue(), false, true) == true)
            {
                lifeBar.setSize(sf::Vector2f(playerStick.getLife(), 10));
                listOfBullet.erase(bulletIte++);
                continue;
            }
        }

        ++bulletIte;
    }
}

void gamePlayedClass::startShake()
{
    isInShake = true;
    shakeTimer.restart();
}

void gamePlayedClass::manageShake()
{
    if(shakeTimer.getElapsedTime().asSeconds() < 0.5)
    {
        shakeCamera();
    }
    else
    {
        isInShake = false;
        view.setViewport(sf::FloatRect(0, 0, 1, 1));
    }
}

void gamePlayedClass::shakeCamera()
{
    sf::FloatRect currentViewPort = view.getViewport();

    currentViewPort.left += ((std::rand() % 101) - 50) / 1000.;

    if(currentViewPort.left < -0.1)
    {
        currentViewPort.left = -0.05;
    }
    else if(currentViewPort.left > 0.1)
    {
        currentViewPort.left = 0.05;
    }

    currentViewPort.top += ((std::rand() % 101) - 50) / 1000.;

    if(currentViewPort.top <= -0.1)
    {
        currentViewPort.top = 0.05;
    }
    else if(currentViewPort.top > 0.1)
    {
        currentViewPort.top = 0.05;
    }

    view.setViewport(currentViewPort);
}

inventoryClass* gamePlayedClass::getPlayerInventory()
{
    return playerStick.getInventory();
}

std::vector<floorStruct>* gamePlayedClass::getListOfFloor()
{
    return level.getListOfFloor();
}

playerStickClass* gamePlayedClass::getPlayer()
{
    return &playerStick;
}

sf::Vector2f gamePlayedClass::getPlayerFootPosition()
{
    needToUpdateHud = true;

    return playerStick.getFootPosition();
}

sf::Vector2i gamePlayedClass::getSizeOfLevel()
{
    return level.getLimitOfLevel();
}

std::string gamePlayedClass::getNextLevel()
{
    return level.getNextLevel();
}

void gamePlayedClass::setCameraToStickman(stickmanClass& stickman)
{
    bool centerToPlayer = (stickman.getSpriteBox().top + stickman.getSpriteBox().height / 2) > minimumYPosForCamera && (stickman.getSpriteBox().top + stickman.getSpriteBox().height / 2) < maximumYPosForCamera;

    if((stickman.getSpriteBox().left + stickman.getSpriteBox().width / 2) < view.getCenter().x - speedView.x)
    {
        view.move(-speedView.x, 0);
    }
    else if((stickman.getSpriteBox().left + stickman.getSpriteBox().width / 2) > view.getCenter().x + speedView.x)
    {
        view.move(speedView.x, 0);
    }
    else
    {
        view.setCenter(stickman.getSpriteBox().left + stickman.getSpriteBox().width / 2, view.getCenter().y);
    }

    if(((stickman.getSpriteBox().top + stickman.getSpriteBox().height / 2) < view.getCenter().y - speedView.y && centerToPlayer == true) ||
        (centerToPlayer == false && minimumYPosForCamera < view.getCenter().y - speedView.y))
    {
        view.move(0, -speedView.y);
    }
    else if(((stickman.getSpriteBox().top + stickman.getSpriteBox().height / 2) > view.getCenter().y + speedView.y && centerToPlayer == true) ||
            (centerToPlayer == false && maximumYPosForCamera > view.getCenter().y + speedView.y))
    {
        view.move(0, speedView.y);
    }
    else
    {
        if((stickman.getSpriteBox().top + stickman.getSpriteBox().height / 2) < minimumYPosForCamera)
        {
            view.setCenter(view.getCenter().x, minimumYPosForCamera);
        }
        else if((stickman.getSpriteBox().top + stickman.getSpriteBox().height / 2) > maximumYPosForCamera)
        {
            view.setCenter(view.getCenter().x, maximumYPosForCamera);
        }
        else
        {
            view.setCenter(view.getCenter().x, stickman.getSpriteBox().top + stickman.getSpriteBox().height / 2);
        }
    }

    if(view.getCenter().x < (WIDTH_SCREEN / 2))
    {
        view.setCenter((WIDTH_SCREEN / 2), view.getCenter().y);
    }
    else if(view.getCenter().x > level.getLimitOfLevel().x - (WIDTH_SCREEN / 2))
    {
        view.setCenter(level.getLimitOfLevel().x - (WIDTH_SCREEN / 2), view.getCenter().y);
    }

    if(view.getCenter().y < (HEIGHT_SCREEN / 2))
    {
        view.setCenter(view.getCenter().x, (HEIGHT_SCREEN / 2));
    }
    else if(view.getCenter().y > level.getLimitOfLevel().y - (HEIGHT_SCREEN / 2))
    {
        view.setCenter(view.getCenter().x, level.getLimitOfLevel().y - (HEIGHT_SCREEN / 2));
    }
}

void gamePlayedClass::setLevel(std::string levelName)
{
    level.loadLevel(levelName);

    playerStick.setPosition(level.getInitalPosition().x, level.getInitalPosition().y);
    view.setCenter(playerStick.getSpriteBox().left + playerStick.getSpriteBox().width / 2, playerStick.getSpriteBox().top + playerStick.getSpriteBox().height / 2);

    playerStick.getInventory()->clearInventory();

    for(itemInInventoryStruct newItem : level.getListOfItemForThisLevel())
    {
        playerStick.getInventory()->addThisItem(newItem);
    }

    selectWeapon(1);

    minimumYPosForCamera = level.getMinimumYPosForCamera(playerStick.getSpriteBox());
    maximumYPosForCamera = level.getMaximumYPosForCamera(playerStick.getSpriteBox());
}
