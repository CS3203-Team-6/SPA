#pragma once

#include <unordered_map>
#include <unordered_set>
#include <utility>

#include "Hasher.h"

/*
	A structure to contain:	the program line to be executed next (next),
							the program line previously executed (previous),
	If such program line does not exist, it'll be set to 0
*/
struct nRelationships
{
	std::unordered_set<int> next;
	std::unordered_set<int> previous;
};

/*
	A storage class to store the Next relationships of program lines
*/
class NextStorage {
public:
	NextStorage();

	/*
		Adds the next relation into the various lists in the storage
		Returns false if the pair is already stored
	*/
	bool addNext(int prev, int next);

	// returns true if nextTable is empty
	bool isEmpty();

	/*
		return a list of program lines that can be executed after 'line'
		return {} if 'line' is not found
	*/
	std::unordered_set<int> getNext(int line);

	/*
		return a list of program lines that can be executed before 'line'
		return {} if 'line' is not found
	*/
	std::unordered_set<int> getPrev(int line);

	// returns a list of all program lines that is executed after another
	std::unordered_set<int> getAllNext();

	// returns a list of all program lines that is before after another
	std::unordered_set<int> getAllPrev();

	// returns a list of all next pairs
	std::unordered_set< std::pair<int, int>, intPairhash> getNextPairs();

	// empty the entire storage
	void erase();
private:
	static std::unordered_map<int, nRelationships> nextTable;
	static std::unordered_set< std::pair<int, int>, intPairhash> nextPairList;
	static std::unordered_set<int> nextList;
	static std::unordered_set<int> prevList;
};
