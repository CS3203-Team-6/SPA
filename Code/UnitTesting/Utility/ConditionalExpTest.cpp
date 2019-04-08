#include "stdafx.h"
#include "CppUnitTest.h"
#include "ConditionalExp.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(ConditionalExpTest)
	{
		TEST_METHOD(testValidInput)
		{
			bool result = ConditionalExp::verifyConditionalExp("x==y+1");
			Assert::AreEqual(true, result);
		}

		TEST_METHOD(testInvalidInput)
		{
			bool result = ConditionalExp::verifyConditionalExp("x)=y+1");
			Assert::AreEqual(false, result);
		}

		TEST_METHOD(testAND)
		{
			bool result = ConditionalExp::verifyConditionalExp("(x==(y+1))&&(z<=3)");
			Assert::AreEqual(true, result);
		}

		TEST_METHOD(testNOT)
		{
			bool result = ConditionalExp::verifyConditionalExp("!(x<(5+3))");
			Assert::AreEqual(true, result);
		}

		TEST_METHOD(testVariables)
		{
			bool result = ConditionalExp::verifyConditionalExp("!(((variable1 != variable2 + variable3 + (variable4 % 5)) && ( variable5<= variable6 +200)) || (!(((variable7 <= (variable8 + 7 + 0)) && (variable9 == variable10 + 3 - 6 * 9 / 4)) || (!(variable11 <= 6) || (9 < 3)))))");
			std::vector<std::string>expectedVariables = { "variable1", "variable2","variable3","variable4","variable5","variable6","variable7","variable8","variable9","variable10","variable11" };
			std::vector<std::string>actualVariables = ConditionalExp::getVariables();
			Assert::IsTrue(expectedVariables == actualVariables);
		}

	};
}