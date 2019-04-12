#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include "Optimizer.h"
#include "PatternEvaluator.h"
#include "WithEvaluator.h"

class QueryEvaluator
{
public:
	static std::list<std::string> projectResult(
		std::unordered_map<std::string, std::string> declarations,
		std::vector<std::string> selectedVar,
		std::vector<std::pair<std::string, std::pair<std::string, std::string>>> suchThatCondition,
		std::vector<std::pair<std::string, std::pair<std::string, std::string>>> patternCondition,
		std::vector<std::pair<std::string, std::string>> withCondition);

private:
	static std::list<std::string> toStringList(
		std::unordered_map<std::string, std::string> declarations,
		std::vector<std::string> selectedVar,
		std::unordered_map<std::string, std::vector<std::string>> projectTable);

	static std::string isTrivialGroupTrue(std::vector<
		std::pair<std::pair<
		std::string, std::string>, std::pair<std::string, std::string>>>
		trivialGroup);

	static std::unordered_map<std::string, std::vector<std::string>> getGroupProjectTable(
		std::unordered_map<std::string, std::string> declarations,
		std::vector<std::string> selectedVar,
		std::unordered_map<std::string, std::vector<std::string>> groupTable);

	static std::unordered_map<std::string, std::vector<std::string>> evaluateGroup(
		std::unordered_map<std::string, std::string> declarations,
		std::vector<std::pair<std::pair<std::string, std::string>, std::pair<std::string, std::string>>>
			clauseGroup);

	static std::string toAttrRefVal(std::unordered_map<std::string, std::string> declarations,
		std::string attrRef, std::string attrMember);
};