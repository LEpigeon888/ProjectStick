#include "gameStateStack.hpp"

gameStateStackClass::gameStateStackClass()
{
    change = false;
    previouslyChange = false;
}

void gameStateStackClass::set(gameStateClass* state)
{
    stateList.clear();
	add(state);
}

void gameStateStackClass::add(gameStateClass* state)
{
	change = true;
	previouslyChange = true;
	stateList.push_back(std::unique_ptr<gameStateClass>(state));
}

void gameStateStackClass::pop()
{
	change = true;
	previouslyChange = true;
	if(stateList.empty() == false)
	{
		stateList.pop_back();
	}
}

void gameStateStackClass::update(sf::RenderWindow& window)
{
	if(stateList.empty() == false)
	{
		stateList.back()->update(window);
	}

	if(change == false)
    {
        previouslyChange = false;
    }
}

void gameStateStackClass::draw(sf::RenderWindow& window)
{
	if(stateList.empty() == false)
	{
		stateList.back()->draw(window);
	}
}

void gameStateStackClass::oldUpdate(sf::RenderWindow& window)
{
    if(stateList.size() >= 2)
    {
        auto it = stateList.rbegin();
        ++it;
        (*it)->update(window);
    }
}

void gameStateStackClass::oldDraw(sf::RenderWindow& window)
{
    if(stateList.size() >= 2)
    {
        auto it = stateList.rbegin();
        ++it;
        (*it)->draw(window);
    }
}

bool gameStateStackClass::getChange()
{
	return change;
}

bool gameStateStackClass::getPreviouslyChange()
{
	return previouslyChange;
}

void gameStateStackClass::setChange(bool newChange)
{
	change = newChange;
}
