#include "TableOfContents.h"

using namespace std;

TableOfContents::TableOfContents()
{

}

/*! ������� ��� ���� ���������� (h1-h6) � ����
 *\param [in] code - html-���
 *\return - ��������� ������� � ������� */
void TableOfContents::findAllHeaders(vector  <string> code)
{
    // ��������� ��� ������� � ���� � ������ �������
    for (int i = 0; i < code.size(); i++)
        for (int j = 0; j < code[i].size(); j++)
            code[i][j] = tolower(code[i][j]);

    // ��� ������� ���� ���������� (h1-h6)
    for (int i = 1; i <= 6; i++)
    {
        LocationInText pos(0, 0); // ������� ������

        bool allCurrentHeadersFound = false;
        while (!allCurrentHeadersFound)
        {
            PairOfTags currentHeader("h");  currentHeader.name.append(to_string(i));

            if (currentHeader.findPairOfTags(code, pos))
            {
                // ��������� ���������
                this->headers.push_back(currentHeader);
                this->levelsOfHeaders.push_back(i);

                // ������ ��������� ����� ����� ��������� ���� ����� ���������
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

void TableOfContents::clearCodeFromComments(std::vector <std::string>& code)
{

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

/*! ���������� ���������� �� ������ � ��������� ��� � ���������� �������
 *\param [in] headers - ��������������� ������� ����������
 *\return - ���������� */
void TableOfContents::generateTableOfContents(const vector <string>& code)
{
    findAllHeaders(code);
    readContentsOfHeaders(code);

     // �������� ���������� � ������� html-����
     // ���� ����� > 0
    if (headers.size() > 0)
    {
        // ��������� ������� ����������� �� ��������, ������� ������ ������� ����
        int currentNestingLvl = 0;
        while (currentNestingLvl < levelsOfHeaders[0])
        {
            string s = createStringWithNestingLvl(currentNestingLvl);
            s.append("<ul>");
            codeOfTableOfContents.push_back(s);
            currentNestingLvl++;
        }

        // ��� ���� ����������
        for (int i = 0; i < headers.size() /*|| currentNesting != 0*/; i++)
        {
            // ������� ������, ������������ � ��� ����� ���������
            string s = createStringWithNestingLvl(currentNestingLvl);

            // ������������ � ��� ����� ���������
            s.append("<li>");
            s.append(headers[i].content);
            s.append("</li>");

            // �������� � � ������
            this->codeOfTableOfContents.push_back(s);

            // ���� ���� ��������� �� ��� ���������
            if (i != headers.size() - 1)
            {
                // ���� ������� ���������� ��������� ������ ������ ��������
                if (levelsOfHeaders[i + 1] > levelsOfHeaders[i])
                {
                    // ��������� ����������� �������
                    for (int j = levelsOfHeaders[i + 1] - levelsOfHeaders[i]; j > 0; j--)
                    {
                        string s = createStringWithNestingLvl(currentNestingLvl);
                        s.append("<ul>");
                        codeOfTableOfContents.push_back(s);
                        currentNestingLvl++;
                    }
                }

                // ���� ��������� ��������� ������ ��������
                else  if (levelsOfHeaders[i + 1] < levelsOfHeaders[i])
                {
                    // ��������� ����������� �������
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

/*! ��������� � ��������� �� ����������
 *\param [in] code - html-���*/
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
