/* \file ������������ ���� � ��������� ������ ����������*/

#pragma once
#include "PairOfTags.h"
#include <fstream>


/// ����� ����������
class TableOfContents
{
private:
    /// "������" ��������������� ����������. 
    /*! ��� ������� ���������� ����� h-���������, 1 ��� h1, 2 ��� h2 � �.�.*/
    std::vector <int> levelsOfHeaders;  

    void sortHeaders();
    void changeNestingLevel(int lvl, int& currentNestingLevel);
    std::string createStringWithTabs(int nestingLevel);

public:
    TableOfContents();

    /// html-��� � ����������� � ���� ����� <ul>,<li>
    std::vector <std::string> codeOfTableOfContents;

    /// ������ ����������
    std::vector <PairOfTags> headers;

    void generateTableOfContents(const std::vector <std::string>& code);
    void findAllHeaders(std::vector <std::string> code);
    static void clearCodeFromComments(std::vector <std::string>& code);
    void readContentsOfHeaders(const std::vector <std::string>& code);
    void writeTableOfContentsInFile(const std::string& path);
};

