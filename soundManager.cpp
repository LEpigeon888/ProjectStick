#include <memory>

#include "soundManager.hpp"

std::unique_ptr<std::map<std::string, sf::SoundBuffer>> soundManagerClass::soundBufferMap;
std::unique_ptr<std::map<std::string, std::vector<sf::Sound>>> soundManagerClass::soundVectorMap;

void soundManagerClass::initialize()
{
    soundBufferMap.reset(new std::map<std::string, sf::SoundBuffer>());
    soundVectorMap.reset(new std::map<std::string, std::vector<sf::Sound>>());
}

void soundManagerClass::addSound(std::string nameOfSound, int numberMaxOfSound)
{
    if(soundBufferMap->find(nameOfSound) == soundBufferMap->end())
    {
        std::map<std::string, sf::SoundBuffer>::iterator soundBufferIte;
        std::vector<sf::Sound> newSoundVector;
        sf::Sound newSound;
        (*soundBufferMap)[nameOfSound] = sf::SoundBuffer();
        soundBufferIte = soundBufferMap->find(nameOfSound);
        soundBufferIte->second.loadFromFile("resource/sound/" + nameOfSound);

        for(int i = 0; i < numberMaxOfSound; ++i)
        {
            newSound = sf::Sound();
            newSound.setBuffer(soundBufferIte->second);

            newSoundVector.push_back(std::move(newSound));
        }

        (*soundVectorMap)[nameOfSound] = std::move(newSoundVector);
    }
}

void soundManagerClass::playThisSound(std::string nameOfSound)
{
    std::map<std::string, std::vector<sf::Sound>>::iterator soundVectorMapIte = soundVectorMap->find(nameOfSound);

    if(soundVectorMapIte != soundVectorMap->end())
    {
        float maxPlayOffset = 0;
        int indexOfMaxPlayOffset = -1;
        int currentIndexOfPlayOffset = 0;

        for(sf::Sound& thisSound : soundVectorMapIte->second)
        {
            if(thisSound.getStatus() != sf::Sound::Playing)
            {
                thisSound.play();
                return;
            }
            else
            {
                if(thisSound.getPlayingOffset().asSeconds() > maxPlayOffset)
                {
                    maxPlayOffset = thisSound.getPlayingOffset().asSeconds();
                    indexOfMaxPlayOffset = currentIndexOfPlayOffset;
                }
            }

            ++currentIndexOfPlayOffset;
        }

        if(indexOfMaxPlayOffset != -1)
        {
            soundVectorMapIte->second[indexOfMaxPlayOffset].play();
            return;
        }
    }
}

void soundManagerClass::clearMemory()
{
    soundVectorMap.reset();
    soundBufferMap.reset();
}
