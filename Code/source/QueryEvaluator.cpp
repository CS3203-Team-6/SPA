#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
#include <sstream>
#include <algorithm>

#include "PKB.h"
#include "QueryEvaluator.h"	
#include "LexicalToken.h"
#include "ExpressionUtil.h"
#include "ContainerUtil.h"

/*
Projects result of the query 
*/
std::unordered_set<std::string> QueryEvaluator::projectResult(
	std::unordered_map<std::string, std::string> declarations,
	std::vector<std::string> selectedVar,
	std::vector<std::pair<std::string, std::pair<std::string, std::string>>> suchThatCondition,
	std::vector<std::pair<std::string, std::pair<std::string, std::string>>> patternCondition,
	std::vector<std::pair<std::string, std::string>> withCondition) {
	std::pair<std::string, std::unordered_map<std::string, std::vector<std::string>>> resultPair = evaluateTable(
		declarations, suchThatCondition, patternCondition, withCondition);
	std::string status = resultPair.first;
	std::unordered_map<std::string, std::vector<std::string>> resultTable = resultPair.second;
	std::unordered_set<std::string> resultSet;
	if (selectedVar[0] == "BOOLEAN") {
		resultSet.insert(status);
	}
	else if (status == "TRUE" && (resultTable.size() == 0 || resultTable.begin()->second.size() != 0)) {
			std::unordered_map<std::string, std::vector<std::string>> projectTable =
				getProjectTable(declarations, selectedVar, resultTable);
			resultSet = toStringSet(declarations, selectedVar, projectTable);
	}
	return resultSet;
}

/*
Translate the projectTable to 
set of strings.
*/
std::unordered_set<std::string> QueryEvaluator::toStringSet(
	std::unordered_map<std::string, std::string> declarations,
	std::vector<std::string> selectedVar,
	std::unordered_map<std::string, std::vector<std::string>> projectTable) {
	std::unordered_set<std::string> resultSet;
	int projectedSize = projectTable.begin()->second.size();
	for (std::vector<std::string>::size_type i = 0; i != projectedSize; i++) {
		std::string tuple;
		for (std::vector<std::string>::size_type j = 0; j != selectedVar.size(); j++) {
			std::string currentVar = selectedVar[j];
			if (tuple.size() == 0) {
				if (hasReference(currentVar)) {
					std::string attr = attrOf(currentVar);
					tuple = toAttrRefVal(declarations, currentVar, projectTable[attr][i]);
				}
				else {
					tuple = projectTable[currentVar][i];
				}
			}
			else {
				if (hasReference(currentVar)) {
					std::string attr = attrOf(currentVar);
					tuple = tuple + " " +
						toAttrRefVal(declarations, currentVar, projectTable[attr][i]);
				}
				else {
					tuple = tuple + " " + projectTable[currentVar][i];
				}
			}
		}
		resultSet.insert(tuple);
	}
	return resultSet;
}

/*
Get the project table  
with selected variables as 
the columns from resultTable.
*/
std::unordered_map<std::string, std::vector<std::string>> QueryEvaluator::getProjectTable(
	std::unordered_map<std::string, std::string> declarations,
	std::vector<std::string> selectedVar,
	std::unordered_map<std::string, std::vector<std::string>> resultTable) {
	std::unordered_map<std::string, std::vector<std::string>> projectTable;
	std::vector<std::string> notInResult;
	for (std::vector<std::string>::size_type i = 0; i != selectedVar.size(); i++) {
		std::string currentVar = selectedVar[i];
		if (hasReference(currentVar)) {
			std::string attr = attrOf(currentVar);
			if (resultTable.count(attr) == 0) {
				notInResult.push_back(attr);
			}
			else {
				projectTable.insert({ attr, resultTable[attr] });
			}
		}
		else if (resultTable.count(currentVar) == 0) {
			notInResult.push_back(currentVar);
		}
		else {
			projectTable.insert({ currentVar, resultTable[currentVar] });
		}
	}
	for (std::vector<std::string>::size_type i = 0; i != notInResult.size(); i++) {
		projectTable = ContainerUtil::product(projectTable, getStmts(declarations, notInResult[i]));
	}
	return projectTable;
}

