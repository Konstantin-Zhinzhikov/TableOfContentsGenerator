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

bool PairOfTags::findPairOfTags(const vector <string>& code)
{
    return false;
}

void PairOfTags::readContentOfTag(const vector <string>& code)
{
}