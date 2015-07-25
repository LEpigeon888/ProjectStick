#ifndef GLOBAL_HPP
#define GLOBAL_HPP

#define WIDTH_SCREEN 1280
#define HEIGHT_SCREEN 720
#define PI 3.14159265
#define NUMBER_OF_KEY 10
#define TIME_FOR_KEY_PRESS 1

#include <SFML/Graphics.hpp>

#include "gameStateStack.hpp"
#include "level.hpp"

/*
Structure: global
Utilité: C'est une structure dont tout les membre sont static, elle sert a sticker toutes les variables globales utilisé pour le jeu.
*/

struct global
{
    static gameStateStackClass* activeGameStateStack;
    static levelClass* activeLevel;
    static sf::Font font;
    static bool easterEggEnable;
    static bool easterLaunchEgg;
};

#endif
