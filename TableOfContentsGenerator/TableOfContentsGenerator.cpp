#include <iostream>
#include <ctime>
#include "TableOfContents.h"

#pragma warning(disable : 4996)

using namespace std;

void readTextFromFile(vector <string>& text, string path);

int main(int argc, char *argv[])
{
    setlocale(LC_ALL, "Russian");

    string path; // Путь к файлу с кодом

    try
    {
        if (argc == 2)
            path = argv[1];
        else
        {
            cout << "Введите путь к файлу:\n";
            cin >> path;
        }

        // Проверить расширение файла
        int dot = path.find_last_of('.');
        if (path.find(".html", dot - 1) == -1)
            throw(string("Файл в неверном формате"));

        vector <string> code; // Код html-страницы

        // Считать входные данные
        readTextFromFile(code, path);

        // Сгенерировать оглавление
        cout << "Начата генерация оглавления\n";
        TableOfContents tableOfContents;
        tableOfContents.generateTableOfContents(code);

        cout << "Оглавление сгенерировано.\n";
        // Записать текущее дату и время в строку
        time_t now = time(0);
        tm* currentTm = localtime(&now);
        char currentDateAndTime[20];
        strftime(currentDateAndTime, 20, "%d.%m.%y_%H.%M", currentTm);

        // Записать результат
        string resultPath = "..\\Results\\";
        resultPath.append(string(currentDateAndTime));
        resultPath.append(".html");

        tableOfContents.writeTableOfContentsInFile(resultPath);
    }

    catch (const string message)
    {
        cerr << "\nОШИБКА: " << message << endl;
    }
}


void readTextFromFile(vector<string>& text, string path)
{
    ifstream source(path);

    if (!source.is_open())
        throw string("Не найден файл");

    string s;
    while (getline(source, s))
        text.push_back(s);
}
