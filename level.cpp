#include <fstream>
#include <algorithm>

#include "level.hpp"
#include "utilities.hpp"

bool levelClass::update(stickmanClass& stickman)
{
    for(floorStruct& thisFloor : listOfFloor)
    {
        for(enemyStickClass& thisEnemy : thisFloor.listOfEnemy)
        {
            thisEnemy.update(stickman);

            checkStickmanCollideWithWall(thisEnemy, true, thisEnemy.applyHorizontalMove());
            checkStickmanCollideWithWall(thisEnemy, false, thisEnemy.applyVerticalMove());
        }

        for(witnessStickClass& thisWitness : thisFloor.listOfWitness)
        {
            checkStickmanCollideWithWall(thisWitness, true, thisWitness.applyHorizontalMove());
            checkStickmanCollideWithWall(thisWitness, false, thisWitness.applyVerticalMove());
        }

        for(itemClass& thisItem : thisFloor.listOfItem)
        {
            thisItem.update();

            checkItemCollideWithWall(thisItem, thisItem.applyVerticalMove());
        }
    }

    return !listOfBulletNeedAdd.empty();
}

void levelClass::drawFloorBackground(sf::RenderWindow& window, sf::Vector2f viewPosition, sf::Vector2f viewSize)
{
    for(floorStruct& thisFloor : listOfFloor)
    {
        if(thisFloor.floorBackground.getPosition().x < viewPosition.x + viewSize.x && thisFloor.floorBackground.getPosition().x + thisFloor.floorBackground.getGlobalBounds().width > viewPosition.x &&
           thisFloor.floorBackground.getPosition().y < viewPosition.y + viewSize.y && thisFloor.floorBackground.getPosition().y + thisFloor.floorBackground.getGlobalBounds().height > viewPosition.y)
        {
            window.draw(thisFloor.floorBackground);

            for(door& thisDoor : thisFloor.listOfDoor)
            {
                window.draw(thisDoor.sprite);
            }
        }
    }
}

void levelClass::drawEnemy(sf::RenderWindow& window, stickmanClass& stickman)
{
    for(floorStruct& thisFloor : listOfFloor)
    {
        if(thisFloor.floorArea.getGlobalBounds().intersects(stickman.getCollideBox()) == true)
        {
            for(enemyStickClass& thisEnemy : thisFloor.listOfEnemy)
            {
                thisEnemy.draw(window);
            }

            for(witnessStickClass& thisWitness : thisFloor.listOfWitness)
            {
                thisWitness.draw(window);
            }
        }
    }
}

void levelClass::drawItem(sf::RenderWindow& window, stickmanClass& stickman)
{
    for(floorStruct& thisFloor : listOfFloor)
    {
        if(thisFloor.floorArea.getGlobalBounds().intersects(stickman.getCollideBox()) == true)
        {
            for(itemClass& thisItem : thisFloor.listOfItem)
            {
                thisItem.draw(window);
            }
        }
    }
}

void levelClass::drawFloorShadow(sf::RenderWindow& window, stickmanClass& stickman)
{
    for(floorStruct& thisFloor : listOfFloor)
    {
        if(thisFloor.floorArea.getGlobalBounds().intersects(stickman.getCollideBox()) == false)
        {
            window.draw(thisFloor.floorArea);
        }
    }
}

void levelClass::checkStickmanCollideWithWall(stickmanClass& stickman, bool horizontalMove, int distanceMove)
{
    sf::Vector2i newPosition;

    newPosition = checkBoxCollideWithWall(stickman.getCollideBox(), horizontalMove, distanceMove, true);

    if(newPosition != sf::Vector2i(-1, -1))
    {
        if(horizontalMove == true)
        {
            if(distanceMove > 0)
            {
                stickman.hasEnterInCollide(RIGHT);
            }
            else
            {
                stickman.hasEnterInCollide(LEFT);
            }
        }
        else
        {
            if(distanceMove > 0)
            {
                stickman.hasEnterInCollide(DOWN);
            }
            else
            {
                stickman.hasEnterInCollide(UP);
            }
        }

        stickman.setPosition(newPosition.x, newPosition.y);
    }

    if(horizontalMove == true)
    {
        for(floorStruct& thisFloor : listOfFloor)
        {
            if(thisFloor.floorArea.getGlobalBounds().intersects(stickman.getCollideBox()) == true)
            {
                for(auto doorIte = thisFloor.listOfDoor.begin(); doorIte != thisFloor.listOfDoor.end(); )
                {
                    if(doorIte->sprite.getGlobalBounds().intersects(stickman.getCollideBox()) == true && stickman.hasThisItem(USEABLE_ITEM, doorIte->nameOfKey) == true)
                    {
                        thisFloor.listOfDoor.erase(doorIte++);
                        continue;
                    }
                    ++doorIte;
                }

                stickmanUseSwitchDoor(stickman, distanceMove, thisFloor);
            }
        }
    }
}

