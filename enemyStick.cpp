#include "enemyStick.hpp"
#include "global.hpp"

enemyStickClass::enemyStickClass()
{
    sprite.setSize(sf::Vector2f(40, 80));
    sprite.setFillColor(sf::Color::Yellow);
    life = 50;
    weight = 2;
    speed = 5;
    enemyIAMode = PATROL_MODE;
    isWaitingForPatrol = false;
    currentFrame = 0;
    maxNumberOfFrame = 3;
    enemyViewDirection = RIGHT_VIEW;
    isInReload = false;
    waitForSoundAlarm = false;
    needToSoundAlarm = false;
    isInAlert = false;
}

bool enemyStickClass::update(stickmanClass& stickman)
{
    verticalVelocity += weight;

    useIA(stickman);

    if(enemyIAMode == PATROL_MODE || enemyIAMode == SOUND_ALARM_MODE)
    {
        if(horizontalVelocity > 0)
        {
            weapon.setRotation(0);
            enemyViewDirection = RIGHT_VIEW;
        }
        else if(horizontalVelocity < 0)
        {
            weapon.setRotation(180);
            enemyViewDirection = LEFT_VIEW;
        }
    }

    if(isInReload == true && reloadTimer.getElapsedTime().asSeconds() >= weapon.getTimeForReload())
    {
        weapon.reload(weapon.getSizeOfMagazine() - weapon.getNumberOfBulletInMagazine());
        isInReload = false;
    }

    ++currentFrame;
    currentFrame %= maxNumberOfFrame;

    return false;
}

void enemyStickClass::draw(sf::RenderWindow& window)
{
    window.draw(sprite);
    weapon.draw(window);
}

void enemyStickClass::useIA(stickmanClass& stickman)
{
    if(currentFrame == frameForCheck || enemyIAMode == SCOPE_MODE)
    {
        checkSeeStickman(stickman);
    }

    if(enemyIAMode == SOUND_ALARM_MODE)
    {
        if(global::activeLevel->getAlarmIsRinging() == true)
        {
            setWaitingScopeMode();
        }
        else
        {
            if(positionOfAlarm < sprite.getPosition().x)
            {
                horizontalVelocity = -speed;
            }
            else if(positionOfAlarm > sprite.getPosition().x + sprite.getSize().x)
            {
                horizontalVelocity = speed;
            }
            else
            {
                if(waitForSoundAlarm == true && waitingTimer.getElapsedTime().asSeconds() > currentReactionTime)
                {
                    global::activeLevel->soundAlarm();
                    setWaitingScopeMode();
                }
                else if(waitForSoundAlarm == false)
                {
                    waitForSoundAlarm = true;
                    horizontalVelocity = 0;
                    waitingTimer.restart();
                }
            }
        }
    }
    else if(enemyIAMode == SCOPE_MODE)
    {
        if(isInReload == false)
        {
            if(weapon.canShot() == true)
            {
                for(int i = 0; i < weapon.getNumberOfBulletWhenShot(); ++i)
                {
                    global::activeLevel->addBulletToList(weapon.shoot(i, true));
                }
            }

            if(weapon.getNumberOfBulletInMagazine() < weapon.getNumberOfBulletWhenShot())
            {
                isInReload = true;
                reloadTimer.restart();
            }
        }
    }
    else if(enemyIAMode == PATROL_MODE)
    {
        if(isWaitingForPatrol == false)
        {
            if(horizontalVelocity == 0)
            {
                if(sprite.getPosition().x <= firstPointPatrol.x)
                {
                    horizontalVelocity = speed;
                }
                else
                {
                    horizontalVelocity = -speed;
                }
            }

            if(horizontalVelocity < 0)
            {
                if(sprite.getPosition().x <= firstPointPatrol.x)
                {
                    isWaitingForPatrol = true;
                    horizontalVelocity = 0;
                    patrolWaitTimer.restart();
                }
            }
            else
            {
                if(sprite.getPosition().x >= secondPointPatrol.x)
                {
                    isWaitingForPatrol = true;
                    horizontalVelocity = 0;
                    patrolWaitTimer.restart();
                }
            }
        }
        else
        {
            if(patrolWaitTimer.getElapsedTime().asSeconds() > waitTimeForPatrol)
            {
                isWaitingForPatrol = false;
            }
        }
    }
}

int enemyStickClass::applyHorizontalMove()
{
    sprite.move(horizontalVelocity, 0);
    weapon.setPosition(sprite.getGlobalBounds().left + (sprite.getGlobalBounds().width / 2), sprite.getGlobalBounds().top + (sprite.getGlobalBounds().height / 2));

    return horizontalVelocity;
}