/*
Merge all the results of each clause
*/
std::pair<std::string, std::unordered_map<std::string, std::vector<std::string>>> QueryEvaluator::evaluateTable(
	std::unordered_map<std::string, std::string> declarations,
	std::vector<std::pair<std::string, std::pair<std::string, std::string>>> suchThatCondition,
	std::vector<std::pair<std::string, std::pair<std::string, std::string>>> patternCondition,
	std::vector<std::pair<std::string, std::string>> withCondition) {
	std::unordered_map<std::string, std::vector<std::string>> resultTable;
	std::string status = "TRUE";
	if (suchThatCondition.size() != 0) {
		for (std::vector<std::pair<std::string, std::pair<std::string, std::string>>>::size_type i = 0;
			i != suchThatCondition.size();
			i++) {
			std::string relation = suchThatCondition[i].first;
			std::string firstArgument = suchThatCondition[i].second.first;
			std::string secondArgument = suchThatCondition[i].second.second;
			std::string trivialness = isSuchThatTrivial(relation, firstArgument, secondArgument);
			if (trivialness == "false") {
				status = "FALSE";
				break;
			}
			if (trivialness == "not trivial") {
				std::unordered_map<std::string, std::vector<std::string>> newTable = evaluateSuchThat(
					declarations, relation, firstArgument, secondArgument);
				resultTable = ContainerUtil::product(resultTable, newTable);
				if (resultTable.begin()->second.size() == 0) {
					status = "FALSE";
					break;
				}
			}
		}
	}
	if (patternCondition.size() != 0 && status == "TRUE") {
		for (std::vector<std::pair<std::string, std::pair<std::string, std::string>>>::size_type i = 0;
			i != patternCondition.size();
			i++) {
			std::unordered_map<std::string, std::vector<std::string>> newTable = evaluatePatternCondition(
				declarations, patternCondition[i]);
			resultTable = ContainerUtil::product(resultTable, newTable);
			if (resultTable.begin()->second.size() == 0) {
				status = "FALSE";
				break;
			}
		}
	}
	if (withCondition.size() != 0 && status == "TRUE") {
		for (std::vector<std::pair<std::string, std::string>>::size_type i = 0;
			i != withCondition.size();
			i++) {
			std::string left = withCondition[i].first;
			std::string right = withCondition[i].second;
			std::string trivialness = isWithTrivial(left, right);
			if (trivialness == "false") {
				status = "FALSE";
				break;
			}
			if (trivialness == "not trivial") {
				std::unordered_map<std::string, std::vector<std::string>> newTable = evaluateWithCondition(
					declarations, left, right);
				resultTable = ContainerUtil::product(resultTable, newTable);
				if (resultTable.begin()->second.size() == 0) {
					status = "FALSE";
					break;
				}
			}
		}
	}
	std::pair<std::string, std::unordered_map<std::string, std::vector<std::string>>> resultPair(status,
		resultTable);
	return resultPair;
}

/*
The function checks if 
the with clause is trivial
and evaluates if it is
*/
std::string QueryEvaluator::isWithTrivial(std::string left, std::string right) {
	if ((isQuoted(left) && isQuoted(right)) || (isInteger(left) && isInteger(right))) {
		return truthValue(left == right);
	}
	return "not trivial";
}

/*
The function returns a 
table consist of 2 columns.
The first one is the attribute
value column and the second one 
is the reference value column, but 
the key is changed to "name". 
For example, if attr is r.varName,
then it returns table with column r 
and r.varName, but the r.varName is 
changed to "name".
*/
std::unordered_map<std::string, std::vector<std::string>> QueryEvaluator::evaluateWithPair(
	std::unordered_map<std::string, std::string> declarations,
	std::string attr) {
	std::string attrType = declarations[attr];
	if (attrType == "call") {
		return ContainerUtil::to_mapvec(attr, "name", PKB().getStmProcCallPairs());
	}
	else if (attrType == "print") {
		return ContainerUtil::to_mapvec(attr, "name", PKB().getPrintPairs());
	}
	else if (attrType == "read") {
		return ContainerUtil::to_mapvec(attr, "name", PKB().getReadPairs());
	}
	else {
		std::vector<std::string> attrVal= getStmts(declarations, attr)[attr];
		std::unordered_map<std::string, std::vector<std::string>> doubleMap;
		doubleMap.insert({ {attr, attrVal}, {"name", attrVal} });
		return doubleMap;
	}
}


