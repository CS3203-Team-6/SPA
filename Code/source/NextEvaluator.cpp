#pragma once

#include "NextEvaluator.h"	

/*
Evaluate trivial Next clause 
*/
std::string NextEvaluator::isTrivial(std::string firstArgument, std::string secondArgument) {
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
			result = PKB().getNext(stoi(firstArgument)).count(stoi(secondArgument)) == 1;
		};
	}
	return QueryUtility::truthValue(result);
}

/*
Evaluate non trivial Next clause
*/
std::unordered_map<std::string, std::vector<std::string>> NextEvaluator::evaluate(
	std::unordered_map<std::string, std::string> declarations,
	std::string firstArgument, std::string secondArgument) {
	std::unordered_map<std::string, std::vector<std::string>> tableResult;
	std::vector<std::string> emptyVec;
	if (QueryUtility::isSynonym(firstArgument)) {
		if (secondArgument == "_") {
			tableResult = QueryUtility::to_mapvec(firstArgument, PKB().getAllPrev());
		}
		else if (QueryUtility::isInteger(secondArgument)) {
			tableResult = QueryUtility::to_mapvec(firstArgument, PKB().getPrev(
				stoi(secondArgument)));
		}
		else if (firstArgument == secondArgument) {
			tableResult.insert({ firstArgument, emptyVec });
		}
		else {
			tableResult = QueryUtility::to_mapvec(firstArgument, secondArgument,
				PKB().getNextPairs());
		}
	}
	if (QueryUtility::isSynonym(secondArgument)) {
		if (firstArgument == "_") {
			tableResult = QueryUtility::to_mapvec(secondArgument, PKB().getAllNext());
		}
		else if (QueryUtility::isInteger(firstArgument)) {
			tableResult = QueryUtility::to_mapvec(secondArgument, PKB().getNext(
				stoi(firstArgument)));
		}
	}
	return tableResult;
}