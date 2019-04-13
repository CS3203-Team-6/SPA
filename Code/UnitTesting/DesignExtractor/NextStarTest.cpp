#include "stdafx.h"
#include "CppUnitTest.h"
#include "RunTimeDesignExtractor.h"
#include "PKB.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(NextStarTest)
	{
	public:
		TEST_METHOD(testNextStar)
		{
			PKB storage = PKB();
			storage.clear();
			storage.addNext(1, 2);
			storage.addNext(2, 3);
			storage.addNext(3, 4);
			storage.addNext(4, 5);
			storage.addNext(2, 5);
			bool result = storage.hasNextStarPair(1, 4);
			Assert::AreEqual(true, result);
		}
		TEST_METHOD(testAllLnAfter)
		{
			PKB storage = PKB();
			storage.clear();
			storage.addNext(1, 2);
			storage.addNext(2, 3);
			storage.addNext(3, 4);
			storage.addNext(4, 5);
			storage.addNext(2, 5);
			std::unordered_set<int> expected;
			expected.insert(2);
			expected.insert(3);
			expected.insert(4);
			expected.insert(5);
			std::unordered_set<int> result = storage.getAllLnAfter(1);
			bool output;
			if (result == expected) {
				output = true;
			}
			Assert::AreEqual(true, output);
		}
		TEST_METHOD(testNextStarPair)
		{
			PKB storage = PKB();
			storage.clear();
			storage.addStatement(1, assign, "proc1");
			storage.addStatement(2, whileStm, "proc1");
			storage.addStatement(3, assign, "proc1");
			storage.addStatement(4, assign, "proc1");
			storage.addStatement(5, assign, "proc1");
			storage.addNext(1, 2);
			storage.addNext(2, 3);
			storage.addNext(3, 4);
			storage.addNext(4, 5);
			storage.addNext(4, 2);
			storage.addNext(2, 5);
			std::unordered_set<std::pair<int, int>, intPairhash> expected;
			expected.emplace(1, 2);
			expected.emplace(1, 3);
			expected.emplace(1, 4);
			expected.emplace(1, 5);
			expected.emplace(2, 2);
			expected.emplace(2, 3);
			expected.emplace(2, 4);
			expected.emplace(2, 5);
			expected.emplace(3, 3);
			expected.emplace(3, 4);
			expected.emplace(3, 5);
			expected.emplace(3, 2);
			expected.emplace(4, 5);
			expected.emplace(4, 2);
			expected.emplace(4, 3);
			expected.emplace(4, 4);
			std::unordered_set<std::pair<int, int>, intPairhash> result = storage.getNextStarPairs();
			bool output;
			if (result == expected) {
				output = true;
			}
			Assert::AreEqual(true, output);
		}
	};
}