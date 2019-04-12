#pragma once

#include "AssignEvaluator.h"

/*
Evaluate assign pattern
*/
std::unordered_map<std::string, std::vector<std::string>> AssignEvaluator::evaluate(
	std::unordered_map<std::string, std::string> declarations,
	std::string patternSynonym, std::string firstArgument, std::string secondArgument) {
	if (secondArgument == "_") {
		if (firstArgument == "_") {
			return QueryUtility::getStmtsMap(declarations, patternSynonym);
		}
		else if (QueryUtility::isQuoted(firstArgument)) {
			return QueryUtility::to_mapvec(patternSynonym,
				PKB().findPattern(QueryUtility::trimFrontEnd(firstArgument), "", false));
		}
		return QueryUtility::to_mapvec(patternSynonym, firstArgument,
			PKB().findPatternPairs("", false));
	}
	else if (QueryUtility::isQuoted(secondArgument)) {
		secondArgument = QueryUtility::trimFrontEnd(secondArgument);
		secondArgument = ExpressionUtil::convertInfixToPrefix(secondArgument);
		if (firstArgument == "_") {
			return QueryUtility::to_mapvec(patternSynonym,
				PKB().findPattern(secondArgument, true));
		}
		else if (QueryUtility::isQuoted(firstArgument)) {
			return QueryUtility::to_mapvec(patternSynonym,
				PKB().findPattern(QueryUtility::trimFrontEnd(firstArgument), secondArgument, true));
		}
		return QueryUtility::to_mapvec(patternSynonym, firstArgument,
			PKB().findPatternPairs(secondArgument, true));
	}
	else {
		secondArgument = QueryUtility::trimFrontEnd(QueryUtility::trimFrontEnd(secondArgument));
		secondArgument = ExpressionUtil::convertInfixToPrefix(secondArgument);
		if (firstArgument == "_") {
			return QueryUtility::to_mapvec(patternSynonym,
				PKB().findPattern(firstArgument, false));
		}
		else if (QueryUtility::isQuoted(firstArgument)) {
			return QueryUtility::to_mapvec(patternSynonym,
				PKB().findPattern(QueryUtility::trimFrontEnd(firstArgument), secondArgument, false));
		}
		return QueryUtility::to_mapvec(patternSynonym, firstArgument,
			PKB().findPatternPairs(secondArgument, false));
	}
}