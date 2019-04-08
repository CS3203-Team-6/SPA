#pragma once

#include "ParentStorage.h"

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
Parent
store.addParent_Child(5, 6);
store.addParent_Child(6, 7);
store.addParent_Child(5, 8);
store.addParent_Child(10, 11);
store.setAncestors(6, ancestors[0]);
store.setAncestors(7, ancestors[1]);
store.setAncestors(8, ancestors[2]);
store.setAncestors(11, ancestors[3]);
store.setDescendants(5, descendants[0]);
store.setDescendants(6, descendants[1]);
store.setDescendants(10, descendants[2]);
*/
std::unordered_set<int> ancestors[4] = { {5}, {5, 6}, {5}, {10} };
std::unordered_set<int> descendants[3] = { {6, 7, 8}, {7}, {11} };
std::unordered_set<int> parents = { 5, 6, 10 };
std::unordered_set<int> children = { 6, 7, 8, 11 };

std::unordered_set<std::pair<int, int>, intPairhash> parentPair =
	{std::pair<int, int>(5, 6), std::pair<int, int>(6, 7), std::pair<int, int>(5, 8),
	std::pair<int, int>(10, 11) };

std::unordered_set<std::pair<int, int>, intPairhash> parentStarPair =
	{std::pair<int, int>(5, 6), std::pair<int, int>(5, 7), std::pair<int, int>(5, 8),
	std::pair<int, int>(6, 7), std::pair<int, int>(10, 11) };