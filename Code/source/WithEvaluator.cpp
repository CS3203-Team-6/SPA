#pragma once

#include "WithEvaluator.h"

/*
The function checks if
the with clause is trivial
and evaluates if it is
*/
std::string WithEvaluator::isTrivial(std::string left, std::string right) {
	if ((QueryUtility::isQuoted(left) && QueryUtility::isQuoted(right)) 
		|| (QueryUtility::isInteger(left) && QueryUtility::isInteger(right))) {
		return QueryUtility::truthValue(left == right);
	}
	return "NON TRIVIAL";
}

/*
The function returns a
table consist of 2 columns.
The first one is the attribute
value column and the second one
is the reference value column, but
the key is changed to "name". 
The attribute is either procName
or varName. For example, if is r,
then it returns table with column r
and r.varName, but the r.varName is
changed to "name".
*/
std::unordered_map<std::string, std::vector<std::string>> WithEvaluator::evaluatePair(
	std::unordered_map<std::string, std::string> declarations,
	std::string attr) {
	std::string attrType = declarations[attr];
	if (attrType == "call") {
		return QueryUtility::to_mapvec(attr, "name", PKB().getStmProcCallPairs());
	}
	else if (attrType == "print") {
		return QueryUtility::to_mapvec(attr, "name", PKB().getPrintPairs());
	}
	else if (attrType == "read") {
		return QueryUtility::to_mapvec(attr, "name", PKB().getReadPairs());
	}
	else {
		std::vector<std::string> attrVal = QueryUtility::getStmtsMap(declarations, attr)[attr];
		std::unordered_map<std::string, std::vector<std::string>> doubleMap;
		doubleMap.insert({ {attr, attrVal}, {"name", attrVal} });
		return doubleMap;
	}
}

/*
The function evaluates the non-trivial
with clauses.
*/
std::unordered_map<std::string, std::vector<std::string>> WithEvaluator::evaluate(
	std::unordered_map<std::string, std::string> declarations,
	std::string left, std::string right) {
	std::unordered_map<std::string, std::vector<std::string>> withMap;
	if (QueryUtility::isSynonym(left) 
		&& !QueryUtility::hasReference(right) && !QueryUtility::isSynonym(right)) {
		if (stoi(right) < PKB().getTotalStmNo()) {
			return QueryUtility::to_mapvec(left, right);
		}
		std::vector<std::string> emptyVec;
		withMap.insert({ left, emptyVec });
		return withMap;
	}
	else if (!QueryUtility::hasReference(left) 
		&& !QueryUtility::isSynonym(left) && QueryUtility::isSynonym(right)) {
		if (stoi(left) < PKB().getTotalStmNo()) {
			return QueryUtility::to_mapvec(right, left);
		}
		std::vector<std::string> emptyVec;
		withMap.insert({ right, emptyVec });
		return withMap;
	}
	else if (QueryUtility::hasReference(left) 
		&& !QueryUtility::hasReference(right) && !QueryUtility::isSynonym(right)) {
		std::string attr = QueryUtility::attrOf(left);
		std::string ref = QueryUtility::refOf(left);
		std::string attrType = declarations[attr];
		if ((attrType == "call") && (ref == "procName")) {
			return QueryUtility::to_mapvec(attr, PKB().getStmCalling(QueryUtility::trimFrontEnd(right)));
		}
		else if ((attrType == "read") && (ref == "varName")) {
			return SuchThatEvaluator::evaluate(declarations, "Modifies", attr, right);
		}
		else if ((attrType == "print") && (ref == "varName")) {
			return SuchThatEvaluator::evaluate(declarations, "Uses", attr, right);
		}
		else {
			std::vector<std::string> attrVal = QueryUtility::getStmtsMap(declarations, attr)[attr];
			if (!QueryUtility::isQuoted(right)
				&& std::find(attrVal.begin(), attrVal.end(), right) != attrVal.end()) {
				return QueryUtility::to_mapvec(attr, right);
			}
			if (QueryUtility::isQuoted(right)
				&& std::find(attrVal.begin(), attrVal.end(), QueryUtility::trimFrontEnd(right)) != attrVal.end()) {
				return QueryUtility::to_mapvec(attr, QueryUtility::trimFrontEnd(right));
			}
			std::vector<std::string> emptyVec;
			withMap.insert({ attr, emptyVec });
			return withMap;
		}
	}
	else if (!QueryUtility::hasReference(left) 
		&& !QueryUtility::isSynonym(left) && QueryUtility::hasReference(right)) {
		std::string attr = QueryUtility::attrOf(right);
		std::string ref = QueryUtility::refOf(right);
		std::string attrType = declarations[attr];
		if ((attrType == "call") && (ref == "procName")) {
			return QueryUtility::to_mapvec(attr, PKB().getStmCalling(QueryUtility::trimFrontEnd(left)));
		}
		else if ((attrType == "read") && (ref == "varName")) {
			return SuchThatEvaluator::evaluate(declarations, "Modifies", attr, left);
		}
		else if ((attrType == "print") && (ref == "varName")) {
			return SuchThatEvaluator::evaluate(declarations, "Uses", attr, left);
		}
		else {
			std::vector<std::string> attrVal = QueryUtility::getStmtsMap(declarations, attr)[attr];
			if (!QueryUtility::isQuoted(left)
				&& std::find(attrVal.begin(), attrVal.end(), left) != attrVal.end()) {
				return QueryUtility::to_mapvec(attr, left);
			}
			if (QueryUtility::isQuoted(left)
				&& std::find(attrVal.begin(), 
					attrVal.end(), QueryUtility::trimFrontEnd(left)) != attrVal.end()) {
				return QueryUtility::to_mapvec(attr, QueryUtility::trimFrontEnd(left));
			}
			std::vector<std::string> emptyVec;
			withMap.insert({ attr, emptyVec });
			return withMap;
		}
	}
	else if (QueryUtility::hasReference(left) && QueryUtility::isSynonym(right)) {
		std::string attr = QueryUtility::attrOf(left);
		return QueryUtility::intersectOne(QueryUtility::getStmtsMap(declarations, attr),
			QueryUtility::getStmtsMap(declarations, right));
	}
	else if (QueryUtility::isSynonym(left) && QueryUtility::hasReference(right)) {
		std::string attr = QueryUtility::attrOf(right);
		return QueryUtility::intersectOne(QueryUtility::getStmtsMap(declarations, left),
			QueryUtility::getStmtsMap(declarations, attr));
	}
	else {
		std::string leftAttr = QueryUtility::attrOf(left);
		std::string leftRef = QueryUtility::refOf(left);
		std::string leftAttrType = declarations[leftAttr];
		std::string rightAttr = QueryUtility::attrOf(right);
		std::string rightAttrType = declarations[rightAttr];
		if (leftRef == "stmt#" || leftRef == "value") {
			return QueryUtility::intersectOne(QueryUtility::getStmtsMap(declarations, leftAttr),
				QueryUtility::getStmtsMap(declarations, rightAttr));
		}
		else {
			return QueryUtility::intersectTwo(evaluatePair(declarations, leftAttr),
				evaluatePair(declarations, rightAttr));
		}
	}
}