#ifndef SOUNDMANAGER_HPP
#define SOUNDMANAGER_HPP

#include <SFML/Audio.hpp>
#include <memory>
#include <map>

/*
Classe: soundManagerClass
Parents: Aucun.
Abstraite: Non.
Utilité: C'est une classe qui permet de charger ainsi que de jouer des sons.
*/

class soundManagerClass
{
public:
    static void initialize();
    static void clearMemory();
    static void addSound(std::string nameOfSound, int numberMaxOfSound);
    static void playThisSound(std::string nameOfSound);
private:
    static std::unique_ptr<std::map<std::string, sf::SoundBuffer>> soundBufferMap;
    static std::unique_ptr<std::map<std::string, std::vector<sf::Sound>>> soundVectorMap;
};

#endif
