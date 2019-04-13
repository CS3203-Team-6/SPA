#include "stdafx.h"
#include "CppUnitTest.h"
#include "FollowStorageStub.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(FollowStorageTest)
	{
		TEST_METHOD_INITIALIZE(FollowStorage_Initializing)
		{
			FollowStorage store;
			store.clear();
			store.addFollow(1, 2);
			store.addFollow(2, 3);
			store.addFollow(3, 4);
			store.addFollow(4, 5);
			store.addFollow(8, 9);
			store.addFollow(10, 11);
			store.addFollow(13, 14);
			store.setAllFollowing(1, { 2, 3, 4, 5 });
			store.setAllFollowing(2, { 3, 4, 5 });
			store.setAllFollowing(3, { 4, 5 });
			store.setAllFollowing(4, { 5 });
			store.setAllFollowing(8, { 9 });
			store.setAllFollowing(10, { 11 });
			store.setAllFollowing(13, { 14 });
			store.setAllFollowedBy(2, { 1 });
			store.setAllFollowedBy(3, { 1, 2 });
			store.setAllFollowedBy(4, { 1, 2, 3 });
			store.setAllFollowedBy(5, { 1, 2, 3, 4 });
			store.setAllFollowedBy(9, { 8 });
			store.setAllFollowedBy(11, { 10 });
			store.setAllFollowedBy(14, { 13 });
		}
		
		TEST_METHOD(addFollow_Duplicate_Fail)
		{
			Assert::IsFalse(FollowStorage().addFollow(1, 2));
		}

		TEST_METHOD(isEmpty_False)
		{
			Assert::IsFalse(FollowStorage().isEmpty());
		}

		TEST_METHOD(getFollower_Correct)
		{
			Assert::IsTrue(FollowStorage().getFollower(1) == followTable.at(1).follower);
		}

		TEST_METHOD(getFollower_NonExistingStm_ReturnZero)
		{
			Assert::AreEqual(FollowStorage().getFollower(0), 0);
		}

		TEST_METHOD(getStmFollowedBy_Correct)
		{
			Assert::IsTrue(FollowStorage().getStmFollowedBy(2) == followTable.at(2).followed);
		}

		TEST_METHOD(getStmFollowedBy_NonExistingStm_ReturnZero)
		{
			Assert::AreEqual(FollowStorage().getStmFollowedBy(0), 0);
		}

		TEST_METHOD(getAllFollowers_Correct)
		{
			Assert::IsTrue(FollowStorage().getAllFollowers() == followers);
		}

		TEST_METHOD(getAllFollowed_Correct)
		{
			Assert::IsTrue(FollowStorage().getAllFollowed() == followed);
		}

		TEST_METHOD(getFollowPairs_Correct)
		{
			Assert::IsTrue(FollowStorage().getFollowPairs() == followPairs);
		}

		TEST_METHOD(hasFollowStarPair_ContainsPair_True)
		{
			Assert::IsTrue(FollowStorage().hasFollowStarPair({1, 2}));
		}

		TEST_METHOD(hasFollowStarPair_NonExistingPair_False)
		{
			Assert::IsFalse(FollowStorage().hasFollowStarPair({ 4, 8 }));
		}

		TEST_METHOD(getAllFollowing_Correct)
		{
			Assert::IsTrue(FollowStorage().getAllFollowing(1) == followTable.at(1).allFollowers);
		}

		TEST_METHOD(getAllFollowedBy_Correct)
		{
			Assert::IsTrue(FollowStorage().getAllFollowedBy(5) == followTable.at(5).allFollowed);
		}

		TEST_METHOD(getFollowStarPairs_Correct)
		{
			Assert::IsTrue(FollowStorage().getFollowStarPairs() == followStarPairs);
		}
	};
}