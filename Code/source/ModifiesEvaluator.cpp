#pragma once

#include "ModifiesEvaluator.h"	

/*
Evaluate trivial Modifies clause
*/
std::string ModifiesEvaluator::isTrivial(std::string firstArgument, std::string secondArgument) {
	bool result;
	if (QueryUtility::isInteger(firstArgument)) {
		if (QueryUtility::isQuoted(secondArgument)) {
			result = PKB().isStmModifying(stoi(firstArgument), QueryUtility::trimFrontEnd(secondArgument));
		}
		else if (secondArgument == "_") {
			result = PKB().getVarModifiedByStm(stoi(firstArgument)).size() > 0;
		}
	}
	else if (QueryUtility::isQuoted(firstArgument)) {
		if (QueryUtility::isQuoted(secondArgument)) {
			result = PKB().isProcModifying(QueryUtility::trimFrontEnd(firstArgument),
				QueryUtility::trimFrontEnd(secondArgument));
		}
		else if (secondArgument == "_") {
			result = PKB().getVarModifiedByProc(QueryUtility::trimFrontEnd(firstArgument)).size() > 0;
		}
	}
	return QueryUtility::truthValue(result);
}

/*
Evaluate non trivial Modifies clause
*/
std::unordered_map<std::string, std::vector<std::string>> ModifiesEvaluator::evaluate(
	std::unordered_map<std::string, std::string> declarations,
	std::string firstArgument, std::string secondArgument) {
	std::unordered_map<std::string, std::vector<std::string>> tableResult;
	std::vector<std::string> emptyVec;
	if (QueryUtility::isSynonym(secondArgument)) {
		if (QueryUtility::isInteger(firstArgument)) {
			tableResult = QueryUtility::to_mapvec(secondArgument, PKB().getVarModifiedByStm(
				stoi(firstArgument)));
		}
		else if (QueryUtility::isQuoted(firstArgument)) {
			tableResult = QueryUtility::to_mapvec(secondArgument, PKB().getVarModifiedByProc(
				QueryUtility::trimFrontEnd(firstArgument)));
		}
		else if (declarations[firstArgument] == "procedure") {
			return QueryUtility::to_mapvec(firstArgument, secondArgument,
				PKB().getProcVarModifyPairs());
		}
		else {
			tableResult = QueryUtility::to_mapvec(firstArgument, secondArgument,
				PKB().getStmVarModifyPairs());
		}
	}
	if (declarations[firstArgument] != "procedure") {
		if (secondArgument == "_") {
			tableResult = QueryUtility::to_mapvec(firstArgument, PKB().getStmModifying(""));
		}
		else if (QueryUtility::isQuoted(secondArgument)) {
			tableResult = QueryUtility::to_mapvec(firstArgument, PKB().getStmModifying(
				QueryUtility::trimFrontEnd(secondArgument)));
		}
	}
	else {
		if (secondArgument == "_") {
			tableResult = QueryUtility::to_mapvec(firstArgument, PKB().getProcModifying(""));
		}
		else if (QueryUtility::isQuoted(secondArgument)) {
			tableResult = QueryUtility::to_mapvec(firstArgument, PKB().getProcModifying(
				QueryUtility::trimFrontEnd(secondArgument)));
		}
	}
	return tableResult;
}