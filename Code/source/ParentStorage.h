#pragma once

#include <unordered_map>
#include <unordered_set>
#include <utility>

using namespace std;

#include "Hasher.h"

struct pRelationships
{
	int parent;
	unordered_set<int> children;
	unordered_set<int> ancestors;
	unordered_set<int> descendants;
};

class ParentStorage {
public:
	ParentStorage();

	bool addParent_Child(int parent, int child);
	bool setAncestors(int descendant, unordered_set<int> ancestors);
	bool setDescendants(int ancestor, unordered_set<int> descendants);

	bool isEmpty();
	bool isParent(int stm);
	bool isChild(int stm);
	bool containsAnc_Desc(pair<int, int> pair);

	int getParentOf(int stm);
	unordered_set<int> getChildrenOf(int stm);
	unordered_set<int> getAncestorsOf(int stm);
	unordered_set<int> getDescendantsOf(int stm);
	unordered_set<int> getParentList();
	unordered_set<int> getChildrenList();
	unordered_set< pair<int, int>, intPairhash> getParent_ChildList();
	unordered_set< pair<int, int>, intPairhash> getAnc_DescList();
	unordered_set<int> getRootList();

private:
	static unordered_map<int, pRelationships> parentTable;
	static unordered_set< pair<int, int>, intPairhash> parent_ChildPairList;
	static unordered_set< pair<int, int>, intPairhash> anc_DescPairList;
	static unordered_set<int> parentList;
	static unordered_set<int> childrenList;
	static unordered_set<int> rootList;
};