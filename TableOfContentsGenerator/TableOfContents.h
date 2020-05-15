#pragma once
#include "PairOfTags.h"
#include <fstream>

class TableOfContents
{

public:
    TableOfContents();

    std::vector <std::string> codeOfTableOfContents;
    std::vector <PairOfTags> headers;

    void generateTableOfContents(const std::vector <std::string>& code);
    void findAllHeaders(std::vector  <std::string> code);
    void readContentsOfHeaders(const std::vector <std::string>& code);
    void writeTableOfContentsInFile(const std::string& path);
};

