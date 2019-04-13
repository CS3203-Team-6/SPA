#include "NextStorage.h"


std::unordered_map<int, nRelationships> NextStorage::nextTable;
std::unordered_set< std::pair<int, int>, intPairhash> NextStorage::nextPairList;
std::unordered_set<int> NextStorage::nextList;
std::unordered_set<int> NextStorage::prevList;

NextStorage::NextStorage()
{
}

bool NextStorage::addNext(int prev, int next)
{
	// if next Pair is already added
	if (!nextPairList.emplace(std::pair<int, int>(prev, next)).second)
	{
		return false;
	}

	// if prev already exist in nextTable
	if (!nextTable.emplace(prev, nRelationships{ { next }, {} }).second)
	{
		nextTable.find(prev)->second.next.emplace(next);
	}

	// if next already exist in nextTable
	if (!nextTable.emplace(next, nRelationships{ {}, { prev } }).second)
	{
		nextTable.find(next)->second.previous.emplace(prev);
	}

	nextList.emplace(next);
	prevList.emplace(prev);
	return true;
}

bool NextStorage::isEmpty()
{
	return nextTable.size() == 0;
}

std::unordered_set<int> NextStorage::getNext(int ln)
{
	if (nextTable.find(ln) != nextTable.end())
	{
		return nextTable.at(ln).next;
	}
	return {};
}

std::unordered_set<int> NextStorage::getPrev(int ln)
{
	if (nextTable.find(ln) != nextTable.end())
	{
		return nextTable.at(ln).previous;
	}
	return {};
}

std::unordered_set<int> NextStorage::getAllNext()
{
	return nextList;
}

std::unordered_set<int> NextStorage::getAllPrev()
{
	return prevList;
}

std::unordered_set<std::pair<int, int>, intPairhash> NextStorage::getNextPairs()
{
	return nextPairList;
}

void NextStorage::clear()
{
	nextTable.clear();
	nextPairList.clear();
	nextList.clear();
	prevList.clear();
}
