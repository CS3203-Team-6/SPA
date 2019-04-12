#pragma once

#include "QueryEvaluator.h"	

/*
Projects result of the query 
*/
std::list<std::string> QueryEvaluator::projectResult(
	std::unordered_map<std::string, std::string> declarations,
	std::vector<std::string> selectedVar,
	std::vector<std::pair<std::string, std::pair<std::string, std::string>>> suchThatCondition,
	std::vector<std::pair<std::string, std::pair<std::string, std::string>>> patternCondition,
	std::vector<std::pair<std::string, std::string>> withCondition) {
	Optimizer op = Optimizer(suchThatCondition, withCondition, patternCondition, selectedVar, declarations);
	op.groupClause();
	std::vector<std::vector<
		std::pair<std::pair<
		std::string, std::string>, std::pair<std::string, std::string>>>> notSelectedGroup = op.getTrivial();
	std::vector<std::vector<
		std::pair<std::pair<
		std::string, std::string>, std::pair<std::string, std::string>>>> selectedGroup = op.getNonTrivial();
	std::vector<
		std::pair<std::pair<
		std::string, std::string>, std::pair<std::string, std::string>>> trivialGroup = op.getNonSynonym();
	std::vector<std::unordered_map<std::string, std::vector<std::string>>> selectedMaps;
	std::string status = isTrivialGroupTrue(trivialGroup);
	std::list<std::string> resultList;
	for (std::vector<std::vector<
		std::pair<std::pair<
		std::string, std::string>, std::pair<std::string, std::string>>>>::size_type i = 0;
		i != notSelectedGroup.size(); i++) {
		if (status == "FALSE") {
			break;
		}
		if (evaluateGroup(declarations, notSelectedGroup[i]).begin()->second.size() == 0) {
			status = "FALSE";
		}
	}
	std::unordered_set<std::string> selectedNotInClauses;
	for (std::vector<std::string>::size_type i = 0; i != selectedVar.size(); i++) {
		selectedNotInClauses.insert(selectedVar[i]);
	}
	for (std::vector<std::vector<
		std::pair<std::pair<
		std::string, std::string>, std::pair<std::string, std::string>>>>::size_type i = 0;
		i != selectedGroup.size(); i++) {
		if (status == "FALSE") {
			break;
		}
		std::unordered_map<std::string, std::vector<std::string>> currentGroup = evaluateGroup(
			declarations, selectedGroup[i]);
		if (currentGroup.begin()->second.size() == 0) {
			status = "FALSE";
		}
		std::unordered_map<std::string, std::vector<std::string>> currentProjection = getGroupProjectTable(
			declarations, selectedVar, currentGroup);
		for (auto it = currentProjection.begin(); it != currentProjection.end(); ++it) {
			selectedNotInClauses.erase(it->first);
		}
		selectedMaps.push_back(getGroupProjectTable(declarations, selectedVar, 
			evaluateGroup(declarations, selectedGroup[i])));
	}
	if (selectedVar[0] == "BOOLEAN") {
		resultList.push_back(status);
	}
	else if (status == "TRUE") {
		std::unordered_map<std::string, std::vector<std::string>> projectMap;
		for (std::vector<std::unordered_map<std::string, std::vector<std::string>>>::size_type i = 0;
			i != selectedMaps.size(); i++) {
			projectMap = QueryUtility::product(projectMap, selectedMaps[i]);
		}
		for (std::unordered_set<std::string>::iterator it = selectedNotInClauses.begin();
			it != selectedNotInClauses.end(); ++it) {
			std::string pointer = *it;
			if (QueryUtility::hasReference(pointer)
				&& (projectMap.count(QueryUtility::attrOf(pointer)) == 0)) {
				projectMap = QueryUtility::product(projectMap,
					QueryUtility::getStmtsMap(declarations, QueryUtility::attrOf(pointer)));
			}
			else if (!QueryUtility::hasReference(pointer)) {
				projectMap = QueryUtility::product(projectMap,
					QueryUtility::getStmtsMap(declarations, pointer));
			}
		}
		resultList = toStringList(declarations, selectedVar, projectMap);
	}
	return resultList;
}

/*
Translate the projectTable to 
set of strings.
*/
std::list<std::string> QueryEvaluator::toStringList(
	std::unordered_map<std::string, std::string> declarations,
	std::vector<std::string> selectedVar,
	std::unordered_map<std::string, std::vector<std::string>> projectTable) {
	std::list<std::string> resultList;
	int projectedSize = projectTable.begin()->second.size();
	for (std::vector<std::string>::size_type i = 0; i != projectedSize; i++) {
		std::string tuple;
		for (std::vector<std::string>::size_type j = 0; j != selectedVar.size(); j++) {
			std::string currentVar = selectedVar[j];
			if (tuple.size() == 0) {
				if (QueryUtility::hasReference(currentVar)) {
					std::string attr = QueryUtility::attrOf(currentVar);
					tuple = toAttrRefVal(declarations, currentVar, projectTable[attr][i]);
				}
				else {
					tuple = projectTable[currentVar][i];
				}
			}
			else {
				if (QueryUtility::hasReference(currentVar)) {
					std::string attr = QueryUtility::attrOf(currentVar);
					tuple = tuple + " " +
						toAttrRefVal(declarations, currentVar, projectTable[attr][i]);
				}
				else {
					tuple = tuple + " " + projectTable[currentVar][i];
				}
			}
		}
		resultList.push_back(tuple);
	}
	return resultList;
}

