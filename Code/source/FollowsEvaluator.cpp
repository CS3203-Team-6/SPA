#pragma once

#include "FollowsEvaluator.h"	

/*
Evaluate trivial Follows clause.
*/
std::string FollowsEvaluator::isTrivial(std::string firstArgument, std::string secondArgument) {
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
			result = PKB().getFollower(stoi(firstArgument)) == stoi(secondArgument);
		};
	}
	return QueryUtility::truthValue(result);
}

/* 
Evaluate non trivial Follows clause
*/
std::unordered_map<std::string, std::vector<std::string>> FollowsEvaluator::evaluate(
	std::unordered_map<std::string, std::string> declarations,
	std::string firstArgument, std::string secondArgument) {
	std::unordered_map<std::string, std::vector<std::string>> tableResult;
	std::vector<std::string> emptyVec;
	if (QueryUtility::isSynonym(firstArgument)) {
		if (secondArgument == "_") {
			tableResult = QueryUtility::to_mapvec(firstArgument, PKB().getAllFollowed());
		}
		else if (QueryUtility::isInteger(secondArgument)) {
			int result = PKB().getStmFollowedBy(stoi(secondArgument));
			if (result > 0) {
				tableResult = QueryUtility::to_mapvec(firstArgument, result);
			}
			else {
				tableResult.insert({ firstArgument, emptyVec });
			}
		}
		else if (firstArgument == secondArgument) {
			tableResult.insert({ firstArgument, emptyVec });
		}
		else {
			tableResult = QueryUtility::to_mapvec(firstArgument, secondArgument,
				PKB().getFollowPairs());
		}
	}
	if (QueryUtility::isSynonym(secondArgument)) {
		if (firstArgument == "_") {
			tableResult = QueryUtility::to_mapvec(secondArgument, PKB().getAllFollowers());
		}
		else if (QueryUtility::isInteger(firstArgument)) {
			int result = PKB().getFollower(stoi(firstArgument));
			if (result > 0) {
				tableResult = QueryUtility::to_mapvec(secondArgument, result);
			}
			else {
				tableResult.insert({ secondArgument, emptyVec });
			}
		}
	}
	return tableResult;
}