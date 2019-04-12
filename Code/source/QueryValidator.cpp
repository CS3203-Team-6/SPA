#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>

#include "QueryValidator.h"
#include "LexicalToken.h"
#include "ExpressionUtil.h"

const std::unordered_set<std::string> validVarType = { "stmt", "read", "print", "call",
	"while", "if", "assign", "variable", "constant", "prog_line", "procedure" };

/*
Checks whether the input string is valid:
- has length more than 0
- has substring "Select"
- has declaration
Output: error message (if any), otherwise empty string
*/
std::string QueryValidator::initialValidation(std::string query) {

	if (query.length() <= 0) {
		return "invalid query";
	}
	else if (query.find("Select") == -1) {
		return "invalid query";
	}
	else if (query.find("Select") == 0) {
		return "None";
	}
	else {
		return "";
	}
}

/*
Validates declaration clauses based on following conditions:
- "Select" clause appears after all declaration clauses
- Each declaration clause consists of varType and varName
*/
std::string QueryValidator::validateClauses(std::vector<std::string> clauses) {
	
	int clausesSize = clauses.size();

	if (clauses[clausesSize - 1].find("Select") == -1) {
		return "invalid query";
	}

	for (int i = 0; i < clausesSize - 1; i++) {
		if (clauses[i].find(" ") == -1) {
			return "invalid query";
		}
	}

	return "";
}

/*
Validates declaration vector based on following conditions:
- design-entity should be valid
- synonym should follow the grammar rule: LETTER (LETTER | DIGIT)*
*/
std::string QueryValidator::validateDeclarations(
	std::unordered_map<std::string, std::string> declarations) {
	
	for (auto x: declarations) {
		if (validVarType.find(x.second) == validVarType.end()) {
			return "invalid query";
		}

		if (!LexicalToken::verifyName(x.first)) {
			return "invalid query";
		}
	}

	return "";
}

/*
Validates vector of selected synonym based on following conditions:
- synonym should follow the grammar rule: LETTER (LETTER | DIGIT)*
- synonym should be declared previously
*/
std::string QueryValidator::validateSelectedVar(std::vector<std::string> selectedVar,
												std::unordered_map<std::string,
												std::string> declarationsMap) {

	std::unordered_set<std::string> intTypeRefStmtNum = { "stmt", "read", "print", "while", "if", "assign", "call" };
	std::unordered_set<std::string> intTypeRefValue = { "constant" };
	std::unordered_set<std::string> stringTypeRefProcName = { "procedure", "call" };
	std::unordered_set<std::string> stringTypeRefVarName = { "variable", "read", "print" };

	for (size_t i = 0; i < selectedVar.size(); i++) {
		
		// select type: BOOLEAN
		if (selectedVar[i] == "BOOLEAN" && selectedVar.size() != 1) {
			return "too many selected variable for boolean";
		}
		
		// select type: synonym with attributes
		if (selectedVar[i].find(".") != -1) {
			int dotIndex = selectedVar[i].find(".");
			std::string left = selectedVar[i].substr(0, dotIndex);
			std::string right = selectedVar[i].substr(dotIndex + 1, selectedVar[i].length() - dotIndex - 1);
			
			if (declarationsMap.find(left) == declarationsMap.end()) {
				return "selected variable not found";
			}
			
			std::string synonymType = declarationsMap.find(left)->second;

			if (right == "stmt#" && (intTypeRefStmtNum.find(synonymType) != intTypeRefStmtNum.end())) {
				// valid stmt# attribute
				continue;
			}
			else if (right == "value" && (intTypeRefValue.find(synonymType) != intTypeRefValue.end())) {
				// valid value attribute
				continue;
			}
			else if (right == "procName" && (stringTypeRefProcName.find(synonymType) != stringTypeRefProcName.end())) {
				// valid procName attribute
				continue;
			}
			else if (right == "varName" && (stringTypeRefVarName.find(synonymType) != stringTypeRefVarName.end())) {
				// valid varName attribute
				continue;
			}
			else {
				return "invalid synonym attributes";
			}

		} else if (!LexicalToken::verifyName(selectedVar[i])) {
			return "invalid variable name";
		}

		// select type: synonym
		if (selectedVar[i] != "BOOLEAN" &&
			declarationsMap.find(selectedVar[i]) == declarationsMap.end()) {
			return "selected variable not found";
		}
	}

	return "";
}

