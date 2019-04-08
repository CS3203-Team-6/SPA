#pragma once

#include "FollowStorage.h"

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
Follows
store.addFollow(1, 2);
store.addFollow(2, 3);
store.addFollow(3, 4);
store.addFollow(4, 5);
store.addFollow(9, 10);
store.setAllFollowing(1, allFollowing[0]);
store.setAllFollowing(2, allFollowing[1]);
store.setAllFollowing(3, allFollowing[2]);
store.setAllFollowing(4, allFollowing[3]);
store.setAllFollowing(9, allFollowing[4]);
store.setAllFollowedBy(2, allFollowedBy[0]);
store.setAllFollowedBy(3, allFollowedBy[1]);
store.setAllFollowedBy(4, allFollowedBy[2]);
store.setAllFollowedBy(5, allFollowedBy[3]);
store.setAllFollowedBy(10, allFollowedBy[4]);
*/
std::unordered_set<int> allFollowing[5] = { {2, 3, 4, 5}, {3, 4, 5}, {4, 5}, {5}, {10} };
std::unordered_set<int> allFollowedBy[5] = { {1}, {1, 2}, {1, 2, 3}, {1, 2, 3, 4}, {9} };
std::unordered_set<int> followers = {2, 3, 4, 5, 10};
std::unordered_set<int> followed = {1, 2, 3, 4, 9};

std::unordered_set<std::pair<int, int>, intPairhash> followPairs = { std::pair<int, int>(1, 2),
	std::pair<int, int>(2, 3), std::pair<int, int>(3, 4), std::pair<int, int>(4, 5),
	std::pair<int, int>(9, 10) };

std::unordered_set<std::pair<int, int>, intPairhash> followStarPairs = { std::pair<int, int>(1, 2),
	std::pair<int, int>(1, 3), std::pair<int, int>(1, 4), std::pair<int, int>(1, 5),
	std::pair<int, int>(9, 10), std::pair<int, int>(2, 3), std::pair<int, int>(2, 4),
	std::pair<int, int>(2, 5), std::pair<int, int>(3, 4), std::pair<int, int>(3, 5),
	std::pair<int, int>(4, 5) };
