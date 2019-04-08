#include "stdafx.h"
#include "CppUnitTest.h"
#include "ReadParser.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(ReadParserTest)
	{

		TEST_METHOD(testValidInput)
		{
			std::string result = ReadParser::parseReadStmt("read   var1");
			std::string expected = "var1";
			Assert::AreEqual(expected, result);
		}

		TEST_METHOD(testInvalidInput)
		{
			bool result = ReadParser::verifyReadStmt("read 2day");
			Assert::IsTrue(result);
		}

		TEST_METHOD(testVerifyVarcName)
		{
			bool result = ReadParser::verifyReadStmt("read ComputeWatever");
			Assert::AreEqual(result, true);
		}

		TEST_METHOD(testVerifyInvalidStmt)
		{
			bool result = ReadParser::verifyReadStmt("read compute30Watever");
			Assert::AreEqual(result, true);
		}
	};
}