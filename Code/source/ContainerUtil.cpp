#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
#include <sstream>
#include <algorithm>

#include "PKB.h"
#include "QueryEvaluator.h"	
#include "LexicalToken.h"
#include "ExpressionUtil.h"
#include "ContainerUtil.h"

/*
The function transforms a set of integers
into a set of strings
*/
std::unordered_set<std::string> ContainerUtil::intSetToStrSet(std::unordered_set<int> intSet) {
	std::unordered_set<std::string> strSet;
	for (std::unordered_set<int>::iterator it = intSet.begin(); it != intSet.end(); ++it) {
		strSet.insert(std::to_string(*it));
	}

	return strSet;
}

/*
The function returns a map of string to
vector of string given a key and an integer.
*/
std::unordered_map<std::string, std::vector<std::string>> ContainerUtil::to_mapvec(std::string key,
	int n) {
	std::unordered_map<std::string, std::vector<std::string>> mapvec;
	std::vector<std::string> keyValue;
	keyValue.push_back(std::to_string(n));
	std::pair<std::string, std::vector<std::string>> keyValuePair(key, keyValue);
	mapvec.insert(keyValuePair);

	return mapvec;
}

/*
The function returns a map of string to
vector of string given a key and a string.
*/
std::unordered_map<std::string, std::vector<std::string>> ContainerUtil::to_mapvec(std::string key,
																				   std::string s) {
	std::unordered_map<std::string, std::vector<std::string>> mapvec;
	std::vector<std::string> keyValue;
	keyValue.push_back(s);
	std::pair<std::string, std::vector<std::string>> keyValuePair(key, keyValue);
	mapvec.insert(keyValuePair);

	return mapvec;
}

/*
The function returns a map of string to
vector of strings given a key and vector 
of integers.
*/
std::unordered_map<std::string, std::vector<std::string>> ContainerUtil::to_mapvec(std::string key, std::vector<int> intVec) {
	std::unordered_map<std::string, std::vector<std::string>> mapvec;
	std::vector<std::string> keyValue;
	for (std::vector<int>::size_type i = 0; i != intVec.size(); i++) {
		keyValue.push_back(std::to_string(intVec[i]));
	}
	std::pair<std::string, std::vector<std::string>> keyValuePair (key, keyValue);
	mapvec.insert(keyValuePair);

	return mapvec;
}

/*
The function returns a map of strings
to vectors of strings given two keys and
vector of pairs of int and string.
*/
std::unordered_map<std::string, std::vector<std::string>> ContainerUtil::to_mapvec(std::string key1, 
	std::string key2, std::vector<std::pair<int, std::string>> intStrVec) {
	std::unordered_map<std::string, std::vector<std::string>> mapvec;
	std::vector<std::string> key1Value;
	std::vector<std::string> key2Value;
	for (std::vector<std::pair<int, std::string>>::size_type i = 0; i != intStrVec.size(); i++) {
		key1Value.push_back(std::to_string(intStrVec[i].first));
		key2Value.push_back(intStrVec[i].second);
	}
	std::pair<std::string, std::vector<std::string>> keyValuePair1 (key1, key1Value);
	std::pair<std::string, std::vector<std::string>> keyValuePair2 (key2, key2Value);
	mapvec.insert(keyValuePair1);
	mapvec.insert(keyValuePair2);

	return mapvec;
}

/*
The function returns a map of string
to vector of string given a key and 
set of integers
*/
std::unordered_map<std::string, std::vector<std::string>> ContainerUtil::to_mapvec(std::string key,
	std::unordered_set<int> intSet) {
	std::unordered_map<std::string, std::vector<std::string>> mapvec;
	std::vector<std::string> keyValue;
	for (std::unordered_set<int>::iterator it = intSet.begin(); it != intSet.end(); ++it) {
		keyValue.push_back(std::to_string(*it));
	}
	std::pair<std::string, std::vector<std::string>> keyValuePair(key, keyValue);
	mapvec.insert(keyValuePair);

	return mapvec;
}

