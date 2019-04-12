#pragma once

#include "CallsEvaluator.h"	

/*
Checks if Calls clause is trivial
*/
std::string CallsEvaluator::isTrivial(std::string firstArgument, std::string secondArgument) {
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
			result = PKB().getCallee(QueryUtility::trimFrontEnd(firstArgument)).count(
				QueryUtility::trimFrontEnd(secondArgument)) == 1;
		}
	}
	return QueryUtility::truthValue(result);
}

/*
Evaluate non trivial Calls clause
*/
std::unordered_map<std::string, std::vector<std::string>> CallsEvaluator::evaluate(
	std::unordered_map<std::string, std::string> declarations,
	std::string firstArgument, std::string secondArgument) {
	std::unordered_map<std::string, std::vector<std::string>> tableResult;
	std::vector<std::string> emptyVec;
	if (QueryUtility::isSynonym(firstArgument)) {
		if (secondArgument == "_") {
			tableResult = QueryUtility::to_mapvec(firstArgument, PKB().getAllCallers());
		}
		else if (QueryUtility::isQuoted(secondArgument)) {
			tableResult = QueryUtility::to_mapvec(firstArgument, PKB().getCaller(
				QueryUtility::trimFrontEnd(secondArgument)));
		}
		else if (firstArgument == secondArgument) {
			tableResult.insert({ firstArgument, emptyVec });
		}
		else {
			tableResult = QueryUtility::to_mapvec(firstArgument, secondArgument,
				PKB().getCallPairs());
		}
	}
	if (QueryUtility::isSynonym(secondArgument)) {
		if (firstArgument == "_") {
			tableResult = QueryUtility::to_mapvec(secondArgument, PKB().getAllCallees());
		}
		else if (QueryUtility::isQuoted(firstArgument)) {
			tableResult = QueryUtility::to_mapvec(secondArgument, PKB().getCallee(
				QueryUtility::trimFrontEnd(firstArgument)));
		}
	}
	return tableResult;
}