/*
Fonction: checkPointCollideWithWall
Particularité: Contrairement à ce que son nom indique en plus de gérer la collision entre un point (souvent une balle) et un mur elle paut aussi s'occuper
d'infliger des dégats aux ennemis ou au joueur (selon de qui provient la balle) si ça lui ai demandé.
*/

bool levelClass::checkPointCollideWithWall(sf::Vector2i point, sf::Vector2i oldPoint, int damageOfBullet, bool checkWithEnemyToo, bool checkWithPlayerToo)
{
    long shortestDistance = 999999999;
    bool inCollision = false;
    bool verticalIsTrue = pointCollideWithTheseWall(point, oldPoint, verticalLevelWall, shortestDistance);
    bool horizontalIsTrue = pointCollideWithTheseWall(point, oldPoint, horizontalLevelWall, shortestDistance, true);

    if(verticalIsTrue == true || horizontalIsTrue == true)
    {
        if(checkWithEnemyToo == false)
        {
            return true;
        }
        else
        {
            inCollision = true;
        }
    }

    for(floorStruct& thisFloor : listOfFloor)
    {
        if(thisFloor.floorArea.getGlobalBounds().contains(point.x, point.y) == true || thisFloor.floorArea.getGlobalBounds().contains(oldPoint.x, oldPoint.y) == true)
        {
            verticalIsTrue = pointCollideWithTheseWall(point, oldPoint, thisFloor.verticalWall, shortestDistance);
            horizontalIsTrue = pointCollideWithTheseWall(point, oldPoint, thisFloor.horizontalWall, shortestDistance, true);

            for(door& thisDoor : thisFloor.listOfDoor)
            {
                verticalIsTrue = pointCollideWithTheseWall(point, oldPoint, thisDoor.listOfWall, shortestDistance) || verticalIsTrue;
            }

            if(verticalIsTrue == true || horizontalIsTrue == true)
            {
                if(checkWithEnemyToo == false)
                {
                    return true;
                }
                else
                {
                    inCollision = true;
                }
            }
        }
    }

    if(checkWithEnemyToo == true)
    {
        long distanceWithEnemy = 999999999;
        long distanceWithWitness = 999999999;

        for(floorStruct& thisFloor : listOfFloor)
        {
            if(thisFloor.floorArea.getGlobalBounds().contains(point.x, point.y) == true || thisFloor.floorArea.getGlobalBounds().contains(oldPoint.x, oldPoint.y) == true)
            {
                for(auto enemyIte = thisFloor.listOfEnemy.begin(); enemyIte != thisFloor.listOfEnemy.end(); )
                {
                    distanceWithEnemy = 999999999;

                    if(pointCollideWithThisBox(point, oldPoint, enemyIte->getCollideBox(), distanceWithEnemy) == true)
                    {
                        inCollision = true;
                        if(distanceWithEnemy <= shortestDistance)
                        {
                            if(enemyIte->damaged(damageOfBullet) == true)
                            {
                                if(enemyIte->getItemLoot().getNumberOfItem() > 0)
                                {
                                    addItemAtThisPoint(enemyIte->getItemLoot(), enemyIte->getFootPosition());
                                }

                                thisFloor.listOfEnemy.erase(enemyIte++);
                                continue;
                            }
                        }
                    }

                    ++enemyIte;
                }

                for(auto witnessIte = thisFloor.listOfWitness.begin(); witnessIte != thisFloor.listOfWitness.end(); )
                {
                    distanceWithWitness = 999999999;

                    if(pointCollideWithThisBox(point, oldPoint, witnessIte->getCollideBox(), distanceWithWitness) == true)
                    {
                        inCollision = true;
                        if(distanceWithWitness <= shortestDistance)
                        {
                            if(witnessIte->damaged(damageOfBullet) == true)
                            {
                                thisFloor.listOfWitness.erase(witnessIte++);
                                continue;
                            }
                        }
                    }

                    ++witnessIte;
                }
            }
        }
    }

    if(checkWithPlayerToo == true)
    {
        long distanceWithPlayer = 999999999;

        if(pointCollideWithThisBox(point, oldPoint, playerStick->getCollideBox(), distanceWithPlayer) == true)
        {
            inCollision = true;
            if(distanceWithPlayer <= shortestDistance)
            {
                playerStick->damaged(damageOfBullet);
            }
        }
    }

    return inCollision;
}

void levelClass::checkItemCollideWithWall(itemClass& item, int distanceMove)
{
    sf::Vector2i newPosition;

    newPosition = checkBoxCollideWithWall(item.getCollideBox(), false, distanceMove, false);

    if(newPosition != sf::Vector2i(-1, -1))
    {
        item.hasEnterInCollide();

        item.setPosition(newPosition.x, newPosition.y);
    }
}

