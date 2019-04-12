#include "stdafx.h"
#include "CppUnitTest.h"
#include "Preprocessor.h"
#include "PKB.h"
#include "Parser.h"
#include "Statement.h"
#include "DesignExtractor.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace IntegrationTesting
{
	TEST_CLASS(FrontEndPKBTest)
	{
	public:

		TEST_METHOD(parserPopulatePKBParentRelationTest)
		{
			PKB pkb = PKB();
			Parser parser = Parser();

			//Line 2 read cat;
			Statement readLine1 = Statement("read cat", 3, 2);
			//Line 3 print dog;
			Statement printLine1 = Statement("print dog", 4, 3);
			//Line 4 x = x + 8;
			Statement assignLine1 = Statement("x = x + 8", 1, 4);

			std::vector<Statement> simple;

			simple.push_back(readLine1);
			simple.push_back(printLine1);
			simple.push_back(assignLine1);

			parser.parse(simple, 1, "test");

			int parentTwo = pkb.getParent(2);
			int parentThree = pkb.getParent(3);
			int parentFour = pkb.getParent(4);

			Assert::AreEqual(1, parentTwo);
			Assert::AreEqual(1, parentThree);
			Assert::AreEqual(1, parentFour);
		}

		TEST_METHOD(parserPopulatePKBNextRelationTest) {
			PKB pkb = PKB();
			pkb.erase();
			Parser parser = Parser();
			DesignExtractor de = DesignExtractor();
			std::vector<Statement>simple;

			//Line 1 assign
			Statement assignLine1 = Statement("x = x + 8", 1, 2);
			//Start of Line 2 while loop
			std::vector<Statement> whileStmt;
			Statement readLine1 = Statement("read cat", 3, 4);
			Statement printLine1 = Statement("print dog", 4, 5);
			whileStmt.push_back(readLine1);
			whileStmt.push_back(printLine1);
			//Start of Line 5 If else
			std::vector<Statement> ifStmt;
			Statement readLine2 = Statement("read horse", 3, 7);
			ifStmt.push_back(readLine2);
			std::vector<Statement> elseStmt;
			Statement printLine2 = Statement("print mouse", 4, 8);
			elseStmt.push_back(printLine2);
			Statement elseLine = Statement("else", elseStmt, 7, 6);
			Statement ifLine = Statement("if(t==t+2)", ifStmt, 6, 6);
			whileStmt.push_back(ifLine);
			whileStmt.push_back(elseLine);
			Statement whileLine = Statement("while (y!=3)", whileStmt, 5, 3);
			//Last line read
			Statement readLine3 = Statement("read dolphine", 3, 9);
			simple.push_back(assignLine1);
			simple.push_back(whileLine);
			simple.push_back(readLine3);
			pkb.addStatement(6, ifStm, "simple");
			parser.parse(simple, 1, "test2");
			de.extractDesigns();

			std::unordered_set<int>nextOfTwo;
			nextOfTwo.insert(3);
			std::unordered_set<int>nextOfThree;
			nextOfThree.insert(4);
			nextOfThree.insert(9);
			std::unordered_set<int>nextOfFive;
			nextOfFive.insert(6);
			std::unordered_set<int>nextOfSix;
			nextOfSix.insert(7);
			nextOfSix.insert(8);
			std::unordered_set<int>nextOfSeven;
			nextOfSeven.insert(3);
			std::unordered_set<int>nextOfNine;
			bool result1;
			bool result2;
			bool result3;
			bool result4;
			bool result5;
			bool result6;
			if (nextOfTwo == pkb.getNext(2)) {
				result1 = true;
			}
			else {
				result1 = false;
			}
			if (nextOfThree == pkb.getNext(3)) {
				result2 = true;
			}
			else {
				result2 = false;
			}
			if (nextOfFive == pkb.getNext(5)) {
				result3 = true;
			}
			else {
				result3 = false;
			}
			if (nextOfSix == pkb.getNext(6)) {
				result4 = true;
			}
			else {
				result4 = false;
			}
			if (nextOfSeven == pkb.getNext(7)) {
				result5 = true;
			}
			else {
				result5 = false;
			}
			if (nextOfNine == pkb.getNext(9)) {
				result6 = true;
			}
			else {
				result6 = false;
			}
			Assert::AreEqual(true, result1);
			Assert::AreEqual(true, result2);
			Assert::AreEqual(true, result3);
			Assert::AreEqual(true, result4);
			Assert::AreEqual(true, result5);
			Assert::AreEqual(true, result6);
		}
	};
}