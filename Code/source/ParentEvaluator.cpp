#pragma once

#include "ParentEvaluator.h"	

/*
Evaluate trivial Parent clause 
*/
std::string ParentEvaluator::isTrivial(std::string firstArgument, std::string secondArgument) {
	bool result;
	if (firstArgument == "_") {
		if (secondArgument == "_") {
			result = PKB().hasParentRelation();
		}
		else if (QueryUtility::isInteger(secondArgument)) {
			result = PKB().isChild(stoi(secondArgument));
		}
	}
	else if (QueryUtility::isInteger(firstArgument)) {
		if (secondArgument == "_") {
			result = PKB().isParent(stoi(firstArgument));
		}
		else if (QueryUtility::isInteger(secondArgument)) {
			result = PKB().getParent(stoi(secondArgument)) == stoi(firstArgument);
		}
	}
	return QueryUtility::truthValue(result);
}

/*
Evaluate non trivial Parent clause
*/
std::unordered_map<std::string, std::vector<std::string>> ParentEvaluator::evaluate(
	std::unordered_map<std::string, std::string> declarations,
	std::string firstArgument, std::string secondArgument) {
	std::unordered_map<std::string, std::vector<std::string>> tableResult;
	std::vector<std::string> emptyVec;
	if (QueryUtility::isSynonym(firstArgument)) {
			if (secondArgument == "_") {
				tableResult = QueryUtility::to_mapvec(firstArgument, PKB().getAllParents());
			}
			else if (QueryUtility::isInteger(secondArgument)) {
				int result = PKB().getParent(stoi(secondArgument));
				if (result > 0) {
					tableResult = QueryUtility::to_mapvec(firstArgument, result);
				}
				else {
					tableResult.insert({ firstArgument, emptyVec });
				}
			}
			else if (firstArgument == secondArgument) {
				tableResult.insert({ firstArgument, emptyVec });
			}
			else {
				tableResult = QueryUtility::to_mapvec(firstArgument, secondArgument, 
					PKB().getParentChildPairs());
			}
		}
		if (QueryUtility::isSynonym(secondArgument)) {
			if (firstArgument == "_") {
				tableResult = QueryUtility::to_mapvec(secondArgument, PKB().getAllChildren());
			}
			else if (QueryUtility::isInteger(firstArgument)) {
				tableResult = QueryUtility::to_mapvec(secondArgument, PKB().getChildren(
					stoi(firstArgument)));
			}
		}
	return tableResult;
}