/*
The function returns a map of string
to vector of string given a key and
set of strings
*/
std::unordered_map<std::string, std::vector<std::string>> ContainerUtil::to_mapvec(std::string key,
	std::unordered_set<std::string> strSet) {
	std::unordered_map<std::string, std::vector<std::string>> mapvec;
	std::vector<std::string> keyValue;
	for (std::unordered_set<std::string>::iterator it = strSet.begin(); it != strSet.end(); ++it) {
		keyValue.push_back(*it);
	}
	std::pair<std::string, std::vector<std::string>> keyValuePair(key, keyValue);
	mapvec.insert(keyValuePair);

	return mapvec;
}

/*
The function returns a map of strings
to vectors of strings given two keys and
set of pairs of integers and integers.
*/
std::unordered_map<std::string, std::vector<std::string>> ContainerUtil::to_mapvec(std::string key1,
	std::string key2, std::unordered_set<std::pair<int, int>, intPairhash> intPairSet) {
	std::unordered_map<std::string, std::vector<std::string>> mapvec;
	std::vector<std::string> key1Value;
	std::vector<std::string> key2Value;
	for (std::unordered_set<std::pair<int, int>, intPairhash>::iterator it = intPairSet.begin(); it != intPairSet.end(); ++it) {
		std::pair<int, int> pointer = *it;
		key1Value.push_back(std::to_string(pointer.first));
		key2Value.push_back(std::to_string(pointer.second));
	}
	std::pair<std::string, std::vector<std::string>> keyValuePair1(key1, key1Value);
	std::pair<std::string, std::vector<std::string>> keyValuePair2(key2, key2Value);
	mapvec.insert(keyValuePair1);
	mapvec.insert(keyValuePair2);

	return mapvec;
}

/*
The function returns a map of strings
to vectors of strings given two keys and
set of pairs of integers and strings.
*/
std::unordered_map<std::string, std::vector<std::string>> ContainerUtil::to_mapvec(std::string key1,
	std::string key2, std::unordered_set<std::pair<int, std::string>, intStringhash> intStringSet) {
	std::unordered_map<std::string, std::vector<std::string>> mapvec;
	std::vector<std::string> key1Value;
	std::vector<std::string> key2Value;
	for (std::unordered_set<std::pair<int, std::string>, intPairhash>::iterator
		 it = intStringSet.begin(); it != intStringSet.end(); ++it) {
		std::pair<int, std::string> pointer = *it;
		key1Value.push_back(std::to_string(pointer.first));
		key2Value.push_back(pointer.second);
	}
	std::pair<std::string, std::vector<std::string>> keyValuePair1(key1, key1Value);
	std::pair<std::string, std::vector<std::string>> keyValuePair2(key2, key2Value);
	mapvec.insert(keyValuePair1);
	mapvec.insert(keyValuePair2);

	return mapvec;
}

/*
The function returns a map of strings
to vectors of strings given two keys and
set of pairs of strings and strings.
*/
std::unordered_map<std::string, std::vector<std::string>> ContainerUtil::to_mapvec(std::string key1,
	std::string key2, std::unordered_set<std::pair<std::string, std::string>, strPairhash> strPairSet) {
	std::unordered_map<std::string, std::vector<std::string>> mapvec;
	std::vector<std::string> key1Value;
	std::vector<std::string> key2Value;
	for (std::unordered_set<std::pair<std::string, std::string>, strPairhash>::iterator it = strPairSet.begin(); it != strPairSet.end(); ++it) {
		std::pair<std::string, std::string> pointer = *it;
		key1Value.push_back(pointer.first);
		key2Value.push_back(pointer.second);
	}
	std::pair<std::string, std::vector<std::string>> keyValuePair1(key1, key1Value);
	std::pair<std::string, std::vector<std::string>> keyValuePair2(key2, key2Value);
	mapvec.insert(keyValuePair1);
	mapvec.insert(keyValuePair2);

	return mapvec;
}

