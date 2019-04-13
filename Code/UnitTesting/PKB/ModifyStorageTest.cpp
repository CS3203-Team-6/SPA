#include "stdafx.h"
#include "CppUnitTest.h"
#include "ModifyStorageStub.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(ModifyStorageTest)
	{
		TEST_METHOD_INITIALIZE(ModifyStorage_Initializing)
		{
			ModifyStorage store;
			store.clear();
			store.addModifiesStm(1, "apple");
			store.addModifiesStm(2, "pineapple");
			store.addModifiesStm(2, "pear");
			store.addModifiesStm(4, "apple");
			store.addModifiesStm(5, "durian");
			store.addModifiesStm(5, "figs");
			store.addModifiesStm(5, "pear");
			store.addModifiesStm(6, "durian");
			store.addModifiesStm(7, "durian");
			store.addModifiesStm(8, "figs");
			store.addModifiesStm(9, "pear");
			store.addModifiesStm(10, "pineapple");
			store.addModifiesStm(11, "pear");
			store.addModifiesStm(12, "pear");
			store.addModifiesStm(14, "pear");
			store.addModifiesStm(15, "pear");
			store.addModifiesProc("fruit", "apple");
			store.addModifiesProc("fruit", "pineapple");
			store.addModifiesProc("fruit", "durian");
			store.addModifiesProc("fruit", "figs");
			store.addModifiesProc("fruit", "pear");
			store.addModifiesProc("fruit2", "pineapple");
			store.addModifiesProc("fruit2", "pear");
			store.addModifiesProc("fruit3", "pear");
			store.addModifiesProc("fruit4", "pear");
		}
		TEST_METHOD(addModifiesStm_Duplicate_Fail)
		{
			Assert::IsFalse(ModifyStorage().addModifiesStm(1, "apple"));
		}

		TEST_METHOD(addModifiesProc_Duplicate_Fail)
		{
			Assert::IsFalse(ModifyStorage().addModifiesProc("fruit", "apple"));
		}

		TEST_METHOD(containsModifyStmPair_PairExist_Success)
		{
			Assert::IsTrue(ModifyStorage().
						   containsStmVarPair(std::pair<int, std::string>(1, "apple")));
		}

		TEST_METHOD(containsModifyProcPair_PairExist_Success)
		{
			Assert::IsTrue(ModifyStorage().
						   containsProcVarPair(std::pair<std::string, std::string>("fruit", "apple")));
		}

		TEST_METHOD(getVarModifiedByStm_Correct)
		{
			Assert::IsTrue(ModifyStorage().getVarModifiedByStm(5) == stmToVarMapModifies.at(5));
		}

		TEST_METHOD(getVarModifiedByProc_Correct)
		{
			Assert::IsTrue(ModifyStorage().getVarModifiedByProc("fruit") ==
						   procToVarMapModifies.at("fruit"));
		}

		TEST_METHOD(getStmModifying_Correct)
		{
			Assert::IsTrue(ModifyStorage().getStmModifying("apple") == varToStmMapModifies.at("apple"));
		}

		TEST_METHOD(getProcModifying_Correct)
		{
			Assert::IsTrue(ModifyStorage().getProcModifying("pear") == varToProcMapModifies.at("pear"));
		}

		TEST_METHOD(getStmModifying_EmptyString_AllStm)
		{
			Assert::IsTrue(ModifyStorage().getStmModifying("") == varToStmMapModifies.at(""));
		}

		TEST_METHOD(getProcModifying_EmptyString_AllProc)
		{
			Assert::IsTrue(ModifyStorage().getProcModifying("") == varToProcMapModifies.at(""));
		}

		TEST_METHOD(ModifyStmPairs_Correct)
		{
			Assert::IsTrue(ModifyStorage().getStmVarPairs() == ModifyStmVarPairs);
		}

		TEST_METHOD(ModifyProcPairs_Correct)
		{
			Assert::IsTrue(ModifyStorage().getProcVarPairs() == ModifyProcVarPairs);
		}

	};
}