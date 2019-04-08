#include "stdafx.h"
#include "CppUnitTest.h"
#include "AssignParser.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(AssignParserTest)
	{
	public:

		TEST_METHOD(testAssignGetAssignee)
		{
			std::string testStatement = "y=x";
			std::string parseExpected = "y";
			AssignParser ap;

			std::string output = ap.getLeft(testStatement);

			Assert::AreEqual(parseExpected, output);
		}

		TEST_METHOD(testAssignGetExpressionVariable)
		{
			std::string testStatement = "p=x+6/gp+abc123abc";
			std::vector<std::string> parseExpected{ "x", "gp", "abc123abc" };
			AssignParser ap;

			std::vector<std::string> output = ap.getRightVariable(testStatement);

			Assert::IsTrue(parseExpected == output);
		}

		TEST_METHOD(testAssignGetExpressionConstants)
		{
			std::string testStatement = "p=x+6/2+200";
			std::vector<std::string> parseExpected{ "6", "2", "200" };
			AssignParser ap;

			std::vector<std::string> output = ap.getRightConstant(testStatement);

			Assert::IsTrue(parseExpected == output);
		}

		TEST_METHOD(testAssignGetInfixExpression)
		{
			std::string testStatement = "p=x+6/2+200";
			std::string parseExpected = " + + x / 6 2 200 ";
			AssignParser ap;

			std::string output = ap.getPrefixExpression(testStatement);

			Assert::AreEqual(parseExpected, output);
		}
	};
}