/*
The function do cross product
of 2 tables in form of a map
*/
std::unordered_map<std::string, std::vector<std::string>> ContainerUtil::crossProduct(
	std::unordered_map<std::string, std::vector<std::string>> oldTable,
	std::unordered_map<std::string, std::vector<std::string>> toAddTable) {
	std::unordered_map<std::string, std::vector<std::string>> newTable;
	int oldSize = oldTable.begin()->second.size();
	int toAddSize = toAddTable.begin()->second.size();
	if (toAddSize == 0) {
		return toAddTable;
	}
	for (auto columnIt = oldTable.begin(); columnIt != oldTable.end(); ++columnIt) {
		std::vector<std::string> newColumn;
		for (int i = 0; i < toAddSize; i++) {
			for (std::vector<std::string>::size_type j = 0; j != oldSize; j++) {
				newColumn.push_back(columnIt->second[j]);
			}
		}
		std::pair<std::string, std::vector<std::string>> toAddPair(columnIt->first, newColumn);
		newTable.insert({ columnIt->first, newColumn });
	}
	for (auto columnIt = toAddTable.begin(); columnIt != toAddTable.end(); ++columnIt) {
		std::vector<std::string> newColumn;
		for (std::vector<std::string>::size_type i = 0; i != toAddSize; i++) {
			for (int j = 0; j < oldSize; j++) {
				newColumn.push_back(columnIt->second[i]);
			}
		}
		newTable.insert({ columnIt->first, newColumn });
	}

	return newTable;
}

/*
The function products two tables
where the second table only contains
one key and it is also in the first 
table.
*/
std::unordered_map<std::string, std::vector<std::string>> ContainerUtil::oneCommonProduct(
	std::unordered_map<std::string, std::vector<std::string>> oldTable,
	std::unordered_map<std::string, std::vector<std::string>> toAddTable) {
	std::unordered_map<std::string, std::vector<std::string>> newTable;
	std::string commonKey = toAddTable.begin()->first;
	int oldSize = oldTable[commonKey].size();
	int toAddSize = toAddTable[commonKey].size();
	for (auto columnIt = oldTable.begin(); columnIt != oldTable.end(); ++columnIt) {
		std::vector<std::string> newColumn;
		for (std::vector<std::string>::size_type i = 0; i != oldSize; i++) {
			for (std::vector<std::string>::size_type j = 0; j != toAddSize; j++) {
				if (toAddTable[commonKey][j] == oldTable[commonKey][i]) {
					newColumn.push_back(columnIt->second[i]);
					break;
				}
			}
		}
		newTable.insert({ columnIt->first, newColumn });
	}

	return newTable;
}

/*
The function products two tables
where the second table only contains
two keys and they are also in the 
first table.
*/
std::unordered_map<std::string, std::vector<std::string>> ContainerUtil::twoCommonProduct(
	std::unordered_map<std::string, std::vector<std::string>> oldTable,
	std::unordered_map<std::string, std::vector<std::string>> toAddTable) {
	std::unordered_map<std::string, std::vector<std::string>> newTable;
	std::vector<std::string> commonKeys;
	for (auto it = toAddTable.begin(); it != toAddTable.end(); ++it) {
		commonKeys.push_back(it->first);
	}
	int oldSize = oldTable[commonKeys[0]].size();
	int toAddSize = toAddTable[commonKeys[0]].size();
	for (auto columnIt = oldTable.begin(); columnIt != oldTable.end(); ++columnIt) {
		std::vector<std::string> newColumn;
		for (std::vector<std::string>::size_type i = 0; i != oldSize; i++) {
			for (std::vector<std::string>::size_type j = 0; j != toAddSize; j++) {
				if (toAddTable[commonKeys[0]][j] == oldTable[commonKeys[0]][i]
					&& toAddTable[commonKeys[1]][j] == oldTable[commonKeys[1]][i]) {
					newColumn.push_back(columnIt->second[i]);
					break;
				}
			}
		}
		newTable.insert({ columnIt->first, newColumn });
	}

	return newTable;
}

