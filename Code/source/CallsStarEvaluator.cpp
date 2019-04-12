#pragma once

#include "CallsStarEvaluator.h"	

/*
Evaluate trivial CallsStar clause 
*/
std::string CallsStarEvaluator::isTrivial(std::string firstArgument, std::string secondArgument) {
	bool result;
	if (firstArgument == "_") {
		if (secondArgument == "_") {
			result = PKB().hasCallRelation();
		}
		else if (QueryUtility::isQuoted(secondArgument)) {
			result = PKB().isCallee(QueryUtility::trimFrontEnd(secondArgument));
		}
	}
	else if (QueryUtility::isQuoted(firstArgument)) {
		if (secondArgument == "_") {
			result = PKB().isCaller(QueryUtility::trimFrontEnd(firstArgument));
		}
		else if (QueryUtility::isQuoted(secondArgument)) {
			result = PKB().hasCallStarPair(QueryUtility::trimFrontEnd(firstArgument),
				QueryUtility::trimFrontEnd(secondArgument));
		}
	}
	return QueryUtility::truthValue(result);
}

/*
Evaluate non trivial CallsStar clause
*/
std::unordered_map<std::string, std::vector<std::string>> CallsStarEvaluator::evaluate(
	std::unordered_map<std::string, std::string> declarations,
	std::string firstArgument, std::string secondArgument) {
	std::unordered_map<std::string, std::vector<std::string>> tableResult;
	std::vector<std::string> emptyVec;
	if (QueryUtility::isSynonym(firstArgument)) {
		if (secondArgument == "_") {
			tableResult = QueryUtility::to_mapvec(firstArgument, PKB().getAllCallers());
		}
		else if (QueryUtility::isQuoted(secondArgument)) {
			tableResult = QueryUtility::to_mapvec(firstArgument, PKB().getCallAnc(
				QueryUtility::trimFrontEnd(secondArgument)));
		}
		else if (firstArgument == secondArgument) {
			tableResult.insert({ firstArgument, emptyVec });
		}
		else {
			tableResult = QueryUtility::to_mapvec(firstArgument, secondArgument,
				PKB().getCallStarPairs());
		}
	}
	if (QueryUtility::isSynonym(secondArgument)) {
		if (firstArgument == "_") {
			tableResult = QueryUtility::to_mapvec(secondArgument, PKB().getAllCallees());
		}
		else if (QueryUtility::isQuoted(firstArgument)) {
			tableResult = QueryUtility::to_mapvec(secondArgument, PKB().getCallDesc(
				QueryUtility::trimFrontEnd(firstArgument)));
		}
	}
	return tableResult;
}