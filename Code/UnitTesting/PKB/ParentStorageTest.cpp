#include "stdafx.h"
#include "CppUnitTest.h"
#include "ParentStorageStub.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(ParentStorageTest)
	{
		TEST_METHOD_INITIALIZE(ParentStorage_Initializing)
		{
			ParentStorage store;
			store.clear();
			store.addParent_Child(5, 6);
			store.addParent_Child(6, 7);
			store.addParent_Child(5, 8);
			store.addParent_Child(5, 9);
			store.addParent_Child(11, 12);
			store.setAncestors(6, { 5 });
			store.setAncestors(7, { 5, 6 });
			store.setAncestors(8, { 5 });
			store.setAncestors(9, { 5 });
			store.setAncestors(12, { 11 });
			store.setDescendants(5, { 6, 7, 8, 9 });
			store.setDescendants(6, { 7 });
			store.setDescendants(11, { 12 });
		}

		TEST_METHOD(addParent_Duplicate_Fail)
		{
			Assert::IsFalse(ParentStorage().addParent_Child(5, 6));
		}

		TEST_METHOD(isEmpty_False)
		{
			Assert::IsFalse(ParentStorage().isEmpty());
		}

		TEST_METHOD(isParent_ExistingParent_true)
		{
			Assert::IsTrue(ParentStorage().isParent(5));
		}

		TEST_METHOD(isParent_StmIsNotParent_false)
		{
			Assert::IsFalse(ParentStorage().isParent(7));
		}

		TEST_METHOD(isParent_NonExistingStm_false)
		{
			Assert::IsFalse(ParentStorage().isParent(0));
		}

		TEST_METHOD(isChild_ExistingChild_true)
		{
			Assert::IsTrue(ParentStorage().isChild(7));
		}

		TEST_METHOD(isChild_StmIsNotChild_false)
		{
			Assert::IsFalse(ParentStorage().isChild(5));
		}

		TEST_METHOD(isChild_NonExistingStm_false)
		{
			Assert::IsFalse(ParentStorage().isChild(0));
		}

		TEST_METHOD(getParent_Correct)
		{
			Assert::IsTrue(ParentStorage().getParent(6) == parentTable.at(6).parent);
		}
		
		TEST_METHOD(getParent_NonExistingStm_ReturnZero)
		{
			Assert::AreEqual(ParentStorage().getParent(1), 0);
		}
		
		TEST_METHOD(getChildren_Correct)
		{
			Assert::IsTrue(ParentStorage().getChildren(5) == parentTable.at(5).children);
		}
		
		TEST_METHOD(getChildren_NonExistingStm_ReturnEmptySet)
		{
			Assert::IsTrue(ParentStorage().getChildren(1) == std::unordered_set<int>{});
		}
		
		TEST_METHOD(getAllParent_Correct)
		{
			Assert::IsTrue(ParentStorage().getAllParent() == parents);
		}
		
		TEST_METHOD(getAllChildren_Correct)
		{
			Assert::IsTrue(ParentStorage().getAllChildren() == children);
		}
		
		TEST_METHOD(getFollowPairs_Correct)
		{
			Assert::IsTrue(ParentStorage().getParentChildPairs() == parentPair);
		}
		
		TEST_METHOD(hasAncDescPair_ContainsPair_True)
		{
			Assert::IsTrue(ParentStorage().hasAncDescPair({ 5, 6 }));
		}

		TEST_METHOD(hasAncDescPair_NonExistingPair_False)
		{
			Assert::IsFalse(ParentStorage().hasAncDescPair({ 4, 8 }));
		}
		
		TEST_METHOD(getAncestors_Correct)
		{
			Assert::IsTrue(ParentStorage().getAncestors(7) == parentTable.at(7).ancestors);
		}
		
		TEST_METHOD(getDescendants_Correct)
		{
			Assert::IsTrue(ParentStorage().getDescendants(5) == parentTable.at(5).descendants);
		}
		
		TEST_METHOD(getAncDescPair_Correct)
		{
			Assert::IsTrue(ParentStorage().getAncDescPair() == parentStarPair);
		}
		
	};
}