bool levelClass::pointCollideWithTheseWall(sf::Vector2i point, sf::Vector2i oldPoint, std::list<wall>& thisListOfWall, long& shortestDistance, bool needSwitch)
{
    sf::Vector2i oldPosition;
    sf::Vector2i newPosition;
    int positionWhenReachWall = 0;

    if(needSwitch == true)
    {
        std::swap(point.x, point.y);
        std::swap(oldPoint.x, oldPoint.y);
    }

    if(point.x != oldPoint.x)
    {
        if(point.x - oldPoint.x > 0)
        {
            oldPosition = oldPoint;
            newPosition = point;
        }
        else
        {
            newPosition = oldPoint;
            oldPosition = point;
        }

        for(wall thisWall : thisListOfWall)
        {
            if(needSwitch == true)
            {
                std::swap(thisWall.firstPoint.x, thisWall.firstPoint.y);
                std::swap(thisWall.secondPoint.x, thisWall.secondPoint.y);
            }

            if(thisWall.typeOfWall == "STICKMAN_WALL")
            {
                continue;
            }

            positionWhenReachWall = oldPosition.y + (((newPosition.y - oldPosition.y) / static_cast<double>(newPosition.x - oldPosition.x)) * static_cast<double>(thisWall.firstPoint.x - oldPosition.x));

            if(oldPosition.x <= thisWall.firstPoint.x && newPosition.x >= thisWall.firstPoint.x &&
               positionWhenReachWall + 1 >= thisWall.firstPoint.y && positionWhenReachWall - 1 <= thisWall.secondPoint.y)
            {
                int distance = (point.x - thisWall.firstPoint.x) * (point.x - thisWall.firstPoint.x) + (point.y - positionWhenReachWall) * (point.y - positionWhenReachWall);

                if(distance < shortestDistance)
                {
                    shortestDistance = distance;
                }

                return true;
            }
        }
    }

    return false;
}

bool levelClass::pointCollideWithThisBox(sf::Vector2i point, sf::Vector2i oldPoint, sf::FloatRect thisBox, long& shortestDistance)
{
    std::list<wall> fakeListVerticalOfWall;
    std::list<wall> fakeListHorizontalOfWall;

    fakeListVerticalOfWall.push_back(wall(sf::Vector2i(thisBox.left, thisBox.top), sf::Vector2i(thisBox.left, thisBox.top + thisBox.height)));
    fakeListVerticalOfWall.push_back(wall(sf::Vector2i(thisBox.left + thisBox.width, thisBox.top), sf::Vector2i(thisBox.left + thisBox.width, thisBox.top + thisBox.height)));

    fakeListHorizontalOfWall.push_back(wall(sf::Vector2i(thisBox.left, thisBox.top), sf::Vector2i(thisBox.left + thisBox.width, thisBox.top)));
    fakeListHorizontalOfWall.push_back(wall(sf::Vector2i(thisBox.left, thisBox.top + thisBox.height), sf::Vector2i(thisBox.left + thisBox.width, thisBox.top + thisBox.height)));

    if(pointCollideWithTheseWall(point, oldPoint, fakeListVerticalOfWall, shortestDistance) == true ||
       pointCollideWithTheseWall(point, oldPoint, fakeListHorizontalOfWall, shortestDistance, true) == true)
    {
        return true;
    }
    else
    {
        return false;
    }
}

