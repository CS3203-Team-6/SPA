#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>

#include "PKB.h"
#include "LexicalToken.h"

namespace QueryUtility
{

	std::string truthValue(bool boolean);

	std::string trimFrontEnd(std::string quotedString);

	bool isInteger(std::string s);

	bool isQuoted(std::string s);

	bool isSynonym(std::string s);

	bool hasReference(std::string s);

	bool isOutOfRange(std::string s);

	std::string attrOf(std::string s);

	std::string refOf(std::string s);

	std::unordered_set<std::string> getAllStms();

	std::unordered_map<std::string, std::vector<std::string>>
		getStmtsMap(std::unordered_map<std::string, std::string> declarations, std::string syn);

	std::unordered_set<std::string> getStmts(std::string);

	std::unordered_set<std::string> intSetToStrSet(std::unordered_set<int> intSet);

	std::unordered_set<int> getTwin(std::unordered_set<std::pair<int, int>, intPairhash> intPairSet);

	std::unordered_map<std::string, std::vector<std::string>> to_mapvec(std::string key,
		int n);

	std::unordered_map<std::string, std::vector<std::string>> to_mapvec(std::string key,
																			   std::string s);

	std::unordered_map<std::string, std::vector<std::string>> to_mapvec(std::string key, 
		std::vector<int> intVec);

	std::unordered_map<std::string, std::vector<std::string>> to_mapvec(std::string key1, 
		std::string key2, std::vector<std::pair<int, std::string>> intStrVec);

	std::unordered_map<std::string, std::vector<std::string>> to_mapvec(std::string key, 
		std::unordered_set<int> intSet);

	std::unordered_map<std::string, std::vector<std::string>> to_mapvec(std::string key,
		std::unordered_set<std::string> strSet);

	std::unordered_map<std::string, std::vector<std::string>> to_mapvec(std::string key1,
		std::string key2, std::unordered_set<std::pair<int, int>, intPairhash> intPairSet);

	std::unordered_map<std::string, std::vector<std::string>> to_mapvec(std::string key1, 
		std::string key2, std::unordered_set<std::pair<int, std::string>, intStringhash> intStringSet);

	std::unordered_map<std::string, std::vector<std::string>> to_mapvec(std::string key1, 
		std::string key2, std::unordered_set<std::pair<std::string, std::string>, strPairhash> strPairSet);

	std::unordered_map<std::string, std::vector<std::string>> crossProduct(
		std::unordered_map<std::string, std::vector<std::string>> oldTable,
		std::unordered_map<std::string, std::vector<std::string>> toAddTable);

	std::unordered_map<std::string, std::vector<std::string>> oneCommonProduct(
		std::unordered_map<std::string, std::vector<std::string>> oldTable,
		std::unordered_map<std::string, std::vector<std::string>> toAddTable);

	std::unordered_map<std::string, std::vector<std::string>> twoCommonProduct(
		std::unordered_map<std::string, std::vector<std::string>> oldTable,
		std::unordered_map<std::string, std::vector<std::string>> toAddTable);

	std::unordered_map<std::string, std::vector<std::string>> mixProduct(
		std::unordered_map<std::string, std::vector<std::string>> oldTable,
		std::unordered_map<std::string, std::vector<std::string>> toAddTable);

	std::unordered_map<std::string, std::vector<std::string>> product(
		std::unordered_map<std::string, std::vector<std::string>> oldTable,
		std::unordered_map<std::string, std::vector<std::string>> toAddTable);
	
	std::unordered_map<std::string, std::vector<std::string>> intersectOne(
		std::unordered_map<std::string, std::vector<std::string>> table1,
		std::unordered_map<std::string, std::vector<std::string>> table2);

	std::unordered_map<std::string, std::vector<std::string>> intersectTwo(
		std::unordered_map<std::string, std::vector<std::string>> table1,
		std::unordered_map<std::string, std::vector<std::string>> table2);
};
