#pragma once

#include "SuchThatEvaluator.h"

/*
THe function evaluates the such that conditions
which gives boolean answer
*/
std::string SuchThatEvaluator::isTrivial(std::string relation, std::string firstArgument,
	std::string secondArgument) {
	if (QueryUtility::isOutOfRange(firstArgument) || QueryUtility::isOutOfRange(secondArgument)) {
		return "FALSE";
	}
	else if (relation == "Follows") {
		return FollowsEvaluator::isTrivial(firstArgument, secondArgument);
	}
	else if (relation == "Follows*") {
		return FollowsStarEvaluator::isTrivial(firstArgument, secondArgument);
	}
	else if (relation == "Parent") {
		return ParentEvaluator::isTrivial(firstArgument, secondArgument);
	}
	else if (relation == "Parent*") {
		return ParentStarEvaluator::isTrivial(firstArgument, secondArgument);
	}
	else if (relation == "Uses") {
		return UsesEvaluator::isTrivial(firstArgument, secondArgument);
	}
	else if (relation == "Modifies") {
		return UsesEvaluator::isTrivial(firstArgument, secondArgument);
	}
	else if (relation == "Calls") {
		return CallsEvaluator::isTrivial(firstArgument, secondArgument);
	}
	else if (relation == "Calls*") {
		return CallsStarEvaluator::isTrivial(firstArgument, secondArgument);
	}
	else if (relation == "Next") {
		return NextEvaluator::isTrivial(firstArgument, secondArgument);
	}
	else if (relation == "Next*") {
		return NextStarEvaluator::isTrivial(firstArgument, secondArgument);
	}
	else if (relation == "Affects") {
		return AffectsEvaluator::isTrivial(firstArgument, secondArgument);
	}
	else if (relation == "Affects*") {
		return AffectsStarEvaluator::isTrivial(firstArgument, secondArgument);
	}
	return "NON TRIVIAL";
}

/*
The function evaluates the non-trivial
Such That Conditions.
*/
std::unordered_map<std::string, std::vector<std::string>> SuchThatEvaluator::evaluate(
	std::unordered_map<std::string, std::string> declarations,
	std::string relation, std::string firstArgument, std::string secondArgument) {
	std::unordered_map<std::string, std::vector<std::string>> tableResult;
	std::vector<std::string> emptyVec;
	if (QueryUtility::isOutOfRange(firstArgument)) {
		tableResult.insert({ secondArgument, emptyVec });
	}
	else if (QueryUtility::isOutOfRange(secondArgument)) {
		tableResult.insert({ firstArgument, emptyVec });
	}
	else if (relation == "Follows") {
		tableResult = FollowsEvaluator::evaluate(declarations,
			firstArgument, secondArgument);
	}
	else if (relation == "Follows*") {
		tableResult = FollowsStarEvaluator::evaluate(declarations,
			firstArgument, secondArgument);
	}
	else if (relation == "Parent") {
		tableResult = ParentEvaluator::evaluate(declarations,
			firstArgument, secondArgument);
	}
	else if (relation == "Parent*") {
		tableResult = ParentStarEvaluator::evaluate(declarations,
			firstArgument, secondArgument);
	}
	else if (relation == "Uses") {
		tableResult = UsesEvaluator::evaluate(declarations,
			firstArgument, secondArgument);
	}
	else if (relation == "Modifies") {
		tableResult = ModifiesEvaluator::evaluate(declarations,
			firstArgument, secondArgument);
	}
	else if (relation == "Calls") {
		tableResult = CallsEvaluator::evaluate(declarations,
			firstArgument, secondArgument);
	}
	else if (relation == "Calls*") {
		tableResult = CallsStarEvaluator::evaluate(declarations,
			firstArgument, secondArgument);
	}
	else if (relation == "Next") {
		tableResult = NextEvaluator::evaluate(declarations,
			firstArgument, secondArgument);
	}
	else if (relation == "Next*") {
		tableResult = NextStarEvaluator::evaluate(declarations,
			firstArgument, secondArgument);
	}
	else if (relation == "Affects") {
		tableResult = AffectsEvaluator::evaluate(declarations,
			firstArgument, secondArgument);
	}
	else if (relation == "Affects*") {
		tableResult = AffectsStarEvaluator::evaluate(declarations,
			firstArgument, secondArgument);
	}

	if (tableResult.size() == 1) {
		if (QueryUtility::isSynonym(firstArgument)) {
			return filterType(firstArgument, declarations, tableResult);
		}
		else {
			return filterType(secondArgument, declarations, tableResult);
		}
	}
	else if (tableResult.size() == 2) {
		return filterType(firstArgument, secondArgument, declarations, tableResult);
	}

	return tableResult;
}

