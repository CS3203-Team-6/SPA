#pragma once

#include "ModifyStorage.h"

/*

	procedure fruit {
1		read apple;
2		call fruit2;
3		print banana;
4		apple = cherry + 10 * banana;
5		if (apple <= 0) then {
6			while ( durian > cherry ) {
7				durian = grapes + banana;
			}
		} else {
8			figs = apple * melon / cherry;
		}
	}

	procedure fruit2 {
9		pineapple = apple + melon;
10		while (10 > banana) {
11			call fruit3;
		}
	}

	procedure fruit3 {
12		print melon;
	}

*/

/*
Modify
store.addModifiesStm(1, "apple");
store.addModifiesStm(2, "pineapple");
store.addModifiesStm(4, "apple");
store.addModifiesStm(5, "durian");
store.addModifiesStm(5, "figs");
store.addModifiesStm(6, "durian");
store.addModifiesStm(7, "durian");
store.addModifiesStm(8, "figs");
store.addModifiesStm(9, "pineapple");
store.addModifiesProc("fruit", "apple");
store.addModifiesProc("fruit", "pineapple");
store.addModifiesProc("fruit", "durian");
store.addModifiesProc("fruit", "figs");
store.addModifiesProc("fruit2", "pineapple");
*/
std::unordered_set<std::string> ModifyStmVarList[8] = { {"apple"}, {"pineapple"}, {"apple"},
	{"durian", "figs"}, {"durian"}, {"durian"}, {"figs"}, {"pineapple"} };

std::unordered_set<std::string> ModifyProcVarList[2] = { {"apple", "pineapple", "durian", "figs"},
	{"pineapple"} };

std::unordered_set<int> ModifyStmList[5] = { {1, 4}, {2, 9}, {5, 6, 7}, {5, 8},
	{1, 2, 4, 5, 6, 7, 8, 9} };

std::unordered_set<std::string> ModifyProcList[5] = { {"fruit"}, {"fruit", "fruit2"}, {"fruit"},
	{"fruit"}, {"fruit", "fruit2"} };

std::unordered_set<std::pair<int, std::string>, intStringhash> ModifyStmVarPairs =
	{ std::pair<int, std::string>(1, "apple"), std::pair<int, std::string>(2, "pineapple"),
	std::pair<int, std::string>(4, "apple"), std::pair<int, std::string>(5, "durian"),
	std::pair<int, std::string>(5, "figs"), std::pair<int, std::string>(6, "durian"),
	std::pair<int, std::string>(7, "durian"), std::pair<int, std::string>(8, "figs"),
	std::pair<int, std::string>(9, "pineapple") };

std::unordered_set<std::pair<std::string, std::string>, strPairhash> ModifyProcVarPairs = {
	std::pair<std::string, std::string>("fruit", "apple"),
	std::pair<std::string, std::string>("fruit", "pineapple"),
	std::pair<std::string, std::string>("fruit", "durian"), 
	std::pair<std::string, std::string>("fruit", "figs"),
	std::pair<std::string, std::string>("fruit2", "pineapple")};