/*
Validates vector of such that parameter based on following conditions:
- valid relation name
- for each relation, first and second argument should be valid
*/
std::string QueryValidator::validateSuchThatParam(std::vector<std::pair<std::string, std::pair<std::string, std::string>>> param,
												  std::unordered_map<std::string, std::string> declarationsMap) {

	std::unordered_set<std::string> validRelation = { "Parent", "Parent*", "Follows",
		"Follows*", "Uses", "Modifies", "Calls", "Calls*", "Next", "Next*", "Affects", "Affects*" };
	std::unordered_set<std::string> validArgs = { "stmt", "read", "print", "while", "if",
		"assign", "call", "prog_line" };
	std::unordered_set<std::string> validFirstArgsParent = { "stmt", "while", "if", "prog_line" };
	std::unordered_set<std::string> validFirstArgsUses = { "stmt", "print", "while", "if",
		"assign", "call", "procedure", "prog_line" };
	std::unordered_set<std::string> validFirstArgsModifies = { "stmt", "read", "while", "if",
"assign", "call", "procedure", "prog_line" };
	std::unordered_set<std::string> validArgsCalls = { "procedure" };
	std::unordered_set<std::string> validArgsAffects = { "stmt", "assign", "prog_line" };

	for (size_t i = 0; i < param.size(); i++) {
		std::string relation = param[i].first;
		std::string firstArgs = param[i].second.first;
		std::string secondArgs = param[i].second.second;
		std::string firstArgsType;
		std::string secondArgsType;

		// Initial Validation
		if (validRelation.find(relation) == validRelation.end()) {
			return "invalid query";
		}

		if (declarationsMap.find(firstArgs) != declarationsMap.end()) {
			firstArgsType = declarationsMap.find(firstArgs)->second;
		}

		if (declarationsMap.find(secondArgs) != declarationsMap.end()) {
			secondArgsType = declarationsMap.find(secondArgs)->second;
		}

		// Switch Cases according to relation
		if (relation == "Parent" || relation == "Parent*") {
			
			// Validating first args
			if (firstArgs == "_" ||
				LexicalToken::verifyInteger(firstArgs) ||
				validFirstArgsParent.find(firstArgsType) != validFirstArgsParent.end()) {
				// valid first args
			}
			else {
				return "semantic error";
			}

			// Validating second args
			if (secondArgs == "_" ||
				LexicalToken::verifyInteger(secondArgs) ||
				validArgs.find(secondArgsType) != validArgs.end()) {
				// valid second args
			}
			else {
				return "semantic error";
			}
		} 
		else if (relation == "Follows" || relation == "Follows*" || relation == "Next" || relation == "Next*") {

			// Validating first args
			if (firstArgs == "_" ||
				LexicalToken::verifyInteger(firstArgs) ||
				validArgs.find(firstArgsType) != validArgs.end()) {
				// valid first args
			}
			else {
				return "semantic error";
			}

			// Validating second args
			if (secondArgs == "_" ||
				LexicalToken::verifyInteger(secondArgs) ||
				validArgs.find(secondArgsType) != validArgs.end()) {
				// valid second args
			}
			else {
				return "semantic error";
			}
		} 
		else if (relation == "Uses") {

			// Validating first args
			if (LexicalToken::verifyInteger(firstArgs) ||
				(firstArgs[0] == '"' && LexicalToken::verifyName(firstArgs.substr(1, firstArgs.length() - 2))) ||
				(firstArgs[0] != '"' && LexicalToken::verifyName(firstArgs) && (validFirstArgsUses.find(firstArgsType) != validFirstArgsUses.end()))) {
				// valid first args
			}
			else {
				return "semantic error";
			}

			// Validating second args
			if (secondArgs == "_" ||
				LexicalToken::verifyInteger(secondArgs) ||
				(secondArgs[0] == '"' && LexicalToken::verifyName(secondArgs.substr(1, secondArgs.length() - 2))) ||
				(secondArgs[0] != '"' && LexicalToken::verifyName(secondArgs))) {
				// valid second args
			}
			else {
				return "semantic error";
			}
		}
		else if (relation == "Modifies") {

			// Validating first args
			if (LexicalToken::verifyInteger(firstArgs) ||
				(firstArgs[0] == '"' && LexicalToken::verifyName(firstArgs.substr(1, firstArgs.length() - 2))) ||
				(firstArgs[0] != '"' && LexicalToken::verifyName(firstArgs) && (validFirstArgsModifies.find(firstArgsType) != validFirstArgsModifies.end()))) {
				// valid first args
			}
			else {
				return "semantic error";
			}

			// Validating second args
			if (secondArgs == "_" ||
				LexicalToken::verifyInteger(secondArgs) ||
				(secondArgs[0] == '"' && LexicalToken::verifyName(secondArgs.substr(1, secondArgs.length() - 2))) ||
				(secondArgs[0] != '"' && LexicalToken::verifyName(secondArgs))) {
				// valid first args
			}
			else {
				return "semantic error";
			}
		}
		else if (relation == "Calls" || relation == "Calls*") {

			// Validating first args
			if (firstArgs == "_" ||
				(firstArgs[0] == '"' && LexicalToken::verifyName(firstArgs.substr(1, firstArgs.length() - 2))) ||
				(firstArgs[0] != '"' && LexicalToken::verifyName(firstArgs) && (validArgsCalls.find(firstArgsType) != validArgsCalls.end()))) {
				// valid first args
			}
			else {
				return "semantic error";
			}

			// Validating second args
			if (secondArgs == "_" ||
				(secondArgs[0] == '"' && LexicalToken::verifyName(secondArgs.substr(1, secondArgs.length() - 2))) ||
				(secondArgs[0] != '"' && LexicalToken::verifyName(secondArgs) && (validArgsCalls.find(secondArgsType) != validArgsCalls.end()))) {
				// valid second args
			}
			else {
				return "semantic error";
			}
		}
		else if (relation == "Affects" || relation == "Affects*") {

			// Validating first args
			if (firstArgs == "_" ||
				LexicalToken::verifyInteger(firstArgs) ||
				(LexicalToken::verifyName(firstArgs) && (validArgsAffects.find(firstArgsType) != validArgsAffects.end()))) {
				// valid first args
			}
			else {
				return "semantic error";
			}

			// Validating second args
			if (secondArgs == "_" ||
				LexicalToken::verifyInteger(secondArgs) ||
				(LexicalToken::verifyName(secondArgs) && (validArgsAffects.find(secondArgsType) != validArgsAffects.end()))) {
				// valid second args
			}
			else {
				return "semantic error";
			}
		}
	}

	return "";
}

