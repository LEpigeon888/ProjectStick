#ifndef UTILITIES_HPP
#define UTILITIES_HPP

#include <string>
#include <vector>

/*
Classe: utilitiesClass
Parents: Aucun.
Abstraite: Non.
Utilité: C'est une classe contenant différentes fonction static qui sont d'ordre pratique (exemple, convertir un int en string, faire l'inverse,
etc).
*/

class utilitiesClass
{
public:
    static std::string readFirstString(std::string& thisString);
    static std::vector<int> readNumber(std::string& thisString, int numberOfNumber);
    static std::vector<std::string> readString(std::string& thisString, int numberOfString);
    static std::string intToString(int thisInt);
    static int stringToInt(std::string thisString);
};

#endif
