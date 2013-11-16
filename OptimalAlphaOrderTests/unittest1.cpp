#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include "..\OptimalAlphaOrderCore\OptimalAlphaOrderCore.h"
#pragma comment(lib, "..\\Debug\\OptimalAlphaOrderCore")

namespace OptimalAlphaOrderTests
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(test_alphabet_string)
		{
			// TODO:  在此输入测试代码
			auto alpha1 = base_alphabet();
			auto alphastr1 = alphabet_string(alpha1);
			Assert::AreEqual<std::string>(alphastr1,
				"ABCDEFGHIJKLMNOPQRSTUVWXYZ", L"Mismatched alphabet string");

			auto alpha2 = reversed_alphabet();
			auto alphastr2 = alphabet_string(alpha2);
			Assert::AreEqual<std::string>(alphastr2,
				"ZYXWVUTSRQPONMLKJIHGFEDCBA", L"Mismatched alphabet string");
		}

		TEST_METHOD(test_swap_letter)
		{
			auto alpha1 = base_alphabet();
			swap_letter(alpha1, 'A', 'B');
			auto alphastr1 = alphabet_string(alpha1);
			Assert::AreEqual<std::string>(alphastr1, "BACDEFGHIJKLMNOPQRSTUVWXYZ");

			auto alpha2 = base_alphabet();
			swap_letter(alpha2, 'Y', 'Z');
			auto alphastr2 = alphabet_string(alpha2);
			Assert::AreEqual<std::string>(alphastr2, "ABCDEFGHIJKLMNOPQRSTUVWXZY");

			auto alpha3 = base_alphabet();
			swap_letter(alpha3, 'A', 'Z');
			auto alphastr3 = alphabet_string(alpha3);
			Assert::AreEqual<std::string>(alphastr3, "ZBCDEFGHIJKLMNOPQRSTUVWXYA");

			auto alpha4 = base_alphabet();
			swap_letter(alpha4, 'I', 'O');
			auto alphastr4 = alphabet_string(alpha4);
			Assert::AreEqual<std::string>(alphastr4, "ABCDEFGHOJKLMNIPQRSTUVWXYZ");
		}

		TEST_METHOD(test_is_ordered)
		{
			auto alpha1 = base_alphabet();
			Assert::IsTrue(is_ordered("ABCD", alpha1));
			Assert::IsTrue(is_ordered("AABCDXYZ", alpha1));
			Assert::IsFalse(is_ordered("AACB", alpha1));

			swap_letter(alpha1, 'A', 'B');
			Assert::IsFalse(is_ordered("ABCD", alpha1));
			Assert::IsFalse(is_ordered("AABCDXYZ", alpha1));
			Assert::IsTrue(is_ordered("BAC", alpha1));
			Assert::IsTrue(is_ordered("BBAAC", alpha1));
			Assert::IsFalse(is_ordered("BCA", alpha1));
		}

		TEST_METHOD(test_alphabet_score)
		{
			std::string arrwords[] = { "THIS", "IS", "A", "SIMPLE", "ALPHABET", "SCORE", "TEST" };
			std::vector<std::string> words(begin(arrwords), end(arrwords));
			Assert::AreEqual(2, alphabet_score(words, base_alphabet()));

			Assert::AreEqual(860, alphabet_score(read_words(), base_alphabet()));
		}
	};
}