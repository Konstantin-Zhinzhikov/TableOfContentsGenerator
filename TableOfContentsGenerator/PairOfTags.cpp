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

/*! ������� ��������� ��������� �/��� ��������� ���� � ������
*\param [in] str - ������, � ������� ����� ������������� �����
*\param [in] pos - ������� ������ ������*/
void PairOfTags::findTagsLocationsInString(const string& str, int pos)
{
    string s = str.substr(pos); // ���������, � ������� ������������ �����

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

/*! ������� ������ ��������� ���� � ����
 *\param [in] code - html-���
 *\param [in] pos - ������� ������ ������
 *\return - 0, ���� ��� �� ������*/
bool PairOfTags::findPairOfTags(const vector <string>& code, LocationInText pos)
{
    bool foundOpening = false, foundClosing = false;

    // ���� ������� ������ �� ��������� �� ����� � ���� �� ������� �������� � �������� ����
    while (pos.stringIndex < (int)code.size() && !(foundOpening && foundClosing))
    {
        PairOfTags tmp(name);

        // ������ ��� � ������
        tmp.findTagsLocationsInString(code[pos.stringIndex], pos.charIndex);
        tmp.openingTagLocation.stringIndex = pos.stringIndex;
        tmp.closingTagLocation.stringIndex = pos.stringIndex;

        // ���� ��� ��� ����������, �� �� ��� �������� ������� ��� ���� ��������
        if (foundOpening && tmp.openingTagLocation.charIndex != -1 &&
            (tmp.closingTagLocation > tmp.openingTagLocation || tmp.closingTagLocation.charIndex == -1))
            throw string("�������������� ���������� ����������� � ����������� �����");

        // ���� ������ �������� � �������� ��� ��� �� ��� ������
        if (tmp.openingTagLocation.charIndex != -1 && !foundOpening)
        {
            this->openingTagLocation = tmp.openingTagLocation;
            foundOpening = true;
        }

        // ���� ������ ��������
        if (tmp.closingTagLocation.charIndex != -1 && !foundClosing)
        {
            // ���� �������� ��� �� ��� ������
            if (!foundOpening)
                throw string("�������������� ���������� ����������� � ����������� �����");

            this->closingTagLocation = tmp.closingTagLocation;
            foundClosing = true;
        }

        // ������� �� ����. ������
        pos.charIndex = 0;
        pos.stringIndex++;
    }

    if (foundOpening != foundClosing)
        throw string("�������������� ���������� ����������� � ����������� �����");

    return foundOpening && foundClosing;
}

/*! ������ �� ���� ���������� ���������
 *\param [in] code - html-���*/
void PairOfTags::readContentOfTag(const vector <string>& code)
{
    // ������ ������ ��������
    LocationInText beginOfContent(code[openingTagLocation.stringIndex].find(">", openingTagLocation.charIndex), openingTagLocation.stringIndex);
    //LocationInText beginOfContent(openingTagLocation.charIndex + name.size() + 1, openingTagLocation.stringIndex);
    beginOfContent.incPos(code[beginOfContent.stringIndex]);

    // ������ ����� ��������
    LocationInText endOfContent = closingTagLocation;
    endOfContent.charIndex--;

    // ������� ������� �� ����� ���������� ������, ���� ����������� ��� � ������ ������
    if (this->closingTagLocation.charIndex < 0)
    {
        endOfContent.stringIndex--;
        endOfContent.charIndex = code[endOfContent.stringIndex].length() - 1;
        //int prevStr = endOfContent.stringIndex - 1;
        //endOfContent.setLocation(code[prevStr].length() - 1, prevStr);
    }

    // ���� ������� ���������� �� ����� ������
    if (beginOfContent.stringIndex == endOfContent.stringIndex)
    {
        // ������������ � ���������� ���� ��������� �� ������ �������� �� ��� �����
        content.append(code[beginOfContent.stringIndex], beginOfContent.charIndex, endOfContent.charIndex - beginOfContent.charIndex + 1);
    }

    else
    {
        // ����������� ��� �� ������ �������� �� ����� ������
        content.append(code[beginOfContent.stringIndex], beginOfContent.charIndex, code[beginOfContent.stringIndex].length());

        // ��� ���� ����� �� ������+1 �� ����� ��������
        for (int i = beginOfContent.stringIndex + 1; i < endOfContent.stringIndex; i++)
        {
            //����������� ������ �������
            content.append(code[i]);
        }

        // ����������� ��������� ������ �� ������ ������ �� ����� ��������
        content.append(code[endOfContent.stringIndex], 0, endOfContent.charIndex + 1);
    }

    // ������� ��� ����� ��������� �� ������
    int tabPos = content.find("\t");
    while (tabPos != -1)
    {
        content.replace(tabPos, 1, "");
        tabPos = content.find("\t");
    }

    // �������� �� ������ ��������� 
    if (this->content.length() == 0)
        throw string("������ ���������");
}