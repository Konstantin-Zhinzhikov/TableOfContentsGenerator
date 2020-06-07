#include "pch.h"
#include "CppUnitTest.h"
#include <fstream>
#include "..\TableOfContentsGenerator\PairOfTags.h"

#define TESTS_PATH "../TESTS/PairOfTags/"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace TestPairOfTags
{
	// Тестовое API
	void ASSERT_tagsLocationsAreEqual(PairOfTags a, PairOfTags b)
	{
		// Проверка открывающих тегов
		Assert::AreEqual(a.openingTagLocation.stringIndex, b.openingTagLocation.stringIndex);
		Assert::AreEqual(a.openingTagLocation.charIndex, b.openingTagLocation.charIndex);

		// Проверка закрывающих тегов
		Assert::AreEqual(a.closingTagLocation.stringIndex, b.closingTagLocation.stringIndex);
		Assert::AreEqual(a.closingTagLocation.charIndex, b.closingTagLocation.charIndex);
	}

	void ASSERT_tagsAreEqual(PairOfTags a, PairOfTags b)
	{
		Assert::AreEqual(a.content, b.content);
		Assert::AreEqual(a.name, b.name);
		ASSERT_tagsLocationsAreEqual(a, b);

	}

	TEST_CLASS(Test_findPairOfTags)
	{
	public:

		TEST_METHOD(OneHeaderInOneString)
		{
			// Входные данные
			string tagName = "h1";

			vector <string> code;
			code.push_back("<h1>Заголовок</h1>");

			// Ожидаемые выходные данные
			LocationInText openingTagLocation(0, 0), closingTagLocation(13, 0);
			PairOfTags expected(tagName, openingTagLocation, closingTagLocation);

			// Вызов функции
			LocationInText pos(0, 0);
			PairOfTags actual(tagName);
			actual.findPairOfTags(code, pos);

			// Проверка результата
			ASSERT_tagsLocationsAreEqual(expected, actual);
		}

		TEST_METHOD(OneHeaderInFewStrings)
		{
			// Входные данные
			string tagName = "h1";

			vector <string> code;
			code.push_back(" <h1>");
			code.push_back("	Заголовок");
			code.push_back(" </h1>");

			// Ожидаемые выходные данные
			LocationInText openingTagLocation(1, 0), closingTagLocation(1, 2);
			PairOfTags expected(tagName, openingTagLocation, closingTagLocation);

			// Вызов функции
			LocationInText pos(0, 0);
			PairOfTags actual(tagName);
			actual.findPairOfTags(code, pos);

			// Проверка результата
			ASSERT_tagsLocationsAreEqual(expected, actual);
		}

		TEST_METHOD(TwoSameHeaders)
		{
			// Входные данные
			string tagName = "h1";

			vector <string> code;
			code.push_back("<h1>Первый заголовок</h1>");
			code.push_back("<h1>Второй заголовок</h1>");

			// Ожидаемые выходные данные
			LocationInText openingTagLocation(0, 0), closingTagLocation(20, 0);
			PairOfTags expected(tagName, openingTagLocation, closingTagLocation);

			// Вызов функции
			LocationInText pos(0, 0);
			PairOfTags actual(tagName);
			actual.findPairOfTags(code, pos);

			// Проверка результата
			ASSERT_tagsLocationsAreEqual(expected, actual);
		}

		TEST_METHOD(TwoSameHeaders_Spaces)
		{
			// Входные данные
			string tagName = "h1";

			vector <string> code;
			code.push_back(" \t<h1> ");
			code.push_back("Первый заголовок</h1><h1>Второй");
			code.push_back(" заголовок  </h1>   ");

			// Ожидаемые выходные данные
			LocationInText openingTagLocation(2, 0), closingTagLocation(16, 1);
			PairOfTags expected(tagName, openingTagLocation, closingTagLocation);

			// Вызов функции
			LocationInText pos(0, 0);
			PairOfTags actual(tagName);
			actual.findPairOfTags(code, pos);

			// Проверка результата
			ASSERT_tagsLocationsAreEqual(expected, actual);
		}

		TEST_METHOD(TwoHeaders_SearchingForFirst_1)
		{
			// Входные данные
			string tagName = "h1";

			vector <string> code;
			code.push_back("<h1>Первый заголовок</h1>");
			code.push_back("<h2>Второй заголовок</h2>");

			// Ожидаемые выходные данные
			LocationInText openingTagLocation(0, 0), closingTagLocation(20, 0);
			PairOfTags expected(tagName, openingTagLocation, closingTagLocation);

			// Вызов функции
			LocationInText pos(0, 0);
			PairOfTags actual(tagName);
			actual.findPairOfTags(code, pos);

			// Проверка результата
			ASSERT_tagsLocationsAreEqual(expected, actual);
		}

		TEST_METHOD(TwoHeaders_SearchingForFirst_2)
		{
			// Входные данные
			string tagName = "h1";

			vector <string> code;
			code.push_back("<h1>");
			code.push_back("\tПервый заголовок");
			code.push_back("</h1>");
			code.push_back("<h2>");
			code.push_back("\tВторой заголовок");
			code.push_back("</h2>");

			// Ожидаемые выходные данные
			LocationInText openingTagLocation(0, 0), closingTagLocation(0, 2);
			PairOfTags expected(tagName, openingTagLocation, closingTagLocation);

			// Вызов функции
			LocationInText pos(0, 0);
			PairOfTags actual(tagName);
			actual.findPairOfTags(code, pos);

			// Проверка результата
			ASSERT_tagsLocationsAreEqual(expected, actual);
		}

		TEST_METHOD(TwoHeaders_SearchingForSecond_1)
		{
			// Входные данные
			string tagName = "h2";

			vector <string> code;
			code.push_back("<h1>Первый заголовок</h1>");
			code.push_back("<h2>Второй заголовок</h2>");

			// Ожидаемые выходные данные
			LocationInText openingTagLocation(0, 1), closingTagLocation(20, 1);
			PairOfTags expected(tagName, openingTagLocation, closingTagLocation);

			// Вызов функции
			LocationInText pos(0, 0);
			PairOfTags actual(tagName);
			actual.findPairOfTags(code, pos);

			// Проверка результата
			ASSERT_tagsLocationsAreEqual(expected, actual);
		}

		TEST_METHOD(TwoHeaders_SearchingForSecond_2)
		{
			// Входные данные
			string tagName = "h2";

			vector <string> code;
			code.push_back("<h1>");
			code.push_back("\tПервый заголовок");
			code.push_back("</h1>");
			code.push_back("<h2>");
			code.push_back("\tВторой заголовок");
			code.push_back("</h2>");

			// Ожидаемые выходные данные
			LocationInText openingTagLocation(0, 3), closingTagLocation(0, 5);
			PairOfTags expected(tagName, openingTagLocation, closingTagLocation);

			// Вызов функции
			LocationInText pos(0, 0);
			PairOfTags actual(tagName);
			actual.findPairOfTags(code, pos);

			// Проверка результата
			ASSERT_tagsLocationsAreEqual(expected, actual);
		}
	};

	TEST_CLASS(Test_findTagsLocationsInString)
	{
	public:

		TEST_METHOD(BothTagsInOneString)
		{
			// Входные данные
			string tagName = "h1";
			string str = "<h1>ЗАГОЛОВОК</h1>";

			// Ожидаемые выходные данные
			LocationInText openingTagLocation(0, -1), closingTagLocation(13, -1);
			PairOfTags expected(tagName, openingTagLocation, closingTagLocation);

			// Вызов функции
			PairOfTags actual(tagName);
			actual.findTagsLocationsInString(str);

			// Проверка результата
			ASSERT_tagsLocationsAreEqual(expected, actual);

		}

		TEST_METHOD(OnlyOpeningTagInBeginningOfString)
		{
			// Входные данные
			string tagName = "h1";
			string str = "<h1>";

			// Ожидаемые выходные данные
			LocationInText openingTagLocation(0, -1), closingTagLocation(-1, -1);
			PairOfTags expected(tagName, openingTagLocation, closingTagLocation);

			// Вызов функции
			PairOfTags actual(tagName);
			actual.findTagsLocationsInString(str);

			// Проверка результата
			ASSERT_tagsLocationsAreEqual(expected, actual);
		}

		TEST_METHOD(OnlyClosingTagInBeginningOfString)
		{
			// Входные данные
			string tagName = "h1";
			string str = "</h1>";

			// Ожидаемые выходные данные
			LocationInText openingTagLocation(-1, -1), closingTagLocation(0, -1);
			PairOfTags expected(tagName, openingTagLocation, closingTagLocation);

			// Вызов функции
			PairOfTags actual(tagName);
			actual.findTagsLocationsInString(str);

			// Проверка результата
			ASSERT_tagsLocationsAreEqual(expected, actual);
		}
	};

	TEST_CLASS(Test_readContentOfTag)
	{
	public:

		TEST_METHOD(OneString)
		{
			// Входные данные
			vector <string> code;
			code.push_back("<p>Content</p>");

			string tagName = "p";
			LocationInText openTagLoc(0, 0), closTagLoc(10, 0);

			// Ожидаемые выходные данные
			PairOfTags expected(tagName, openTagLoc, closTagLoc);
			expected.content = "Content";

			// Вызов функции
			PairOfTags actual(tagName, openTagLoc, closTagLoc);
			actual.readContentOfTag(code);

			// Проверка результата
			ASSERT_tagsAreEqual(expected, actual);
		}

		TEST_METHOD(ThreeStrings)
		{
			// Входные данные
			string tagName = "p";
			LocationInText openTagLoc(0, 0), closTagLoc(0, 2);
			ifstream codeIn(TESTS_PATH"readContentOfTag/ThreeStrings_code.txt");
			vector <string> code;
			string s;
			while (getline(codeIn, s))
			{
				code.push_back(s);
			}

			// Ожидаемые выходные данные
			ifstream expectedIn(TESTS_PATH"readContentOfTag/ThreeStrings_expected.txt");
			PairOfTags expected(tagName, openTagLoc, closTagLoc);
			getline(expectedIn, expected.content);

			// Вызов функции
			PairOfTags actual(tagName, openTagLoc, closTagLoc);
			actual.readContentOfTag(code);

			// Проверка результата
			ASSERT_tagsAreEqual(expected, actual);
		}

		TEST_METHOD(ContentsOthersTags)
		{
			// Входные данные
			string tagName = "h3";
			LocationInText openTagLoc(1, 0), closTagLoc(1, 2);
			ifstream codeIn(TESTS_PATH"readContentOfTag/ContentsOthersTags_code.txt");
			vector <string> code;
			string s;
			while (getline(codeIn, s))
			{
				code.push_back(s);
			}

			// Ожидаемые выходные данные
			ifstream expectedIn(TESTS_PATH"readContentOfTag/ContentsOthersTags_expected.txt");
			PairOfTags expected(tagName, openTagLoc, closTagLoc);
			getline(expectedIn, expected.content);

			// Вызов функции
			PairOfTags actual(tagName, openTagLoc, closTagLoc);
			actual.readContentOfTag(code);

			// Проверка результата
			ASSERT_tagsAreEqual(expected, actual);
		}

		TEST_METHOD(ContentOnThreeStrings)
		{
			// Входные данные
			string tagName = "h2";
			LocationInText openTagLoc(0, 1), closTagLoc(0, 5);
			ifstream codeIn(TESTS_PATH"readContentOfTag/ContentOnThreeStrings_code.txt");
			vector <string> code;
			string s;
			while (getline(codeIn, s))
			{
				code.push_back(s);
			}

			// Ожидаемые выходные данные
			ifstream expectedIn(TESTS_PATH"readContentOfTag/ContentOnThreeStrings_expected.txt");
			PairOfTags expected(tagName, openTagLoc, closTagLoc);
			getline(expectedIn, expected.content);

			// Вызов функции
			PairOfTags actual(tagName, openTagLoc, closTagLoc);
			actual.readContentOfTag(code);

			// Проверка результата
			ASSERT_tagsAreEqual(expected, actual);
		}

		TEST_METHOD(VeryDividedContent)
		{
			// Входные данные
			string tagName = "h1";
			LocationInText openTagLoc(0, 0), closTagLoc(1, 10);
			ifstream codeIn(TESTS_PATH"readContentOfTag/VeryDividedContent_code.txt");
			vector <string> code;
			string s;
			while (getline(codeIn, s))
			{
				code.push_back(s);
			}

			// Ожидаемые выходные данные
			ifstream expectedIn(TESTS_PATH"readContentOfTag/VeryDividedContent_expected.txt");
			PairOfTags expected(tagName, openTagLoc, closTagLoc);
			getline(expectedIn, expected.content);

			// Вызов функции
			PairOfTags actual(tagName, openTagLoc, closTagLoc);
			actual.readContentOfTag(code);

			// Проверка результата
			ASSERT_tagsAreEqual(expected, actual);
		}
	};
}
