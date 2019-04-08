#include "stdafx.h"
#include "CppUnitTest.h"
#include "ContainerUtil.h" 

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(ContainerUtilTest)
	{
	public:

		TEST_METHOD(intVecToMapVec_empty_success)
		{
			std::vector<int> emptyIntVec;
			std::string key ("key");
			std::unordered_map<std::string, std::vector<std::string>> result = ContainerUtil::to_mapvec(key, 
				emptyIntVec);
			std::unordered_map<std::string, std::vector<std::string>> toCompare;
			std::vector<std::string> emptyVec;
			std::pair<std::string, std::vector<std::string>> keyValuePair (key, emptyVec);
			toCompare.insert(keyValuePair);

			Assert::AreEqual(result == toCompare, true);
		}
		TEST_METHOD(intVecToMapVec_nonEmpty_success)
		{
			std::vector<int> nonEmptyIntVec { 1, 2, 3 };
			std::string key ("key");
			std::unordered_map<std::string, std::vector<std::string>> result = ContainerUtil::to_mapvec(key,
				nonEmptyIntVec);
			std::unordered_map<std::string, std::vector<std::string>> toCompare;
			std::vector<std::string> vec { "1", "2", "3" };
			std::pair<std::string, std::vector<std::string>> keyValuePair (key, vec);
			toCompare.insert(keyValuePair);

			Assert::AreEqual(result == toCompare, true);
		}

		TEST_METHOD(intStrVecToMapVec_empty_success)
		{
			std::vector<std::pair<int, std::string>> emptyIntStrVec;
			std::string key1 ("key1");
			std::string key2 ("key2");
			std::unordered_map<std::string, std::vector<std::string>> result = ContainerUtil::to_mapvec(key1, 
				key2, emptyIntStrVec);
			std::unordered_map<std::string, std::vector<std::string>> toCompare;
			std::vector<std::string> emptyVec;
			std::pair<std::string, std::vector<std::string>> keyValuePair1 (key1, emptyVec);
			std::pair<std::string, std::vector<std::string>> keyValuePair2 (key2, emptyVec);
			toCompare.insert(keyValuePair1);
			toCompare.insert(keyValuePair2);
			Assert::AreEqual(result == toCompare, true);
		}

		TEST_METHOD(intStrVecToMapVec_nonEmpty_success)
		{
			std::vector<std::pair<int, std::string>> nonEmptyIntStrVec;
		    std::pair<int, std::string> firstPair (1, "one");
			std::pair<int, std::string> secondPair (2, "two");
			nonEmptyIntStrVec.push_back(firstPair);
			nonEmptyIntStrVec.push_back(secondPair);
			std::string key1 ("key1");
			std::string key2 ("key2");
			std::unordered_map<std::string, std::vector<std::string>> result =
				ContainerUtil::to_mapvec(key1, key2, nonEmptyIntStrVec);
			std::unordered_map<std::string, std::vector<std::string>> toCompare;
			std::vector<std::string> vec1 { "1", "2" };
			std::vector<std::string> vec2 { "one", "two" };
			std::pair<std::string, std::vector<std::string>> keyValuePair1 (key1, vec1);
			std::pair<std::string, std::vector<std::string>> keyValuePair2 (key2, vec2);
			toCompare.insert(keyValuePair1);
			toCompare.insert(keyValuePair2);
			Assert::AreEqual(result == toCompare, true);
		}

		TEST_METHOD(intSetToMapVec_empty_success)
		{
			std::unordered_set<int> emptyIntSet;
			std::string key ("key");
			std::unordered_map<std::string, std::vector<std::string>> result =
				ContainerUtil::to_mapvec(key, emptyIntSet);
			std::unordered_map<std::string, std::vector<std::string>> toCompare;
			std::vector<std::string> emptyVec;
			std::pair<std::string, std::vector<std::string>> keyValuePair (key, emptyVec);
			toCompare.insert(keyValuePair);
			Assert::AreEqual(result == toCompare, true);
		}

		TEST_METHOD(inSetToMapVec_nonEmpty_success)
		{
			std::unordered_set<int> nonEmptyIntSet ({ 1, 2, 3 });
			std::string key ("key");
			std::unordered_map<std::string, std::vector<std::string>> result =
				ContainerUtil::to_mapvec(key, nonEmptyIntSet);
			Assert::AreEqual(result.count(key) == 1, true);
			std::vector<std::string> toCheck = result[key];
			for (std::vector<std::string>::size_type i = 0; i != toCheck.size(); i++) {
				Assert::AreEqual(nonEmptyIntSet.count(stoi(toCheck[i])) == 1, true);
			}
		}

		TEST_METHOD(intPairSetToMapVec_empty_success)
		{
			std::unordered_set<std::pair<int, int>, intPairhash> emptyIntPairSet;
			std::string key1 ("key1");
			std::string key2 ("key2");
			std::unordered_map<std::string, std::vector<std::string>> result =
				ContainerUtil::to_mapvec(key1, key2, emptyIntPairSet);
			std::unordered_map<std::string, std::vector<std::string>> toCompare;
			std::vector<std::string> emptyVec;
			std::pair<std::string, std::vector<std::string>> keyValuePair1 (key1, emptyVec);
			std::pair<std::string, std::vector<std::string>> keyValuePair2 (key2, emptyVec);
			toCompare.insert(keyValuePair1);
			toCompare.insert(keyValuePair2);
			Assert::AreEqual(result == toCompare, true);
		}

		TEST_METHOD(intPairSetToMapVec_nonEmpty_success)
		{
			std::unordered_set<std::pair<int, int>, intPairhash> nonEmptyIntPairSet;
			std::pair<int, int> firstPair(1, 2);
			std::pair<int, int> secondPair(3, 4);
			nonEmptyIntPairSet.insert(firstPair);
			nonEmptyIntPairSet.insert(secondPair);
			std::string key1 ("key1");
			std::string key2 ("key2");
			std::unordered_map<std::string, std::vector<std::string>> result = ContainerUtil::to_mapvec(key1,
				key2, nonEmptyIntPairSet);
			Assert::AreEqual(result.count(key1) == 1, true);
			Assert::AreEqual(result.count(key2) == 1, true);
			std::vector<std::string> toCheck1 = result[key1];
			std::vector<std::string> toCheck2 = result[key2];
			for (std::vector<std::string>::size_type i = 0; i != toCheck1.size(); i++) {
				int firstOfPair = stoi(toCheck1[i]);
				int secondOfPair = stoi(toCheck2[i]);
				std::pair<int, int> currentPair (firstOfPair, secondOfPair);
				Assert::AreEqual(nonEmptyIntPairSet.count(currentPair) == 1, true);
			}
		}

		TEST_METHOD(intStrSetToMapVec_empty_success)
		{
			std::unordered_set<std::pair<int, std::string>, intStringhash> emptyIntStrSet;
			std::string key1 ("key1");
			std::string key2 ("key2");
			std::unordered_map<std::string, std::vector<std::string>> result =
				ContainerUtil::to_mapvec(key1, key2, emptyIntStrSet);
			std::unordered_map<std::string, std::vector<std::string>> toCompare;
			std::vector<std::string> emptyVec;
			std::pair<std::string, std::vector<std::string>> keyValuePair1 (key1, emptyVec);
			std::pair<std::string, std::vector<std::string>> keyValuePair2 (key2, emptyVec);
			toCompare.insert(keyValuePair1);
			toCompare.insert(keyValuePair2);
			Assert::AreEqual(result == toCompare, true);
		}

		TEST_METHOD(intStrSetToMapVec_nonEmpty_success)
		{
			std::unordered_set<std::pair<int, std::string>, intStringhash> nonEmptyIntStringSet;
			std::pair<int, std::string> firstPair (1, "baby");
			std::pair<int, std::string> secondPair (2, "shark");
			nonEmptyIntStringSet.insert(firstPair);
			nonEmptyIntStringSet.insert(secondPair);
			std::string key1 ("key1");
			std::string key2 ("key2");
			std::unordered_map<std::string, std::vector<std::string>> result =
				ContainerUtil::to_mapvec(key1, key2, nonEmptyIntStringSet);
			Assert::AreEqual(result.count(key1) == 1, true);
			Assert::AreEqual(result.count(key2) == 1, true);
			std::vector<std::string> toCheck1 = result[key1];
			std::vector<std::string> toCheck2 = result[key2];
			for (std::vector<std::string>::size_type i = 0; i != toCheck1.size(); i++) {
				int firstOfPair = stoi(toCheck1[i]);
				std::string secondOfPair = toCheck2[i];
				std::pair<int, std::string> currentPair(firstOfPair, secondOfPair);
				Assert::AreEqual(nonEmptyIntStringSet.count(currentPair) == 1, true);
			}
		}

		TEST_METHOD(strPairSetToMapVec_empty_success)
		{
			std::unordered_set<std::pair<std::string, std::string>, strPairhash> emptyStrPairSet;
			std::string key1 ("key1");
			std::string key2 ("key2");
			std::unordered_map<std::string, std::vector<std::string>> result =
				ContainerUtil::to_mapvec(key1, key2, emptyStrPairSet);
			std::unordered_map<std::string, std::vector<std::string>> toCompare;
			std::vector<std::string> emptyVec;
			std::pair<std::string, std::vector<std::string>> keyValuePair1 (key1, emptyVec);
			std::pair<std::string, std::vector<std::string>> keyValuePair2 (key2, emptyVec);
			toCompare.insert(keyValuePair1);
			toCompare.insert(keyValuePair2);
			Assert::AreEqual(result == toCompare, true);
		}

		TEST_METHOD(strPairSetToMapVec_nonEmpty_success)
		{
			std::unordered_set<std::pair<std::string, std::string>, strPairhash> nonEmptyStrPairSet;
			std::pair<std::string, std::string> firstPair ("baby", "shark");
			std::pair<std::string, std::string> secondPair ("telling", "lies");
			nonEmptyStrPairSet.insert(firstPair);
			nonEmptyStrPairSet.insert(secondPair);
			std::string key1 ("key1");
			std::string key2 ("key2");
			std::unordered_map<std::string, std::vector<std::string>> result =
				ContainerUtil::to_mapvec(key1, key2, nonEmptyStrPairSet);
			Assert::AreEqual(result.count(key1) == 1, true);
			Assert::AreEqual(result.count(key2) == 1, true);
			std::vector<std::string> toCheck1 = result[key1];
			std::vector<std::string> toCheck2 = result[key2];
			for (std::vector<std::string>::size_type i = 0; i != toCheck1.size(); i++) {
				std::string firstOfPair = toCheck1[i];
				std::string secondOfPair = toCheck2[i];
				std::pair<std::string, std::string> currentPair(firstOfPair, secondOfPair);
				Assert::AreEqual(nonEmptyStrPairSet.count(currentPair) == 1, true);
			}
		}

		TEST_METHOD(crossProduct_noEmpty_success)
		{
			std::unordered_map<std::string, std::vector<std::string>> oldTable;
			std::unordered_map<std::string, std::vector<std::string>> toAddTable;
			std::string key1("key1");
			std::string key2("key2");
			std::string key3("key3");
			std::string key4("key4");
			std::vector<std::string> val1{ "1", "2" };
			std::vector<std::string> val2{ "3", "4" };
			std::vector<std::string> val3{ "5", "6" };
			std::vector<std::string> val4{ "7", "8" };
			oldTable.insert({ {key1, val1}, {key2, val2} });
			toAddTable.insert({ { key3, val3 }, { key4, val4 } });
			std::unordered_map<std::string, std::vector<std::string>> newTable = ContainerUtil::product(
				oldTable, toAddTable);
			std::unordered_map<std::string, std::vector<std::string>> toCompare;
			std::vector<std::string> newVal1{ "1", "2", "1", "2" };
			std::vector<std::string> newVal2{ "3", "4", "3", "4" };
			std::vector<std::string> newVal3{ "5", "5", "6", "6" };
			std::vector<std::string> newVal4{ "7", "7", "8", "8" };
			toCompare.insert({ {key1, newVal1}, {key2, newVal2}, {key3, newVal3}, {key4, newVal4} });
			Assert::AreEqual(newTable == toCompare, true);
		}

		TEST_METHOD(oneCommonProduct_noEmpty_success)
		{
			std::unordered_map<std::string, std::vector<std::string>> oldTable;
			std::unordered_map<std::string, std::vector<std::string>> toAddTable;
			std::string key1("key1");
			std::string key2("key2");
			std::vector<std::string> val1{ "1", "2", "3" };
			std::vector<std::string> val2{ "3", "4", "5" };
			std::vector<std::string> val3{ "2", "3" };
			oldTable.insert({ {key1, val1}, {key2, val2} });
			toAddTable.insert({ key1, val3 });
			std::unordered_map<std::string, std::vector<std::string>> newTable = ContainerUtil::product(
				oldTable, toAddTable);
			std::unordered_map<std::string, std::vector<std::string>> toCompare;
			std::vector<std::string> newVal1{ "4", "5" };
			toCompare.insert({ {key1, val3}, {key2, newVal1} });
			Assert::AreEqual(newTable == toCompare, true);
		}

		TEST_METHOD(twoCommonProduct_noEmpty_success)
		{
			std::unordered_map<std::string, std::vector<std::string>> oldTable;
			std::unordered_map<std::string, std::vector<std::string>> toAddTable;
			std::string key1("key1");
			std::string key2("key2");
			std::vector<std::string> val1{ "1", "2", "5", "6"};
			std::vector<std::string> val2{ "3", "4" , "7", "8"};
			std::vector<std::string> val3{ "5", "6" };
			std::vector<std::string> val4{ "7", "8" };
			oldTable.insert({ {key1, val1}, {key2, val2} });
			toAddTable.insert({ { key1, val3 }, { key2, val4 } });
			std::unordered_map<std::string, std::vector<std::string>> newTable = ContainerUtil::product(
				oldTable, toAddTable);
			std::unordered_map<std::string, std::vector<std::string>> toCompare;
			toCompare.insert({ {key1, val3}, {key2, val4} });
			Assert::AreEqual(newTable == toCompare, true);
		}
		TEST_METHOD(mixProduct_noEmpty_success)
		{
			std::unordered_map<std::string, std::vector<std::string>> oldTable;
			std::unordered_map<std::string, std::vector<std::string>> toAddTable;
			std::string key1("key1");
			std::string key2("key2");
			std::string key3("key3");
			std::vector<std::string> val1{ "1", "2", "3" };
			std::vector<std::string> val2{ "4", "5", "6" };
			std::vector<std::string> val3{ "5", "6", "7" };
			std::vector<std::string> val4{ "2", "3", "4" };
			oldTable.insert({ {key1, val1}, {key2, val2} });
			toAddTable.insert({ { key3, val3 }, { key1, val4 } });
			std::unordered_map<std::string, std::vector<std::string>> newTable = ContainerUtil::product(
				oldTable, toAddTable);
			std::unordered_map<std::string, std::vector<std::string>> toCompare;
			std::vector<std::string> newVal1{ "2", "3" };
			std::vector<std::string> newVal2{ "5", "6" };
			toCompare.insert({ {key1, newVal1}, {key2, newVal2}, {key3, newVal2} });
			Assert::AreEqual(newTable == toCompare, true);
		}

		TEST_METHOD(product_noEmpty_success)
		{
			std::unordered_map<std::string, std::vector<std::string>> oldTable;
			std::unordered_map<std::string, std::vector<std::string>> toAddTable;
			std::string key1("key1");
			std::string key2("key2");
			std::string key3("key3");
			std::string key4("key4");
			std::vector<std::string> val1{ "1", "1", "2", "3" };
			std::vector<std::string> val2{ "3", "4", "6", "8" };
			std::vector<std::string> val3{ "x", "y", "z", "z" };
			std::vector<std::string> val4{ "1", "2", "4" };
			std::vector<std::string> val5{ "y", "z", "z" };
			oldTable.insert({ {key1, val1}, {key2, val2}, {key3, val3} });
			toAddTable.insert({ { key1, val4 }, { key4, val5 } });
			std::unordered_map<std::string, std::vector<std::string>> newTable = ContainerUtil::product(
				oldTable, toAddTable);
			std::unordered_map<std::string, std::vector<std::string>> toCompare;
			std::vector<std::string> newVal1{ "1", "1", "2" };
			std::vector<std::string> newVal2{ "3", "4", "6" };
			std::vector<std::string> newVal3{ "x", "y", "z" };
			std::vector<std::string> newVal4{ "y", "y", "z" };
			toCompare.insert({ {key1, newVal1}, {key2, newVal2}, {key3, newVal3}, {key4, newVal4} });
			Assert::AreEqual(newTable == toCompare, true);
		}
	};
}