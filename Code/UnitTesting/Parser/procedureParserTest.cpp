#include "stdafx.h"
#include "CppUnitTest.h"
#include "ProcedureParser.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(procedureParserTest)
	{

		TEST_METHOD(testValidInput)
		{
			std::string result = ProcedureParser::parseProcName("procedure Main");
			std::string expected = "Main";
			Assert::AreEqual(expected, result);
		}

		TEST_METHOD(testInvalidInput)
		{
			bool result = ProcedureParser::verifyProcName("procedure 2day");
			Assert::IsTrue(result);
		}

		TEST_METHOD(testVerifyProcName)
		{
			bool result = ProcedureParser::verifyProcName("procedure ComputeWatever");
			Assert::AreEqual(result, true);
		}

		TEST_METHOD(testVerifyProcInvalidName)
		{
			bool result = ProcedureParser::verifyProcName("procedure Com@puteWatever");
			Assert::AreEqual(result, false);
		}
	};
}