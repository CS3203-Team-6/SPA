#pragma once

#include "IfEvaluator.h"

/*
Evaluate if pattern
*/
std::unordered_map<std::string, std::vector<std::string>> IfEvaluator::evaluate(
	std::unordered_map<std::string, std::string> declarations,
	std::string patternSynonym, std::string firstArgument, std::string secondArgument) {
	if (firstArgument == "_") {
		return QueryUtility::to_mapvec(patternSynonym, PKB().getAllIfWithControls());
	}
	else if (QueryUtility::isQuoted(firstArgument)) {
		return QueryUtility::to_mapvec(patternSynonym, PKB().getIfStmWithControlVariable(
			QueryUtility::trimFrontEnd(firstArgument)));
	}
	else {
		return QueryUtility::to_mapvec(patternSynonym, firstArgument,
			PKB().getIfStmControlVariablePair());
	}
}