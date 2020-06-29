/* \file Заголовочный файл с описанием класса оглавления*/

#pragma once
#include "PairOfTags.h"
#include <fstream>


/// Класс оглавления
class TableOfContents
{
private:
    /// "Уровни" соответствующих заголовков. 
    /*! Под уровнем понимается номер h-заголовка, 1 для h1, 2 для h2 и т.д.*/
    std::vector <int> levelsOfHeaders;  

    void sortHeaders();
    void changeNestingLevel(int lvl, int& currentNestingLevel);
    std::string createStringWithTabs(int nestingLevel);

public:
    TableOfContents();
        
    std::vector <std::string> codeOfTableOfContents; ///< html-код с оглавлением в виде тегов <ul>,<li>   
    std::vector <PairOfTags> headers;                ///< Массив заголовков

    void generateTableOfContents(const std::vector <std::string>& code);
    void findAllHeaders(std::vector <std::string> code);
    static void clearCodeFromComments(std::vector <std::string>& code);
    void readContentsOfHeaders(const std::vector <std::string>& code);
    void writeTableOfContentsInFile(const std::string& path);
};

