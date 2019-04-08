#include "stdafx.h"
#include "CppUnitTest.h"
#include "ParentStorageStub.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(ParentStorageTest)
	{
		TEST_METHOD_CLEANUP(ParentStorage_Cleanup)
		{
			ParentStorage store;
			store.erase();
		}

		TEST_METHOD(addParent_NormalValues_Success)
		{
			ParentStorage store;
			Assert::IsTrue(store.addParent_Child(5, 6));
		}

		TEST_METHOD(addParent_Duplicate_Fail)
		{
			ParentStorage store;
			store.addParent_Child(5, 6);
			Assert::IsFalse(store.addParent_Child(5, 6));
		}

		TEST_METHOD(isEmpty_AddedParentElements_False)
		{
			ParentStorage store;
			store.addParent_Child(5, 6);
			Assert::IsFalse(store.isEmpty());
		}

		TEST_METHOD(isParent_ExistingParent_true)
		{
			ParentStorage store;
			store.addParent_Child(5, 6);
			Assert::IsTrue(store.isParent(5));
		}

		TEST_METHOD(isParent_StmIsNotParent_false)
		{
			ParentStorage store;
			store.addParent_Child(6, 7);
			Assert::IsFalse(store.isParent(7));
		}

		TEST_METHOD(isParent_NonExistingStm_false)
		{
			ParentStorage store;
			store.addParent_Child(6, 7);
			Assert::IsFalse(store.isParent(2));
		}

		TEST_METHOD(isChild_ExistingChild_true)
		{
			ParentStorage store;
			store.addParent_Child(6, 7);
			Assert::IsTrue(store.isChild(7));
		}

		TEST_METHOD(isChild_StmIsNotChild_false)
		{
			ParentStorage store;
			store.addParent_Child(5, 6);
			Assert::IsFalse(store.isChild(5));
		}

		TEST_METHOD(isChild_NonExistingStm_false)
		{
			ParentStorage store;
			store.addParent_Child(6, 7);
			Assert::IsFalse(store.isChild(2));
		}

		TEST_METHOD(getParent_ExistingStm_Success)
		{
			ParentStorage store;
			store.addParent_Child(5, 6);
			Assert::AreEqual(store.getParent(6), 5);
		}
		
		TEST_METHOD(getParent_NonExistingStm_ReturnZero)
		{
			ParentStorage store;
			Assert::AreEqual(store.getParent(1), 0);
		}
		
		TEST_METHOD(getChildren_ExistingStm_Success)
		{
			ParentStorage store;
			store.addParent_Child(5, 6);
			store.addParent_Child(6, 7);
			store.addParent_Child(5, 8);
			Assert::IsTrue(store.getChildren(5) == std::unordered_set<int>{6, 8});
		}
		
		TEST_METHOD(getChildren_NonExistingStm_ReturnEmptySet)
		{
			ParentStorage store;
			Assert::IsTrue(store.getChildren(1).empty());
		}
		
		TEST_METHOD(getAllParent_NormalOperation_Success)
		{
			ParentStorage store;
			store.addParent_Child(5, 6);
			store.addParent_Child(6, 7);
			store.addParent_Child(5, 8);
			store.addParent_Child(10, 11);
			Assert::IsTrue(store.getAllParent() == parents);
		}
		
		TEST_METHOD(getAllChildren_NormalOperation_Success)
		{
			ParentStorage store;
			store.addParent_Child(5, 6);
			store.addParent_Child(6, 7);
			store.addParent_Child(5, 8);
			store.addParent_Child(10, 11);
			Assert::IsTrue(store.getAllChildren() == children);
		}
		
		TEST_METHOD(getFollowPairs_NormalOperation_Success)
		{
			ParentStorage store;
			store.addParent_Child(5, 6);
			store.addParent_Child(6, 7);
			store.addParent_Child(5, 8);
			store.addParent_Child(10, 11);
			Assert::IsTrue(store.getParentChildPairs() == parentPair);
		}
		
		TEST_METHOD(hasAncDescPair_ContainsPair_True)
		{
			ParentStorage store;
			store.addParent_Child(5, 6);
			store.setDescendants(5, descendants[0]);
			Assert::IsTrue(store.hasAncDescPair({ 5, 6 }));
		}

		TEST_METHOD(hasAncDescPair_NonExistingPair_False)
		{
			ParentStorage store;
			store.addParent_Child(5, 6);
			store.setDescendants(5, { 6 });
			Assert::IsFalse(store.hasAncDescPair({ 4, 8 }));
		}
		
		TEST_METHOD(getAncestors_NormalOperation_Success)
		{
			ParentStorage store;
			store.addParent_Child(5, 6);
			store.addParent_Child(6, 7);
			store.addParent_Child(5, 8);
			store.setAncestors(7, ancestors[1]);
			Assert::IsTrue(store.getAncestors(7) == ancestors[1]);
		}
		
		TEST_METHOD(getDescendants_NormalOperation_Success)
		{
			ParentStorage store;
			store.addParent_Child(5, 6);
			store.setDescendants(5, descendants[0]);
			Assert::IsTrue(store.getDescendants(5) == descendants[0]);
		}
		
		TEST_METHOD(getAncDescPair_NormalOperation_Success)
		{
			ParentStorage store;
			store.addParent_Child(5, 6);
			store.addParent_Child(6, 7);
			store.addParent_Child(5, 8);
			store.addParent_Child(10, 11);
			store.setAncestors(6, ancestors[0]);
			store.setAncestors(7, ancestors[1]);
			store.setAncestors(8, ancestors[2]);
			store.setAncestors(11, ancestors[3]);
			store.setDescendants(5, descendants[0]);
			store.setDescendants(6, descendants[1]);
			store.setDescendants(10, descendants[2]);
			Assert::IsTrue(store.getAncDescPair() == parentStarPair);
		}
		
	};
}