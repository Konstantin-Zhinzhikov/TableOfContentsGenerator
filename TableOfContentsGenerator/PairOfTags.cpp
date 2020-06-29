#include "PairOfTags.h"

using namespace std;

PairOfTags::PairOfTags(string name, LocationInText openingTagLocation, LocationInText closingTagLocation)
{
    this->name = name;
    this->openingTagLocation = openingTagLocation;
    this->closingTagLocation = closingTagLocation;
}

PairOfTags::PairOfTags(string name, int openCharIndex, int openStrIndex, int closCharIndex, int closStrIndex)
{
    this->name = name;
    this->openingTagLocation = LocationInText(openCharIndex, openStrIndex);
    this->closingTagLocation = LocationInText(closCharIndex, closStrIndex);
}

/*! Ќаходит положени€ открытого и/или закрытого тега в строке
*\param [in] str - строка, в которой будет производитьс€ поиск
*\param [in] pos - позици€ начала поиска*/
void PairOfTags::findTagsLocationsInString(const string& str, int pos)
{
    string s = str.substr(pos); // ѕодстрока, в которой производитс€ поиск

    smatch m;

    string tmp1;
    tmp1.append("<");    tmp1.append(name);    tmp1.append(".*>");
    regex regexpOfOpeningTag{ tmp1 };

    if (regex_search(s, m, regexpOfOpeningTag))
        openingTagLocation.charIndex = m.position();

    string tmp2;
    tmp2.append("<\/");    tmp2.append(name);    tmp2.append(".*>");
    regex regexpOfClosingTag{ tmp2 };

    if (regex_search(s, m, regexpOfClosingTag))
        closingTagLocation.charIndex = m.position();
}

/*! Ќаходит первое вхождение тега в коде
 *\param [in] code - html-код
 *\param [in] pos - позици€ начала поиска
 *\return - 0, если тег не найден*/
bool PairOfTags::findPairOfTags(const vector <string>& code, LocationInText pos)
{
    bool foundOpening = false, foundClosing = false;

    // ѕока позици€ поиска не указывает на конец » пока не найдены открытый и закрытый теги
    while (pos.stringIndex < (int)code.size() && !(foundOpening && foundClosing))
    {
        PairOfTags tmp(name);

        // »скать тег в строке
        tmp.findTagsLocationsInString(code[pos.stringIndex], pos.charIndex);
        tmp.openingTagLocation.stringIndex = pos.stringIndex;
        tmp.closingTagLocation.stringIndex = pos.stringIndex;

        // ≈сли тег уже открывалс€, но до его закрыти€ найдено еще одно открытие
        if (foundOpening && tmp.openingTagLocation.charIndex != -1 &&
            (tmp.closingTagLocation > tmp.openingTagLocation || tmp.closingTagLocation.charIndex == -1))
            throw string("Ќесоответствие количества открывающих и закрывающих тегов");

        // ≈сли найден открытый и открытый тег еще не был найден
        if (tmp.openingTagLocation.charIndex != -1 && !foundOpening)
        {
            this->openingTagLocation = tmp.openingTagLocation;
            foundOpening = true;
        }

        // ≈сли найден закрытый
        if (tmp.closingTagLocation.charIndex != -1 && !foundClosing)
        {
            // ≈сли открытый еще не был найден
            if (!foundOpening)
                throw string("Ќесоответствие количества открывающих и закрывающих тегов");

            this->closingTagLocation = tmp.closingTagLocation;
            foundClosing = true;
        }

        // ѕерейти на след. строку
        pos.charIndex = 0;
        pos.stringIndex++;
    }

    if (foundOpening != foundClosing)
        throw string("Ќесоответствие количества открывающих и закрывающих тегов");

    return foundOpening && foundClosing;
}

/*! „итает из кода содержание заголовка
 *\param [in] code - html-код*/
void PairOfTags::readContentOfTag(const vector <string>& code)
{
    // »ндекс начала контента
    LocationInText beginOfContent(code[openingTagLocation.stringIndex].find(">", openingTagLocation.charIndex), openingTagLocation.stringIndex);
    //LocationInText beginOfContent(openingTagLocation.charIndex + name.size() + 1, openingTagLocation.stringIndex);
    beginOfContent.incPos(code[beginOfContent.stringIndex]);

    // »ндекс конца контента
    LocationInText endOfContent = closingTagLocation;
    endOfContent.charIndex--;

    // ѕеренос индекса на конец предыдущей строки, если закрывающий тег в начале строки
    if (this->closingTagLocation.charIndex < 0)
    {
        endOfContent.stringIndex--;
        endOfContent.charIndex = code[endOfContent.stringIndex].length() - 1;
        //int prevStr = endOfContent.stringIndex - 1;
        //endOfContent.setLocation(code[prevStr].length() - 1, prevStr);
    }

    // ≈сли контент содержитс€ на одной строке
    if (beginOfContent.stringIndex == endOfContent.stringIndex)
    {
        // ѕрисоединить к содержанию тега подстроку от начала контента до его конца
        content.append(code[beginOfContent.stringIndex], beginOfContent.charIndex, endOfContent.charIndex - beginOfContent.charIndex + 1);
    }

    else
    {
        // —копировать все от начала контента до конца строки
        content.append(code[beginOfContent.stringIndex], beginOfContent.charIndex, code[beginOfContent.stringIndex].length());

        // ƒл€ всех строк от начала+1 до конца контента
        for (int i = beginOfContent.stringIndex + 1; i < endOfContent.stringIndex; i++)
        {
            //—копировать строку целиком
            content.append(code[i]);
        }

        // —копировать последнюю строку от начала строки до конца контента
        content.append(code[endOfContent.stringIndex], 0, endOfContent.charIndex + 1);
    }

    // ”далить все знаки табул€ции из строки
    int tabPos = content.find("\t");
    while (tabPos != -1)
    {
        content.replace(tabPos, 1, "");
        tabPos = content.find("\t");
    }

    // ѕроверка на пустой заголовок 
    if (this->content.length() == 0)
        throw string("ѕустой заголовок");
}