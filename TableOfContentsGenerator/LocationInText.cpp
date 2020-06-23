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

/*! »нкремент позиции
 *\param [in] curString - строка, на которую указывает this->stringIndex*/
void LocationInText::incPos(const string& string)
{
    charIndex++;

    if (charIndex >= string.length())
    {
        charIndex = 0;
        stringIndex++;
    }
}

/* ќпераци€ сравнени€ местоположений в тексте
*\param [in/out] another Ц второй объект местоположени€
*\return Ц true, если первое местоположение находитс€ в тексе позже, чем второе*/
bool LocationInText::operator> (LocationInText another)
{
    if (this->stringIndex > another.stringIndex)
        return true;
    else if (this->stringIndex == another.stringIndex && this->charIndex > another.charIndex)
        return true;
    else
        return false;
}

/* ќпераци€ сравнени€ местоположений в тексте
*\param [in/out] another Ц второй объект местоположени€
*\return Ц true, если первое местоположение находитс€ в тексе раньше, чем второе*/
bool LocationInText::operator< (LocationInText another)
{
    if (this->stringIndex < another.stringIndex)
        return true;
    else if (this->stringIndex == another.stringIndex && this->charIndex < another.charIndex)
        return true;
    else
        return false;
}