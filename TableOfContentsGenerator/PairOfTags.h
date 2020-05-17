#pragma once
#include "LocationInText.h"
#include <string>
#include <vector>
#include <iostream>
#include <regex>

class PairOfTags
{
public:
    LocationInText openingTagLocation, closingTagLocation; // �������������� ����
    std::string name;                   // �������� ����
    std::string content;                // ���������� ����

    PairOfTags(std::string name = "", LocationInText openingTagLocation = LocationInText(),
        LocationInText closingTagLocation = LocationInText());

    bool findPairOfTags(const std::vector <std::string>& code, LocationInText startOfSearch = LocationInText (0,0));
    void findTagsLocationsInString(const std::string& str, int pos = 0);
    void readContentOfTag(const std::vector <std::string>& code);
};
