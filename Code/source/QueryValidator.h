#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

class QueryValidator {
public:

	static std::string initialValidation(std::string query);
	static std::string validateClauses(std::vector<std::string> clauses);
	static std::string validateDeclarations(std::unordered_map<std::string,
											std::string> declarations);
	static std::string validateSelectedVar(
		std::vector<std::string> selectedVar,
		std::unordered_map<std::string, std::string> declarationsMap);

	static std::string validateSuchThatParam(
		std::vector<std::pair<std::string, std::pair<std::string, std::string>>> param,
		std::unordered_map<std::string, std::string> declarationsMap);

	static std::string validatePatternParam(
		std::vector<std::pair<std::string, std::pair<std::string, std::string>>> param,
		std::unordered_map<std::string, std::string> declarationsMap);

	static std::string validateWithParam(std::vector<std::pair<std::string, std::string>> param,
										 std::unordered_map<std::string, std::string> declarationsMap);
};