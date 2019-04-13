#include "stdafx.h"
#include "CppUnitTest.h"
#include "CallStorageStub.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(CallStorageTest)
	{
		TEST_METHOD_INITIALIZE(CallStorage_Initialize)
		{
			CallStorage store;
			store.clear();
			store.addCall("fruit", "fruit2", 2);
			store.addCall("fruit", "fruit3", 9);
			store.addCall("fruit2", "fruit3", 12);
			store.addCall("fruit3", "fruit4", 14);
			store.setCallAnc("fruit4", std::unordered_set<std::string>{"fruit", "fruit2", "fruit3"});
			store.setCallAnc("fruit3", std::unordered_set<std::string>{"fruit", "fruit2"});
			store.setCallAnc("fruit2", std::unordered_set<std::string>{"fruit"});
			store.setCallDesc("fruit", std::unordered_set<std::string>{"fruit2", "fruit3", "fruit4"});
			store.setCallDesc("fruit2", std::unordered_set<std::string>{"fruit3", "fruit4"});
			store.setCallDesc("fruit3", std::unordered_set<std::string>{"fruit4"});
		}

		TEST_METHOD(isEmpty_CallStorage_False)
		{
			CallStorage store;
			Assert::IsFalse(store.isEmpty());
		}

		TEST_METHOD(hasCallStarPair_ExistingPair_True)
		{
			CallStorage store;
			Assert::IsTrue(store.hasCallStarPair(std::pair<std::string, std::string>("fruit", "fruit2")));
		}

		TEST_METHOD(hasCallStarPair_NonExistingPair_False)
		{
			CallStorage store;
			Assert::IsFalse(store.hasCallStarPair(std::pair<std::string, std::string>("fruit", "nonExistant")));
		}

		TEST_METHOD(isCaller_ProcIsCaller_True)
		{
			CallStorage store;
			Assert::IsTrue(store.isCaller("fruit"));
		}

		TEST_METHOD(isCaller_ProcIsNotCaller_False)
		{
			CallStorage store;
			Assert::IsFalse(store.isCaller("fruit4"));
		}

		TEST_METHOD(isCallee_ProcIsCallee_True)
		{
			CallStorage store;
			Assert::IsTrue(store.isCallee("fruit2"));
		}

		TEST_METHOD(isCallee_ProcIsNotCallee_False)
		{
			CallStorage store;
			Assert::IsFalse(store.isCallee("fruit"));
		}

		TEST_METHOD(getCaller_MultipleCallers_ReturnSet)
		{
			CallStorage store;
			Assert::IsTrue(store.getCaller("fruit3") == caller[2]);
		}

		TEST_METHOD(getCaller_NonExistingProc_ReturnEmptySet)
		{
			CallStorage store;
			Assert::IsTrue(store.getCaller("nonExistant") == std::unordered_set<std::string>());
		}

		TEST_METHOD(getCallee_MultipleCallees_ReturnSet)
		{
			CallStorage store;
			Assert::IsTrue(store.getCallee("fruit") == callees[0]);
		}

		TEST_METHOD(getCallee_NoCallee_ReturnEmptySet)
		{
			CallStorage store;
			Assert::IsTrue(store.getCallee("fruit4") == std::unordered_set<std::string>());
		}

		TEST_METHOD(getCallAnc_MultipleAnc_ReturnSet)
		{
			CallStorage store;
			Assert::IsTrue(store.getCallAnc("fruit4") == callAnc[3]);
		}

		TEST_METHOD(getCallAnc_NoCallAnc_ReturnEmptySet)
		{
			CallStorage store;
			Assert::IsTrue(store.getCallAnc("fruit") == std::unordered_set<std::string>());
		}

		TEST_METHOD(getCallDesc_MultipleDesc_ReturnSet)
		{
			CallStorage store;
			Assert::IsTrue(store.getCallDesc("fruit") == callDesc[0]);
		}

		TEST_METHOD(getCallDesc_NonExistingProc_ReturnEmptySet)
		{
			CallStorage store;
			Assert::IsTrue(store.getCallDesc("nonExistant") == std::unordered_set<std::string>());
		}

		TEST_METHOD(getAllCallers_ReturnSet)
		{
			CallStorage store;
			Assert::IsTrue(store.getAllCallers() == callerList);
		}

		TEST_METHOD(getAllCallees_ReturnSet)
		{
			CallStorage store;
			Assert::IsTrue(store.getAllCallees() == calleeList);
		}

		TEST_METHOD(getCallPairs_ReturnSet)
		{
			CallStorage store;
			Assert::IsTrue(store.getCallPairs() == callPairList);
		}

		TEST_METHOD(getCallStarPairs_ReturnSet)
		{
			CallStorage store;
			Assert::IsTrue(store.getCallStarPairs() == callStarPairList);
		}

		TEST_METHOD(getProcCalledBy_ExistingStm_ReturnProc)
		{
			CallStorage store;
			Assert::IsTrue(store.getProcCalledBy(2) == procCalled[0]);
		}

		TEST_METHOD(getProcCalledBy_NonExistingStm_ReturnEmptyString)
		{
			CallStorage store;
			Assert::IsTrue(store.getProcCalledBy(0) == "");
		}

		TEST_METHOD(getStmCalling_MultipleStm_ReturnStm)
		{
			CallStorage store;
			Assert::IsTrue(store.getStmCalling("fruit3") == stmCalling[1]);
		}

		TEST_METHOD(getStmCalling_NonExistingProc_ReturnEmptySet)
		{
			CallStorage store;
			Assert::IsTrue(store.getStmCalling("nonExistant") == std::unordered_set<int>());
		}

		TEST_METHOD(getStmProcCallPairs_ReturnSet)
		{
			CallStorage store;
			Assert::IsTrue(store.getStmProcCallPairs() == stmProcCallPairList);
		}

	};
}