/*
The function does filtering
for singleton results from PKB
so that the output satisfy a given
type.
*/
std::unordered_map<std::string, std::vector<std::string>> SuchThatEvaluator::filterType(std::string synonym,
	std::unordered_map<std::string, std::string> declarations,
	std::unordered_map<std::string, std::vector<std::string>> toBeFiltered) {
	std::unordered_map<std::string, std::vector<std::string>> filteredTable;
	std::string synonymType = declarations[synonym];
	std::unordered_set<std::string> filter = QueryUtility::getStmts(synonymType);
	if (synonymType == "stmt" || synonymType == "variable") {
		return toBeFiltered;
	}
	else {
		std::vector<std::string> oldColumn = toBeFiltered[synonym];
		std::vector<std::string> newColumn;
		for (std::vector<std::string>::size_type i = 0; i != oldColumn.size(); i++) {
			if (filter.count(oldColumn[i]) == 1) {
				newColumn.push_back(oldColumn[i]);
			}
		}
		filteredTable.insert({ synonym, newColumn });
		return filteredTable;
	}
}

/*
The function filters the set of strings
of pairs so that the first member and
second member is in the set of all
statements in a first type and a second
type respectively.
*/
std::unordered_map<std::string, std::vector<std::string>> SuchThatEvaluator::filterType(std::string synonym1,
	std::string synonym2, std::unordered_map<std::string, std::string> declarations,
	std::unordered_map<std::string, std::vector<std::string>> toBeFiltered) {
	std::unordered_map<std::string, std::vector<std::string>> filteredSet;
	std::string synonym1Type = declarations[synonym1];
	std::string synonym2Type = declarations[synonym2];
	std::unordered_set<std::string> filter1 = QueryUtility::getStmts(synonym1Type);
	std::unordered_set<std::string> filter2 = QueryUtility::getStmts(synonym2Type);
	std::vector<std::string> oldColumn1 = toBeFiltered[synonym1];
	std::vector<std::string> oldColumn2 = toBeFiltered[synonym2];
	if (synonym1Type == "stmt" && (synonym2Type == "stmt" || synonym2Type == "variable")) {
		return toBeFiltered;
	}
	else if (synonym1Type == "stmt" && synonym2Type != "stmt" && synonym2Type != "variable") {
		for (auto it = toBeFiltered.begin(); it != toBeFiltered.end(); ++it) {
			std::vector<std::string> newColumn;
			for (std::vector<std::string>::size_type i = 0; i != oldColumn2.size(); i++) {
				if (filter2.count(oldColumn2[i]) == 1) {
					newColumn.push_back(it->second[i]);
				}
			}
			filteredSet.insert({ it->first, newColumn });
		}
	}
	else if (synonym1Type != "stmt" && (synonym2Type == "stmt" || synonym2Type == "variable")) {
		for (auto it = toBeFiltered.begin(); it != toBeFiltered.end(); ++it) {
			std::vector<std::string> newColumn;
			for (std::vector<std::string>::size_type i = 0; i != oldColumn1.size(); i++) {
				if (filter1.count(oldColumn1[i]) == 1) {
					newColumn.push_back(it->second[i]);
				}
			}
			filteredSet.insert({ it->first, newColumn });
		}
	}
	else if (synonym1Type != "stmt" && synonym2Type != "stmt" && synonym2Type != "variable") {
		for (auto it = toBeFiltered.begin(); it != toBeFiltered.end(); ++it) {
			std::vector<std::string> newColumn;
			for (std::vector<std::string>::size_type i = 0; i != oldColumn1.size(); i++) {
				if ((filter1.count(oldColumn1[i]) == 1) && (filter2.count(oldColumn2[i]) == 1)) {
					newColumn.push_back(it->second[i]);
				}
			}
			filteredSet.insert({ it->first, newColumn });
		}
	}
	return filteredSet;
}