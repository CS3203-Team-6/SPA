#include "stdafx.h"
#include "CppUnitTest.h"
#include "ModifyStorageStub.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(ModifyStorageTest)
	{
		TEST_METHOD_CLEANUP(ModifyStorage_Cleanup)
		{
			ModifyStorage store;
			store.erase();
		}
		TEST_METHOD(addModifiesStm_Duplicate_Fail)
		{
			ModifyStorage store;
			store.addModifiesStm(1, "apple");
			Assert::IsFalse(store.addModifiesStm(1, "apple"));
		}

		TEST_METHOD(addModifiesProc_Duplicate_Fail)
		{
			ModifyStorage store;
			store.addModifiesProc("fruit", "apple");
			Assert::IsFalse(store.addModifiesProc("fruit", "apple"));
		}

		TEST_METHOD(containsModifyStmPair_PairExist_Success)
		{
			ModifyStorage store;
			store.addModifiesStm(1, "apple");
			Assert::IsTrue(store.containsStmVarPair(pair<int, string>(1, "apple")));
		}

		TEST_METHOD(containsModifyProcPair_PairExist_Success)
		{
			ModifyStorage store;
			store.addModifiesProc("fruit", "apple");
			Assert::IsTrue(store.containsProcVarPair(pair<string, string>("fruit", "apple")));
		}

		TEST_METHOD(getVarModifiedByStm_NormalOperation_Success)
		{
			ModifyStorage store;
			store.addModifiesStm(5, "durian");
			store.addModifiesStm(5, "figs");
			Assert::IsTrue(store.getVarModifiedByStm(5) == ModifyStmVarList[3]);
		}

		TEST_METHOD(getVarModifiedByProc_NormalOperation_Success)
		{
			ModifyStorage store;
			store.addModifiesProc("fruit", "apple");
			store.addModifiesProc("fruit", "pineapple");
			store.addModifiesProc("fruit", "pineapple");
			store.addModifiesProc("fruit", "durian");
			store.addModifiesProc("fruit", "figs");
			Assert::IsTrue(store.getVarModifiedByProc("fruit") == ModifyProcVarList[0]);
		}

		TEST_METHOD(getStmModifying_NormalOperation_Success)
		{
			ModifyStorage store;
			store.addModifiesStm(1, "apple");
			store.addModifiesStm(4, "apple");
			Assert::IsTrue(store.getStmModifying("apple") == ModifyStmList[0]);
		}

		TEST_METHOD(getProcModifying_NormalOperation_Success)
		{
			ModifyStorage store;
			store.addModifiesProc("fruit2", "pineapple");
			store.addModifiesProc("fruit", "pineapple");
			Assert::IsTrue(store.getProcModifying("pineapple") == ModifyProcList[1]);
		}

		TEST_METHOD(getStmModifying_EmptyString_AllStm)
		{
			ModifyStorage store;
			store.addModifiesStm(1, "apple");
			store.addModifiesStm(2, "pineapple");
			store.addModifiesStm(4, "apple");
			store.addModifiesStm(5, "durian");
			store.addModifiesStm(5, "figs");
			store.addModifiesStm(6, "durian");
			store.addModifiesStm(7, "durian");
			store.addModifiesStm(8, "figs");
			store.addModifiesStm(9, "pineapple");
			Assert::IsTrue(store.getStmModifying("") == ModifyStmList[4]);
		}

		TEST_METHOD(getProcModifying_EmptyString_AllProc)
		{
			ModifyStorage store;
			store.addModifiesProc("fruit", "apple");
			store.addModifiesProc("fruit", "pineapple");
			store.addModifiesProc("fruit", "durian");
			store.addModifiesProc("fruit", "figs");
			store.addModifiesProc("fruit2", "pineapple");
			Assert::IsTrue(store.getProcModifying("") == ModifyProcList[4]);
		}

		TEST_METHOD(ModifyStmPairs_NormalOperation_Success)
		{
			ModifyStorage store;
			store.addModifiesStm(1, "apple");
			store.addModifiesStm(2, "pineapple");
			store.addModifiesStm(4, "apple");
			store.addModifiesStm(5, "durian");
			store.addModifiesStm(5, "figs");
			store.addModifiesStm(6, "durian");
			store.addModifiesStm(7, "durian");
			store.addModifiesStm(8, "figs");
			store.addModifiesStm(9, "pineapple");
			Assert::IsTrue(store.getStmVarPairs() == ModifyStmVarPairs);
		}

		TEST_METHOD(ModifyProcPairs_NormalOperation_Success)
		{
			ModifyStorage store;
			store.addModifiesProc("fruit", "apple");
			store.addModifiesProc("fruit", "pineapple");
			store.addModifiesProc("fruit", "durian");
			store.addModifiesProc("fruit", "figs");
			store.addModifiesProc("fruit2", "pineapple");
			Assert::IsTrue(store.getProcVarPairs() == ModifyProcVarPairs);
		}

	};
}