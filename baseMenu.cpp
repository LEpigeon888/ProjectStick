#include "baseMenu.hpp"
#include "global.hpp"

void baseMenuClass::draw(sf::RenderWindow& window)
{
    for(buttonStruct& thisButton : listOfButton)
    {
        window.draw(thisButton.buttonText);
    }
}

void baseMenuClass::addButton(int x, int y, std::string text)
{
    buttonStruct newButtonStruc;

    newButtonStruc.buttonText.setFont(global::font);
    newButtonStruc.buttonText.setColor(sf::Color::Black);
    newButtonStruc.buttonText.setCharacterSize(40);
    newButtonStruc.buttonText.setString(text);
    newButtonStruc.buttonText.setOrigin(0, static_cast<int>(newButtonStruc.buttonText.getLocalBounds().top));
    newButtonStruc.buttonText.setPosition(x, y);

    newButtonStruc.isAClickableButton = false;

    listOfButton.push_back(std::move(newButtonStruc));
}

void baseMenuClass::setLastButttonClickable(std::function<void(void)> newSlot)
{
    if(listOfButton.size() > 0)
    {
        listOfButton.back().slot = newSlot;
        listOfButton.back().isAClickableButton = true;
    }
}

void baseMenuClass::centerXLastButton()
{
    if(listOfButton.size() > 0)
    {
        listOfButton.back().buttonText.setPosition((WIDTH_SCREEN / 2) - (listOfButton.back().buttonText.getGlobalBounds().width / 2), listOfButton.back().buttonText.getPosition().y);
    }
}

void baseMenuClass::centerYAllButton(int spaceBetweenButton)
{
    int ySizeOfAllButton = 0;
    int lastYPosition = 0;

    if(listOfButton.size() > 0)
    {
        ySizeOfAllButton = (listOfButton.size() - 1) * spaceBetweenButton;

        for(buttonStruct& thisButton : listOfButton)
        {
            ySizeOfAllButton += thisButton.buttonText.getGlobalBounds().height;
        }

        lastYPosition = (HEIGHT_SCREEN / 2) - (ySizeOfAllButton / 2);

        for(buttonStruct& thisButton : listOfButton)
        {
            thisButton.buttonText.setPosition(thisButton.buttonText.getPosition().x, lastYPosition);

            lastYPosition += thisButton.buttonText.getGlobalBounds().height + spaceBetweenButton;
        }
    }
}

void baseMenuClass::reversePositionOfLastButton(bool inXAxe, bool inYAxe)
{
    if(listOfButton.size() > 0)
    {
        if(inXAxe == true)
        {
            listOfButton.back().buttonText.setPosition(WIDTH_SCREEN - listOfButton.back().buttonText.getGlobalBounds().width - listOfButton.back().buttonText.getPosition().x,
                                                        listOfButton.back().buttonText.getPosition().y);
        }

        if(inYAxe == true)
        {
            listOfButton.back().buttonText.setPosition(listOfButton.back().buttonText.getPosition().x,
                                                        HEIGHT_SCREEN - listOfButton.back().buttonText.getGlobalBounds().height - listOfButton.back().buttonText.getPosition().y);
        }
    }
}

void baseMenuClass::removeLastButton()
{
    if(listOfButton.size() > 0)
    {
        listOfButton.pop_back();
    }
}

void baseMenuClass::playerClickHere(int x, int y)
{
    for(buttonStruct& thisButton : listOfButton)
    {
        if(thisButton.buttonText.getGlobalBounds().contains(x, y) == true && thisButton.isAClickableButton == true)
        {
            thisButton.slot();
            return;
        }
    }
}

void baseMenuClass::playerMoveMouseHere(int x, int y)
{
    for(buttonStruct& thisButton : listOfButton)
    {
        if(thisButton.isAClickableButton == true)
        {
            if(thisButton.buttonText.getGlobalBounds().contains(x, y) == true)
            {
                thisButton.buttonText.setColor(sf::Color(128, 128, 128));
            }
            else
            {
                thisButton.buttonText.setColor(sf::Color::Black);
            }
        }
    }
}