sf::Vector2i levelClass::checkBoxCollideWithWall(sf::FloatRect collideBox, bool horizontalMove, int distanceMove, bool isStickman)
{
    sf::FloatRect oldCollideBox = collideBox;
    sf::Vector2i newPosition = sf::Vector2i(-1, -1);
    sf::Vector2i tmpPosition = sf::Vector2i(-1, -1);

    if(distanceMove == 0)
    {
        return newPosition;
    }

    if(horizontalMove == true)
    {
        oldCollideBox.left -= distanceMove;
    }
    else
    {
        oldCollideBox.top -= distanceMove;
    }

    for(floorStruct& thisFloor : listOfFloor)
    {
        if(thisFloor.floorArea.getGlobalBounds().intersects(collideBox) == true || thisFloor.floorArea.getGlobalBounds().intersects(oldCollideBox) == true)
        {
            if(horizontalMove == true)
            {
                tmpPosition = boxCollideWithTheseWall(collideBox, thisFloor.verticalWall, distanceMove, isStickman);
                if(tmpPosition != sf::Vector2i(-1, -1))
                {
                    newPosition = tmpPosition;
                    if(distanceMove > 0)
                    {
                        distanceMove -= fabs(newPosition.x - collideBox.left);
                    }
                    else
                    {
                        distanceMove += fabs(newPosition.x - collideBox.left);
                    }
                    collideBox.left = newPosition.x;
                }

                for(door& thisDoor : thisFloor.listOfDoor)
                {
                    tmpPosition = boxCollideWithTheseWall(collideBox, thisDoor.listOfWall, distanceMove, isStickman);
                    if(tmpPosition != sf::Vector2i(-1, -1))
                    {
                        newPosition = tmpPosition;
                        if(distanceMove > 0)
                        {
                            distanceMove -= fabs(newPosition.x - collideBox.left);
                        }
                        else
                        {
                            distanceMove += fabs(newPosition.x - collideBox.left);
                        }
                        collideBox.left = newPosition.x;
                    }
                }
            }
            else
            {
                tmpPosition = boxCollideWithTheseWall(collideBox, thisFloor.horizontalWall, distanceMove, isStickman, true);
                if(tmpPosition != sf::Vector2i(-1, -1))
                {
                    newPosition = tmpPosition;
                    if(distanceMove > 0)
                    {
                        distanceMove -= fabs(newPosition.y - collideBox.top);
                    }
                    else
                    {
                        distanceMove += fabs(newPosition.y - collideBox.top);
                    }
                    collideBox.top = newPosition.y;
                }
            }
        }
    }

    if(newPosition == sf::Vector2i(-1, -1))
    {
        if(horizontalMove == true)
        {
            newPosition = boxCollideWithTheseWall(collideBox, verticalLevelWall, distanceMove, isStickman);
        }
        else
        {
            newPosition = boxCollideWithTheseWall(collideBox, horizontalLevelWall, distanceMove, isStickman, true);
        }
    }

    return newPosition;
}

sf::Vector2i levelClass::boxCollideWithTheseWall(sf::FloatRect collideBox, std::list<wall>& thisListOfWall, int distanceMove, bool isStickman, bool needSwitch)
{
    sf::Vector2i newPosition = sf::Vector2i(-1, -1);

    if(needSwitch == true)
    {
        std::swap(collideBox.left, collideBox.top);
        std::swap(collideBox.width, collideBox.height);
    }

    for(wall thisWall : thisListOfWall)
    {
        if(needSwitch == true)
        {
            std::swap(thisWall.firstPoint.x, thisWall.firstPoint.y);
            std::swap(thisWall.secondPoint.x, thisWall.secondPoint.y);
        }

        if(isStickman == false && thisWall.typeOfWall == "STICKMAN_WALL")
        {
            continue;
        }

        if(distanceMove > 0)
        {
            if(collideBox.left + collideBox.width - distanceMove <= thisWall.firstPoint.x && collideBox.left + collideBox.width > thisWall.firstPoint.x
               && collideBox.top <= thisWall.secondPoint.y && collideBox.top + collideBox.height > thisWall.firstPoint.y)
            {
                newPosition = sf::Vector2i(thisWall.firstPoint.x - collideBox.width, collideBox.top);
                distanceMove -= fabs(newPosition.x - collideBox.left);
                collideBox.left = newPosition.x;
            }
        }
        else
        {
            if(collideBox.left - distanceMove > thisWall.firstPoint.x && collideBox.left <= thisWall.firstPoint.x
               && collideBox.top <= thisWall.secondPoint.y && collideBox.top + collideBox.height > thisWall.firstPoint.y)
            {
                newPosition = sf::Vector2i(thisWall.firstPoint.x + 1, collideBox.top);
                distanceMove += fabs(newPosition.x - collideBox.left);
                collideBox.left = newPosition.x;
            }
        }
    }

    if(needSwitch == true)
    {
        std::swap(newPosition.x, newPosition.y);
    }

    return newPosition;
}

bool levelClass::stickmanUseSwitch(stickmanClass& thisStickman)
{
    for(floorStruct& thisFloor : listOfFloor)
    {
        if(thisFloor.floorArea.getGlobalBounds().intersects(thisStickman.getCollideBox()) == true)
        {
            for(switchFloorStruct& thisSwitch : thisFloor.listOfSwitch)
            {
                if(thisSwitch.switchArea.intersects(thisStickman.getCollideBox()) == true && (thisSwitch.nameOfKey == "NOTHING" || thisStickman.hasThisItem(USEABLE_ITEM, thisSwitch.nameOfKey) == true))
                {
                    sf::FloatRect secondSwitchArea = listOfFloor[thisSwitch.numberOfFloorOfSecondSwitch].listOfSwitch[thisSwitch.idOfSecondSwitch].switchArea;
                    thisStickman.setPosition(secondSwitchArea.left + (secondSwitchArea.width / 2) - (thisStickman.getCollideBox().width / 2),
                                             secondSwitchArea.top + secondSwitchArea.height - thisStickman.getCollideBox().height);
                    return true;
                }
            }
        }
    }

    return false;
}

