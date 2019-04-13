#include "stdafx.h"
#include "CppUnitTest.h"
#include "PKBStub.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(PKBTest)
	{
		TEST_METHOD_INITIALIZE(PKB_Initialize)
		{
			PKB store;
			store.clear();

			store.addProc("fruit");
			store.addProc("fruit2");
			store.addProc("fruit3");
			store.addProc("fruit4");

			store.addStatement(1, read, "fruit");
			store.addStatement(2, call, "fruit");
			store.addStatement(3, print, "fruit");
			store.addStatement(4, assign, "fruit");
			store.addStatement(5, ifStm, "fruit");
			store.addStatement(6, whileStm, "fruit");
			store.addStatement(7, assign, "fruit");
			store.addStatement(8, assign, "fruit");
			store.addStatement(9, call, "fruit");
			store.addStatement(10, assign, "fruit2");
			store.addStatement(11, whileStm, "fruit2");
			store.addStatement(12, call, "fruit2");
			store.addStatement(13, print, "fruit3");
			store.addStatement(14, call, "fruit3");
			store.addStatement(15, assign, "fruit4");

			store.addUsesStm(3, "banana");
			store.addUsesStm(13, "melon");
			store.addModifiesStm(1, "apple");

			store.addAssignPattern(4, "apple", " + cherry * 10 banana ");
			store.addAssignPattern(7, "durian", " + grapes banana ");
			store.addAssignPattern(8, "figs", " * apple / melon cherry ");
			store.addAssignPattern(10, "pineapple", " + apple melon ");
			store.addAssignPattern(15, "pear", " + jackfruit apple ");

			store.addVariable("apple");
			store.addVariable("banana");
			store.addVariable("cherry");
			store.addVariable("durian");
			store.addVariable("durian");
			store.addVariable("grapes");
			store.addVariable("figs");
			store.addVariable("melon");
			store.addVariable("pineapple");
			store.addVariable("pear");
			store.addVariable("jackfruit");

			store.addConstant("10");
			store.addConstant("0");

			store.addIfContainerStatement(5, 6);
			store.addIfContainerStatement(5, 7);

			store.addElseContainerStatement(5, 8);
			store.addElseContainerStatement(5, 9);

			store.addWhileContainerStatement(6, 7);
			store.addWhileContainerStatement(11, 12);
		}

		TEST_METHOD(getProcList_CorrectProc)
		{
			Assert::IsTrue(PKB().getProcList() == procList);
		}

		TEST_METHOD(getStmList_CorrectStm)
		{
			Assert::IsTrue(PKB().getStmList("fruit4") == procStmMap.at("fruit4"));
		}

		TEST_METHOD(getStmList_NonExistingProc_ReturnEmptySet)
		{
			Assert::IsTrue(PKB().getStmList("nonExistant") == std::vector<int>{});
		}

		TEST_METHOD(getTotalStmNo_CorrectValue)
		{
			Assert::AreEqual(PKB().getTotalStmNo(), 15);
		}

		TEST_METHOD(getStmType_CorrectType)
		{
			Assert::IsTrue(PKB().getStmType(5) == ifStm);
		}

		TEST_METHOD(getStmType_NonExistantStm_CorrectType)
		{
			Assert::IsTrue(PKB().getStmType(0) == nonExistant);
		}

		TEST_METHOD(getReadStms_Correct)
		{
			Assert::IsTrue(PKB().getReadStms() == readStmList);
		}

		TEST_METHOD(getPrintStms_Correct)
		{
			Assert::IsTrue(PKB().getPrintStms() == printStmList);
		}

		TEST_METHOD(getAssignStms_Correct)
		{
			Assert::IsTrue(PKB().getAssignStms() == assignStmList);
		}

		TEST_METHOD(getIfStms_Correct)
		{
			Assert::IsTrue(PKB().getIfStms() == ifStmList);
		}

		TEST_METHOD(getWhileStms_Correct)
		{
			Assert::IsTrue(PKB().getWhileStms() == whileStmList);
		}

		TEST_METHOD(getCallStms_Correct)
		{
			Assert::IsTrue(PKB().getCallStms() == callStmList);
		}

		TEST_METHOD(getVariables_Correct)
		{
			Assert::IsTrue(PKB().getVariables() == varList);
		}

		TEST_METHOD(getConstants_Correct)
		{
			Assert::IsTrue(PKB().getConstants() == constList);
		}

		TEST_METHOD(getReadPairs_CorrectPair)
		{
			Assert::IsTrue(PKB().getReadPairs() == readPairList);
		}

		TEST_METHOD(getPrintPairs_CorrectPair)
		{
			Assert::IsTrue(PKB().getPrintPairs() == printPairList);
		}

		TEST_METHOD(getWhileLastStm_CorrectStm)
		{
			Assert::AreEqual(PKB().getWhileLastStm(6), 7);
		}

		TEST_METHOD(getWhileLastStm_NonExistingStm_ReturnZero)
		{
			Assert::AreEqual(PKB().getWhileLastStm(0), 0);
		}

		TEST_METHOD(getIfLastStms_CorrectPair)
		{
			Assert::IsTrue(PKB().getIfLastStms(5) == std::pair<int, int>(7, 9));
		}

		TEST_METHOD(getIfLastStms_NonExistingStm_PairOfZeros)
		{
			Assert::IsTrue(PKB().getIfLastStms(0) == std::pair<int, int>(0, 0));
		}

		TEST_METHOD(getIfStmContainer_Correct)
		{
			Assert::IsTrue(PKB().getIfStmContainer(5) == ifBlockStmLists.at(5));
		}

		TEST_METHOD(getIfStmContainer_NonExistingStm_ReturnEmptySet)
		{
			Assert::IsTrue(PKB().getIfStmContainer(0) == std::vector<int>{});
		}

		TEST_METHOD(getElseStmContainer_Correct)
		{
			Assert::IsTrue(PKB().getElseStmContainer(5) == elseBlockStmLists.at(5));
		}

		TEST_METHOD(getElseStmContainer_NonExistingStm_ReturnEmptySet)
		{
			Assert::IsTrue(PKB().getElseStmContainer(0) == std::vector<int>{});
		}

		TEST_METHOD(getWhileStmContainer_Correct)
		{
			Assert::IsTrue(PKB().getWhileStmContainer(6) == whileBlockStmLists.at(6));
		}

		TEST_METHOD(getWhileStmContainer_NonExistingStm_ReturnEmptySet)
		{
			Assert::IsTrue(PKB().getWhileStmContainer(0) == std::vector<int>{});
		}

		TEST_METHOD(getProcOfStm_CorrectProc)
		{
			Assert::IsTrue(PKB().getProcOfStm(10) == "fruit2");
		}

		TEST_METHOD(getProcOfStm_NonExistingStm_ReturnEmptyString)
		{
			Assert::IsTrue(PKB().getProcOfStm(0) == "");
		}

		TEST_METHOD(findPattern_EmptyVariableString_ReturnEmptySet)
		{
			Assert::IsTrue(PKB().findPattern("", "apple", true) == std::unordered_set<int>{});
		}

		TEST_METHOD(findPattern_EmptyExprAndExclusive_ReturnEmptySet)
		{
			Assert::IsTrue(PKB().findPattern("pear", "", true) == std::unordered_set<int>{});
		}

		TEST_METHOD(findPattern_EmptyExprAndNotExclusive_ReturnAllAssignStmWithVariable)
		{
			Assert::IsTrue(PKB().findPattern("pear", "", false) == std::unordered_set<int>{15});
		}

		TEST_METHOD(findPattern_NoSuchVariable_ReturnEmptySet)
		{
			Assert::IsTrue(PKB().findPattern("nonExistant", "apple", false) ==
						   std::unordered_set<int>{});
		}

		TEST_METHOD(findPattern_NoSuchExpr_ReturnEmptySet)
		{
			Assert::IsTrue(PKB().findPattern("pear", "nonExistant", true) ==
						   std::unordered_set<int>{});
		}

		TEST_METHOD(findPattern_findExactMatch_CorrectResult)
		{
			Assert::IsTrue(PKB().findPattern("pear", " + jackfruit apple ", true) ==
						   std::unordered_set<int>{15});
		}

		TEST_METHOD(findPattern_findMatchinSubstring_CorrectResult)
		{
			Assert::IsTrue(PKB().findPattern("apple", " * 10 banana ", false) ==
						   std::unordered_set<int>{4});
		}

		TEST_METHOD(findPatternWithoutVariable_EmptyExprAndExclusive_ReturnEmptySet)
		{
			Assert::IsTrue(PKB().findPattern("", true) == std::unordered_set<int>{});
		}

		TEST_METHOD(findPatternWithoutVariable_EmptyExprAndNotExclusive_ReturnAllAssignStm)
		{
			Assert::IsTrue(PKB().findPattern("", false) == assignStmList);
		}

		TEST_METHOD(findPatternWithoutVariable_NoSuchExpr_ReturnEmptySet)
		{
			Assert::IsTrue(PKB().findPattern("nonExistant", true) == std::unordered_set<int>{});
		}

		TEST_METHOD(findPatternWithoutVariable_findExactMatch_CorrectResult)
		{
			Assert::IsTrue(PKB().findPattern(" + jackfruit apple ", true) ==
						   std::unordered_set<int>{15});
		}

		TEST_METHOD(findPatternWithoutVariable_findMatchinSubstring_CorrectResult)
		{
			Assert::IsTrue(PKB().findPattern(" * 10 banana ", false) ==
						   std::unordered_set<int>{4});
		}

	};
}

