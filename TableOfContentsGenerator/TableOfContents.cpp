#include "TableOfContents.h"

using namespace std;

TableOfContents::TableOfContents()
{

}

/*! Находит все теги заголовков (h1-h6) в коде
 *\param [in] code - html-код
 *\return - Параметры функции в массиве */
void TableOfContents::findAllHeaders(vector  <string>& code)
{
    // Для каждого типа заголовков (h1-h6)
    for (int i = 1; i <= 6; i++)
    {
        PairOfTags currentHeader("h");  currentHeader.name.append(to_string(i));

        LocationInText pos(0, 0);

        bool found = true;
        while (found)
        {
            found = currentHeader.findPairOfTags(code, pos);

            if (found)
            {
                this->headers.push_back(currentHeader);
                this->levelsOfHeaders.push_back(i);

                pos = currentHeader.closingTagLocation;
                pos.incPos(code[pos.stringIndex]);
            }
        }
    }
    this->sortHeaders();
}

void TableOfContents::sortHeaders()
{
    PairOfTags tmp_header; int tmp_headerLvl;
    for (int i = 0; i < headers.size(); i++)
    {
        for (int j = headers.size() - 1; j >= i + 1; j--)
        {
            if (headers[j].openingTagLocation < headers[j - 1].openingTagLocation)
            {
                tmp_header = headers[j];
                headers[j] = headers[j - 1];
                headers[j - 1] = tmp_header;

                tmp_headerLvl = levelsOfHeaders[j];
                levelsOfHeaders[j] = levelsOfHeaders[j - 1];
                levelsOfHeaders[j - 1] = tmp_headerLvl;
            }
        }
    }
}

void TableOfContents::generateTableOfContents(const vector <string>& code)
{

}

void TableOfContents::readContentsOfHeaders(const vector <string>& code)
{

}

void TableOfContents::writeTableOfContentsInFile(const string& path)
{

}
