#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>

class QueryParser {
public:
	static std::unordered_set<std::string> parse(std::string query);

	static std::vector<std::string> splitClauses(std::string query);

	static std::unordered_map<std::string, std::string>
		splitDeclarations(std::vector<std::string> clauses);

	static std::vector<std::string> splitSelectParameter(std::string selectStatement);

	static std::vector<std::pair<std::string, std::pair<std::string, std::string>>>
		splitSuchThatCondition(std::vector<std::string> suchThatClause);

	static std::vector<std::pair<std::string, std::pair<std::string, std::string>>>
		splitPatternCondition(std::vector<std::string> patternClause);

	static std::vector<std::pair<std::string, std::string>>
		splitWithCondition(std::vector<std::string> withClause);

	static std::unordered_set<std::string> evaluateSelectConditions(
		std::unordered_map<std::string, std::string> declarations,
		std::vector<std::string> selectedVar,
		std::vector<std::pair<std::string, std::pair<std::string, std::string>>> suchThatCondition,
		std::vector<std::pair<std::string, std::pair<std::string, std::string>>> patternCondition,
		std::vector<std::pair<std::string, std::string>> withCondition);

	static int getMinimumValue(std::vector<int> indexes);
};