/*
The function products two tables
where the second table only contains
two keys and exactly one of them is 
in the firsttable.
*/
std::unordered_map<std::string, std::vector<std::string>> ContainerUtil::mixProduct(
	std::unordered_map<std::string, std::vector<std::string>> oldTable,
	std::unordered_map<std::string, std::vector<std::string>> toAddTable) {
	std::string commonKey;
	for (auto it = toAddTable.begin(); it != toAddTable.end(); ++it) {
		if (oldTable.count(it->first) == 1) {
			commonKey = it->first;
		}
	}
	int oldSize = oldTable[commonKey].size();
	int toAddSize = toAddTable[commonKey].size();
	std::unordered_map<std::string, std::vector<std::string>> intermediateTable1;
	std::unordered_map<std::string, std::vector<std::string>> intermediateTable2;
	intermediateTable1.insert({ commonKey, toAddTable[commonKey] });
	intermediateTable2.insert({ commonKey, oldTable[commonKey] });
	intermediateTable1 = oneCommonProduct(oldTable, intermediateTable1);
	intermediateTable2 = oneCommonProduct(toAddTable, intermediateTable2);
	std::unordered_map<std::string, std::vector<std::string>> newTable;
	for (auto columnIt = oldTable.begin(); columnIt != oldTable.end(); ++columnIt) {
		std::vector<std::string> newColumn;
		for (std::vector<std::string>::size_type i = 0; i != toAddSize; i++) {
			for (std::vector<std::string>::size_type j = 0; j != oldSize; j++) {
				if (oldTable[commonKey][j] == toAddTable[commonKey][i]) {
					newColumn.push_back(columnIt->second[j]);
				}
			}
		}
		newTable.insert({ columnIt->first, newColumn });
	}
	for (auto columnIt = toAddTable.begin(); columnIt != toAddTable.end(); ++columnIt) {
		std::vector<std::string> newColumn;
		for (std::vector<std::string>::size_type i = 0; i != toAddSize; i++) {
			for (std::vector<std::string>::size_type j = 0; j != oldSize; j++) {
				if (oldTable[commonKey][j] == toAddTable[commonKey][i]) {
					newColumn.push_back(columnIt->second[i]);
				}
			}
		}
		newTable.insert({ columnIt->first, newColumn });
	}
	return newTable;
}

/*
Collate all the product functions
*/
std::unordered_map<std::string, std::vector<std::string>> ContainerUtil::product(
	std::unordered_map<std::string, std::vector<std::string>> oldTable,
	std::unordered_map<std::string, std::vector<std::string>> toAddTable) {
	if (oldTable.size() == 0 || toAddTable.begin()->second.size() == 0) {
		return toAddTable;
	}
	if (toAddTable.size() == 1) {
		if (oldTable.count(toAddTable.begin()->first) == 1) {
			return oneCommonProduct(oldTable, toAddTable);
		}
		else {
			return crossProduct(oldTable, toAddTable);
		}
	}
	if (toAddTable.size() == 2) {
		auto it = toAddTable.begin();
		std::string firstKey = it->first;
		++it;
		std::string secondKey = it->first;
		if (oldTable.count(firstKey) + oldTable.count(secondKey) == 2) {
			return twoCommonProduct(oldTable, toAddTable);
		}
		else if (oldTable.count(firstKey) + oldTable.count(secondKey) == 1) {
			return mixProduct(oldTable, toAddTable);
		}
		else {
			return crossProduct(oldTable, toAddTable);
		}
	}
	throw "product method failed";
}

/*
The function intersect two tables
with one column and combine them.
*/
std::unordered_map<std::string, std::vector<std::string>> ContainerUtil::intersectOne(
	std::unordered_map<std::string, std::vector<std::string>> table1,
	std::unordered_map<std::string, std::vector<std::string>> table2) {
	std::unordered_map<std::string, std::vector<std::string>> newTable;
	newTable.insert({ table1.begin()->first, table2.begin()->second });
	newTable = product(table1, newTable);
	newTable.insert({ table2.begin()->first, newTable.begin()->second });
	return newTable;
}

/*
The function intersect two tables
with two columns and combine the
columns that are not common key.
Assume common key is the first column.
*/
std::unordered_map<std::string, std::vector<std::string>> ContainerUtil::intersectTwo(
	std::unordered_map<std::string, std::vector<std::string>> table1,
	std::unordered_map<std::string, std::vector<std::string>> table2) {
	std::string commonKey;
	for (auto it = table2.begin(); it != table2.end(); ++it) {
		if (table1.count(it->first) == 1) {
			commonKey = it->first;
		}
	}
	std::unordered_map<std::string, std::vector<std::string>> intermediateTable = product(
		table1, table2);
	std::unordered_map<std::string, std::vector<std::string>> newTable;
	for (auto it = intermediateTable.begin(); it != intermediateTable.end(); ++it) {
		if (it->first != commonKey) {
			newTable.insert({ it->first, it->second });
		}
	}
	return newTable;
}