/*
The function evaluates the non-trivial
with clauses.
*/
std::unordered_map<std::string, std::vector<std::string>> QueryEvaluator::evaluateWithCondition(
	std::unordered_map<std::string, std::string> declarations,
	std::string left, std::string right) {
	std::unordered_map<std::string, std::vector<std::string>> withMap;
	if (isSynonym(left) && !hasReference(right) && !isSynonym(right)) {
		if (stoi(right) < PKB().getTotalStmNo()) {
			return ContainerUtil::to_mapvec(left, right);
		}
		std::vector<std::string> emptyVec;
		withMap.insert({ left, emptyVec });
		return withMap;
	}
	else if (!hasReference(left) && !isSynonym(left) && isSynonym(right)) {
		if (stoi(left) < PKB().getTotalStmNo()) {
			return ContainerUtil::to_mapvec(right, left);
		}
		std::vector<std::string> emptyVec;
		withMap.insert({ right, emptyVec });
		return withMap;
	}
	else if (hasReference(left) && !hasReference(right) && !isSynonym(right)) {
		std::string attr = attrOf(left);
		std::string ref = refOf(left);
		std::string attrType = declarations[attr];
		if ((attrType == "call") && (ref == "procName")) {
			return ContainerUtil::to_mapvec(attr, PKB().getStmCalling(trimFrontEnd(right)));
		}
		else if ((attrType == "read") && (ref == "varName")) {
			return evaluateSuchThat(declarations, "Modifies", attr, right);
		}
		else if ((attrType == "print") && (ref == "varName")) {
			return evaluateSuchThat(declarations, "Uses", attr, right);
		}
		else {
			std::vector<std::string> attrVal = getStmts(declarations, attr)[attr];
			if (!isQuoted(right)
				&& std::find(attrVal.begin(), attrVal.end(), right) != attrVal.end()) {
				return ContainerUtil::to_mapvec(attr, right);
			}
			if (isQuoted(right)
				&& std::find(attrVal.begin(), attrVal.end(), trimFrontEnd(right)) != attrVal.end()) {
				return ContainerUtil::to_mapvec(attr, trimFrontEnd(right));
			}
			std::vector<std::string> emptyVec;
			withMap.insert({ attr, emptyVec });
			return withMap;
		}
	}
	else if (!hasReference(left) && !isSynonym(left) && hasReference(right)) {
		std::string attr = attrOf(right);
		std::string ref = refOf(right);
		std::string attrType = declarations[attr];
		if ((attrType == "call") && (ref == "procName")) {
			return ContainerUtil::to_mapvec(attr, PKB().getStmCalling(trimFrontEnd(left)));
		}
		else if ((attrType == "read") && (ref == "procName")) {
			return evaluateSuchThat(declarations, "Modifies", attr, left);
		}
		else if ((attrType == "print") && (ref == "procName")) {
			return evaluateSuchThat(declarations, "Uses", attr, left);
		}
		else {
			std::vector<std::string> attrVal = getStmts(declarations, attr)[attr];
			if (!isQuoted(left)
				&& std::find(attrVal.begin(), attrVal.end(), left) != attrVal.end()) {
				return ContainerUtil::to_mapvec(attr, left);
			}
			if (isQuoted(left)
				&& std::find(attrVal.begin(), attrVal.end(), trimFrontEnd(left)) != attrVal.end()) {
				return ContainerUtil::to_mapvec(attr, trimFrontEnd(left));
			}
			std::vector<std::string> emptyVec;
			withMap.insert({ attr, emptyVec });
			return withMap;
		}
	}
	else if (hasReference(left) && isSynonym(right)) {
		std::string attr = attrOf(left);
		return ContainerUtil::intersectOne(getStmts(declarations, attr),
			getStmts(declarations, right));
	}
	else if (isSynonym(left) && hasReference(right)) {
		std::string attr = attrOf(right);
		return ContainerUtil::intersectOne(getStmts(declarations, left),
			getStmts(declarations, attr));
	}
	else {
		std::string leftAttr = attrOf(left);
		std::string leftRef = refOf(left);
		std::string leftAttrType = declarations[leftAttr];
		std::string rightAttr = attrOf(right);
		std::string rightAttrType = declarations[rightAttr];
		if (leftRef == "stmt#" || leftRef == "value") {
			return ContainerUtil::intersectOne(getStmts(declarations, leftAttr),
				getStmts(declarations, rightAttr));
		}
		else {
			return ContainerUtil::intersectTwo(evaluateWithPair(declarations, leftAttr),
				evaluateWithPair(declarations, rightAttr));
		}
	}
}

/*
The function evaluates
pattern clauses
*/
std::unordered_map<std::string, std::vector<std::string>> QueryEvaluator::evaluatePatternCondition(
	std::unordered_map<std::string, std::string> declarations,
	std::pair<std::string, std::pair<std::string, std::string>> pattern) {
	std::string patternSynonym = pattern.first;
	std::string patternType = declarations[patternSynonym];
	std::string leftArgument = pattern.second.first;
	std::string rightArgument = pattern.second.second;

	if (patternType == "assign") {
		if (rightArgument == "_") {
			if (leftArgument == "_") {
				return getStmts(declarations, patternSynonym);
			}
			else if (isQuoted(leftArgument)) {
				return ContainerUtil::to_mapvec(patternSynonym,
					PKB().findPattern(trimFrontEnd(leftArgument), "", false));
			}
			return ContainerUtil::to_mapvec(patternSynonym, leftArgument, 
				PKB().findPatternPairs("", false));
		}
		else if (isQuoted(rightArgument)) {
			rightArgument = trimFrontEnd(rightArgument);
			rightArgument = ExpressionUtil::convertInfixToPrefix(rightArgument);
			if (leftArgument == "_") {
				return ContainerUtil::to_mapvec(patternSynonym, 
					PKB().findPattern(rightArgument, true));
			}
			else if (isQuoted(leftArgument)) {
				return ContainerUtil::to_mapvec(patternSynonym,
					PKB().findPattern(trimFrontEnd(leftArgument), rightArgument, true));
			}
			return ContainerUtil::to_mapvec(patternSynonym, leftArgument, 
				PKB().findPatternPairs(rightArgument, true));
		}
		else {
			rightArgument = trimFrontEnd(trimFrontEnd(rightArgument));
			rightArgument = ExpressionUtil::convertInfixToPrefix(rightArgument);
			if (leftArgument == "_") {
				return ContainerUtil::to_mapvec(patternSynonym, 
					PKB().findPattern(rightArgument, false));
			}
			else if (isQuoted(leftArgument)) {
				return ContainerUtil::to_mapvec(patternSynonym,
					PKB().findPattern(trimFrontEnd(leftArgument), rightArgument, false));
			}
			return ContainerUtil::to_mapvec(patternSynonym, leftArgument, 
				PKB().findPatternPairs(rightArgument, false));
		}
	}
	if (patternType == "if") {
		if (leftArgument == "_") {
			return ContainerUtil::to_mapvec(patternSynonym, PKB().getAllIfWithControls());
		}
		else if (isQuoted(leftArgument)) {
			return ContainerUtil::to_mapvec(patternSynonym, PKB().getIfStmWithControlVariable(
				trimFrontEnd(leftArgument)));
		}
		else {
			return ContainerUtil::to_mapvec(patternSynonym, leftArgument,
				PKB().getIfStmControlVariablePair());
		}
	}
	if (patternType == "while") {
		if (leftArgument == "_") {
			return ContainerUtil::to_mapvec(patternSynonym, PKB().getAllWhileWithControls());
		}
		else if (isQuoted(leftArgument)) {
			return ContainerUtil::to_mapvec(patternSynonym, PKB().getWhileStmWithControlVariable(
				trimFrontEnd(leftArgument)));
		}
		else {
			return ContainerUtil::to_mapvec(patternSynonym, leftArgument,
				PKB().getWhileStmControlVariablePair());
		}
	}
}

