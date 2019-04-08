#pragma once

#include <unordered_map>
#include <unordered_set>
#include <utility>

#include "Hasher.h"

/* 
	A structure to contain:	the statement being followed directly (followed),
							it's immediate follower (follower),
							a list of statements that being directly/indirectly followed (allFollowed),
							a list of statements directly/indirectly following it (allFollowers)
	If such statement or list does not exist, it'll be set to 0 or {} respectively
*/
struct fRelationships
{
	int followed;
	int follower;
	std::unordered_set<int> allFollowed;
	std::unordered_set<int> allFollowers;
};

/*
	A storage class to store the Follows & Follows* relationships of statements
*/
class FollowStorage {
public:
	FollowStorage();

	/*
		Adds the follows relation into the various lists in the storage
		Returns false if	1) the pair is already stored
							2) the followed statement has another follower stored
							3) the follower is following another statement
	*/
	bool addFollow(int followed, int follower);

	/*
		Sets "allFollowers" of 'followed'
		Each followed - follower pair is entered into followStarPairList
		If 'followed' already has a list of followers, it is not replaced and it return false
	*/
	bool setAllFollowing(int followed, std::unordered_set<int> followers);

	/*
		Sets "allFollowed" of 'follower'
		Each followed - follower pair is entered into followStarPairList
		If 'follower' already has a list of followed, it is not replaced and it return false
	*/
	bool setAllFollowedBy(int follower, std::unordered_set<int> followed);

	// returns true if followTable is empty
	bool isEmpty();

	// returns true if the specified follows* pair is found
	bool hasFollowStarPair(std::pair<int, int> pair);

	/*
		return the statement following 'stm'
		return 0 if 'stm' is not found
	*/
	int getFollower(int stm);

	/*
		return the statement followed by 'stm'
		return 0 if 'stm' is not found
	*/
	int getStmFollowedBy(int stm);

	/*
		return a list of statements that is directly/indirectly following 'stm'
		return an empty set if 'stm' is not found
	*/
	std::unordered_set<int> getAllFollowing(int stm);

	/*
		return a list of statements that is directly/indirectly followed by 'stm'
		return an empty set if 'stm' is not found
	*/
	std::unordered_set<int> getAllFollowedBy(int stm);

	// returns a list of all statements that follows another
	std::unordered_set<int> getAllFollowers();

	// returns a list of all statements that is followed by another
	std::unordered_set<int> getAllFollowed();

	// returns a list of all follows pairs
	std::unordered_set< std::pair<int, int>, intPairhash> getFollowPairs();

	// returns a list of all follows* pairs
	std::unordered_set< std::pair<int, int>, intPairhash> getFollowStarPairs();

	// empty the entire storage
	void erase();

private:
	static std::unordered_map<int, fRelationships> followTable;
	static std::unordered_set< std::pair<int, int>, intPairhash> followPairList;
	static std::unordered_set< std::pair<int, int>, intPairhash> followStarPairList;
	static std::unordered_set<int> followerList;
	static std::unordered_set<int> followedList;
};