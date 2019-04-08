#include "stdafx.h"
#include "CppUnitTest.h"
#include "PrintParser.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(PrintParserTest)
	{

		TEST_METHOD(testValidInput)
		{
			std::string result = PrintParser::parsePrintStmt("print var1");
			std::string expected = "var1";
			Assert::AreEqual(expected, result);
		}

		TEST_METHOD(testInvalidInput)
		{
			bool result = PrintParser::verifyPrintStmt("print 2day");
			Assert::IsTrue(result);
		}

		TEST_METHOD(testVerifyVarcName)
		{
			bool result = PrintParser::verifyPrintStmt("print ComputeWatever");
			Assert::AreEqual(result, true);
		}

		TEST_METHOD(testVerifyInvalidStmt)
		{
			bool result = PrintParser::verifyPrintStmt("printed computeWatever");
			Assert::AreEqual(result, false);
		}
	};
}