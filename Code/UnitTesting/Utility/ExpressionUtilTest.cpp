#include "stdafx.h"
#include "CppUnitTest.h"
#include "ExpressionUtil.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(ExpressionUtilTest)
	{
	public:
		TEST_METHOD(convertInfix1Variable)
		{
			std::string testStatement = "a";
			std::string expectedOutput = " a ";
			bool validExpression = ExpressionUtil::verifyInfixExpression(testStatement);

			Assert::IsTrue(validExpression);

			std::string prefixExpr = ExpressionUtil::convertInfixToPrefix(testStatement);


			Assert::AreEqual(expectedOutput, prefixExpr);
		}

		TEST_METHOD(verifyValidExpression)
		{
			std::string testStatement = "5*((a+b) / 5)";

			bool validExpression = ExpressionUtil::verifyInfixExpression(testStatement);

			Assert::IsTrue(validExpression);
		}

		TEST_METHOD(verifyInvalidBracketsNumbers)
		{
			std::string testStatement = "5*((a+b) / 5";

			bool validExpression = ExpressionUtil::verifyInfixExpression(testStatement);

			Assert::IsFalse(validExpression);
		}

		TEST_METHOD(verifyInvalidEntities)
		{
			std::string testStatement = "5*(a+bc3) / 5a";

			bool validExpression = ExpressionUtil::verifyInfixExpression(testStatement);

			Assert::IsFalse(validExpression);
		}

		TEST_METHOD(verifyInvalidBracketPlacement)
		{
			std::string testStatement = "5)*(a+b / 5";

			bool validExpression = ExpressionUtil::verifyInfixExpression(testStatement);

			Assert::IsFalse(validExpression);
		}

		TEST_METHOD(test2OperandConversion)
		{
			std::string testStatement = "x+6";
			std::string expectedConvert = " + x 6 ";

			std::string output = ExpressionUtil::convertInfixToPrefix(testStatement);

			Assert::AreEqual(expectedConvert, output);
		}

		TEST_METHOD(test3OperandConversion)
		{
			std::string testStatement = "xy+995+lcz";
			std::string expectedConvert = " + + xy 995 lcz ";

			std::string output = ExpressionUtil::convertInfixToPrefix(testStatement);

			Assert::AreEqual(expectedConvert, output);
		}

		TEST_METHOD(test4OperandConversion)
		{
			std::string testStatement = "5*95+                vvv/4";
			std::string expectedConvert = " + * 5 95 / vvv 4 ";

			std::string output = ExpressionUtil::convertInfixToPrefix(testStatement);

			Assert::AreEqual(expectedConvert, output);
		}

		TEST_METHOD(test4OperandWithBracketPreProcess)
		{
			std::string testStatement = "5*(a+b) / 5";

			

			std::string expectedConvert = "5 * ( a + b ) / 5";

			ExpressionUtil::expressionPreprocess(testStatement);

			Assert::AreEqual(expectedConvert, testStatement);
		}

		TEST_METHOD(test4OperandWithBracketConversion)
		{
			std::string testStatement = "5*(a+b) / 5";



			std::string expectedConvert = " / * 5 + a b 5 ";

			std::string output = ExpressionUtil::convertInfixToPrefix(testStatement);

			Assert::AreEqual(expectedConvert, output);
		}

		TEST_METHOD(testExtractConstants)
		{
			std::string testStatement = "5*(a+b) / 5";

			std::vector<std::string> expectedConstants{ "5", "5" };

			std::vector<std::string> output = ExpressionUtil::getConstants(testStatement);

			Assert::IsTrue(expectedConstants == output);
		}

		TEST_METHOD(testExtractVariables)
		{
			std::string testStatement = "5*(ac+bb) / 5";

			std::vector<std::string> expectedVariables{ "ac", "bb" };

			std::vector<std::string> output = ExpressionUtil::getVariables(testStatement);

			Assert::IsTrue(expectedVariables == output);
		}

		TEST_METHOD(testExtractVariables2)
		{
			std::string testStatement = "5*(ac+bb) / 5 + (abc1512)";

			std::vector<std::string> expectedVariables{ "ac", "bb", "abc1512" };

			std::vector<std::string> output = ExpressionUtil::getVariables(testStatement);

			Assert::IsTrue(expectedVariables == output);
		}
	};
}