#pragma once

#include "ParentStarEvaluator.h"	

/*
Evaluate trivial ParentStar clause
*/
std::string ParentStarEvaluator::isTrivial(std::string firstArgument, std::string secondArgument) {
	bool result;
	if (firstArgument == "_") {
		if (secondArgument == "_") {
			result = PKB().hasParentRelation();
		}
		else if (QueryUtility::isInteger(secondArgument)) {
			result = PKB().isChild(stoi(secondArgument)) > 0;
		}
	}
	else if (QueryUtility::isInteger(firstArgument)) {
		if (secondArgument == "_") {
			result = PKB().isParent(stoi(firstArgument)) > 0;
		}
		else if (QueryUtility::isInteger(secondArgument)) {
			result = PKB().hasAncDescPair(stoi(firstArgument), stoi(secondArgument));
		}
	}
	return QueryUtility::truthValue(result);
}

/*
Evaluate non trivial ParentStar clause
*/
std::unordered_map<std::string, std::vector<std::string>> ParentStarEvaluator::evaluate(
	std::unordered_map<std::string, std::string> declarations,
	std::string firstArgument, std::string secondArgument) {
	std::unordered_map<std::string, std::vector<std::string>> tableResult;
	std::vector<std::string> emptyVec;
	if (QueryUtility::isSynonym(firstArgument)) {
		if (secondArgument == "_") {
			tableResult = QueryUtility::to_mapvec(firstArgument, PKB().getAllParents());
		}
		else if (QueryUtility::isInteger(secondArgument)) {
			tableResult = QueryUtility::to_mapvec(firstArgument, PKB().getAncestors(
				stoi(secondArgument)));
		}
		else if (firstArgument == secondArgument) {
			tableResult.insert({ firstArgument, emptyVec });
		}
		else {
			tableResult = QueryUtility::to_mapvec(firstArgument, secondArgument,
				PKB().getAncDescPairs());
		}
	}
	if (QueryUtility::isSynonym(secondArgument)) {
		if (firstArgument == "_") {
			tableResult = QueryUtility::to_mapvec(secondArgument, PKB().getAllChildren());
		}
		else if (QueryUtility::isInteger(firstArgument)) {
			tableResult = QueryUtility::to_mapvec(secondArgument, PKB().getDescendants(
				stoi(firstArgument)));
		}
	}
	return tableResult;
}