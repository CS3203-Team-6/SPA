#pragma once

#include "FollowsStarEvaluator.h"	

/*
Evaluate trivial FollowsStar clause 
*/
std::string FollowsStarEvaluator::isTrivial(std::string firstArgument, std::string secondArgument) {
	bool result;
	if (firstArgument == "_") {
		if (secondArgument == "_") {
			result = PKB().hasFollowRelation();
		}
		else if (QueryUtility::isInteger(secondArgument)) {
			result = PKB().getStmFollowedBy(stoi(secondArgument)) > 0;
		}
	}
	else if (QueryUtility::isInteger(firstArgument)) {
		if (secondArgument == "_") {
			result = PKB().getFollower(stoi(firstArgument)) > 0;
		}
		else if (QueryUtility::isInteger(secondArgument)) {
			result = PKB().hasFollowStarPair(stoi(firstArgument), stoi(secondArgument));
		}
	}
	return QueryUtility::truthValue(result);
}

/*
Evaluate non trivial FollowsStar clause
*/
std::unordered_map<std::string, std::vector<std::string>> FollowsStarEvaluator::evaluate(
	std::unordered_map<std::string, std::string> declarations,
	std::string firstArgument, std::string secondArgument) {
	std::unordered_map<std::string, std::vector<std::string>> tableResult;
	std::vector<std::string> emptyVec;
	if (QueryUtility::isSynonym(firstArgument)) {
		if (secondArgument == "_") {
			tableResult = QueryUtility::to_mapvec(firstArgument, PKB().getAllFollowed());
		}
		else if (QueryUtility::isInteger(secondArgument)) {
			tableResult = QueryUtility::to_mapvec(firstArgument, PKB().getAllFollowedBy(
				stoi(secondArgument)));
		}
		else if (firstArgument == secondArgument) {
			tableResult.insert({ firstArgument, emptyVec });
		}
		else {
			tableResult = QueryUtility::to_mapvec(firstArgument, secondArgument,
				PKB().getFollowStarPairs());
		}
	}
	if (QueryUtility::isSynonym(secondArgument)) {
		if (firstArgument == "_") {
			tableResult = QueryUtility::to_mapvec(secondArgument, PKB().getAllFollowers());
		}
		else if (QueryUtility::isInteger(firstArgument)) {
			tableResult = QueryUtility::to_mapvec(secondArgument, PKB().getAllFollowing(
				stoi(firstArgument)));
		}
	}
	return tableResult;
}