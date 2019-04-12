#pragma once

#include "WhileEvaluator.h"

/*
Evaluate while pattern
*/
std::unordered_map<std::string, std::vector<std::string>> WhileEvaluator::evaluate(
	std::unordered_map<std::string, std::string> declarations,
	std::string patternSynonym, std::string firstArgument, std::string secondArgument) {
	if (firstArgument == "_") {
		return QueryUtility::to_mapvec(patternSynonym, PKB().getAllWhileWithControls());
	}
	else if (QueryUtility::isQuoted(firstArgument)) {
		return QueryUtility::to_mapvec(patternSynonym, PKB().getWhileStmWithControlVariable(
			QueryUtility::trimFrontEnd(firstArgument)));
	}
	else {
		return QueryUtility::to_mapvec(patternSynonym, firstArgument,
			PKB().getWhileStmControlVariablePair());
	}
}