/*
Validates vector of pattern parameter based on following conditions:
- synonym should follow the grammar rule: LETTER (LETTER | DIGIT)*
- synonym should be declared previously
- synonym should be assign/if/while type
- for each pattern clause, first and second argument should be valid

Note: for if pattern, the third args will be removed
*/
std::string QueryValidator::validatePatternParam(std::vector<std::pair<std::string, std::pair<std::string, std::string>>> param,
												 std::unordered_map<std::string, std::string> declarationsMap) {
	
	for (size_t i = 0; i < param.size(); i++) {
		std::string stmt = param[i].first;
		std::string stmtType;
		std::string firstArgs = param[i].second.first;
		std::string secondArgs = param[i].second.second;

		// Assign synonym validation
		if (!LexicalToken::verifyName(stmt)) {
			return "invalid synonym";
		}

		if (declarationsMap.find(stmt) == declarationsMap.end()) {
			return "statement not found";
		}
		else {
			stmtType = (declarationsMap.find(stmt))->second;
		}

		// First argument validation
		if (firstArgs == "_" ||
			(firstArgs[0] != '"' && (declarationsMap.find(firstArgs) != declarationsMap.end()) && ((declarationsMap.find(firstArgs))->second == "variable")) ||
			(firstArgs[0] == '"' && LexicalToken::verifyName(firstArgs.substr(1, firstArgs.length() - 2)))) {
			// valid first args
		}
		else {
			return "semantic error";
		}

		// Second argument validation
		if (stmtType == "assign") {
			if (secondArgs == "_" ||
				(secondArgs[0] == '_' && ExpressionUtil::verifyInfixExpression(secondArgs.substr(2, secondArgs.length() - 4))) ||
				(secondArgs[0] == '"' && ExpressionUtil::verifyInfixExpression(secondArgs.substr(1, secondArgs.length() - 2)))) {
				// valid second args
			}
			else {
				return "semantic error";
			}

		}
		else if (stmtType == "if") {
			if (secondArgs == "_,_") {
				// valid second args
			}
			else {
				return "semantic error";
			}

		}
		else if (stmtType == "while") {
			if (secondArgs == "_") {
				// valid second args
			}
			else {
				return "semantic error";
			}

		}
		else {
			return "statement type is invalid";
		}
	}

	return "";
}

