#pragma once

#include "UsesEvaluator.h"	

/*
Evaluate trivial Uses clause 
*/
std::string UsesEvaluator::isTrivial(std::string firstArgument, std::string secondArgument) {
	bool result;
	if (QueryUtility::isInteger(firstArgument)) {
		if (QueryUtility::isQuoted(secondArgument)) {
			result = PKB().isStmUsing(stoi(firstArgument), QueryUtility::trimFrontEnd(secondArgument));
		}
		else if (secondArgument == "_") {
			result = PKB().getVarUsedByStm(stoi(firstArgument)).size() > 0;
		}
	}
	else if (QueryUtility::isQuoted(firstArgument)) {
		if (QueryUtility::isQuoted(secondArgument)) {
			result = PKB().isProcUsing(QueryUtility::trimFrontEnd(firstArgument), 
				QueryUtility::trimFrontEnd(secondArgument));
		}
		else if (secondArgument == "_") {
			result = PKB().getVarUsedByProc(QueryUtility::trimFrontEnd(firstArgument)).size() > 0;
		}
	}
	return QueryUtility::truthValue(result);
}

/*
Evaluate non trivial Uses clause
*/
std::unordered_map<std::string, std::vector<std::string>> UsesEvaluator::evaluate(
	std::unordered_map<std::string, std::string> declarations,
	std::string firstArgument, std::string secondArgument) {
	std::unordered_map<std::string, std::vector<std::string>> tableResult;
	std::vector<std::string> emptyVec;
	if (QueryUtility::isSynonym(secondArgument)) {
		if (QueryUtility::isInteger(firstArgument)) {
			tableResult = QueryUtility::to_mapvec(secondArgument, PKB().getVarUsedByStm(
				stoi(firstArgument)));
		}
		else if (QueryUtility::isQuoted(firstArgument)) {
			tableResult = QueryUtility::to_mapvec(secondArgument, PKB().getVarUsedByProc(
				QueryUtility::trimFrontEnd(firstArgument)));
		}
		else if (declarations[firstArgument] == "procedure") {
			return QueryUtility::to_mapvec(firstArgument, secondArgument,
				PKB().getProcVarUsePairs());
		}
		else {
			tableResult = QueryUtility::to_mapvec(firstArgument, secondArgument,
				PKB().getStmVarUsePairs());
		}
	}
	if (declarations[firstArgument] != "procedure") {
		if (secondArgument == "_") {
			tableResult = QueryUtility::to_mapvec(firstArgument, PKB().getStmUsing(""));
		}
		else if (QueryUtility::isQuoted(secondArgument)) {
			tableResult = QueryUtility::to_mapvec(firstArgument, PKB().getStmUsing(
				QueryUtility::trimFrontEnd(secondArgument)));
		}
	}
	else {
		if (secondArgument == "_") {
			tableResult = QueryUtility::to_mapvec(firstArgument, PKB().getProcUsing(""));
		}
		else if (QueryUtility::isQuoted(secondArgument)) {
			tableResult = QueryUtility::to_mapvec(firstArgument, PKB().getProcUsing(
				QueryUtility::trimFrontEnd(secondArgument)));
		}
	}
	return tableResult;
}