/*
THe function evaluates the such that conditions 
which gives boolean answer
*/
std::string QueryEvaluator::isSuchThatTrivial(std::string relation, std::string firstArgument, 
	std::string secondArgument) {
	bool result;
	if (relation == "Follows") {
		if (firstArgument == "_") {
			if (secondArgument == "_") {
				return truthValue(PKB().hasFollowRelation());
			}
			else if (isInteger(secondArgument)) {
				result = PKB().getStmFollowedBy(stoi(secondArgument)) > 0;
				return truthValue(result);
			}
		}
		else if (isInteger(firstArgument)) {
			if (secondArgument == "_") {
				result = PKB().getFollower(stoi(firstArgument)) > 0;
				return truthValue(result);
 			}
			else if (isInteger(secondArgument)) {
				result = PKB().getFollower(stoi(firstArgument)) == stoi(secondArgument);
				return truthValue(result);
			};
		}
		else if (firstArgument == secondArgument) {
			return "false";
		}
	}
	else if (relation == "Follows*") {
		if (firstArgument == "_") {
			if (secondArgument == "_") {
				return truthValue(PKB().hasFollowRelation());
			}
			else if (isInteger(secondArgument)) {
				result = PKB().getStmFollowedBy(stoi(secondArgument)) > 0;
				return truthValue(result);
			}
		}
		else if (isInteger(firstArgument)) {
			if (secondArgument == "_") {
				result = PKB().getFollower(stoi(firstArgument)) > 0;
				return truthValue(result);
			}
			else if (isInteger(secondArgument)) {
				result = PKB().hasFollowStarPair(stoi(firstArgument), stoi(secondArgument));
				return truthValue(result);
			}
		}
		else if (firstArgument == secondArgument) {
			return "false";
		}
	}
	else if (relation == "Parent") {
		if (firstArgument == "_") {
			if (secondArgument == "_") {
				return truthValue(PKB().hasParentRelation());
			}
			else if (isInteger(secondArgument)) {
				result = PKB().isChild(stoi(secondArgument)) > 0;
				return truthValue(result);
			}
		}
		else if (isInteger(firstArgument)) {
			if (secondArgument == "_") {
				result = PKB().isParent(stoi(firstArgument)) > 0;
				return truthValue(result);
			}
			else if (isInteger(secondArgument)) {
				result = PKB().getParent(stoi(secondArgument)) == stoi(firstArgument);
				return truthValue(result);
			}
		}
		else if (firstArgument == secondArgument) {
			return "false";
		}
	}
	else if (relation == "Parent*") {
		if (firstArgument == "_") {
			if (secondArgument == "_") {
				return truthValue(PKB().hasParentRelation());
			}
			else if (isInteger(secondArgument)) {
				result = PKB().isChild(stoi(secondArgument)) > 0;
				return truthValue(result);
			}
		}
		else if (isInteger(firstArgument)) {
			if (secondArgument == "_") {
				result = PKB().isParent(stoi(firstArgument)) > 0;
				return truthValue(result);
			}
			else if (isInteger(secondArgument)) {
				result = PKB().hasAncDescPair(stoi(firstArgument), stoi(secondArgument));
				return truthValue(result);
			}
		}
		else if (firstArgument == secondArgument) {
			return "false";
		}
	}
	else if (relation == "Uses") {
		if (isInteger(firstArgument)) {
			if (isQuoted(secondArgument)) {
				result = PKB().isStmUsing(stoi(firstArgument), trimFrontEnd(secondArgument));
				return truthValue(result);
			}
			else if (secondArgument == "_") {
				result = PKB().getVarUsedByStm(stoi(firstArgument)).size() > 0;
				return truthValue(result);
			}
		}
		else if (isQuoted(firstArgument)) {
			if (isQuoted(secondArgument)) {
				result = PKB().isProcUsing(trimFrontEnd(firstArgument), trimFrontEnd(secondArgument));
				return truthValue(result);
			}
			else if (secondArgument == "_") {
				result = PKB().getVarUsedByProc(trimFrontEnd(firstArgument)).size() > 0;
				return truthValue(result);
			}
		}
	}
	else if (relation == "Modifies") {
		if (isInteger(firstArgument)) {
			if (isQuoted(secondArgument)) {
				result = PKB().isStmModifying(stoi(firstArgument), trimFrontEnd(secondArgument));
				return truthValue(result);
			}
			else if (secondArgument == "_") {
				result = PKB().getVarModifiedByStm(stoi(firstArgument)).size() > 0;
				return truthValue(result);
			}
		}
		else if (isQuoted(firstArgument)) {
			if (isQuoted(secondArgument)) {
				result = PKB().isProcModifying(trimFrontEnd(firstArgument), trimFrontEnd(secondArgument));
				return truthValue(result);
			}
			else if (secondArgument == "_") {
				result = PKB().getVarModifiedByProc(trimFrontEnd(firstArgument)).size() > 0;
				return truthValue(result);
			}
		}
	}
	else if (relation == "Calls") {
		if (firstArgument == "_") {
			if (secondArgument == "_") {
				return truthValue(PKB().hasCallRelation());
			}
			else if (isQuoted(secondArgument)) {
				return truthValue(PKB().isCallee(trimFrontEnd(secondArgument)));
			}
		}
		else if (isQuoted(firstArgument)) {
			if (secondArgument == "_") {
				return truthValue(PKB().isCaller(trimFrontEnd(firstArgument)));
			}
			else if (isQuoted(secondArgument)) {
				result = PKB().getCallee(trimFrontEnd(firstArgument)).count(trimFrontEnd(secondArgument)) == 1;
				return truthValue(result);
			}
		}
		else if (firstArgument == secondArgument) {
			return "false";
		}
	}
	else if (relation == "Calls*") {
		if (firstArgument == "_") {
			if (secondArgument == "_") {
				return truthValue(PKB().hasCallRelation());
			}
			else if (isQuoted(secondArgument)) {
				return truthValue(PKB().isCallee(trimFrontEnd(secondArgument)));
			}
		}
		else if (isQuoted(firstArgument)) {
			if (secondArgument == "_") {
				return truthValue(PKB().isCaller(trimFrontEnd(firstArgument)));
			}
			else if (isQuoted(secondArgument)) {
				result = PKB().hasCallStarPair(trimFrontEnd(firstArgument), trimFrontEnd(secondArgument));
				return truthValue(result);
			}
		}
		else if (firstArgument == secondArgument) {
			return "false";
		}
	}
	if (relation == "Next") {
		if (firstArgument == "_") {
			if (secondArgument == "_") {
				return truthValue(PKB().hasNextRelation());
			}
			else if (isInteger(secondArgument)) {
				result = PKB().getPrev(stoi(secondArgument)).size() > 0;
				return truthValue(result);
			}
		}
		else if (isInteger(firstArgument)) {
			if (secondArgument == "_") {
				result = PKB().getNext(stoi(firstArgument)).size() > 0;
				return truthValue(result);
			}
			else if (isInteger(secondArgument)) {
				result = PKB().getNext(stoi(firstArgument)).count(stoi(secondArgument)) == 1;
				return truthValue(result);
			};
		}
		else if (firstArgument == secondArgument) {
			return "false";
		}
	}
	if (relation == "Next*") {
		if (firstArgument == "_") {
			if (secondArgument == "_") {
				return truthValue(PKB().hasNextRelation());
			}
			else if (isInteger(secondArgument)) {
				result = PKB().getPrev(stoi(secondArgument)).size() > 0;
				return truthValue(result);
			}
		}
		else if (isInteger(firstArgument)) {
			if (secondArgument == "_") {
				result = PKB().getNext(stoi(firstArgument)).size() > 0;
				return truthValue(result);
			}
			else if (isInteger(secondArgument)) {
				result = PKB().hasNextStarPair(stoi(firstArgument), stoi(secondArgument)) == 1;
				return truthValue(result);
			};
		}
	}
	return "not trivial";
}

