#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <limits>

#include "QueryEvaluator.h"
#include "QueryParser.h"
#include "QueryValidator.h"
#include "StringUtil.h"

std::string whitespace = " \f\n\r\t\v";
int maxInt = std::numeric_limits<int>::max();

/*
Main parser function for input query.
Passes the following to query evaluator:
1. vector<pair<string, string>> declarations
2. vector<string> selectedVar
3. vector<pair<string, pair<string, string>>> suchThatCondition
4. vector<pair<string, pair<string, string>>> patternCondition
*/
std::unordered_set<std::string> QueryParser::parse(std::string query) {
	
	std::string errorString;
	std::unordered_set<std::string> result;

	// initial query validation
	errorString = QueryValidator::initialValidation(query);
	if (errorString != "") {
		result.insert("error");
		return result;
	}

	// splitting clauses
	std::vector<std::string> clauses = splitClauses(query);

	// validating clauses
	errorString = QueryValidator::validateClauses(clauses);
	if (errorString != "") {
		result.insert("error");
		return result;
	}

	// parsing declarations
	std::unordered_map<std::string, std::string> declarations = splitDeclarations(clauses);

	// validating declarations
	errorString = QueryValidator::validateDeclarations(declarations);
	if (errorString != "") {
		result.insert("error");
		return result;
	}

	// parsing "Select" statement
	std::string selectStatement = clauses[clauses.size() - 1];

	std::vector<std::string> selectedVar;
	std::vector<std::pair<std::string, std::pair<std::string, std::string>>> suchThatCondition;
	std::vector<std::pair<std::string, std::pair<std::string, std::string>>> patternCondition;
	std::vector<std::pair<std::string, std::string>> withCondition;

	std::vector<std::string> suchThatClauses;
	std::vector<std::string> patternClauses;
	std::vector<std::string> withClauses;
	int suchThatIndex = selectStatement.find("such that");
	int patternIndex = selectStatement.find("pattern");
	int withIndex = selectStatement.find("with");
	int andIndex = maxInt;
	std::string previousClause;

	std::vector<int> indexes = { suchThatIndex, patternIndex, withIndex };
	int nextIndex = getMinimumValue(indexes);

	if (nextIndex == -1) {
		selectedVar = splitSelectParameter(selectStatement);
	}
	else {
		selectedVar = splitSelectParameter(selectStatement.substr(0, nextIndex));
		selectStatement = selectStatement.substr(nextIndex);
	}

	while (selectStatement.length() > 0 && nextIndex != -1) {
		suchThatIndex = selectStatement.substr(1).find("such that");
		patternIndex = selectStatement.substr(1).find("pattern");
		withIndex = selectStatement.substr(1).find("with");
		andIndex = selectStatement.substr(1).find("and");
		indexes = { suchThatIndex, patternIndex, withIndex, andIndex };
		nextIndex = getMinimumValue(indexes) + 1;
		
		if (nextIndex == 0) {
			nextIndex = selectStatement.length();
		}

		std::string currentClause = selectStatement.substr(0, nextIndex);
		if (currentClause.find("such that") != -1 || (currentClause.find("and") != -1 && previousClause == "such that")) {
			previousClause = "such that";
			suchThatClauses.push_back(currentClause);
		}
		else if (currentClause.find("pattern") != -1 || (currentClause.find("and") != -1 && previousClause == "pattern")) {
			previousClause = "pattern";
			patternClauses.push_back(currentClause);
		}
		else if (currentClause.find("with") != -1 || (currentClause.find("and") != -1 && previousClause == "with")) {
			previousClause = "with";
			withClauses.push_back(currentClause);
		}

		selectStatement = selectStatement.substr(nextIndex);
	}

	suchThatCondition = splitSuchThatCondition(suchThatClauses);
	patternCondition = splitPatternCondition(patternClauses);
	withCondition = splitWithCondition(withClauses);

	// validating 'Select' parameter
	errorString = QueryValidator::validateSelectedVar(selectedVar, declarations);
	if (errorString != "") {
		result.insert("error");
		return result;
	}

	// checking select boolean or synonym
	bool selectBoolean = false;
	if (selectedVar.size() == 1 && selectedVar[0] == "BOOLEAN") {
		selectBoolean = true;
	}

	// validating 'such that' parameter
	errorString = QueryValidator::validateSuchThatParam(suchThatCondition, declarations);
	if (errorString == "semantic error" && selectBoolean) {
		result.insert("FALSE");
		return result;
	}
	else if (errorString != "") {
		result.insert("error");
		return result;
	}

	// validating 'pattern' parameter
	errorString = QueryValidator::validatePatternParam(patternCondition, declarations);
	if (errorString == "semantic error" && selectBoolean) {
		result.insert("FALSE");
		return result;
	}
	else if (errorString != "") {
		result.insert("error");
		return result;
	}

	// validating 'with' parameter
	errorString = QueryValidator::validateWithParam(withCondition, declarations);
	if (errorString == "semantic error" && selectBoolean) {
		result.insert("FALSE");
		return result;
	}
	else if (errorString != "") {
		result.insert("error");
		return result;
	}

	result = evaluateSelectConditions(declarations, selectedVar, suchThatCondition, patternCondition, withCondition);

	return result;
}