int enemyStickClass::applyVerticalMove()
{
    sprite.move(0, verticalVelocity);
    weapon.setPosition(sprite.getGlobalBounds().left + (sprite.getGlobalBounds().width / 2), sprite.getGlobalBounds().top + (sprite.getGlobalBounds().height / 2));

    return verticalVelocity;
}

bool enemyStickClass::damaged(int valueOfDamage)
{
    if(global::easterEggEnable == true)
    {
        valueOfDamage *= 2;
    }

    life -= valueOfDamage;

    currentReactionTime = reactionTime / 2;
    currentDistanceOfView = distanceOfViewAfterScope;
    needToSoundAlarm = true;

    if(enemyIAMode == PATROL_MODE)
    {
        setWaitingScopeMode();
    }

    if(life <= 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void enemyStickClass::hasEnterInCollide(direction dir)
{
    if(dir == UP || dir == DOWN)
    {
        verticalVelocity = 0;
    }
}

void enemyStickClass::lookAt(int x, int y, int cameraX, int cameraY)
{
    weapon.setLookAt(x, y, cameraX, cameraY);
}

bool enemyStickClass::hasThisItem(typeOfItem itemType, std::string nameOfItem)
{
    (void)itemType;
    (void)nameOfItem;

    return false;
}

seeMode enemyStickClass::seeThisPoint(sf::Vector2i centerOfEnemy, sf::Vector2i thisPoint)
{
    seeMode thisSeeMode;
    int distanceBetweenPoint = sqrt(((centerOfEnemy.x - thisPoint.x) * (centerOfEnemy.x - thisPoint.x)) + ((centerOfEnemy.y - thisPoint.y) * (centerOfEnemy.y - thisPoint.y)));

    if(distanceBetweenPoint < distanceWithInstantReactionTime)
    {
        thisSeeMode = INSTANT_SEE;
    }
    else if(distanceBetweenPoint < currentDistanceOfView)
    {
        thisSeeMode = CAN_SEE;

        if(distanceBetweenPoint < currentDistanceOfView / 2)
        {
            currentReactionTime = reactionTime / 2;
        }
    }
    else
    {
        thisSeeMode = CANT_SEE;
    }

    if(thisSeeMode != CANT_SEE)
    {
        if(global::activeLevel->checkPointCollideWithWall(centerOfEnemy, thisPoint) == true)
        {
            thisSeeMode = CANT_SEE;
        }
    }

    return thisSeeMode;
}

/*
Fonction: checkSeeStickman
Particularité: En plus de regarder si l'ennemi voit le stickman passé en paramètre (le joueur) elle adapte son comportement en fonction de différent paramètres.
*/

void enemyStickClass::checkSeeStickman(stickmanClass& stickman)
{
    sf::Vector2i centerOfEnemy = sf::Vector2i(sprite.getGlobalBounds().left + (sprite.getGlobalBounds().width / 2), sprite.getGlobalBounds().top + (sprite.getGlobalBounds().height / 2));
    sf::Vector2i centerOfStickman = sf::Vector2i(stickman.getSpriteBox().left + (stickman.getSpriteBox().width / 2), stickman.getSpriteBox().top + (stickman.getSpriteBox().height / 2));
    sf::Vector2i topPoint = sf::Vector2i(stickman.getSpriteBox().left + (stickman.getSpriteBox().width / 2), stickman.getSpriteBox().top);
    sf::Vector2i rightPoint = sf::Vector2i(stickman.getSpriteBox().left + stickman.getSpriteBox().width, stickman.getSpriteBox().top + (stickman.getSpriteBox().height / 2));
    sf::Vector2i bottomPoint = sf::Vector2i(stickman.getSpriteBox().left + (stickman.getSpriteBox().width / 2), stickman.getSpriteBox().top + stickman.getSpriteBox().height);
    sf::Vector2i leftPoint = sf::Vector2i(stickman.getSpriteBox().left, stickman.getSpriteBox().top + (stickman.getSpriteBox().height / 2));

    if(stickman.getIsHidden() == true && enemyIAMode == PATROL_MODE)
    {
        return;
    }

    if(global::activeLevel->getListOfFloor()->at(idFloorOfEnemy).floorArea.getGlobalBounds().intersects(stickman.getSpriteBox()) == true)
    {
        if(enemyIAMode == SOUND_ALARM_MODE)
        {
            if(iaType != "FEAR_IA")
            {
                if(((centerOfStickman.x < positionOfAlarm && centerOfStickman.x > centerOfEnemy.x) || (centerOfStickman.x > positionOfAlarm && centerOfStickman.x < centerOfEnemy.x)) ||
                    iaType == "ATTACK_IA")
                {
                    seeMode canSeeTopPoint = seeThisPoint(centerOfEnemy, topPoint);
                    seeMode canSeeRightPoint = seeThisPoint(centerOfEnemy, rightPoint);
                    seeMode canSeeBottomPoint = seeThisPoint(centerOfEnemy, bottomPoint);
                    seeMode canSeeLeftPoint = seeThisPoint(centerOfEnemy, leftPoint);

                    if(canSeeRightPoint == CAN_SEE || canSeeRightPoint == INSTANT_SEE)
                    {
                        lookAt(rightPoint.x, rightPoint.y, 0, 0);
                        setScopeMode();
                    }
                    else if(canSeeLeftPoint == CAN_SEE || canSeeLeftPoint == INSTANT_SEE)
                    {
                        lookAt(leftPoint.x, leftPoint.y, 0, 0);
                        setScopeMode();
                    }
                    else if(canSeeTopPoint == CAN_SEE || canSeeTopPoint == INSTANT_SEE)
                    {
                        lookAt(topPoint.x, topPoint.y, 0, 0);
                        setScopeMode();
                    }
                    else if(canSeeBottomPoint == CAN_SEE || canSeeBottomPoint == INSTANT_SEE)
                    {
                        lookAt(bottomPoint.x, bottomPoint.y, 0, 0);
                        setScopeMode();
                    }
                }
            }
        }
        else if(enemyIAMode != WAITING_SCOPE_MODE || (enemyIAMode == WAITING_SCOPE_MODE && waitingTimer.getElapsedTime().asSeconds() > currentReactionTime))
        {
            if(enemyIAMode == SCOPE_MODE || enemyIAMode == WAITING_SCOPE_MODE || ((enemyViewDirection == RIGHT_VIEW && rightPoint.x >= sprite.getPosition().x)
               || (enemyViewDirection == LEFT_VIEW && leftPoint.x <= sprite.getPosition().x + sprite.getGlobalBounds().width)))
            {
                seeMode canSeeTopPoint = seeThisPoint(centerOfEnemy, topPoint);
                seeMode canSeeRightPoint = seeThisPoint(centerOfEnemy, rightPoint);
                seeMode canSeeBottomPoint = seeThisPoint(centerOfEnemy, bottomPoint);
                seeMode canSeeLeftPoint = seeThisPoint(centerOfEnemy, leftPoint);

                if(((enemyIAMode == WAITING_SCOPE_MODE || enemyIAMode == SCOPE_MODE) && (canSeeTopPoint == CAN_SEE || canSeeRightPoint == CAN_SEE || canSeeBottomPoint == CAN_SEE
                   || canSeeLeftPoint == CAN_SEE)) || canSeeTopPoint == INSTANT_SEE || canSeeRightPoint == INSTANT_SEE || canSeeBottomPoint == INSTANT_SEE || canSeeLeftPoint == INSTANT_SEE)
                {
                    if(iaType == "FEAR_IA" && global::activeLevel->getAlarmIsRinging() == false)
                    {
                        setSoundAlarmMode();
                    }
                    else
                    {
                        if(canSeeRightPoint != CANT_SEE)
                        {
                            lookAt(rightPoint.x, rightPoint.y, 0, 0);
                        }
                        else if(canSeeLeftPoint != CANT_SEE)
                        {
                            lookAt(leftPoint.x, leftPoint.y, 0, 0);
                        }
                        else if(canSeeTopPoint != CANT_SEE)
                        {
                            lookAt(topPoint.x, topPoint.y, 0, 0);
                        }
                        else if(canSeeBottomPoint != CANT_SEE)
                        {
                            lookAt(bottomPoint.x, bottomPoint.y, 0, 0);
                        }

                        setScopeMode();
                    }
                }
                else if(canSeeTopPoint == CAN_SEE || canSeeRightPoint == CAN_SEE || canSeeBottomPoint == CAN_SEE || canSeeLeftPoint == CAN_SEE)
                {
                    setWaitingScopeMode();
                }
                else if(enemyIAMode == SCOPE_MODE)
                {
                    setWaitingScopeMode();
                }
                else if(enemyIAMode != PATROL_MODE)
                {
                    if(needToSoundAlarm == true && global::activeLevel->getAlarmIsRinging() == false)
                    {
                        setSoundAlarmMode();
                    }
                    else
                    {
                        setPatrolMode();
                    }
                }
            }
            else if(enemyIAMode != PATROL_MODE)
            {
                if(needToSoundAlarm == true && global::activeLevel->getAlarmIsRinging() == false)
                {
                    setSoundAlarmMode();
                }
                else
                {
                    setPatrolMode();
                }
            }
        }
    }
    else if(enemyIAMode != PATROL_MODE && enemyIAMode != SOUND_ALARM_MODE)
    {
        if(needToSoundAlarm == true && global::activeLevel->getAlarmIsRinging() == false)
        {
            setSoundAlarmMode();
        }
        else
        {
            setPatrolMode();
        }
    }
}

void enemyStickClass::alarmIsRinging()
{
    if(isInAlert == false)
    {
        waitTimeForPatrol /= 4;
        reactionTime /= 2;
        currentReactionTime /= 2;
        distanceOfView = distanceOfViewAfterScope;
        currentDistanceOfView = distanceOfView;
        isInAlert = true;
    }
}

bool enemyStickClass::getIsHidden()
{
    return false;
}

itemClass& enemyStickClass::getItemLoot()
{
    return itemLoot;
}

void enemyStickClass::setPatrolMode()
{
    enemyIAMode = PATROL_MODE;
    isWaitingForPatrol = true;
    if(global::activeLevel->getAlarmIsRinging() == false)
    {
        currentReactionTime = reactionTime;
        currentDistanceOfView = distanceOfView;
    }
    patrolWaitTimer.restart();
    sprite.setFillColor(sf::Color::Yellow);

    if(enemyViewDirection == RIGHT_VIEW)
    {
        weapon.setRotation(0);
    }
    else if(enemyViewDirection == LEFT_VIEW)
    {
        weapon.setRotation(180);
    }
}

void enemyStickClass::setWaitingScopeMode()
{
    enemyIAMode = WAITING_SCOPE_MODE;
    sprite.setFillColor(sf::Color(255, 128, 0));
    horizontalVelocity = 0;
    waitingTimer.restart();
}

void enemyStickClass::setScopeMode()
{
    enemyIAMode = SCOPE_MODE;
    sprite.setFillColor(sf::Color::Red);
    currentReactionTime = reactionTime / 2;
    currentDistanceOfView = distanceOfViewAfterScope;
    horizontalVelocity = 0;
    needToSoundAlarm = true;
}

void enemyStickClass::setSoundAlarmMode()
{
    enemyIAMode = SOUND_ALARM_MODE;
    sprite.setFillColor(sf::Color(80, 210, 150));
    positionOfAlarm = global::activeLevel->getListOfFloor()->at(idFloorOfEnemy).alarm.x;

    if(positionOfAlarm == -1)
    {
        alarmIsRinging();
        setScopeMode();
        needToSoundAlarm = false;
    }
}

void enemyStickClass::setPosition(int x, int y)
{
    sprite.setPosition(x, y);
    weapon.setPosition(sprite.getGlobalBounds().left + (sprite.getGlobalBounds().width / 2), sprite.getGlobalBounds().top + (sprite.getGlobalBounds().height / 2));
}

void enemyStickClass::setPatrolPoints(sf::Vector2f firstPoint, sf::Vector2f secondPoint)
{
    firstPointPatrol = firstPoint;
    secondPointPatrol = secondPoint;
}

void enemyStickClass::setWaitForPatrolTime(float time)
{
    waitTimeForPatrol = time;
}

void enemyStickClass::setDistanceOfViewInfo(int newDistanceOfView, int newDistanceOfViewAfterScope, int newDistanceWithInstantReactionTime)
{
    distanceOfView = newDistanceOfView;
    currentDistanceOfView = distanceOfView;
    distanceOfViewAfterScope = newDistanceOfViewAfterScope;
    distanceWithInstantReactionTime = newDistanceWithInstantReactionTime;
}

void enemyStickClass::setFrameForCheck(int newFrameForCheck)
{
    frameForCheck = newFrameForCheck;
}

void enemyStickClass::setIdFloorOfEnemy(int newIdFloor)
{
    idFloorOfEnemy = newIdFloor;
}

void enemyStickClass::setReactionTime(float time)
{
    reactionTime = time;
    currentReactionTime = reactionTime;
}

void enemyStickClass::setIAType(std::string newIAType)
{
    iaType = newIAType;
}

void enemyStickClass::setLife(int newLife)
{
    life = newLife;
}

void enemyStickClass::setIsHidden(bool newVal)
{
    (void)newVal;

    return;
}

void enemyStickClass::setItemLoot(typeOfItem newType, std::string newName, int newNumber)
{
    itemLoot = itemClass(newType, newName, newNumber);
}

void enemyStickClass::setWeapon(std::string newWeapon)
{
    weapon = weaponClass(newWeapon);
}
