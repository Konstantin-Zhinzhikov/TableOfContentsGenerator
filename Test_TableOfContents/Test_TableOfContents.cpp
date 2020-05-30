#include "pch.h"
#include "CppUnitTest.h"
#include "..\TableOfContentsGenerator\TableOfContents.h"

#define TESTS_PATH "../TESTS/TableOfContents/"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace TestTableOfContents
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
		ASSERT_tagsLocationsAreEqual(a, b);
		Assert::AreEqual(a.content, b.content);
		Assert::AreEqual(a.name, b.name);
	}

	void ASSERT_tablesOfContentsAreEqual(TableOfContents a, TableOfContents b)
	{
		Assert::AreEqual(a.headers.size(), b.headers.size());

		for (int i = 0; i < a.headers.size(); i++)
		{
			ASSERT_tagsLocationsAreEqual(a.headers[i], b.headers[i]);
		}
	}

	void ASSERT_areTextsEqual(vector<string> a, vector<string> b)
	{
		Assert::AreEqual(a.size(), b.size());

		for (int i = 0; i < a.size(); i++)
			Assert::AreEqual(a[i], b[i]);
	}

	TEST_CLASS(Test_findAllHeaders)
	{
	public:

		TEST_METHOD(TextWithHeader)
		{
			// Входные данные
			ifstream codeIn(TESTS_PATH"textWithHeaders_code.html");
			vector <string> code;
			string s;
			while (getline(codeIn, s))
				code.push_back(s);

			// Ожидаемые выходные данные
			vector <PairOfTags> expectedHeaders;
			expectedHeaders.push_back(PairOfTags("h1", 0, 0, 0, 2));
			expectedHeaders.push_back(PairOfTags("h2", 0, 4, 0, 6));
			expectedHeaders.push_back(PairOfTags("h2", 0, 12, 0, 14));
			expectedHeaders.push_back(PairOfTags("h3", 0, 16, 0, 18));
			expectedHeaders.push_back(PairOfTags("h3", 0, 24, 0, 26));
			expectedHeaders.push_back(PairOfTags("h2", 0, 32, 0, 34));


			// Вызов функции
			TableOfContents actual;
			actual.findAllHeaders(code);

			//Проверка результата
			Assert::AreEqual(actual.headers.size(), expectedHeaders.size());
			for (int i = 0; i < expectedHeaders.size(); i++)
			{
				ASSERT_tagsAreEqual(actual.headers[i], expectedHeaders[i]);
			}
		}

		TEST_METHOD(AllHeadersOneAterAnother)
		{
			// Входные данные
			ifstream codeIn(TESTS_PATH"AllHeadersOneAfterAnother_code.html");
			vector <string> code;
			string s;
			while (getline(codeIn, s))
				code.push_back(s);

			// Ожидаемые выходные данные
			vector <PairOfTags> expectedHeaders;
			expectedHeaders.push_back(PairOfTags("h1", 0, 0, 16, 0));
			expectedHeaders.push_back(PairOfTags("h2", 0, 2, 16, 2));
			expectedHeaders.push_back(PairOfTags("h3", 0, 4, 16, 4));
			expectedHeaders.push_back(PairOfTags("h4", 0, 5, 16, 5));
			expectedHeaders.push_back(PairOfTags("h5", 0, 7, 16, 7));
			expectedHeaders.push_back(PairOfTags("h6", 0, 9, 16, 9));

			// Вызов функции
			TableOfContents actual;
			actual.findAllHeaders(code);

			//Проверка результата
			Assert::AreEqual(actual.headers.size(), expectedHeaders.size());
			for (int i = 0; i < expectedHeaders.size(); i++)
			{
				ASSERT_tagsAreEqual(actual.headers[i], expectedHeaders[i]);
			}
		}
	};

	TEST_CLASS(Test_generateTableOfContents)
	{
	public:
		TEST_METHOD(TextWithHeaders)
		{
			string s; // Переменная для ввода данных

			// Входные данные
			vector <string> code;
			ifstream codeIn(TESTS_PATH"TextWithHeaders_code.html");
			while (getline(codeIn, s))
				code.push_back(s);

			// Ожидаемые выходные данные
			vector <string> expected;
			ifstream expectedIn(TESTS_PATH "TextWithHeaders_expected.html");
			while (getline(expectedIn, s))
				expected.push_back(s);

			// Вызов функции
			TableOfContents contents;
			contents.generateTableOfContents(code);

			// Проверка результата
			ASSERT_areTextsEqual(expected, contents.codeOfTableOfContents);
		}

		TEST_METHOD(AllHeadersOneAfterAnother)
		{
			string s; // Переменная для ввода данных

			// Входные данные
			vector <string> code;
			ifstream codeIn(TESTS_PATH"AllHeadersOneAfterAnother_code.html");
			while (getline(codeIn, s))
				code.push_back(s);

			// Ожидаемые выходные данные
			vector <string> expected;
			ifstream expectedIn(TESTS_PATH "AllHeadersOneAfterAnother_expected.html");
			while (getline(expectedIn, s))
				expected.push_back(s);

			// Вызов функции
			TableOfContents contents;
			contents.generateTableOfContents(code);

			// Проверка результата
			ASSERT_areTextsEqual(expected, contents.codeOfTableOfContents);
		}

		TEST_METHOD(BadStyle)
		{
			string s; // Переменная для ввода данных

			// Входные данные
			vector <string> code;
			ifstream codeIn(TESTS_PATH "BadStyle_code.html");
			while (getline(codeIn, s))
				code.push_back(s);

			// Ожидаемые выходные данные
			vector <string> expected;
			ifstream expectedIn(TESTS_PATH "BadStyle_expected.html");
			while (getline(expectedIn, s))
				expected.push_back(s);

			// Вызов функции
			TableOfContents contents;
			contents.generateTableOfContents(code);

			// Проверка результата
			ASSERT_areTextsEqual(expected, contents.codeOfTableOfContents);
		}
	};
}
