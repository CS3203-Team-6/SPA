#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>

#include "PKB.h"

class ContainerUtil
{
public:
	static std::unordered_map<std::string, std::vector<std::string>> to_mapvec(std::string key,
		int n);
	static std::unordered_map<std::string, std::vector<std::string>> to_mapvec(std::string key,
																			   std::string s);
	static std::unordered_map<std::string, std::vector<std::string>> to_mapvec(std::string key, 
		std::vector<int> intVec);
	static std::unordered_map<std::string, std::vector<std::string>> to_mapvec(std::string key1, 
		std::string key2, std::vector<std::pair<int, std::string>> intStrVec);
	static std::unordered_map<std::string, std::vector<std::string>> to_mapvec(std::string key, 
		std::unordered_set<int> intSet);
	static std::unordered_map<std::string, std::vector<std::string>> to_mapvec(std::string key,
		std::unordered_set<std::string> strSet);
	static std::unordered_map<std::string, std::vector<std::string>> to_mapvec(std::string key1,
		std::string key2, std::unordered_set<std::pair<int, int>, intPairhash> intPairSet);
	static std::unordered_map<std::string, std::vector<std::string>> to_mapvec(std::string key1, 
		std::string key2, std::unordered_set<std::pair<int, std::string>, intStringhash> intStringSet);
	static std::unordered_map<std::string, std::vector<std::string>> to_mapvec(std::string key1, 
		std::string key2, std::unordered_set<std::pair<std::string, std::string>, strPairhash> strPairSet);
	static std::unordered_map<std::string, std::vector<std::string>> crossProduct(
		std::unordered_map<std::string, std::vector<std::string>> oldTable,
		std::unordered_map<std::string, std::vector<std::string>> toAddTable);
	static std::unordered_map<std::string, std::vector<std::string>> oneCommonProduct(
		std::unordered_map<std::string, std::vector<std::string>> oldTable,
		std::unordered_map<std::string, std::vector<std::string>> toAddTable);
	static std::unordered_map<std::string, std::vector<std::string>> twoCommonProduct(
		std::unordered_map<std::string, std::vector<std::string>> oldTable,
		std::unordered_map<std::string, std::vector<std::string>> toAddTable);
	static std::unordered_map<std::string, std::vector<std::string>> mixProduct(
		std::unordered_map<std::string, std::vector<std::string>> oldTable,
		std::unordered_map<std::string, std::vector<std::string>> toAddTable);
	static std::unordered_map<std::string, std::vector<std::string>> product(
		std::unordered_map<std::string, std::vector<std::string>> oldTable,
		std::unordered_map<std::string, std::vector<std::string>> toAddTable);
	static std::unordered_map<std::string, std::vector<std::string>> intersectOne(
		std::unordered_map<std::string, std::vector<std::string>> table1,
		std::unordered_map<std::string, std::vector<std::string>> table2);
	static std::unordered_map<std::string, std::vector<std::string>> intersectTwo(
		std::unordered_map<std::string, std::vector<std::string>> table1,
		std::unordered_map<std::string, std::vector<std::string>> table2);
};
