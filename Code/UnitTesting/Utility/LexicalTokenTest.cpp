#include "stdafx.h"
#include "CppUnitTest.h"
#include "LexicalToken.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(LexicalTokenTest)
	{
	public:

		TEST_METHOD(testValidVariable)
		{
			bool result = LexicalToken::verifyName("test");
			Assert::AreEqual(true, result);
		}

		TEST_METHOD(testEmptyVariable)
		{
			bool result = LexicalToken::verifyName("");
			Assert::AreEqual(false, result);
		}

		TEST_METHOD(testWhitespaceVariable)
		{
			bool result = LexicalToken::verifyName(" ");
			Assert::AreEqual(false, result);
		}

		TEST_METHOD(testWhitespaceBetweenVariable)
		{
			bool result = LexicalToken::verifyName("aasd vasd");
			Assert::AreEqual(false, result);
		}

		TEST_METHOD(testInvalidVariable)
		{
			bool result = LexicalToken::verifyName("1test");
			Assert::AreEqual(false, result);
		}

		TEST_METHOD(testInvalidVariable2)
		{
			bool result = LexicalToken::verifyName("512");
			Assert::AreEqual(false, result);
		}

		TEST_METHOD(testValidInteger)
		{
			bool result = LexicalToken::verifyInteger("55");
			Assert::AreEqual(true, result);
		}

		TEST_METHOD(testLongValidInteger)
		{
			bool result = LexicalToken::verifyInteger("999999999999999999999999999999999999999999999999999999999999999999");
			Assert::AreEqual(true, result);
		}

		TEST_METHOD(testInvalidInteger)
		{
			bool result = LexicalToken::verifyInteger("asd123");
			Assert::AreEqual(false, result);
		}

		TEST_METHOD(testInvalidInteger2)
		{
			bool result = LexicalToken::verifyInteger("12381023a12");
			Assert::AreEqual(false, result);
		}

	};
}