std::string QueryEvaluator::isTrivialGroupTrue(std::vector<
	std::pair<std::pair<
	std::string, std::string>, std::pair<std::string, std::string>>>
	trivialGroup) {
	std::string status = "TRUE";
	for (std::vector<
		std::pair<std::pair<
		std::string, std::string>, std::pair<std::string, std::string>>>::size_type i = 0;
		i != trivialGroup.size(); i++) {
		std::string clauseType = trivialGroup[i].first.first;
		std::string relation = trivialGroup[i].first.second;
		std::string firstArgument = trivialGroup[i].second.first;
		std::string secondArgument = trivialGroup[i].second.second;
		if ((clauseType == "st" 
			&& (SuchThatEvaluator::isTrivial(relation, firstArgument, secondArgument) == "FALSE"))
			|| (clauseType == "with" && (WithEvaluator::isTrivial(firstArgument, secondArgument) == "FALSE"))) {
			status = "FALSE";
			break;
		}
	}
	return status;
}

/*
Get the project table  
with selected variables as 
the columns from resultTable.
*/
std::unordered_map<std::string, std::vector<std::string>> QueryEvaluator::getGroupProjectTable(
	std::unordered_map<std::string, std::string> declarations,
	std::vector<std::string> selectedVar,
	std::unordered_map<std::string, std::vector<std::string>> groupTable) {
	std::unordered_map<std::string, std::vector<std::string>> groupProjectTable;
	for (std::vector<std::string>::size_type i = 0; i != selectedVar.size(); i++) {
		std::string currentVar = selectedVar[i];
		if (QueryUtility::hasReference(currentVar)) {
			std::string attr = QueryUtility::attrOf(currentVar);
			if (groupTable.count(attr) == 1) {
				groupProjectTable.insert({ attr, groupTable[attr] });
			}
		}
		if (groupTable.count(currentVar) == 1) {
			groupProjectTable.insert({ currentVar, groupTable[currentVar] });
		}
	}
	return groupProjectTable;
}

/*
Merge all the results of each clause
*/
std::unordered_map<std::string, std::vector<std::string>> QueryEvaluator::evaluateGroup(
	std::unordered_map<std::string, std::string> declarations,
	std::vector<std::pair<std::pair<std::string, std::string>, std::pair<std::string, std::string>>>
	groupClause) {
	std::unordered_map<std::string, std::vector<std::string>> resultTable;
	std::unordered_map<std::string, std::vector<std::string>> toAddTable;
	for (std::vector<
		std::pair<std::pair<std::string, std::string>, std::pair<std::string, std::string>>>::size_type
		i = 0; i != groupClause.size(); i++) {
		std::string clauseType = groupClause[i].first.first;
		std::string relation = groupClause[i].first.second;
		std::string firstArgument = groupClause[i].second.first;
		std::string secondArgument = groupClause[i].second.second;
		if (clauseType == "st") {
			toAddTable = SuchThatEvaluator::evaluate(declarations, relation, firstArgument,
				secondArgument);
		}
		else if (clauseType == "pattern") {
			toAddTable = PatternEvaluator::evaluate(declarations, relation, firstArgument,
				secondArgument);
		}
		else {
			toAddTable = WithEvaluator::evaluate(declarations, firstArgument, secondArgument);
		}
		resultTable = QueryUtility::product(resultTable, toAddTable);
		if (resultTable.begin()->second.size() == 0) {
			break;
		}
	}

	return resultTable;
}

/*
The function evaluate the reference value
given the value of its attribute. 
For example: 
Given statement 6: call MeMaybe.
Let attrRef br c.procName, where c 
in declarations are mapped to 
call. Let attrMember be 6, which is
a member of the call statements. Then  
then function returns 6.procName
*/
std::string QueryEvaluator::toAttrRefVal(std::unordered_map<std::string, std::string> declarations,
	std::string attrRef, std::string attrMember) {
	std::string attr = QueryUtility::attrOf(attrRef);
	std::string ref = QueryUtility::refOf(attrRef);
	std::string attrType = declarations[attr];
	if ((attrType == "call") && (ref == "procName")) {
		return PKB().getProcCalledBy(stoi(attrMember));
	}
	else if ((attrType == "read") && (ref == "varName")) {
		return *PKB().getVarModifiedByStm(stoi(attrMember)).begin();
	}
	else if ((attrType == "print") && (ref == "varName")) {
		return *PKB().getVarUsedByStm(stoi(attrMember)).begin();
	}
	else {
		return attrMember;
	}
}

	
