#include "TableOfContents.h"

using namespace std;

TableOfContents::TableOfContents()
{

}

/*! Находит все теги заголовков (h1-h6) в коде
 *\param [in] code - html-код
 *\return - Параметры функции в массиве */
void TableOfContents::findAllHeaders(vector  <string> code)
{
    // Заменить все комментарии в коде символами 'c'
    clearCodeFromComments(code);

    // Перевести все символы в коде в нижний регистр
    for (int i = 0; i < code.size(); i++)
        for (int j = 0; j < code[i].size(); j++)
            code[i][j] = tolower(code[i][j]);

    // Для каждого типа заголовков (h1-h6)
    for (int i = 1; i <= 6; i++)
    {
        LocationInText pos(0, 0); // Позиция поиска

        bool allCurrentHeadersFound = false;
        while (!allCurrentHeadersFound)
        {
            PairOfTags currentHeader("h");  currentHeader.name.append(to_string(i));

            if (currentHeader.findPairOfTags(code, pos))
            {
                // Сохранить заголовок
                this->headers.push_back(currentHeader);
                this->levelsOfHeaders.push_back(i);

                // Начать следующий поиск после закрытого тега этого заголовка
                //pos = currentHeader.closingTagLocation;
                pos.charIndex = currentHeader.closingTagLocation.charIndex + 4;
                pos.stringIndex = currentHeader.closingTagLocation.stringIndex;
                pos.incPos(code[pos.stringIndex]);
            }
            else
            {
                allCurrentHeadersFound = true;
            }
        }
    }
    sortHeaders();
}

/* Заменяет в коде все комментарии символами 'c'
*\param [in/out] code - html-код страницы*/
void TableOfContents::clearCodeFromComments(std::vector <std::string>& code)
{
    bool commentIsOpenedBefore = false;

    for (int i = 0; i < code.size(); i++)
    {
        bool commentOpensInCurrString = commentIsOpenedBefore;
        int openIndx = -1, closIndx = -1; // Считать, что комментарий не открывается в этой строке        

        // ---- Поиск комментариев ----
        // Если комментарий не был открыт ранее
        if (!commentIsOpenedBefore)
        {
            openIndx = code[i].find("<!--"); // Искать открытие комментария в строке
            // Если найден
            if (openIndx != -1)
            {
                commentOpensInCurrString = true; //Cчитать, что он открыт в этой строке
                closIndx = code[i].find("-->", openIndx + 3); // Искать конец комментария после открытия
                // Если найден
                if (closIndx != -1)
                    commentOpensInCurrString = false; // Считать, что комментарий закрывается в этой строке
            }
        }
        // Иначе если комментарий был открыт ранее
        else if (commentIsOpenedBefore)
        {
            closIndx = code[i].find("-->"); // Искать конец комментария с начала строки
            // Если найден
            if (closIndx != -1)
                commentOpensInCurrString = false; // Считать, что комментарий закрывается в этой строке
        }

        // ---- Удаление ----
        // Если комментарий был открыт в этой строке
        if (openIndx != -1 && !commentIsOpenedBefore)
        {
            // Если комментарий НЕ был закрыт в этой строке
            if (closIndx == -1)
            {
                // Удалить все после открытия комментария
                for (int j = openIndx; j < code[i].size(); j++)
                    code[i].replace(j, 1, "c");
            }
            // Если комментарий БЫЛ закрыт в этой строке
            else if (closIndx != -1)
            {
                // Удалить все после открытия комментария и до закрытия
                for (int j = openIndx; j < closIndx + 3; j++)
                    code[i].replace(j, 1, "c");
                i--; // Не переходить не следующую строку, т.к. в этой строке может быть ещё один комментарий
            }
        }

        // Если комментарий был открыт ранее
        else if (openIndx == -1 && commentIsOpenedBefore)
        {
            // Если комментарий НЕ был закрыт в этой строке
            if (closIndx == -1)
            {
                // Удалить строку
                for (int j = 0; j < code[i].size(); j++)
                    code[i].replace(j, 1, "c");
            }
            // Если комментарий БЫЛ закрыт в этой строке
            else if (closIndx != -1)
            {
                // Удалить все до закрытия комментария
                for (int j = 0; j < closIndx + 3; j++)
                    code[i].replace(j, 1, "c");

                i--; // Не переходить не следующую строку, т.к. в этой строке может быть ещё один комментарий
            }
        }
        commentIsOpenedBefore = commentOpensInCurrString; // считать что комментарий открыт, если в этой строке он не был закрыт или был открыт 
    }
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

/*! Генерирует оглавление из текста и сохраняет его в переменной объекта
 *\param [in] headers - отсортированный спискок заголовков
 *\return - оглавление */
void TableOfContents::generateTableOfContents(const vector <string>& code)
{
    findAllHeaders(code);
    readContentsOfHeaders(code);

     // Записать оглавление в формате html-кода
     // Если тегов > 0
    if (headers.size() > 0)
    {
        // Увеличить уровень вложенности до значения, равного уровню первого тега
        int currentNestingLvl = 0;
        while (currentNestingLvl < levelsOfHeaders[0])
        {
            string s = createStringWithNestingLvl(currentNestingLvl);
            s.append("<ul>");
            codeOfTableOfContents.push_back(s);
            currentNestingLvl++;
        }

        // Для всех заголовков
        for (int i = 0; i < headers.size() /*|| currentNesting != 0*/; i++)
        {
            // Создать строку, присоединить к ней знаки табуляции
            string s = createStringWithNestingLvl(currentNestingLvl);

            // Присоединить к ней текст заголовка
            s.append("<li>");
            s.append(headers[i].content);
            s.append("</li>");

            // Закинуть её в вектор
            this->codeOfTableOfContents.push_back(s);

            // Если этот заголовок не был последним
            if (i != headers.size() - 1)
            {
                // Если уровень следующего заголовка больше уровня текущего
                if (levelsOfHeaders[i + 1] > levelsOfHeaders[i])
                {
                    // Увеличить вложенность таблицы
                    for (int j = levelsOfHeaders[i + 1] - levelsOfHeaders[i]; j > 0; j--)
                    {
                        string s = createStringWithNestingLvl(currentNestingLvl);
                        s.append("<ul>");
                        codeOfTableOfContents.push_back(s);
                        currentNestingLvl++;
                    }
                }

                // Если следующий заголовок меньше текущего
                else  if (levelsOfHeaders[i + 1] < levelsOfHeaders[i])
                {
                    // Уменьшить вложенность таблицы
                    for (int j = levelsOfHeaders[i] - levelsOfHeaders[i + 1]; j > 0; j--)
                    {
                        currentNestingLvl--;
                        string s = createStringWithNestingLvl(currentNestingLvl);
                        s.append("</ul>");
                        codeOfTableOfContents.push_back(s);
                    }
                }
            }
        }

        do
        {
            currentNestingLvl--;
            string s = createStringWithNestingLvl(currentNestingLvl);
            s.append("</ul>");
            codeOfTableOfContents.push_back(s);
        } while (currentNestingLvl > 0);
    }
}

string TableOfContents::createStringWithNestingLvl(int nestingLevel)
{
    string s;
    for (int i = 0; i < nestingLevel; i++)
        s.append("\t");
    return s;
}

/*! Считывает в заголовки их содержание
 *\param [in] code - html-код*/
void TableOfContents::readContentsOfHeaders(const vector <string>& code)
{
    for (int i = 0; i < this->headers.size(); i++)
    {
        headers[i].readContentOfTag(code);
    }
}

void TableOfContents::writeTableOfContentsInFile(const string& path)
{

}
