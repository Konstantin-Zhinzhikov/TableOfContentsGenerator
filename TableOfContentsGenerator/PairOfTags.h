/// \file ������������ ���� � ��������� ������ ���� �����

#pragma once
#include "LocationInText.h"
#include <string>
#include <vector>
#include <iostream>
#include <regex>

/// ����� ���� �����
class PairOfTags
{
public:
    LocationInText openingTagLocation;  ///< �������������� ������������ ����
    LocationInText closingTagLocation;  ///< �������������� ������������ ����
    std::string name;                   ///< �������� ����
    std::string content;                ///< ���������� ����

    PairOfTags(std::string name = "", LocationInText openingTagLocation = LocationInText(), LocationInText closingTagLocation = LocationInText());
    PairOfTags(std::string name, int openCharIndex, int openStrIndex, int closCharIndex, int closStrIndex);

    bool findPairOfTags(const std::vector <std::string>& code, LocationInText& pos);
    void findTagsLocationsInString(const std::string& str, int pos = 0);
    void readContentOfTag(const std::vector <std::string>& code);
};
