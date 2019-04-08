#include "stdafx.h"
#include "CppUnitTest.h"
#include "Statement.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(StatementTest)
	{
	public:
		TEST_METHOD(testStringGetter1)
		{
			std::string expected = "a = b";
			std::string actual = Statement("a = b", 1, 0).getString();
			Assert::AreEqual(expected, actual);
		}
		TEST_METHOD(testStringGetter2)
		{
			std::string expected = "if (i == 1) then";
			std::string actual = Statement("a = b", 1, 0).getString();
			Assert::AreNotEqual(expected, actual);
		}
		TEST_METHOD(testStringGetter3)
		{
			//different type numbers
			std::string expected = Statement("a = b", 2, 0).getString();
			std::string actual = Statement("a = b", 1, 0).getString();
			Assert::AreEqual(expected, actual);
		}
		TEST_METHOD(testStmtGetter1)
		{
			bool expected = false;
			bool actual = Statement("a = b", 1, 0).hasStmtLst();
			Assert::AreEqual(expected, actual);
		}
		TEST_METHOD(testStmtGetter2)
		{
			std::vector<Statement> r; 
			r.push_back(Statement("a = b", 1, 0));
			std::string w = "while (i == 0)";
			bool actual = Statement(w, r, 5, 0).hasStmtLst();
			Assert::AreEqual(true, actual);
		}
		TEST_METHOD(testTypeGetter)
		{
			//different type numbers
			int expected = Statement("a = b", 2, 0).getType();
			int actual = Statement("a = b", 1, 0).getType();
			Assert::AreNotEqual(expected, actual);
		}
	};
}
