#pragma once

#include "CallStorage.h"

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
9			call fruit3;
		}
	}

	procedure fruit2 {
10		pineapple = apple + melon;
11		while (10 > banana) {
12			call fruit3;
		}
	}

	procedure fruit3 {
13		print melon;
14		call fruit4;
	}

	procedure fruit4 {
15		pear = jackfruit + apple;
	}

*/

/*
Call
store.addCall("fruit", "fruit2", 2);
store.addCall("fruit", "fruit3", 9);
store.addCall("fruit2", "fruit3", 12);
store.addCall("fruit3", "fruit4", 14);
store.setCallAnc("fruit4", std::unordered_set<std::string>{"fruit", "fruit2", "fruit3"});
store.setCallAnc("fruit3", std::unordered_set<std::string>{"fruit", "fruit2"});
store.setCallAnc("fruit2", std::unordered_set<std::string>{"fruit"});
store.setCallDesc("fruit", std::unordered_set<std::string>{"fruit2", "fruit3", "fruit4"});
store.setCallDesc("fruit2", std::unordered_set<std::string>{"fruit3", "fruit4"});
store.setCallDesc("fruit3", std::unordered_set<std::string>{"fruit4"});
*/
std::unordered_set<std::string> caller[4] = { {}, {"fruit"}, {"fruit", "fruit2"}, {"fruit3"} };
std::unordered_set<std::string> callees[4] = { {"fruit2", "fruit3"}, {"fruit3"}, {"fruit4"}, {} };
std::unordered_set<std::string> callAnc[4] = { {}, {"fruit"}, {"fruit", "fruit2"},
	{"fruit", "fruit2", "fruit3"} };
std::unordered_set<std::string> callDesc[4] = { {"fruit2", "fruit3", "fruit4"}, {"fruit3", "fruit4"},
	{"fruit4"}, {} };


std::unordered_set< std::pair<std::string, std::string>, strPairhash> callPairList = 
	{ std::pair<std::string, std::string>("fruit", "fruit2"),
	std::pair<std::string, std::string>("fruit", "fruit3"),
	std::pair<std::string, std::string>("fruit2", "fruit3"),
	std::pair<std::string, std::string>("fruit3", "fruit4") };

std::unordered_set< std::pair<std::string, std::string>, strPairhash> callStarPairList =
	{ std::pair<std::string, std::string>("fruit", "fruit2"),
	std::pair<std::string, std::string>("fruit", "fruit3"),
	std::pair<std::string, std::string>("fruit", "fruit4"),
	std::pair<std::string, std::string>("fruit2", "fruit3"),
	std::pair<std::string, std::string>("fruit2", "fruit4"),
	std::pair<std::string, std::string>("fruit3", "fruit4") };

std::unordered_set<std::string> callerList = {"fruit", "fruit2", "fruit3"};
std::unordered_set<std::string> calleeList = {"fruit2", "fruit3", "fruit4"};

// getProcCalledBy(stm) stm = {2, 9, 12, 14}
std::string procCalled[4] = {"fruit2", "fruit3", "fruit3", "fruit4"};
// getStmCalling(proc) proc = {fruit2, fruit3, fruit4}
std::unordered_set<int> stmCalling[3] = { {2}, {9, 12}, {14} };
std::unordered_set< std::pair<int, std::string>, intStringhash> stmProcCallPairList = 
	{ std::pair<int, std::string>(2, "fruit2"), std::pair<int, std::string>(9, "fruit3"),
	std::pair<int, std::string>(12, "fruit3"), std::pair<int, std::string>(14, "fruit4") };