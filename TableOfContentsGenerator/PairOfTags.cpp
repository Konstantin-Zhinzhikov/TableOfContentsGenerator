#include "PairOfTags.h"

using namespace std;

PairOfTags::PairOfTags(string name, LocationInText openingTagLocation, LocationInText closingTagLocation)
{
    this->name = name;
    this->openingTagLocation = openingTagLocation;
    this->closingTagLocation = closingTagLocation;
}

void PairOfTags::findTagsLocationsInString(const string& str, int pos)
{
   
}

/*! ������� ������ ��������� ���� � ����
 *\param [in] code - html-���*/
bool PairOfTags::findPairOfTags(const vector <string>& code, LocationInText startOfSearch)
{
    // bool found = false;
    bool foundOpening = false, foundClosing = false;

    PairOfTags tmp(this->name);

    int charPos = startOfSearch.charIndex;

    // ��� ������ ������ ���� ��� ���� �� �������
    for (int i = startOfSearch.stringIndex; i < (int)code.size() || !(foundOpening && foundClosing); i++)
    {
        tmp.findTagsLocationsInString(code[i], charPos);

        if (tmp.openingTagLocation.charIndex != -1 && !foundOpening)
        {
            this->openingTagLocation.charIndex = tmp.openingTagLocation.charIndex;
            this->openingTagLocation.stringIndex = i;
            foundOpening = true;
        }

        if (tmp.closingTagLocation.charIndex != -1 && !foundClosing)
        {
            this->closingTagLocation.charIndex = tmp.closingTagLocation.charIndex;
            this->closingTagLocation.stringIndex = i;
            foundClosing = true;
        }

        charPos = 0;
        //found = foundOpening && foundClosing;

    }
    return foundOpening && foundClosing;
}

void PairOfTags::readContentOfTag(const vector <string>& code)
{
}