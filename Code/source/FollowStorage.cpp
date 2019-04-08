#include "FollowStorage.h"

std::unordered_map<int, fRelationships> FollowStorage::followTable;
std::unordered_set< std::pair<int, int>, intPairhash> FollowStorage::followPairList;
std::unordered_set< std::pair<int, int>, intPairhash> FollowStorage::followStarPairList;
std::unordered_set<int> FollowStorage::followerList;
std::unordered_set<int> FollowStorage::followedList;

FollowStorage::FollowStorage()
{
}

bool FollowStorage::addFollow(int followed, int follower)
{
	// if follows Pair is already added
	if (!followPairList.emplace(std::pair<int, int>(followed, follower)).second)
	{
		return false;
	}

	if (followTable.find(followed) != followTable.end() &&
		followTable.find(followed)->second.follower != 0)
	{
		followPairList.erase(std::pair<int, int>(followed, follower));
		return false;
	}

	if (followTable.find(follower) != followTable.end() &&
		followTable.find(follower)->second.followed != 0)
	{
		followPairList.erase(std::pair<int, int>(followed, follower));
		return false;
	}

	// if followed already exist in followTable
	if (!followTable.emplace(followed, fRelationships{ 0, follower, {}, {} }).second)
	{
		followTable.find(followed)->second.follower = follower;
	}

	// if follower already exist in followTable
	if (!followTable.emplace(follower, fRelationships{ followed, 0, {}, {} }).second)
	{
		followTable.find(follower)->second.followed = followed;
	}

	followedList.emplace(followed);
	followerList.emplace(follower);
	return true;
}

bool FollowStorage::setAllFollowing(int followed, std::unordered_set<int> followers)
{
	if (followTable.find(followed)->second.allFollowers.size() != 0)
	{
		return false;
	}

	followTable.find(followed)->second.allFollowers = followers;

	for (auto itr = followers.cbegin(); itr != followers.cend(); ++itr)
	{
		followStarPairList.emplace(std::pair<int, int>(followed, *itr));
	}
	return true;
}

bool FollowStorage::setAllFollowedBy(int follower, std::unordered_set<int> followed)
{
	if (followTable.find(follower)->second.allFollowed.size() != 0)
	{
		return false;
	}

	followTable.find(follower)->second.allFollowed = followed;

	for (auto itr = followed.cbegin(); itr != followed.cend(); ++itr)
	{
		followStarPairList.emplace(std::pair<int, int>(*itr, follower));
	}
	return true;
}

bool FollowStorage::isEmpty()
{
	return followTable.size() == 0;
}

bool FollowStorage::hasFollowStarPair(std::pair<int, int> pair)
{
	return followStarPairList.find(pair) != followStarPairList.end();
}

int FollowStorage::getFollower(int stm)
{
	if (followTable.find(stm) != followTable.end())
	{
		return followTable.at(stm).follower;
	}
	return 0;
}

int FollowStorage::getStmFollowedBy(int stm)
{
	if (followTable.find(stm) != followTable.end())
	{
		return followTable.at(stm).followed;
	}
	return 0;
}

std::unordered_set<int> FollowStorage::getAllFollowing(int stm)
{
	if (followTable.find(stm) != followTable.end())
	{
		return followTable.at(stm).allFollowers;
	}
	return {};
}

std::unordered_set<int> FollowStorage::getAllFollowedBy(int stm)
{
	if (followTable.find(stm) != followTable.end())
	{
		return followTable.at(stm).allFollowed;
	}
	return {};
}

std::unordered_set<int> FollowStorage::getAllFollowers()
{
	return followerList;
}

std::unordered_set<int> FollowStorage::getAllFollowed()
{
	return followedList;
}

std::unordered_set<std::pair<int, int>, intPairhash> FollowStorage::getFollowPairs()
{
	return followPairList;
}

std::unordered_set<std::pair<int, int>, intPairhash> FollowStorage::getFollowStarPairs()
{
	return followStarPairList;
}

void FollowStorage::erase()
{
	followTable.erase(followTable.begin(), followTable.end());
	followerList.erase(followerList.begin(), followerList.end());
	followedList.erase(followedList.begin(), followedList.end());
	followPairList.erase(followPairList.begin(), followPairList.end());
	followStarPairList.erase(followStarPairList.begin(), followStarPairList.end());
}
