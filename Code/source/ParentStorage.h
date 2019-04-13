#pragma once

#include <unordered_map>
#include <unordered_set>
#include <utility>

#include "Hasher.h"

/*
	A structure to contain:	The statement's Parent,
							a list of it's children,
							a list of ancestors including the direct parent,
							a list of descendants including its' direct children
	If such statement or list does not exist, it'll be set to 0 or {} respectively
*/
struct pRelationships
{
	int parent;
	std::unordered_set<int> children;
	std::unordered_set<int> ancestors;
	std::unordered_set<int> descendants;
};

/*
	A storage class to store the Parent & Parent* relationships of statements
*/
class ParentStorage {
public:
	ParentStorage();

	/*
		Adds the parent relation into the various lists in the storage
		Returns false if	1) the pair is already stored
							2) the child already has another parent
	*/
	bool addParent_Child(int parent, int child);

	/*
		Sets "ancestors" of 'descendant'
		Each ancestor - descendant pair is entered into anc_DescPairList
		If 'descendant' already has a list of ancestors, it is not replaced and it return false
	*/
	bool setAncestors(int descendant, std::unordered_set<int> ancestors);

	/*
		Sets "descendants" of 'ancestor'
		Each ancestor - descendant pair is entered into anc_DescPairList
		If 'ancestor' already has a list of descendants, it is not replaced and it return false
	*/
	bool setDescendants(int ancestor, std::unordered_set<int> descendants);

	// return true if parentTable is empty
	bool isEmpty();

	// returns true if the specified statement is a parent of another
	bool isParent(int stm);

	// returns true if the specified statement is a child of another
	bool isChild(int stm);

	// returns true if the specified parent* pair is found
	bool hasAncDescPair(std::pair<int, int> pair);

	/*
		return the statement that is the parent of 'stm'
		return 0 if 'stm' is not found
	*/
	int getParent(int stm);

	/*
		return the list of statements that is the children of 'stm'
		return an empty set if 'stm' is not found
	*/
	std::unordered_set<int> getChildren(int stm);

	/*
		return the list of statements that is the ancestor of 'stm'
		return an empty set if 'stm' is not found
	*/
	std::unordered_set<int> getAncestors(int stm);

	/*
		return the list of statements that is the descendants of 'stm'
		return an empty set if 'stm' is not found
	*/
	std::unordered_set<int> getDescendants(int stm);

	// returns a list of all statements that is the parent of another
	std::unordered_set<int> getAllParent();

	// returns a list of all statements that is the child of another
	std::unordered_set<int> getAllChildren();

	// returns a list of all parent pairs
	std::unordered_set< std::pair<int, int>, intPairhash> getParentChildPairs();

	// returns a list of all parent* pairs
	std::unordered_set< std::pair<int, int>, intPairhash> getAncDescPair();

	// empty the entire storage
	void clear();

private:
	static std::unordered_map<int, pRelationships> parentTable;
	static std::unordered_set< std::pair<int, int>, intPairhash> parent_ChildPairList;
	static std::unordered_set< std::pair<int, int>, intPairhash> anc_DescPairList;
	static std::unordered_set<int> parentList;
	static std::unordered_set<int> childrenList;
};