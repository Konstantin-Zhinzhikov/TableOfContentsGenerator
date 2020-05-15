#include "pch.h"
#include "CppUnitTest.h"
#include "..\TableOfContentsGenerator\TableOfContents.h"

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

	TEST_CLASS(TestTableOfContents)
	{
	public:
		
		TEST_METHOD(Test_GenerateTableOfContents)
		{
		}
	};
}