/*
Splits the query string by char ";".
Returns a vector<string> consisting of every declaration clauses (without trailing whitespaces) and the "Select" statement
*/
std::vector<std::string> QueryParser::splitClauses(std::string query) {
	
	std::vector<std::string> output;
	char delimiter = ';';
	int startPoint = 0;
	int endPoint = query.find(delimiter);

	while (endPoint != -1) {
		output.push_back(StringUtil::trim(query.substr(startPoint, endPoint - startPoint), whitespace));
		startPoint = endPoint + 1;
		endPoint = query.find(delimiter, startPoint);
	}

	output.push_back(StringUtil::trim(query.substr(startPoint), whitespace));

	return output;
}

/*
Splits each declarations clause by whitespaces.
Returns a vector<pair<string, string>> consisting of design-entity and synonym
*/
std::unordered_map<std::string, std::string> QueryParser::splitDeclarations(std::vector<std::string> clauses) {
	
	std::unordered_map<std::string, std::string> output;
	int clausesSize = clauses.size();

	for (int i = 0; i < clausesSize - 1; i++) {
		std::string currentClauses = clauses[i];
		int charSpaceLoc = currentClauses.find_first_of(whitespace);
		std::string type = currentClauses.substr(0, charSpaceLoc);
		std::string varName = currentClauses.substr(charSpaceLoc);

		if (varName.find(",") != -1) {
			char delimiter = ',';
			int startPoint = 0;
			int endPoint = varName.find(delimiter);

			while (endPoint != -1) {
				std::string varNameSplitted = StringUtil::trim(varName.substr(startPoint, endPoint - startPoint), whitespace);
				output.insert(make_pair(varNameSplitted, type));
				startPoint = endPoint + 1;
				endPoint = varName.find(delimiter, startPoint);
			}

			output.insert(make_pair(StringUtil::trim(varName.substr(startPoint), whitespace), type));
		}
		else {
			output.insert(make_pair(StringUtil::trim(varName, whitespace), type));
		}
	}

	return output;
}

/*
Splits select clause by whitespaces.
Returns a vector<string> consisting of design-entity selected
*/
std::vector<std::string> QueryParser::splitSelectParameter(std::string selectStatement) {
	
	std::vector<std::string> output;

	int firstSpace = selectStatement.find_first_of(whitespace);
	std::string varName = StringUtil::removeAllWhitespaces(selectStatement.substr(firstSpace));

	if (varName.find("<") != -1 && varName.find(">") != -1) {
		varName = varName.substr(1, varName.length() - 2);

		while (varName.find(",") != -1) {
			int delimiterComma = varName.find(",");
			output.push_back(varName.substr(0, delimiterComma));
			varName = varName.substr(delimiterComma + 1);
		}		
	}
	
	output.push_back(varName);
	
	return output;
}

