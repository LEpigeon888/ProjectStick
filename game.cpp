#include <cstdlib>
#include <ctime>

#include "game.hpp"
#include "mainMenuState.hpp"
#include "soundManager.hpp"
#include "global.hpp"

gameClass::gameClass()
{
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    window.create(sf::VideoMode(WIDTH_SCREEN, HEIGHT_SCREEN), "Project: Stick", sf::Style::Titlebar | sf::Style::Close, settings);
	window.setVerticalSyncEnabled(true);
	window.setKeyRepeatEnabled(false);

    global::activeGameStateStack = &gameStateStack;
    global::font.loadFromFile("resource/font/Munro.ttf");
    global::easterEggEnable = false;

    soundManagerClass::initialize();
    soundManagerClass::addSound("piou.ogg", 50);

    weaponClass::initializeWeapon();
    bulletClass::initializeBullet();
    itemClass::initializeItem();
    std::srand(std::time(0));

    if((std::rand() % 101) == 88)
    {
        global::easterLaunchEgg = true;
    }
    else
    {
        global::easterLaunchEgg = false;
    }

	gameStateStack.set(new mainMenuStateClass());
}

void gameClass::run()
{
	while(window.isOpen())
    {
        do
        {
            gameStateStack.setChange(false);
            gameStateStack.update(window);
        }
        while(gameStateStack.getChange() == true);

        gameStateStack.draw(window);
        window.display();
    }

    soundManagerClass::clearMemory();
}
