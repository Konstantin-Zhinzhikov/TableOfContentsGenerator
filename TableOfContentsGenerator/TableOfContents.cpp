#include "TableOfContents.h"

using namespace std;

TableOfContents::TableOfContents()
{

}

/*! ������� ��� ���� ���������� (h1-h6) � ����
 *\param [in] code - html-���*/
void TableOfContents::findAllHeaders(vector  <string> code)
{
    // �������� ��� ����������� � ���� ��������� 'c'
    clearCodeFromComments(code);

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

/* �������� � ���� ��� ����������� ��������� 'c'
*\param [in/out] code - html-��� ��������*/
void TableOfContents::clearCodeFromComments(std::vector <std::string>& code)
{
    bool commentIsOpenedBefore = false;

    for (int i = 0; i < code.size(); i++)
    {
        bool commentOpensInCurrString = commentIsOpenedBefore;
        int openIndx = -1, closIndx = -1; // �������, ��� ����������� �� ����������� � ���� ������        

        // ---- ����� ������������ ----
        // ���� ����������� �� ��� ������ �����
        if (!commentIsOpenedBefore)
        {
            openIndx = code[i].find("<!--"); // ������ �������� ����������� � ������
            // ���� ������
            if (openIndx != -1)
            {
                commentOpensInCurrString = true; //C������, ��� �� ������ � ���� ������
                closIndx = code[i].find("-->", openIndx + 3); // ������ ����� ����������� ����� ��������
                // ���� ������
                if (closIndx != -1)
                    commentOpensInCurrString = false; // �������, ��� ����������� ����������� � ���� ������
            }
        }
        // ����� ���� ����������� ��� ������ �����
        else if (commentIsOpenedBefore)
        {
            closIndx = code[i].find("-->"); // ������ ����� ����������� � ������ ������
            // ���� ������
            if (closIndx != -1)
                commentOpensInCurrString = false; // �������, ��� ����������� ����������� � ���� ������
        }

        // ---- �������� ----
        // ���� ����������� ��� ������ � ���� ������
        if (openIndx != -1 && !commentIsOpenedBefore)
        {
            // ���� ����������� �� ��� ������ � ���� ������
            if (closIndx == -1)
            {
                // ������� ��� ����� �������� �����������
                for (int j = openIndx; j < code[i].size(); j++)
                    code[i].replace(j, 1, "c");
            }
            // ���� ����������� ��� ������ � ���� ������
            else if (closIndx != -1)
            {
                // ������� ��� ����� �������� ����������� � �� ��������
                for (int j = openIndx; j < closIndx + 3; j++)
                    code[i].replace(j, 1, "c");
                i--; // �� ���������� �� ��������� ������, �.�. � ���� ������ ����� ���� ��� ���� �����������
            }
        }

        // ���� ����������� ��� ������ �����
        else if (openIndx == -1 && commentIsOpenedBefore)
        {
            // ���� ����������� �� ��� ������ � ���� ������
            if (closIndx == -1)
            {
                // ������� ������
                for (int j = 0; j < code[i].size(); j++)
                    code[i].replace(j, 1, "c");
            }
            // ���� ����������� ��� ������ � ���� ������
            else if (closIndx != -1)
            {
                // ������� ��� �� �������� �����������
                for (int j = 0; j < closIndx + 3; j++)
                    code[i].replace(j, 1, "c");

                i--; // �� ���������� �� ��������� ������, �.�. � ���� ������ ����� ���� ��� ���� �����������
            }
        }
        commentIsOpenedBefore = commentOpensInCurrString; // ������� ��� ����������� ������, ���� � ���� ������ �� �� ��� ������ ��� ��� ������ 
    }
}

/*! ���������� ���������� �� �� ��������������*/
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

/*! ���������� ���������� html-��������
 *\param [in] code - ��� html-��������*/
void TableOfContents::generateTableOfContents(const vector <string>& code)
{
    findAllHeaders(code);
    
    if (headers.size() > 0)
    {
        readContentsOfHeaders(code);
        int currentNestingLvl = 0;

        // ��������� ������� ����������� �� ��������, ������� ������ ������� ����
        changeNestingLevel(levelsOfHeaders[0], currentNestingLvl);

        // ��� ���� ����������
        for (int i = 0; i < headers.size(); i++)
        {
            string s = createStringWithTabs(currentNestingLvl);
            s.append("<li>");
            s.append(headers[i].content);
            s.append("</li>");
            this->codeOfTableOfContents.push_back(s);

            // ���� ���� ��������� �� ��� ���������
            if (i != headers.size() - 1)
            {
                // �������� ������� ����������� �� ������ ���������� ����
                changeNestingLevel(levelsOfHeaders[i + 1], currentNestingLvl);
            }
        }

        // ��������� ������� ����������� �� ����
        changeNestingLevel(0, currentNestingLvl);
    }
}

/*! ��������� ������ �����������
 *\param [in] requiedLvl - �������, �� �������� ��������� ������� �����������
 *\param [in/out] currentLvl - ������� ������� �����������*/
void TableOfContents::changeNestingLevel(int requiredLvl, int& currentLvl)
{
    // ��������� ������� �����������
    while (currentLvl < requiredLvl)
    {
        string s = createStringWithTabs(currentLvl);
        s.append("<ul>");
        codeOfTableOfContents.push_back(s);
        currentLvl++;
    }

    // ��������� ������� �����������
    while (currentLvl > requiredLvl)
    {
        currentLvl--;
        string s = createStringWithTabs(currentLvl);
        s.append("</ul>");
        codeOfTableOfContents.push_back(s);
    }
}

/*! ������� ������ � �����������
 *\param [in] numberOfTabs - ���������� ������ ���������
 *\return - ������ �� ������� ���������*/
string TableOfContents::createStringWithTabs(int numberOfTabs)
{
    string s;
    for (int i = 0; i < numberOfTabs; i++)
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

/* ������ ���������� � ����
*\param [in] path - ���� � ������ �����*/
void TableOfContents::writeTableOfContentsInFile(const string& path)
{
    ofstream out(path);
    for (int i = 0; i < codeOfTableOfContents.size(); i++)
        out << this->codeOfTableOfContents[i] << "\n";
}