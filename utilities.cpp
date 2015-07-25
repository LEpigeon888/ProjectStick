#include <sstream>

#include "utilities.hpp"

std::string utilitiesClass::readFirstString(std::string& thisString)
{
    size_t spacePos = thisString.find(' ');
    std::string tmpStr = thisString.substr(0, spacePos);

    if(spacePos == std::string::npos)
    {
        thisString.erase(0);
    }
    else
    {
        thisString.erase(0, spacePos + 1);
    }

    return tmpStr;
}

std::vector<int> utilitiesClass::readNumber(std::string& thisString, int numberOfNumber)
{
    std::vector<int> listOfNumber;

    for(int i = 0; i < numberOfNumber; ++i)
    {
        listOfNumber.push_back(stringToInt(readFirstString(thisString)));
    }

    return listOfNumber;
}

std::vector<std::string> utilitiesClass::readString(std::string& thisString, int numberOfString)
{
    std::vector<std::string> listOfString;

    for(int i = 0; i < numberOfString; ++i)
    {
        listOfString.push_back(readFirstString(thisString));
    }

    return listOfString;
}

std::string utilitiesClass::intToString(int thisInt)
{
    std::stringstream ss;

    ss << thisInt;

    return ss.str();
}

int utilitiesClass::stringToInt(std::string thisString)
{
    std::istringstream iss(thisString);
    int tmpInt;

    iss >> tmpInt;

    return tmpInt;
}