bool levelClass::stickmanUseHideZone(stickmanClass& thisStickman)
{
    if(thisStickman.getIsHidden() == true)
    {
        thisStickman.setIsHidden(false);
        return true;
    }

    for(floorStruct& thisFloor : listOfFloor)
    {
        if(thisFloor.floorArea.getGlobalBounds().intersects(thisStickman.getCollideBox()) == true)
        {
            for(sf::FloatRect& thisHideZone : thisFloor.listOfHideZone)
            {
                if(thisStickman.getCollideBox().left >= thisHideZone.left && thisStickman.getCollideBox().left + thisStickman.getCollideBox().width <= thisHideZone.left + thisHideZone.width &&
                   thisStickman.getCollideBox().top >= thisHideZone.top && thisStickman.getCollideBox().top + thisStickman.getCollideBox().height <= thisHideZone.top + thisHideZone.height)
                {
                    thisStickman.setIsHidden(true);
                    return true;
                }
            }
        }
    }

    return false;
}

bool levelClass::stickmanUseChest(stickmanClass& thisStickman)
{
    itemClass newItem;

    for(floorStruct& thisFloor : listOfFloor)
    {
        if(thisFloor.floorArea.getGlobalBounds().intersects(thisStickman.getCollideBox()) == true)
        {
            for(chest& thisChest : thisFloor.listOfChest)
            {
                if(thisChest.chestIsUsed == false && thisChest.chestArea.intersects(thisStickman.getCollideBox()) == true)
                {
                    newItem = itemClass(thisChest.itemType, thisChest.nameOfItem, thisChest.numberOfItem);

                    thisChest.chestIsUsed = true;
                    addItemAtThisPoint(newItem, sf::Vector2f(thisChest.chestArea.left + (thisChest.chestArea.width / 2), thisChest.chestArea.top + thisChest.chestArea.height));
                    return true;
                }
            }
        }
    }

    return false;
}

int levelClass::stickmanTakeItem(stickmanClass& thisStickman, std::list<itemClass>::iterator& thisItem)
{
    for(unsigned int i = 0; i < listOfFloor.size(); ++i)
    {
        if(listOfFloor[i].floorArea.getGlobalBounds().intersects(thisStickman.getCollideBox()) == true)
        {
            for(auto itemIte = listOfFloor[i].listOfItem.begin(); itemIte != listOfFloor[i].listOfItem.end(); ++itemIte)
            {
                if(itemIte->getCollideBox().intersects(thisStickman.getCollideBox()) == true)
                {
                    thisItem = itemIte;
                    return i;
                }
            }
        }
    }

    return -1;
}

void levelClass::stickmanUseSwitchDoor(stickmanClass& thisStickman, int distanceMove, floorStruct& currentFloor)
{
    std::list<wall> tmpListOfWall;
    int isRightWall = 0;

    for(switchDoorFloorStruct& thisSwitch : currentFloor.listOfSwitchDoor)
    {
        if(thisSwitch.dirOfSwitchWall == RIGHT)
        {
            isRightWall = 0;
        }
        else
        {
            isRightWall = 1;
        }

        if(((thisSwitch.dirOfSwitchWall == LEFT && distanceMove < 0) || (thisSwitch.dirOfSwitchWall == RIGHT && distanceMove > 0)) &&
           (thisSwitch.nameOfKey == "NOTHING" || thisStickman.hasThisItem(USEABLE_ITEM, thisSwitch.nameOfKey) == true))
        {
            tmpListOfWall.clear();
            tmpListOfWall.push_back(thisSwitch.switchWall);

            if(boxCollideWithTheseWall(thisStickman.getCollideBox(), tmpListOfWall, distanceMove, true, false) != sf::Vector2i(-1, -1))
            {
                thisStickman.setPosition(listOfFloor[thisSwitch.numberOfFloorOfSecondSwitch].listOfSwitchDoor[thisSwitch.idOfSecondSwitch].switchWall.firstPoint.x
                                          - (thisStickman.getCollideBox().width * isRightWall), thisStickman.getCollideBox().top);
                return;
            }
        }
    }
}

void levelClass::addBulletToList(bulletClass thisBullet)
{
    listOfBulletNeedAdd.push_back(thisBullet);
}

void levelClass::clearListOfBullet()
{
    listOfBulletNeedAdd.clear();
}

void levelClass::addItemAtThisPoint(itemClass& newItem, sf::Vector2f point)
{
    newItem.setIsOnTheGround(true);
    newItem.setPosition(point.x - (newItem.getCollideBox().width / 2), point.y - newItem.getCollideBox().height);

    listOfFloor[getFloorForThisPoint(point)].listOfItem.push_back(std::move(newItem));
}

