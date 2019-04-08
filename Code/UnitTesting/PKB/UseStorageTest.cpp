#include "stdafx.h"
#include "CppUnitTest.h"
#include "UseStorageStub.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(UseStorageTest)
	{
		TEST_METHOD_CLEANUP(UseStorage_Cleanup)
		{
			UseStorage store;
			store.erase();
		}
		TEST_METHOD(addUsesStm_Duplicate_Fail)
		{
			UseStorage store;
			store.addUsesStm(9, "apple");
			Assert::IsFalse(store.addUsesStm(9, "apple"));
		}

		TEST_METHOD(addUsesProc_Duplicate_Fail)
		{
			UseStorage store;
			store.addUsesProc("fruit2", "apple");
			Assert::IsFalse(store.addUsesProc("fruit2", "apple"));
		}

		TEST_METHOD(containsUseStmPair_PairExist_Success)
		{
			UseStorage store;
			store.addUsesStm(9, "apple");
			Assert::IsTrue(store.containsStmVarPair(std::pair<int, std::string>(9, "apple")));
		}

		TEST_METHOD(containsUseProcPair_PairExist_Success)
		{
			UseStorage store;
			store.addUsesProc("fruit2", "apple");
			Assert::IsTrue(store.containsProcVarPair(
				std::pair<std::string, std::string>("fruit2", "apple")));
		}

		TEST_METHOD(getVarUsedByStm_NormalOperation_Success)
		{
			UseStorage store;
			store.addUsesStm(9, "apple");
			store.addUsesStm(9, "melon");
			Assert::IsTrue(store.getVarUsedByStm(9) == stmVarList[0]);
		}

		TEST_METHOD(getVarUsedByProc_NormalOperation_Success)
		{
			UseStorage store;
			store.addUsesProc("fruit2", "apple");
			store.addUsesProc("fruit2", "melon");
			store.addUsesProc("fruit2", "banana");
			Assert::IsTrue(store.getVarUsedByProc("fruit2") == procVarList[0]);
		}

		TEST_METHOD(getStmUsing_NormalOperation_Success)
		{
			UseStorage store;
			store.addUsesStm(9, "apple");
			Assert::IsTrue(store.getStmUsing("apple") == stmList[0]);
		}

		TEST_METHOD(getProcUsing_NormalOperation_Success)
		{
			UseStorage store;
			store.addUsesProc("fruit2", "melon");
			store.addUsesProc("fruit3", "melon");
			Assert::IsTrue(store.getProcUsing("melon") == procList[1]);
		}

		TEST_METHOD(getStmUsing_EmptyString_AllStm)
		{
			UseStorage store;
			store.addUsesStm(9, "apple");
			store.addUsesStm(9, "melon");
			store.addUsesStm(10, "banana");
			store.addUsesStm(12, "melon");
			store.addUsesStm(11, "melon");
			store.addUsesStm(10, "melon");
			Assert::IsTrue(store.getStmUsing("") == stmList[3]);
		}

		TEST_METHOD(getProcUsing_EmptyString_AllProc)
		{
			UseStorage store;
			store.addUsesProc("fruit2", "apple");
			store.addUsesProc("fruit2", "melon");
			store.addUsesProc("fruit2", "banana");
			store.addUsesProc("fruit3", "melon");
			store.addUsesProc("fruit2", "melon");
			Assert::IsTrue(store.getProcUsing("") == procList[3]);
		}

		TEST_METHOD(getUseStmPairs_NormalOperation_Success)
		{
			UseStorage store;
			store.addUsesStm(9, "apple");
			store.addUsesStm(9, "melon");
			store.addUsesStm(10, "banana");
			store.addUsesStm(12, "melon");
			store.addUsesStm(11, "melon");
			store.addUsesStm(10, "melon");
			Assert::IsTrue(store.getStmVarPairs() == stmVarPairs);
		}

		TEST_METHOD(getUseProcPairs_NormalOperation_Success)
		{
			UseStorage store;
			store.addUsesProc("fruit2", "apple");
			store.addUsesProc("fruit2", "melon");
			store.addUsesProc("fruit2", "banana");
			store.addUsesProc("fruit3", "melon");
			store.addUsesProc("fruit2", "melon");
			Assert::IsTrue(store.getProcVarPairs() == procVarPairs);
		}

	};
}