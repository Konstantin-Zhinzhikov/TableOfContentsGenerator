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

    PairOfTags();

    bool findPairOfTags(const std::vector <std::string>& code);
    void findTagsLocationsInString(const std::string& str, int pos = 0);
    void readContentOfTag(const std::vector <std::string>& code);
};
