/// \file ������������ ���� � ��������� ������ �������������� � ������
#pragma once
#include <string>

/// ����� �������������� ������� � ������
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