/*
Splits such that clause by open bracket, comma, and close bracket.
Returns a vector<pair<string, pair<string, string>>> consisting of relation, stmtRef/entRef, stmtRef/entRef
*/
std::vector<std::pair<std::string, std::pair<std::string, std::string>>> 
	QueryParser::splitSuchThatCondition(std::vector<std::string> suchThatClause) {
	
	std::vector<std::pair<std::string, std::pair<std::string, std::string>>> output;

	for(int i = 0; i < suchThatClause.size(); i++) {
		int openBracket = suchThatClause[i].find("(");
		int comma = suchThatClause[i].find(",");
		int closeBracket = suchThatClause[i].find(")");
		int strLen = suchThatClause[i].length();
		std::string condition;

		if (suchThatClause[i].find("such") != -1) {
			condition =
				StringUtil::removeAllWhitespaces(suchThatClause[i].substr(9, openBracket - 9));
		}
		else {
			condition =
				StringUtil::removeAllWhitespaces(suchThatClause[i].substr(3, openBracket - 3));
		}
		std::string firstVar =
			StringUtil::removeAllWhitespaces(suchThatClause[i].substr(openBracket + 1,
																	  comma - openBracket - 1));
		std::string secondVar =
			StringUtil::removeAllWhitespaces(suchThatClause[i].substr(comma + 1,
																	  closeBracket - comma - 1));

		output.push_back(make_pair(condition, make_pair(firstVar, secondVar)));
	}

	return output;
}

/*
Splits pattern clause by open bracket, comma, and close bracket.
Returns a vector<pair<string, pair<string, string>>> consisting of design-entity, entRef, expression-spec
*/
std::vector<std::pair<std::string, std::pair<std::string, std::string>>>
	QueryParser::splitPatternCondition(std::vector<std::string> patternClause) {
	
	std::vector<std::pair<std::string, std::pair<std::string, std::string>>> output;

	for (int i = 0; i < patternClause.size(); i++) {
		int openBracket = patternClause[i].find("(");
		int comma = patternClause[i].find(",");
		int closeBracket = patternClause[i].find(")");
		int strLen = patternClause[i].length();
		std::string varName;

		if (patternClause[i].find("pattern") != -1) {
			varName =
				StringUtil::removeAllWhitespaces(patternClause[i].substr(7, openBracket - 7));
		}
		else {
			varName =
				StringUtil::removeAllWhitespaces(patternClause[i].substr(3, openBracket - 3));
		}

		std::string firstPattern =
			StringUtil::removeAllWhitespaces(patternClause[i].substr(openBracket + 1,
																	 comma - openBracket - 1));
		std::string secondPattern =
			StringUtil::removeAllWhitespaces(patternClause[i].substr(comma + 1,
																	 closeBracket - comma - 1));

		output.push_back(make_pair(varName, make_pair(firstPattern, secondPattern)));
	}

	return output;
}

/*
Splits with clause by equal sign.
Returns a vector<pair<string, string>> consisting of firstRef and secondRef
*/
std::vector<std::pair<std::string, std::string>>
	QueryParser::splitWithCondition(std::vector<std::string> withClause) {

	std::vector<std::pair<std::string, std::string>> output;

	for (int i = 0; i < withClause.size(); i++) {
		int equalSign = withClause[i].find("=");
		int strLen = withClause[i].length();
		std::string firstArgs;

		if (withClause[i].find("with") != -1) {
			firstArgs = StringUtil::removeAllWhitespaces(withClause[i].substr(4, equalSign - 4));
		}
		else {
			firstArgs = StringUtil::removeAllWhitespaces(withClause[i].substr(3, equalSign - 3));
		}
		std::string secondArgs =
			StringUtil::removeAllWhitespaces(withClause[i].substr(equalSign + 1));

		output.push_back(make_pair(firstArgs, secondArgs));
	}

	return output;
}

/*
Calls QueryEvaluator to evaluate the query result
Returns a unordered_set<string> consisting of results
*/
std::unordered_set<std::string>
	QueryParser::evaluateSelectConditions(std::unordered_map<std::string, std::string> declarations,
										  std::vector<std::string> selectedVar,
										  std::vector<std::pair<std::string,
										  std::pair<std::string, std::string>>> suchThatCondition,
										  std::vector<std::pair<std::string,
										  std::pair<std::string, std::string>>> patternCondition,
										  std::vector<std::pair<std::string, std::string>> withCondition) {

	return QueryEvaluator::projectResult(declarations, selectedVar, suchThatCondition,
										 patternCondition, withCondition);
}

/*
Get minimum value from the vector args
*/
int QueryParser::getMinimumValue(std::vector<int> indexes) {

	int minIndex = maxInt;
	for (int i = 0; i < indexes.size(); i++) {
		if (indexes[i] != -1 && indexes[i] < minIndex) {
			minIndex = indexes[i];
		}
	}

	if (minIndex == maxInt) {
		return -1;
	}
	else {
		return minIndex;
	}
}