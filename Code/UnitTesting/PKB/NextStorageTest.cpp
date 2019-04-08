#include "stdafx.h"
#include "CppUnitTest.h"
#include "NextStorageStub.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(NextStorageTest)
	{
		TEST_CLASS_INITIALIZE(NextStorage_Initialize)
		{
			NextStorage store;
			store.addNext(1, 2);
			store.addNext(2, 3);
			store.addNext(3, 4);
			store.addNext(4, 5);
			store.addNext(5, 6);
			store.addNext(6, 7);
			store.addNext(7, 6);
			store.addNext(5, 8);
			store.addNext(8, 9);
			store.addNext(10, 11);
			store.addNext(11, 12);
			store.addNext(12, 11);
			store.addNext(13, 14);
		}

		TEST_CLASS_CLEANUP(NextStorage_CleanUp)
		{
			NextStorage().erase();
		}
		TEST_METHOD(isEmpty_NextStorage_False)
		{
			NextStorage store;
			Assert::IsFalse(store.isEmpty());
		}

		TEST_METHOD(getNext_MultipleNext_ReturnSet)
		{
			NextStorage store;
			Assert::IsTrue(store.getNext(5) == next[4]);
		}

		TEST_METHOD(getNext_NonExistingStm_ReturnEmptySet)
		{
			NextStorage store;
			Assert::IsTrue(store.getNext(0) == std::unordered_set<int>());
		}

		TEST_METHOD(getPrev_MultiplePrev_ReturnSet)
		{
			NextStorage store;
			Assert::IsTrue(store.getPrev(6) == previous[4]);
		}

		TEST_METHOD(getPrev_NoPrev_ReturnEmptySet)
		{
			NextStorage store;
			Assert::IsTrue(store.getPrev(13) == std::unordered_set<int>());
		}

		TEST_METHOD(getAllNext_ReturnSet)
		{
			NextStorage store;
			Assert::IsTrue(store.getAllNext() == nextList);
		}

		TEST_METHOD(getAllPrev_ReturnSet)
		{
			NextStorage store;
			Assert::IsTrue(store.getAllPrev() == prevList);
		}

		TEST_METHOD(getNextPairs_ReturnSet)
		{
			NextStorage store;
			Assert::IsTrue(store.getNextPairs() == nextPairList);
		}

	};
}