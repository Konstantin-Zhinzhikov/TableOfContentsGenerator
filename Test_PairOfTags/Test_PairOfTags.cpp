#include "pch.h"
#include "CppUnitTest.h"
#include "..\TableOfContentsGenerator\PairOfTags.h"

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
		
		TEST_METHOD(aaaa)
		{

		}
	};
}