void levelClass::soundAlarm()
{
    alarmIsRinging = true;

    for(floorStruct& thisFloor : listOfFloor)
    {
        for(enemyStickClass& thisEnemy : thisFloor.listOfEnemy)
        {
            thisEnemy.alarmIsRinging();
        }
    }
}

std::list<itemInInventoryStruct>& levelClass::getListOfItemForThisLevel()
{
    return listOfItemForThisLevel;
}

std::vector<floorStruct>* levelClass::getListOfFloor()
{
    return &listOfFloor;
}

std::list<bulletClass>& levelClass::getListOfBulletNeedAdd()
{
    return listOfBulletNeedAdd;
}

sf::Vector2i levelClass::getLimitOfLevel()
{
    return limitOfLevel;
}

sf::Vector2i levelClass::getInitalPosition()
{
    return initialPosition;
}

bool levelClass::getAlarmIsRinging()
{
    return alarmIsRinging;
}

bool levelClass::getAllWitnessAreDead()
{
    for(floorStruct& thisFloor : listOfFloor)
    {
        if(thisFloor.listOfWitness.empty() == false)
        {
            return false;
        }
    }

    return true;
}

int levelClass::getMinimumYPosForCamera(sf::FloatRect thisCollideBox)
{
    for(floorStruct& thisFloor : listOfFloor)
    {
        if(thisFloor.floorArea.getGlobalBounds().intersects(thisCollideBox) == true)
        {
            return thisFloor.minimumYPosForCamera;
        }
    }

    return 0;
}

int levelClass::getMaximumYPosForCamera(sf::FloatRect thisCollideBox)
{
    for(floorStruct& thisFloor : listOfFloor)
    {
        if(thisFloor.floorArea.getGlobalBounds().intersects(thisCollideBox) == true)
        {
            return thisFloor.maximumYPosForCamera;
        }
    }

    return limitOfLevel.y;
}

int levelClass::getFloorForThisPoint(sf::Vector2f point)
{
    for(unsigned int i = 0; i < listOfFloor.size(); ++i)
    {
        if(listOfFloor[i].floorArea.getGlobalBounds().contains(point) == true)
        {
            return i;
        }
    }

    return -1;
}

std::string levelClass::getNextLevel()
{
    return nextLevel;
}

void levelClass::setPlayerPointer(playerStickClass* newPointer)
{
    playerStick = newPointer;
}