/*
Validates vector of with parameter based on following conditions:
- ...
*/
std::string QueryValidator::validateWithParam(std::vector<std::pair<std::string, std::string>> param,
											  std::unordered_map<std::string, std::string> declarationsMap) {

	std::unordered_set<std::string> intTypeNoRef = { "prog_line" };
	std::unordered_set<std::string> intTypeRefStmtNum = { "stmt", "read", "print", "while",
		"if", "assign", "call" };
	std::unordered_set<std::string> intTypeRefValue = { "constant" };
	std::unordered_set<std::string> stringTypeRefProcName = { "procedure", "call" };
	std::unordered_set<std::string> stringTypeRefVarName = { "variable", "read", "print" };

	for (size_t i = 0; i < param.size(); i++) {
		std::string firstArgs = param[i].first;
		std::string secondArgs = param[i].second;
		std::string firstArgsType = "";
		std::string secondArgsType = "";
		std::string synonym;
		std::string synonymType;

		// check firstArgs type(int / string)

		// type: int
		if (LexicalToken::verifyInteger(firstArgs)) {
			firstArgsType = "int";
		}
		// type: synonym of "prog_line"
		else if (LexicalToken::verifyName(firstArgs)) {
			if (declarationsMap.find(firstArgs) != declarationsMap.end()) {
				synonymType = declarationsMap.find(firstArgs)->second;
			}
			else {
				return "semantic error";
			}
			
			if (intTypeNoRef.find(synonymType) != intTypeNoRef.end()) {
				firstArgsType = "int";
			}
			else {
				return "semantic error";
			}
		}
		// type: attrRef with attribute value .stmt#
		else if (firstArgs.find(".stmt#") != -1) {
			synonym = firstArgs.substr(0, firstArgs.find("."));

			if (declarationsMap.find(synonym) != declarationsMap.end()) {
				synonymType = declarationsMap.find(synonym)->second;
			}
			else {
				return "semantic error";
			}

			if (intTypeRefStmtNum.find(synonymType) != intTypeRefStmtNum.end()) {
				firstArgsType = "int";
			}
			else {
				return "semantic error";
			}
		}
		// type: attrRef with attribute value .value
		else if (firstArgs.find(".value") != -1) {
			synonym = firstArgs.substr(0, firstArgs.find("."));

			if (declarationsMap.find(synonym) != declarationsMap.end()) {
				synonymType = declarationsMap.find(synonym)->second;
			}
			else {
				return "semantic error";
			}

			if (intTypeRefValue.find(synonymType) != intTypeRefValue.end()) {
				firstArgsType = "int";
			}
			else {
				return "semantic error";
			}
		}
		// type: "IDENT"
		else if (firstArgs[0] == '"' && LexicalToken::verifyName(firstArgs.substr(1, firstArgs.length() - 2))) {
			firstArgsType = "str";
		}
		// type: attrRef with attribute value .procName
		else if (firstArgs.find(".procName") != -1) {
			synonym = firstArgs.substr(0, firstArgs.find("."));

			if (declarationsMap.find(synonym) != declarationsMap.end()) {
				synonymType = declarationsMap.find(synonym)->second;
			}
			else {
				return "semantic error";
			}

			if (stringTypeRefProcName.find(synonymType) != stringTypeRefProcName.end()) {
				firstArgsType = "str";
			}
			else {
				return "semantic error";
			}
		}
		// type: attrRef with attribute value .varName
		else if (firstArgs.find(".varName") != -1) {
			synonym = firstArgs.substr(0, firstArgs.find("."));

			if (declarationsMap.find(synonym) != declarationsMap.end()) {
				synonymType = declarationsMap.find(synonym)->second;
			}
			else {
				return "semantic error";
			}

			if (stringTypeRefVarName.find(synonymType) != stringTypeRefVarName.end()) {
				firstArgsType = "str";
			}
			else {
				return "semantic error";
			}
		}

		// check secondArgs type (int/string), exact same logic like firstArgs
		
		// type: int
		if (LexicalToken::verifyInteger(secondArgs)) {
			secondArgsType = "int";
		}
		// type: synonym of "prog_line"
		else if (LexicalToken::verifyName(secondArgs)) {
			if (declarationsMap.find(secondArgs) != declarationsMap.end()) {
				synonymType = declarationsMap.find(secondArgs)->second;
			}
			else {
				return "semantic error";
			}

			if (intTypeNoRef.find(synonymType) != intTypeNoRef.end()) {
				secondArgsType = "int";
			}
			else {
				return "semantic error";
			}
		}
		// type: attrRef with attribute value .stmt#
		else if (secondArgs.find(".stmt#") != -1) {
			synonym = secondArgs.substr(0, secondArgs.find("."));

			if (declarationsMap.find(synonym) != declarationsMap.end()) {
				synonymType = declarationsMap.find(synonym)->second;
			}
			else {
				return "semantic error";
			}

			if (intTypeRefStmtNum.find(synonymType) != intTypeRefStmtNum.end()) {
				secondArgsType = "int";
			}
			else {
				return "semantic error";
			}
		}
		// type: attrRef with attribute value .value
		else if (secondArgs.find(".value") != -1) {
			synonym = secondArgs.substr(0, secondArgs.find("."));

			if (declarationsMap.find(synonym) != declarationsMap.end()) {
				synonymType = declarationsMap.find(synonym)->second;
			}
			else {
				return "semantic error";
			}

			if (intTypeRefValue.find(synonymType) != intTypeRefValue.end()) {
				secondArgsType = "int";
			}
			else {
				return "semantic error";
			}
		}
		// type: "IDENT"
		else if (secondArgs[0] == '"' && LexicalToken::verifyName(secondArgs.substr(1, secondArgs.length() - 2))) {
			secondArgsType = "str";
		}
		// type: attrRef with attribute value .procName
		else if (secondArgs.find(".procName") != -1) {
			synonym = secondArgs.substr(0, secondArgs.find("."));

			if (declarationsMap.find(synonym) != declarationsMap.end()) {
				synonymType = declarationsMap.find(synonym)->second;
			}
			else {
				return "semantic error";
			}

			if (stringTypeRefProcName.find(synonymType) != stringTypeRefProcName.end()) {
				secondArgsType = "str";
			}
			else {
				return "semantic error";
			}
		}
		// type: attrRef with attribute value .varName
		else if (secondArgs.find(".varName") != -1) {
			synonym = secondArgs.substr(0, secondArgs.find("."));

			if (declarationsMap.find(synonym) != declarationsMap.end()) {
				synonymType = declarationsMap.find(synonym)->second;
			}
			else {
				return "semantic error";
			}

			if (stringTypeRefVarName.find(synonymType) != stringTypeRefVarName.end()) {
				secondArgsType = "str";
			}
			else {
				return "semantic error";
			}
		}
		
		if (firstArgsType == "" || secondArgsType == "" || firstArgsType != secondArgsType) {
			return "semantic error";
		}
	}

	return "";
}