#pragma once

#include "UseStorage.h"

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
Uses
ln9 - 12
store.addUsesStm(9, "apple");
store.addUsesStm(9, "melon");
store.addUsesStm(10, "banana");
store.addUsesStm(12, "melon");
store.addUsesStm(11, "melon");
store.addUsesStm(10, "melon");
store.addUsesProc("fruit2", "apple");
store.addUsesProc("fruit2", "melon");
store.addUsesProc("fruit2", "banana");
store.addUsesProc("fruit3", "melon");
store.addUsesProc("fruit2", "melon");
*/
std::unordered_set<std::string> stmVarList[4] = { {"apple", "melon"}, {"banana", "melon"}, {"melon"}, {"melon"} };
std::unordered_set<std::string> procVarList[2] = { {"apple", "melon", "banana"}, {"melon"} };
std::unordered_set<int> stmList[4] = { {9}, {9, 10, 11, 12}, {10}, {9, 10, 11, 12} };
std::unordered_set<std::string> procList[4] = { {"fruit2"}, {"fruit2", "fruit3"}, {"fruit2"},
	{"fruit2", "fruit3"} };

std::unordered_set<std::pair<int, std::string>, intStringhash> stmVarPairs =
	{ std::pair<int, std::string>(9, "apple"), std::pair<int, std::string>(9, "melon"),
	std::pair<int, std::string>(10, "banana"), std::pair<int, std::string>(10, "melon"),
	std::pair<int, std::string>(11, "melon"), std::pair<int, std::string>(12, "melon") };

std::unordered_set<std::pair<std::string, std::string>, strPairhash> procVarPairs = {
	std::pair<std::string, std::string>("fruit2", "apple"),
	std::pair<std::string, std::string>("fruit2", "melon"),
	std::pair<std::string, std::string>("fruit2", "banana"),
	std::pair<std::string, std::string>("fruit3", "melon") };