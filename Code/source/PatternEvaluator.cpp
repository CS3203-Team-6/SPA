#pragma once

#include "PatternEvaluator.h"

/*
The function evaluates
pattern clauses
*/
std::unordered_map<std::string, std::vector<std::string>> PatternEvaluator::evaluate(
	std::unordered_map<std::string, std::string> declarations,
	std::string patternSynonym, std::string firstArgument, std::string secondArgument) {
	std::string patternType = declarations[patternSynonym];

	if (patternType == "assign") {
		return AssignEvaluator::evaluate(declarations, patternSynonym, firstArgument, secondArgument);
	}
	else if (patternType == "if") {
		return IfEvaluator::evaluate(declarations, patternSynonym, firstArgument, secondArgument);
	}
	else if (patternType == "while") {
		return WhileEvaluator::evaluate(declarations, patternSynonym, firstArgument, secondArgument);
	}
	std::unordered_map<std::string, std::vector<std::string>> emptyMap;
	return emptyMap;
}