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

/*! Определяет искомый тег в строке
 *\param [in] pairOfTags - объект пары тегов (необходимо как минимум инициализированное имя)
 *\param [in] string - строка кода*/
void PairOfTags::findTagsLocationsInString(const string& str, int pos)
{
    string s = str.substr(pos); // Подстрока, в которой производится поиск

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

/*! Находит первое вхождение тега в коде
 *\param [in] code - html-код*/
bool PairOfTags::findPairOfTags(const vector <string>& code, LocationInText startOfSearch)
{
    // bool found = false;
    bool foundOpening = false, foundClosing = false;

    PairOfTags tmp(this->name);

    int charPos = startOfSearch.charIndex;

    // Для каждой строки кода И пока не найдено
    for (int i = startOfSearch.stringIndex; i < (int)code.size() && !(foundOpening && foundClosing); i++)
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

/*! Читает из кода содержание заголовка
 *\param [in] code - html-код*/
void PairOfTags::readContentOfTag(const vector <string>& code)
{
    // Индекс начала контента
    LocationInText beginOfContent(code[openingTagLocation.stringIndex].find(">", openingTagLocation.charIndex), openingTagLocation.stringIndex);
    //LocationInText beginOfContent(openingTagLocation.charIndex + name.size() + 1, openingTagLocation.stringIndex);
    beginOfContent.incPos(code[beginOfContent.stringIndex]);

    // Индекс конца контента
    LocationInText endOfContent = closingTagLocation;
    endOfContent.charIndex--;

    // Перенос индекса на конец предыдущей строки, если закрывающий тег в начале строки
    if (this->closingTagLocation.charIndex < 0)
    {
        endOfContent.stringIndex--;
        endOfContent.charIndex = code[endOfContent.stringIndex].length() - 1;
        //int prevStr = endOfContent.stringIndex - 1;
        //endOfContent.setLocation(code[prevStr].length() - 1, prevStr);
    }

    // Если контент содержится на одной строке
    if (beginOfContent.stringIndex == endOfContent.stringIndex)
    {
        // Присоединить к содержанию тега подстроку от начала контента до его конца
        content.append(code[beginOfContent.stringIndex], beginOfContent.charIndex, endOfContent.charIndex - beginOfContent.charIndex + 1);
    }

    else
    {
        // Скопировать все от начала контента до конца строки
        content.append(code[beginOfContent.stringIndex], beginOfContent.charIndex, code[beginOfContent.stringIndex].length());

        // Для всех строк от начала+1 до конца контента
        for (int i = beginOfContent.stringIndex + 1; i < endOfContent.stringIndex; i++)
        {
            //Скопировать строку целиком
            content.append(code[i]);
        }

        // Скопировать последнюю строку от начала строки до конца контента
        content.append(code[endOfContent.stringIndex], 0, endOfContent.charIndex + 1);
    }

    // Удалить все знаки табуляции из строки
    int tabPos = content.find("\t");
    while (tabPos != -1)
    {
        content.replace(tabPos, 1, "");
        tabPos = content.find("\t");
    }

    // Проверка на пустой заголовок 
    if (this->content.length() == 0)
        throw string("Пустой заголовок");
}