#pragma once

#include "NextStorage.h"

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
Next
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
*/

// stm = 1, 2, 3, 4, 5, 6, 7, 8, 10, 11, 12, 13
std::unordered_set<int> next[12] = { {2}, {3}, {4}, {5}, {6, 8}, {7}, {6}, {9}, {11}, {12}, {11}, {14} };
// stm = 2, 3, 4, 5, 6, 7, 8, 9, 11, 12, 14
std::unordered_set<int> previous[11] = { {1}, {2}, {3}, {4}, {5, 7}, {6}, {5}, {8}, {10, 12}, {11}, {13} };

std::unordered_set< std::pair<int, int>, intPairhash> nextPairList = { std::pair<int, int>(1, 2),
	std::pair<int, int>(2, 3), std::pair<int, int>(3, 4), std::pair<int, int>(4, 5),
	std::pair<int, int>(5, 6), std::pair<int, int>(6, 7), std::pair<int, int>(7, 6),
	std::pair<int, int>(5, 8), std::pair<int, int>(8, 9), std::pair<int, int>(10, 11),
	std::pair<int, int>(11, 12), std::pair<int, int>(12, 11), std::pair<int, int>(13, 14) };

std::unordered_set<int> nextList = { 2, 3, 4, 5, 6, 7, 8, 9, 11, 12, 14 };
std::unordered_set<int> prevList = { 1, 2, 3, 4, 5, 6, 7, 8, 10, 11, 12, 13};