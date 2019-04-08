#include "stdafx.h"
#include "CppUnitTest.h"
#include "FollowStorageStub.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(FollowStorageTest)
	{
		TEST_METHOD_CLEANUP(FollowStorage_Cleanup)
		{
			FollowStorage store;
			store.erase();
		}
		
		TEST_METHOD(addFollow_NormalValues_Success)
		{
			FollowStorage store;
			Assert::IsTrue(store.addFollow(1, 2));
		}

		TEST_METHOD(addFollow_Duplicate_Fail)
		{
			FollowStorage store;
			store.addFollow(1, 2);
			Assert::IsFalse(store.addFollow(1, 2));
		}

		TEST_METHOD(isEmpty_AddedElements_False)
		{
			FollowStorage store;
			store.addFollow(2, 3);
			Assert::IsFalse(store.isEmpty());
		}
		/*
		TEST_METHOD(isEmpty_NoElements_True)
		{
			FollowStorage store;
			Assert::IsTrue(store.isEmpty());
		}
		*/
		TEST_METHOD(getFollower_ExistingStm_Success)
		{
			FollowStorage store;
			store.addFollow(1, 2);
			Assert::AreEqual(store.getFollower(1), 2);
		}

		TEST_METHOD(getFollower_NonExistingStm_ReturnZero)
		{
			FollowStorage store;
			Assert::AreEqual(store.getFollower(8), 0);
		}

		TEST_METHOD(getStmFollowedBy_ExistingStm_Success)
		{
			FollowStorage store;
			store.addFollow(1, 2);
			Assert::AreEqual(store.getStmFollowedBy(2), 1);
		}

		TEST_METHOD(getStmFollowedBy_NonExistingStm_ReturnZero)
		{
			FollowStorage store;
			Assert::AreEqual(store.getStmFollowedBy(8), 0);
		}

		TEST_METHOD(getAllFollowers_NormalOperation_Success)
		{
			FollowStorage store;
			store.addFollow(1, 2);
			store.addFollow(2, 3);
			store.addFollow(3, 4);
			store.addFollow(4, 5);
			store.addFollow(9, 10);
			Assert::IsTrue(store.getAllFollowers() == followers);
		}

		TEST_METHOD(getAllFollowed_NormalOperation_Success)
		{
			FollowStorage store;
			store.addFollow(1, 2);
			store.addFollow(2, 3);
			store.addFollow(3, 4);
			store.addFollow(4, 5);
			store.addFollow(9, 10);
			Assert::IsTrue(store.getAllFollowed() == followed);
		}

		TEST_METHOD(getFollowPairs_NormalOperation_Success)
		{
			FollowStorage store;
			store.addFollow(1, 2);
			store.addFollow(2, 3);
			store.addFollow(3, 4);
			store.addFollow(4, 5);
			store.addFollow(9, 10);
			Assert::IsTrue(store.getFollowPairs() == followPairs);
		}

		TEST_METHOD(hasFollowStarPair_ContainsPair_True)
		{
			FollowStorage store;
			store.addFollow(1, 2);
			store.setAllFollowing(1, allFollowing[0]);
			Assert::IsTrue(store.hasFollowStarPair({1, 2}));
		}

		TEST_METHOD(hasFollowStarPair_NonExistingPair_False)
		{
			FollowStorage store;
			store.addFollow(1, 2);
			store.setAllFollowing(1, allFollowing[0]);
			Assert::IsFalse(store.hasFollowStarPair({ 4, 8 }));
		}

		TEST_METHOD(getAllFollowing_NormalOperation_Success)
		{
			FollowStorage store;
			store.addFollow(1, 2);
			store.setAllFollowing(1, allFollowing[0]);
			Assert::IsTrue(store.getAllFollowing(1) == allFollowing[0]);
		}

		TEST_METHOD(getAllFollowedBy_NormalOperation_Success)
		{
			FollowStorage store;
			store.addFollow(4, 5);
			store.setAllFollowedBy(5, allFollowedBy[3]);
			Assert::IsTrue(store.getAllFollowedBy(5) == allFollowedBy[3]);
		}

		TEST_METHOD(getFollowStarPairs_NormalOperation_Success)
		{
			FollowStorage store;
			store.addFollow(1, 2);
			store.addFollow(2, 3);
			store.addFollow(3, 4);
			store.addFollow(4, 5);
			store.addFollow(9, 10);
			store.setAllFollowing(1, allFollowing[0]);
			store.setAllFollowing(2, allFollowing[1]);
			store.setAllFollowing(3, allFollowing[2]);
			store.setAllFollowing(4, allFollowing[3]);
			store.setAllFollowing(9, allFollowing[4]);
			store.setAllFollowedBy(2, allFollowedBy[0]);
			store.setAllFollowedBy(3, allFollowedBy[1]);
			store.setAllFollowedBy(4, allFollowedBy[2]);
			store.setAllFollowedBy(5, allFollowedBy[3]);
			store.setAllFollowedBy(10, allFollowedBy[4]);
			Assert::IsTrue(store.getFollowStarPairs() == followStarPairs);
		}
	};
}