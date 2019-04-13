#include "stdafx.h"
#include "CppUnitTest.h"
#include "UseStorageStub.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(UseStorageTest)
	{
		TEST_METHOD_INITIALIZE(UseStorage_Initializing)
		{
			UseStorage store;
			store.clear();
			store.addUsesStm(2, "apple");
			store.addUsesStm(2, "melon");
			store.addUsesStm(2, "banana");
			store.addUsesStm(2, "jackfruit");
			store.addUsesStm(3, "banana");
			store.addUsesStm(4, "banana");
			store.addUsesStm(4, "cherry");
			store.addUsesStm(5, "apple");
			store.addUsesStm(5, "durian");
			store.addUsesStm(5, "cherry");
			store.addUsesStm(5, "grapes");
			store.addUsesStm(5, "banana");
			store.addUsesStm(5, "melon");
			store.addUsesStm(5, "jackfruit");
			store.addUsesStm(6, "durian");
			store.addUsesStm(6, "cherry");
			store.addUsesStm(6, "grapes");
			store.addUsesStm(6, "banana");
			store.addUsesStm(7, "grapes");
			store.addUsesStm(7, "banana");
			store.addUsesStm(8, "apple");
			store.addUsesStm(8, "melon");
			store.addUsesStm(8, "cherry");
			store.addUsesStm(9, "apple");
			store.addUsesStm(9, "melon");
			store.addUsesStm(9, "jackfruit");
			store.addUsesStm(10, "apple");
			store.addUsesStm(10, "melon");
			store.addUsesStm(11, "banana");
			store.addUsesStm(11, "melon");
			store.addUsesStm(11, "apple");
			store.addUsesStm(11, "jackfruit");
			store.addUsesStm(12, "melon");
			store.addUsesStm(12, "apple");
			store.addUsesStm(12, "jackfruit");
			store.addUsesStm(13, "melon");
			store.addUsesStm(14, "jackfruit");
			store.addUsesStm(14, "apple");
			store.addUsesStm(15, "jackfruit");
			store.addUsesStm(15, "apple");

			store.addUsesProc("fruit", "apple");
			store.addUsesProc("fruit", "banana");
			store.addUsesProc("fruit", "cherry");
			store.addUsesProc("fruit", "durian");
			store.addUsesProc("fruit", "grapes");
			store.addUsesProc("fruit", "melon");
			store.addUsesProc("fruit", "jackfruit");
			store.addUsesProc("fruit2", "apple");
			store.addUsesProc("fruit2", "melon");
			store.addUsesProc("fruit2", "banana");
			store.addUsesProc("fruit2", "jackfruit");
			store.addUsesProc("fruit3", "melon");
			store.addUsesProc("fruit3", "jackfruit");
			store.addUsesProc("fruit3", "apple");
			store.addUsesProc("fruit4", "jackfruit");
			store.addUsesProc("fruit4", "apple");
		}

		TEST_METHOD(addUsesStm_Duplicate_Fail)
		{
			Assert::IsFalse(UseStorage().addUsesStm(9, "apple"));
		}

		TEST_METHOD(addUsesProc_Duplicate_Fail)
		{
			Assert::IsFalse(UseStorage().addUsesProc("fruit2", "apple"));
		}

		TEST_METHOD(containsUseStmPair_PairExist_True)
		{
			Assert::IsTrue(UseStorage().containsStmVarPair(std::pair<int, std::string>(9, "apple")));
		}

		TEST_METHOD(containsUseProcPair_PairExist_True)
		{
			Assert::IsTrue(UseStorage().containsProcVarPair(
				std::pair<std::string, std::string>("fruit2", "apple")));
		}

		TEST_METHOD(containsUseStmPair_PairDoNotExist_False)
		{
			Assert::IsFalse(UseStorage().containsStmVarPair(std::pair<int, std::string>(15, "cherry")));
		}

		TEST_METHOD(containsUseProcPair_PairDoNotExist_False)
		{
			Assert::IsFalse(UseStorage().containsProcVarPair(
				std::pair<std::string, std::string>("fruit4", "durian")));
		}

		TEST_METHOD(getVarUsedByStm_Correct)
		{
			Assert::IsTrue(UseStorage().getVarUsedByStm(5) == stmToVarMapUses.at(5));
		}

		TEST_METHOD(getVarUsedByProc_Correct)
		{
			Assert::IsTrue(UseStorage().getVarUsedByProc("fruit") == procToVarMapUses.at("fruit"));
		}

		TEST_METHOD(getStmUsing_Correct)
		{
			Assert::IsTrue(UseStorage().getStmUsing("apple") == varToStmMapUses.at("apple"));
		}

		TEST_METHOD(getProcUsing_Correct)
		{
			Assert::IsTrue(UseStorage().getProcUsing("melon") == varToProcMapUses.at("melon"));
		}

		TEST_METHOD(getStmUsing_EmptyString_AllStm)
		{
			Assert::IsTrue(UseStorage().getStmUsing("") == varToStmMapUses.at(""));
		}

		TEST_METHOD(getProcUsing_EmptyString_AllProc)
		{
			Assert::IsTrue(UseStorage().getProcUsing("") == varToProcMapUses.at(""));
		}

		TEST_METHOD(getUseStmPairs_Correct)
		{
			Assert::IsTrue(UseStorage().getStmVarPairs() == stmVarPairs);
		}

		TEST_METHOD(getUseProcPairs_Correct)
		{
			Assert::IsTrue(UseStorage().getProcVarPairs() == procVarPairs);
		}

	};
}