void levelClass::loadLevel(std::string nameOfLevel)
{
    size_t spacePos;
    std::string currentLine;
    std::string currentType;
    std::string firstWordOfLine;
    std::vector<int> listOfNumber;
    std::vector<std::string> listOfString;
    std::ifstream file;
    file.open("level/" + nameOfLevel + "/levelinfo.txt");

    backgroudTexture.loadFromFile("level/" + nameOfLevel + "/background.png");
    alarmIsRinging = false;
    nextLevel = nameOfLevel;

    listOfFloor.clear();

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
                wall newWall = wall(sf::Vector2i(listOfNumber[0], listOfNumber[1]), sf::Vector2i(listOfNumber[2], listOfNumber[3]));

                if(firstWordOfLine == "NEW_WALL")
                {
                    newWall.typeOfWall = "WALL";
                }
                else if(firstWordOfLine == "NEW_STICKMAN_WALL")
                {
                    newWall.typeOfWall = "STICKMAN_WALL";
                }

                if(listOfNumber[0] == listOfNumber[2])
                {
                    listOfFloor.back().verticalWall.push_back(newWall);
                }
                else if(listOfNumber[1] == listOfNumber[3])
                {
                    listOfFloor.back().horizontalWall.push_back(newWall);
                }
            }
            else if(firstWordOfLine == "NEW_DOOR")
            {
                listOfNumber = utilitiesClass::readNumber(currentLine, 4);

                listOfFloor.back().listOfDoor.push_back(door());
                listOfFloor.back().listOfDoor.back().sprite.setPosition(listOfNumber[0], listOfNumber[1]);
                listOfFloor.back().listOfDoor.back().sprite.setSize(sf::Vector2f(listOfNumber[2], listOfNumber[3]));
                listOfFloor.back().listOfDoor.back().listOfWall.push_back(wall(sf::Vector2i(listOfNumber[0] + 1, listOfNumber[1]),
                                                                               sf::Vector2i(listOfNumber[0] + 1, listOfNumber[1] + listOfNumber[3] - 1)));
                listOfFloor.back().listOfDoor.back().listOfWall.push_back(wall(sf::Vector2i(listOfNumber[0] + listOfNumber[2] - 2, listOfNumber[1]),
                                                                               sf::Vector2i(listOfNumber[0] + listOfNumber[2] - 2, listOfNumber[1] + listOfNumber[3] - 1)));
                listOfFloor.back().listOfDoor.back().nameOfKey = currentLine;
            }
            else if(firstWordOfLine == "NEW_SWITCH")
            {
                listOfNumber = utilitiesClass::readNumber(currentLine, 6);

                listOfFloor.back().listOfSwitch.push_back(switchFloorStruct());
                listOfFloor.back().listOfSwitch.back().switchArea.left = listOfNumber[0];
                listOfFloor.back().listOfSwitch.back().switchArea.top = listOfNumber[1];
                listOfFloor.back().listOfSwitch.back().switchArea.width = listOfNumber[2];
                listOfFloor.back().listOfSwitch.back().switchArea.height = listOfNumber[3];
                listOfFloor.back().listOfSwitch.back().numberOfFloorOfSecondSwitch = listOfNumber[4];
                listOfFloor.back().listOfSwitch.back().idOfSecondSwitch = listOfNumber[5];
                listOfFloor.back().listOfSwitch.back().nameOfKey = currentLine;
            }
            else if(firstWordOfLine == "NEW_SWITCH_DOOR")
            {
                listOfNumber = utilitiesClass::readNumber(currentLine, 7);

                listOfFloor.back().listOfSwitchDoor.push_back(switchDoorFloorStruct());
                listOfFloor.back().listOfSwitchDoor.back().switchWall = wall(sf::Vector2i(listOfNumber[0], listOfNumber[1]), sf::Vector2i(listOfNumber[2], listOfNumber[3]));
                listOfFloor.back().listOfSwitchDoor.back().numberOfFloorOfSecondSwitch = listOfNumber[4];
                listOfFloor.back().listOfSwitchDoor.back().idOfSecondSwitch = listOfNumber[5];
                listOfFloor.back().listOfSwitchDoor.back().dirOfSwitchWall = static_cast<direction>(listOfNumber[6]);
                listOfFloor.back().listOfSwitchDoor.back().nameOfKey = currentLine;
            }
            else if(firstWordOfLine == "NEW_ENEMY")
            {
                listOfNumber = utilitiesClass::readNumber(currentLine, 14);
                listOfString = utilitiesClass::readString(currentLine, 3);

                listOfFloor.back().listOfEnemy.push_back(enemyStickClass());
                listOfFloor.back().listOfEnemy.back().setPosition(listOfNumber[0], listOfNumber[1]);
                listOfFloor.back().listOfEnemy.back().setPatrolPoints(sf::Vector2f(listOfNumber[2], listOfNumber[3]), sf::Vector2f(listOfNumber[4], listOfNumber[5]));
                listOfFloor.back().listOfEnemy.back().setWaitForPatrolTime(listOfNumber[6] / 10.);
                listOfFloor.back().listOfEnemy.back().setDistanceOfViewInfo(listOfNumber[7], listOfNumber[8], listOfNumber[9]);
                listOfFloor.back().listOfEnemy.back().setFrameForCheck((listOfFloor.back().listOfEnemy.size() - 1) % 3);
                listOfFloor.back().listOfEnemy.back().setIdFloorOfEnemy(listOfFloor.size() - 1);
                listOfFloor.back().listOfEnemy.back().setReactionTime(listOfNumber[10] / 10.);
                listOfFloor.back().listOfEnemy.back().setLife(listOfNumber[11]);
                listOfFloor.back().listOfEnemy.back().setItemLoot(static_cast<typeOfItem>(listOfNumber[13]), listOfString[0], listOfNumber[12]);
                listOfFloor.back().listOfEnemy.back().setWeapon(listOfString[1]);
                listOfFloor.back().listOfEnemy.back().setIAType(listOfString[2]);
            }
            else if(firstWordOfLine == "NEW_WITNESS")
            {
                listOfNumber = utilitiesClass::readNumber(currentLine, 2);

                listOfFloor.back().listOfWitness.push_back(witnessStickClass());
                listOfFloor.back().listOfWitness.back().setPosition(listOfNumber[0], listOfNumber[1]);
            }
            else if(firstWordOfLine == "NEW_ALARM")
            {
                listOfNumber = utilitiesClass::readNumber(currentLine, 2);

                listOfFloor.back().alarm.x = listOfNumber[0];
                listOfFloor.back().alarm.y = listOfNumber[1];
            }
            else if(firstWordOfLine == "NEW_HIDE_ZONE")
            {
                listOfNumber = utilitiesClass::readNumber(currentLine, 4);

                listOfFloor.back().listOfHideZone.push_back(sf::FloatRect());
                listOfFloor.back().listOfHideZone.back().left = listOfNumber[0];
                listOfFloor.back().listOfHideZone.back().top = listOfNumber[1];
                listOfFloor.back().listOfHideZone.back().width = listOfNumber[2];
                listOfFloor.back().listOfHideZone.back().height = listOfNumber[3];
            }
            else if(firstWordOfLine == "NEW_CHEST")
            {
                listOfNumber = utilitiesClass::readNumber(currentLine, 6);

                listOfFloor.back().listOfChest.push_back(chest());
                listOfFloor.back().listOfChest.back().chestArea.left = listOfNumber[0];
                listOfFloor.back().listOfChest.back().chestArea.top = listOfNumber[1];
                listOfFloor.back().listOfChest.back().chestArea.width = listOfNumber[2];
                listOfFloor.back().listOfChest.back().chestArea.height = listOfNumber[3];
                listOfFloor.back().listOfChest.back().numberOfItem = listOfNumber[4];
                listOfFloor.back().listOfChest.back().itemType = static_cast<typeOfItem>(listOfNumber[5]);
                listOfFloor.back().listOfChest.back().nameOfItem = currentLine;
            }
            else if(firstWordOfLine == "LIMITS_OF_CAMERA")
            {
                listOfNumber = utilitiesClass::readNumber(currentLine, 2);

                listOfFloor.back().minimumYPosForCamera = listOfNumber[0];
                listOfFloor.back().maximumYPosForCamera = listOfNumber[1];
            }
            else if(firstWordOfLine == "END_OF_NEW_FLOOR")
            {
                currentType.clear();
            }
        }
        else if(currentType == "ITEM_IN_INVENTORY")
        {
            if(firstWordOfLine == "NEW_ITEM")
            {
                itemInInventoryStruct newItem;
                listOfString = utilitiesClass::readString(currentLine, 1);
                listOfNumber = utilitiesClass::readNumber(currentLine, 5);

                newItem.name = listOfString[0];
                newItem.type = static_cast<typeOfItem>(listOfNumber[0]);
                newItem.number = listOfNumber[1];
                newItem.isTurn = static_cast<bool>(listOfNumber[2]);
                newItem.posX = listOfNumber[3];
                newItem.posY = listOfNumber[4];

                listOfItemForThisLevel.push_back(newItem);
            }
            else if(firstWordOfLine == "END")
            {
                currentType.clear();
            }
        }
        else
        {
            if(firstWordOfLine == "SIZE_OF_LEVEL")
            {
                limitOfLevel.x = utilitiesClass::stringToInt(utilitiesClass::readFirstString(currentLine));
                limitOfLevel.y = utilitiesClass::stringToInt(utilitiesClass::readFirstString(currentLine));

                horizontalLevelWall.push_back(wall(sf::Vector2i(-1, -1), sf::Vector2i(limitOfLevel.x, -1)));
                horizontalLevelWall.push_back(wall(sf::Vector2i(-1, limitOfLevel.y), sf::Vector2i(limitOfLevel.x, -1)));
                verticalLevelWall.push_back(wall(sf::Vector2i(-1, -1), sf::Vector2i(-1, limitOfLevel.y)));
                verticalLevelWall.push_back(wall(sf::Vector2i(limitOfLevel.x, -1), sf::Vector2i(-1, limitOfLevel.y)));
            }
            else if(firstWordOfLine == "PLAYER_POSITION")
            {
                initialPosition.x = utilitiesClass::stringToInt(utilitiesClass::readFirstString(currentLine));
                initialPosition.y = utilitiesClass::stringToInt(utilitiesClass::readFirstString(currentLine));
            }
            else if(firstWordOfLine == "NEXT_LEVEL")
            {
                nextLevel = currentLine;
            }
            else if(firstWordOfLine == "NEW_FLOOR")
            {
                listOfNumber = utilitiesClass::readNumber(currentLine, 4);

                listOfFloor.push_back(floorStruct());
                listOfFloor.back().floorArea.setPosition(listOfNumber[0], listOfNumber[1]);
                listOfFloor.back().floorArea.setSize(sf::Vector2f(listOfNumber[2], listOfNumber[3]));
                listOfFloor.back().floorBackground.setTexture(backgroudTexture);
                listOfFloor.back().floorBackground.setTextureRect(sf::IntRect(listOfNumber[0], listOfNumber[1], listOfNumber[2], listOfNumber[3]));
                listOfFloor.back().floorBackground.setPosition(listOfNumber[0], listOfNumber[1]);
                listOfFloor.back().minimumYPosForCamera = 0;
                listOfFloor.back().maximumYPosForCamera = limitOfLevel.y;

                listOfFloor.back().listOfSwitchDoor.clear();

                currentType = "NEW_FLOOR";
            }
            else if(firstWordOfLine == "ITEM_IN_INVENTORY")
            {
                currentType = "ITEM_IN_INVENTORY";
            }
        }
    }

    file.close();
}
