#include "LocationInText.h"

using namespace std;

LocationInText::LocationInText(int chIndex, int strIndex)
{
    charIndex = chIndex;
    stringIndex = strIndex;
}

void LocationInText::setLocation(int chIndex, int strIndex)
{
    charIndex = chIndex;
    stringIndex = strIndex;
}

/*! ��������� �������
 *\param [in] curString - ������, �� ������� ��������� this->stringIndex*/
void LocationInText::incPos(const string& string)
{
    charIndex++;

    if (charIndex >= string.length())
    {
        charIndex = 0;
        stringIndex++;
    }
}

/* �������� ��������� �������������� � ������
*\param [in/out] another � ������ ������ ��������������
*\return � true, ���� ������ �������������� ��������� � ����� �����, ��� ������*/
bool LocationInText::operator> (LocationInText another)
{
    if (this->stringIndex > another.stringIndex)
        return true;
    else if (this->stringIndex == another.stringIndex && this->charIndex > another.charIndex)
        return true;
    else
        return false;
}

/* �������� ��������� �������������� � ������
*\param [in/out] another � ������ ������ ��������������
*\return � true, ���� ������ �������������� ��������� � ����� ������, ��� ������*/
bool LocationInText::operator< (LocationInText another)
{
    if (this->stringIndex < another.stringIndex)
        return true;
    else if (this->stringIndex == another.stringIndex && this->charIndex < another.charIndex)
        return true;
    else
        return false;
}