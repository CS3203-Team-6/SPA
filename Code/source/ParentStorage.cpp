#include "ParentStorage.h"

std::unordered_map<int, pRelationships> ParentStorage::parentTable;
std::unordered_set< std::pair<int, int>, intPairhash> ParentStorage::parent_ChildPairList;
std::unordered_set< std::pair<int, int>, intPairhash> ParentStorage::anc_DescPairList;
std::unordered_set<int> ParentStorage::parentList;
std::unordered_set<int> ParentStorage::childrenList;

ParentStorage::ParentStorage()
{
}

bool ParentStorage::addParent_Child(int parent, int child)
{
	// if Parent-Child Pair is already added
	if (!parent_ChildPairList.emplace(std::pair<int, int>(parent, child)).second)
	{
		return false;
	}

	// if child exist in parentTable and has parent already initialized
	auto itrpr = parentTable.emplace(child, pRelationships{ parent, {}, {}, {} });
	if (!itrpr.second && parentTable.find(child)->second.parent != 0)
	{
		parent_ChildPairList.erase(std::pair<int, int>(parent, child));
		return false;
	}
	else if (!itrpr.second)		// if child exist but does not have parent initialized
	{
		itrpr.first->second.parent = parent;
	}

	// if parent exist in parentTable
	if (!parentTable.emplace(parent, pRelationships{ 0, {child}, {}, {} }).second)
	{
		parentTable.find(parent)->second.children.emplace(child);
	}

	parentList.emplace(parent);
	childrenList.emplace(child);
	return true;
}

bool ParentStorage::setAncestors(int descendant, std::unordered_set<int> ancestors)
{
	if (parentTable.find(descendant)->second.ancestors.size() != 0)
	{
		return false;
	}
	parentTable.find(descendant)->second.ancestors = ancestors;

	for (auto itr = ancestors.cbegin(); itr != ancestors.cend(); ++itr)
	{
		anc_DescPairList.emplace(std::pair<int, int>(*itr, descendant));
	}
	return true;
}

bool ParentStorage::setDescendants(int ancestor, std::unordered_set<int> descendants)
{
	if (parentTable.find(ancestor)->second.descendants.size() != 0)
	{
		return false;
	}
	parentTable.find(ancestor)->second.descendants = descendants;

	for (auto itr = descendants.cbegin(); itr != descendants.cend(); ++itr)
	{
		anc_DescPairList.emplace(std::pair<int, int>(ancestor, *itr));
	}
	return true;
}

bool ParentStorage::isEmpty()
{
	return parentTable.size() == 0;
}

bool ParentStorage::isParent(int stm)
{
	return parentList.find(stm) != parentList.end();
}

bool ParentStorage::isChild(int stm)
{
	return childrenList.find(stm) != childrenList.end();
}

bool ParentStorage::hasAncDescPair(std::pair<int, int> pair)
{
	return anc_DescPairList.find(pair) != anc_DescPairList.end();
}

int ParentStorage::getParent(int stm)
{
	if (parentTable.find(stm) != parentTable.end())
	{
		return parentTable.at(stm).parent;
	}
	return 0;
}

std::unordered_set<int> ParentStorage::getChildren(int stm)
{
	if (parentTable.find(stm) != parentTable.end())
	{
		return parentTable.at(stm).children;
	}
	return {};
}

std::unordered_set<int> ParentStorage::getAncestors(int stm)
{
	if (parentTable.find(stm) != parentTable.end())
	{
		return parentTable.at(stm).ancestors;
	}
	return {};
}

std::unordered_set<int> ParentStorage::getDescendants(int stm)
{
	if (parentTable.find(stm) != parentTable.end())
	{
		return parentTable.at(stm).descendants;
	}
	return {};
}

std::unordered_set<int> ParentStorage::getAllParent()
{
	return parentList;
}

std::unordered_set<int> ParentStorage::getAllChildren()
{
	return childrenList;
}

std::unordered_set<std::pair<int, int>, intPairhash> ParentStorage::getParentChildPairs()
{
	return parent_ChildPairList;
}

std::unordered_set<std::pair<int, int>, intPairhash> ParentStorage::getAncDescPair()
{
	return anc_DescPairList;
}

void ParentStorage::clear()
{
	parentTable.clear();
	parent_ChildPairList.clear();
	anc_DescPairList.clear();
	parentList.clear();
	childrenList.clear();
}
