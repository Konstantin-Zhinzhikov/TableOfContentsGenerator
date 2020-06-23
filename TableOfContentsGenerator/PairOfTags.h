/// \file Заголовочный файл с описанием класса пары тегов

#pragma once
#include "LocationInText.h"
#include <string>
#include <vector>
#include <iostream>
#include <regex>

/// Класс пары тегов
class PairOfTags
{
public:
    LocationInText openingTagLocation;  ///< Местоположение открывающего тега
    LocationInText closingTagLocation;  ///< Местоположение закрывающего тега
    std::string name;                   ///< Название тега
    std::string content;                ///< Содержимое тега

    PairOfTags(std::string name = "", LocationInText openingTagLocation = LocationInText(), LocationInText closingTagLocation = LocationInText());
    PairOfTags(std::string name, int openCharIndex, int openStrIndex, int closCharIndex, int closStrIndex);

    bool findPairOfTags(const std::vector <std::string>& code, LocationInText& pos);
    void findTagsLocationsInString(const std::string& str, int pos = 0);
    void readContentOfTag(const std::vector <std::string>& code);
};
