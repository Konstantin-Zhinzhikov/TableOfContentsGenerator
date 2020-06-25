/// \file Заголовочный файл с описанием класса местоположения в тексте
#pragma once
#include <string>

/// Класс местоположения символа в тексте
class LocationInText
{
public:
    int charIndex;
    int stringIndex;

    LocationInText(int chIndex = -1, int strIndex = -1);
    
    void incPos(const std::string& curString);
    bool operator< (LocationInText);
    bool operator> (LocationInText);
};