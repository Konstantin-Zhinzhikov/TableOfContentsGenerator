#pragma once
#include "PairOfTags.h"
#include <fstream>

class TableOfContents
{
private:
    void sortHeaders();
    std::string createStringWithNestingLvl(int nestingLevel);

public:
    TableOfContents();

    std::vector <std::string> codeOfTableOfContents;
    std::vector <PairOfTags> headers;
    std::vector <int> levelsOfHeaders;

    void generateTableOfContents(const std::vector <std::string>& code);
    void findAllHeaders(const std::vector  <std::string> &code);
    void readContentsOfHeaders(const std::vector <std::string>& code);
    void writeTableOfContentsInFile(const std::string& path);
};

