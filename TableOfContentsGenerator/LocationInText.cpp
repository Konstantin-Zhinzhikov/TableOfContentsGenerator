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

bool LocationInText::operator> (LocationInText another)
{
    if (this->stringIndex > another.stringIndex)
        return true;
    else if (this->stringIndex == another.stringIndex && this->charIndex > another.charIndex)
        return true;
    else
        return false;
}

bool LocationInText::operator< (LocationInText another)
{
    if (this->stringIndex < another.stringIndex)
        return true;
    else if (this->stringIndex == another.stringIndex && this->charIndex < another.charIndex)
        return true;
    else
        return false;
}