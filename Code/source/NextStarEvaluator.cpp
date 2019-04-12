#pragma once

#include "NextStarEvaluator.h"	

/*
Evaluate trivial NextStar clause 
*/
std::string NextStarEvaluator::isTrivial(std::string firstArgument, std::string secondArgument) {
	bool result;
	if (firstArgument == "_") {
		if (secondArgument == "_") {
			result = PKB().hasNextRelation();
		}
		else if (QueryUtility::isInteger(secondArgument)) {
			result = PKB().getPrev(stoi(secondArgument)).size() > 0;
		}
	}
	else if (QueryUtility::isInteger(firstArgument)) {
		if (secondArgument == "_") {
			result = PKB().getNext(stoi(firstArgument)).size() > 0;
		}
		else if (QueryUtility::isInteger(secondArgument)) {
			result = PKB().hasNextStarPair(stoi(firstArgument), stoi(secondArgument)) == 1;
		};
	}
	return QueryUtility::truthValue(result);
}

/*
Evaluate non trivial NextStar clause
*/
std::unordered_map<std::string, std::vector<std::string>> NextStarEvaluator::evaluate(
	std::unordered_map<std::string, std::string> declarations,
	std::string firstArgument, std::string secondArgument) {
	std::unordered_map<std::string, std::vector<std::string>> tableResult;
	std::vector<std::string> emptyVec;
	if (QueryUtility::isSynonym(firstArgument)) {
		if (secondArgument == "_") {
			tableResult = QueryUtility::to_mapvec(firstArgument, PKB().getAllPrev());
		}
		else if (QueryUtility::isInteger(secondArgument)) {
			tableResult = QueryUtility::to_mapvec(firstArgument, PKB().getAllLnBefore(
				stoi(secondArgument)));
		}
		else if (firstArgument == secondArgument) {
			tableResult = QueryUtility::to_mapvec(firstArgument, QueryUtility::getTwin(
				PKB().getNextStarPairs()));
		}
		else {
			tableResult = QueryUtility::to_mapvec(firstArgument, secondArgument,
				PKB().getNextStarPairs());
		}
	}
	if (QueryUtility::isSynonym(secondArgument)) {
		if (firstArgument == "_") {
			tableResult = QueryUtility::to_mapvec(secondArgument, PKB().getAllNext());
		}
		else if (QueryUtility::isInteger(firstArgument)) {
			tableResult = QueryUtility::to_mapvec(secondArgument, PKB().getAllLnAfter(
				stoi(firstArgument)));
		}
	}
	return tableResult;
}