/*
The function evaluates the non-trivial 
Such That Conditions.
*/
std::unordered_map<std::string, std::vector<std::string>> QueryEvaluator::evaluateSuchThat(
	std::unordered_map<std::string, std::string> declarations,
	std::string relation, std::string firstArgument, std::string secondArgument) {
	std::unordered_map<std::string, std::vector<std::string>> tableResult;
	if (relation == "Follows") {
		if (isSynonym(firstArgument)) {
			if (secondArgument == "_") {
				tableResult = ContainerUtil::to_mapvec(firstArgument, PKB().getAllFollowed());
			}
			else if (isInteger(secondArgument)) {
				int result = PKB().getStmFollowedBy(stoi(secondArgument));
				if (result > 0) {
					tableResult = ContainerUtil::to_mapvec(firstArgument, result);
				}
				else {
					std::vector<std::string> emptyVec;
					tableResult.insert({ firstArgument, emptyVec });
				}
			}
			else {
				tableResult = ContainerUtil::to_mapvec(firstArgument, secondArgument, 
					PKB().getFollowPairs());
			}
		}
		if (isSynonym(secondArgument)) {
			if (firstArgument == "_") {
				tableResult = ContainerUtil::to_mapvec(secondArgument, PKB().getAllFollowers());
			}
			else if (isInteger(firstArgument)) {
				tableResult = ContainerUtil::to_mapvec(secondArgument, PKB().getFollower(
					stoi(firstArgument)));
			}
		}
	}
	else if (relation == "Follows*") {
		if (isSynonym(firstArgument)) {
			if (secondArgument == "_") {
				tableResult = ContainerUtil::to_mapvec(firstArgument, PKB().getAllFollowed());
			}
			else if (isInteger(secondArgument)) {
				tableResult = ContainerUtil::to_mapvec(firstArgument, PKB().getAllFollowedBy(
					stoi(secondArgument)));
			}
			else {
				tableResult = ContainerUtil::to_mapvec(firstArgument, secondArgument, 
					PKB().getFollowStarPairs());
			}
		}
		if (isSynonym(secondArgument)) {
			if (firstArgument == "_") {
				tableResult = ContainerUtil::to_mapvec(secondArgument, PKB().getAllFollowers());
			}
			else if (isInteger(firstArgument)) {
				tableResult = ContainerUtil::to_mapvec(secondArgument, PKB().getAllFollowing(
					stoi(firstArgument)));
			}
		}
	}
	if (relation == "Parent") {
		if (isSynonym(firstArgument)) {
			if (secondArgument == "_") {
				tableResult = ContainerUtil::to_mapvec(firstArgument, PKB().getAllParents());
			}
			else if (isInteger(secondArgument)) {
				int result = PKB().getParent(stoi(secondArgument));
				if (result > 0) {
					tableResult = ContainerUtil::to_mapvec(firstArgument, result);
				}
				else {
					std::vector<std::string> emptyVec;
					tableResult.insert({ firstArgument, emptyVec });
				}

			}
			else {
				tableResult = ContainerUtil::to_mapvec(firstArgument, secondArgument, 
					PKB().getParentChildPairs());
			}
		}
		if (isSynonym(secondArgument)) {
			if (firstArgument == "_") {
				tableResult = ContainerUtil::to_mapvec(secondArgument, PKB().getAllChildren());
			}
			else if (isInteger(firstArgument)) {
				tableResult = ContainerUtil::to_mapvec(secondArgument, PKB().getChildren(
					stoi(firstArgument)));
			}
		}
	}
	else if (relation == "Parent*") {
		if (isSynonym(firstArgument)) {
			if (secondArgument == "_") {
				tableResult = ContainerUtil::to_mapvec(firstArgument, PKB().getAllParents());
			}
			else if (isInteger(secondArgument)) {
				tableResult = ContainerUtil::to_mapvec(firstArgument, PKB().getAncestors(
					stoi(secondArgument)));
			}
			else {
				tableResult = ContainerUtil::to_mapvec(firstArgument, secondArgument, 
					PKB().getAncDescPairs());
			}
		}
		if (isSynonym(secondArgument)) {
			if (firstArgument == "_") {
				tableResult = ContainerUtil::to_mapvec(secondArgument, PKB().getAllChildren());
			}
			else if (isInteger(firstArgument)) {
				tableResult = ContainerUtil::to_mapvec(secondArgument, PKB().getDescendants(
					stoi(firstArgument)));
			}
		}
	}
	else if (relation == "Uses") {
		if (isSynonym(secondArgument)) {
			if (isInteger(firstArgument)) {
				tableResult = ContainerUtil::to_mapvec(secondArgument, PKB().getVarUsedByStm(
					stoi(firstArgument)));
			}
			else if (isQuoted(firstArgument)) {
				tableResult = ContainerUtil::to_mapvec(secondArgument, PKB().getVarUsedByProc(
					trimFrontEnd(firstArgument)));
			}
			else if (declarations[firstArgument] == "procedure") {
				return ContainerUtil::to_mapvec(firstArgument, secondArgument, 
					PKB().getProcVarUsePairs());
			}
			else {
				tableResult = ContainerUtil::to_mapvec(firstArgument, secondArgument,
					PKB().getStmVarUsePairs());
			}
		}
		if (declarations[firstArgument] != "procedure") {
			if (secondArgument == "_") {
				tableResult = ContainerUtil::to_mapvec(firstArgument, PKB().getStmUsing(""));
			}
			else if (isQuoted(secondArgument)) {
				tableResult = ContainerUtil::to_mapvec(firstArgument, PKB().getStmUsing(
					trimFrontEnd(secondArgument)));
			}
		}
		else {
			if (secondArgument == "_") {
				tableResult = ContainerUtil::to_mapvec(firstArgument, PKB().getProcUsing(""));
			}
			else if (isQuoted(secondArgument)) {
				tableResult = ContainerUtil::to_mapvec(firstArgument, PKB().getProcUsing(
					trimFrontEnd(secondArgument)));
			}
		}
	}
	else if (relation == "Modifies") {
		if (isSynonym(secondArgument)) {
			if (isInteger(firstArgument)) {
				tableResult = ContainerUtil::to_mapvec(secondArgument, PKB().getVarModifiedByStm(
				stoi(firstArgument)));
			}
			else if (isQuoted(firstArgument)) {
				tableResult = ContainerUtil::to_mapvec(secondArgument, PKB().getVarModifiedByProc(
				trimFrontEnd(firstArgument)));
			}
			else if (declarations[firstArgument] == "procedure") {
				return ContainerUtil::to_mapvec(firstArgument, secondArgument, 
					PKB().getProcVarModifyPairs());
			}
			else {
				tableResult = ContainerUtil::to_mapvec(firstArgument, secondArgument,
					PKB().getStmVarModifyPairs());
			}
		}
		if (declarations[firstArgument] != "procedure") {
			if (secondArgument == "_") {
				tableResult = ContainerUtil::to_mapvec(firstArgument, PKB().getStmModifying(""));
			}
			else if (isQuoted(secondArgument)) {
				tableResult = ContainerUtil::to_mapvec(firstArgument, PKB().getStmModifying(
					trimFrontEnd(secondArgument)));
			}
		}
		else {
			if (secondArgument == "_") {
				tableResult = ContainerUtil::to_mapvec(firstArgument, PKB().getProcModifying(""));
			}
			else if (isQuoted(secondArgument)) {
				tableResult = ContainerUtil::to_mapvec(firstArgument, PKB().getProcModifying(
					trimFrontEnd(secondArgument)));
			}
		}
	}
	if (relation == "Calls") {
		if (isSynonym(firstArgument)) {
			if (secondArgument == "_") {
				tableResult = ContainerUtil::to_mapvec(firstArgument, PKB().getAllCallers());
			}
			else if (isQuoted(secondArgument)) {
				tableResult = ContainerUtil::to_mapvec(firstArgument, PKB().getCaller(
					trimFrontEnd(secondArgument)));
			}
			else {
				tableResult = ContainerUtil::to_mapvec(firstArgument, secondArgument,
					PKB().getCallPairs());
			}
		}
		if (isSynonym(secondArgument)) {
			if (firstArgument == "_") {
				tableResult = ContainerUtil::to_mapvec(secondArgument, PKB().getAllCallees());
			}
			else if (isQuoted(firstArgument)) {
				tableResult = ContainerUtil::to_mapvec(secondArgument, PKB().getCallee(
					trimFrontEnd(firstArgument)));
			}
		}
	}
	if (relation == "Calls*") {
		if (isSynonym(firstArgument)) {
			if (secondArgument == "_") {
				tableResult = ContainerUtil::to_mapvec(firstArgument, PKB().getAllCallers());
			}
			else if (isQuoted(secondArgument)) {
				tableResult = ContainerUtil::to_mapvec(firstArgument, PKB().getCallAnc(
					trimFrontEnd(secondArgument)));
			}
			else {
				tableResult = ContainerUtil::to_mapvec(firstArgument, secondArgument,
					PKB().getCallStarPairs());
			}
		}
		if (isSynonym(secondArgument)) {
			if (firstArgument == "_") {
				tableResult = ContainerUtil::to_mapvec(secondArgument, PKB().getAllCallees());
			}
			else if (isQuoted(firstArgument)) {
				tableResult = ContainerUtil::to_mapvec(secondArgument, PKB().getCallDesc(
					trimFrontEnd(firstArgument)));
			}
		}
	}
	if (relation == "Next") {
		if (isSynonym(firstArgument)) {
			if (secondArgument == "_") {
				tableResult = ContainerUtil::to_mapvec(firstArgument, PKB().getAllPrev());
			}
			else if (isInteger(secondArgument)) {
				tableResult = ContainerUtil::to_mapvec(firstArgument, PKB().getPrev(
					stoi(secondArgument)));
			}
			else {
				tableResult = ContainerUtil::to_mapvec(firstArgument, secondArgument,
					PKB().getNextPairs());
			}
		}
		if (isSynonym(secondArgument)) {
			if (firstArgument == "_") {
				tableResult = ContainerUtil::to_mapvec(secondArgument, PKB().getAllNext());
			}
			else if (isInteger(firstArgument)) {
				tableResult = ContainerUtil::to_mapvec(secondArgument, PKB().getNext(
					stoi(firstArgument)));
			}
		}
	}
	if (relation == "Next*") {
		if (isSynonym(firstArgument)) {
			if (secondArgument == "_") {
				tableResult = ContainerUtil::to_mapvec(firstArgument, PKB().getAllPrev());
			}
			else if (isInteger(secondArgument)) {
				tableResult = ContainerUtil::to_mapvec(firstArgument, PKB().getAllLnBefore(
					stoi(secondArgument)));
			}
			else {
				tableResult = ContainerUtil::to_mapvec(firstArgument, secondArgument,
					PKB().getNextStarPairs());
			}
		}
		if (isSynonym(secondArgument)) {
			if (firstArgument == "_") {
				tableResult = ContainerUtil::to_mapvec(secondArgument, PKB().getAllNext());
			}
			else if (isInteger(firstArgument)) {
				tableResult = ContainerUtil::to_mapvec(secondArgument, PKB().getAllLnAfter(
					stoi(firstArgument)));
			}
		}
	}

	if (tableResult.size() == 1) {
		if (isSynonym(firstArgument)) {
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
The function returns the list of all statements.
*/
std::unordered_set<std::string> QueryEvaluator::getAllStms() {
	unordered_set<std::string> allStms;
	for (int i = 1; i <= PKB().getTotalStmNo(); i++) {
		allStms.insert(to_string(i));
	}

	return allStms;
}

/*
The function retrieves all statements 
of a given type
*/
std::unordered_map<std::string, std::vector<std::string>> QueryEvaluator::getStmts(
	std::unordered_map<std::string, std::string> declarations,
	std::string syn) {
	std::string synType = declarations[syn];
	if (synType == "stmt" || synType == "prog_line") {
		return ContainerUtil::to_mapvec(syn, getAllStms());
	}
	else if (synType == "read") {
		return ContainerUtil::to_mapvec(syn, PKB().getReadStms());
	}
	else if (synType == "print") {
		return ContainerUtil::to_mapvec(syn, PKB().getPrintStms());
	}
	else if (synType == "while") {
		return ContainerUtil::to_mapvec(syn, PKB().getWhileStms());
	}
	else if (synType == "if") {
		return ContainerUtil::to_mapvec(syn, PKB().getIfStms());
	}
	else if (synType == "assign") {
		return ContainerUtil::to_mapvec(syn, PKB().getAssignStms());
	}
	else if (synType == "variable") {
		return ContainerUtil::to_mapvec(syn, PKB().getVariables());
	}
	else if (synType == "constant") {
		return ContainerUtil::to_mapvec(syn, PKB().getConstants());
	}
	else if (synType == "procedure") {
		return ContainerUtil::to_mapvec(syn, PKB().getProcList());
	}
	else if (synType == "call") {
		return ContainerUtil::to_mapvec(syn, PKB().getCallStms());
	}
}

/*
The function does filtering
for singleton results from PKB 
so that the output satisfy a given
type.
*/
std::unordered_map<std::string, std::vector<std::string>> QueryEvaluator::filterType(std::string synonym,
	std::unordered_map<std::string, std::string> declarations,
	std::unordered_map<std::string, std::vector<std::string>> toBeFiltered) {
	std::unordered_map<std::string, std::vector<std::string>> filteredTable;
	std::string synonymType = declarations[synonym];
	std::unordered_map<std::string, std::vector<std::string>> typeRequiredSet = getStmts(
		declarations, synonym);
	std::vector<std::string> filter = typeRequiredSet[synonym];
	if (synonymType == "stmt") {
		return toBeFiltered;
	} 
	else {
		std::vector<std::string> oldColumn = toBeFiltered[synonym];
		std::vector<std::string> newColumn;
		for (std::vector<std::string>::size_type i = 0; i != oldColumn.size(); i++) {
			if (std::find(filter.begin(), filter.end(), oldColumn[i]) != filter.end()) {
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
std::unordered_map<std::string, std::vector<std::string>> QueryEvaluator::filterType(std::string synonym1, 
	std::string synonym2, std::unordered_map<std::string, std::string> declarations,
	std::unordered_map<std::string, std::vector<std::string>> toBeFiltered) {
	std::unordered_map<std::string, std::vector<std::string>> filteredSet;
	std::string synonym1Type = declarations[synonym1];
	std::string synonym2Type = declarations[synonym2];
	std::unordered_map<std::string, std::vector<std::string>> type1RequiredSet = getStmts(
		declarations, synonym1);
	std::unordered_map<std::string, std::vector<std::string>> type2RequiredSet = getStmts(
		declarations, synonym2);
	std::vector<std::string> filter1 = type1RequiredSet[synonym1];
	std::vector<std::string> filter2 = type2RequiredSet[synonym2];
	std::vector<std::string> oldColumn1 = toBeFiltered[synonym1];
	std::vector<std::string> oldColumn2 = toBeFiltered[synonym2];
	if (synonym1Type == "stmt" && synonym2Type == "stmt") {
		return toBeFiltered;
	}
	else if (synonym1Type == "stmt" && synonym2Type != "stmt") {
		for (auto it = toBeFiltered.begin(); it != toBeFiltered.end(); ++it) {
			std::vector<std::string> newColumn;
			for (std::vector<std::string>::size_type i = 0; i != oldColumn2.size(); i++) {
				if (std::find(filter2.begin(), filter2.end(), oldColumn2[i]) != filter2.end()) {
					newColumn.push_back(it->second[i]);
				}
			}
			filteredSet.insert({ it->first, newColumn });
		}
	}
	else if (synonym1Type != "stmt" && synonym2Type == "stmt") {
		for (auto it = toBeFiltered.begin(); it != toBeFiltered.end(); ++it) {
			std::vector<std::string> newColumn;
			for (std::vector<std::string>::size_type i = 0; i != oldColumn1.size(); i++) {
				if (std::find(filter1.begin(), filter1.end(), oldColumn1[i]) != filter1.end()) {
					newColumn.push_back(it->second[i]);
				}
			}
			filteredSet.insert({ it->first, newColumn });
		}
	}
	else if (synonym1Type != "stmt" && synonym2Type != "stmt") {
		for (auto it = toBeFiltered.begin(); it != toBeFiltered.end(); ++it) {
			std::vector<std::string> newColumn;
			for (std::vector<std::string>::size_type i = 0; i != oldColumn1.size(); i++) {
				if (std::find(filter1.begin(), filter1.end(), oldColumn1[i]) != filter1.end() 
					&& std::find(filter2.begin(), filter2.end(), oldColumn2[i]) != filter2.end()) {
					newColumn.push_back(it->second[i]);
				}
			}
			filteredSet.insert({ it->first, newColumn });
		}
	}
	return filteredSet;
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
	std::string attr = attrOf(attrRef);
	std::string ref = refOf(attrRef);
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

/*
The function transforms a boolean value
into a string.
*/
std::string QueryEvaluator::truthValue(bool boolean) {
	if (boolean) {
		return "true";
	}

	return "false";
}

/*
Trims quote in front and end of a string.
*/
std::string QueryEvaluator::trimFrontEnd(std::string quotedString) {
	return quotedString.substr(1, quotedString.size() - 2);
}

/*
Checks if the string is an integer
*/
bool QueryEvaluator::isInteger(std::string s) {
	bool result = LexicalToken::verifyInteger(s);
	return result;
}

/*
Checks if the string is quoted
*/
bool QueryEvaluator::isQuoted(std::string s) {
	bool result = s[0] == '"';
	return result;
}

/*
Checks if the string is a synonym
*/
bool QueryEvaluator::isSynonym(std::string s) {
	bool result = !isInteger(s) && !isQuoted(s) && !hasReference(s) && s != "_";
	return result;
}

/*
Checks if the string contains a reference
*/
bool QueryEvaluator::hasReference(std::string s) {
	bool result = s.find(".") != std::string::npos;
	return result;
}

/*
Get attribute of an
attribute reference.
*/
std::string QueryEvaluator::attrOf(std::string s) {
	std::size_t dotPos = s.find(".");
	return s.substr(0, dotPos);
}

/*
Get reference of an
attribute reference.
*/
std::string QueryEvaluator::refOf(std::string s) {
	std::size_t dotPos = s.find(".");
	return s.substr(dotPos + 1, s.size() - dotPos - 1);
}
	
