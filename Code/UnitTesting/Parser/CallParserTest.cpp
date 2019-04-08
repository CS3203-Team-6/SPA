#include "stdafx.h"
#include "CppUnitTest.h"
#include "CallParser.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

std::string invalidProcedureCall() {
		std::string testStatement = "call 1234123";

		CallParser cp;

		return cp.parseCallStmt(testStatement);
}

namespace UnitTesting
{
	TEST_CLASS(CallParserTest)
	{
	public:

		TEST_METHOD(testCallValidProcedure)
		{
			std::string testStatement = "call Procedure";
			std::string parseExpected = "Procedure";
			CallParser cp;

			std::string output = cp.parseCallStmt(testStatement);

			Assert::AreEqual(parseExpected, output);
		}

		TEST_METHOD(testCallValidProcedureWithNumbers)
		{
			std::string testStatement = "call asd123asd";
			std::string parseExpected = "asd123asd";
			CallParser cp;

			std::string output = cp.parseCallStmt(testStatement);

			Assert::AreEqual(parseExpected, output);
		}


		TEST_METHOD(testCallValidProcedureWithWhitespaces)
		{
			std::string testStatement = " call                proc1     ";
			std::string parseExpected = "proc1";
			CallParser cp;

			std::string output = cp.parseCallStmt(testStatement);

			Assert::AreEqual(parseExpected, output);
		}
	
		TEST_METHOD(testInvalidCallProcedure) {

			Assert::ExpectException<std::string>(invalidProcedureCall);
		}
	};
}