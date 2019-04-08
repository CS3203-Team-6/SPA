#include"RunTimeDesignExtractor.h"

std::unordered_set<int> RunTimeDesignExtractor::extractNextStar(PKB* pkb, int stmt) {
	std::unordered_set<int>visitedStmts;
	std::unordered_set<int>stmtsAfter;
	DFSRecursiveNext(pkb, stmt, visitedStmts, stmtsAfter);
	return stmtsAfter;
}

std::unordered_set<int> RunTimeDesignExtractor::extractPreviousStar(PKB* pkb, int stmt) {
	std::unordered_set<int>visitedStmts;
	std::unordered_set<int>stmtsBefore;
	DFSRecursivePrevious(pkb, stmt, visitedStmts,stmtsBefore);
	return stmtsBefore;
}

bool RunTimeDesignExtractor::extractNextStarPair(PKB* pkb, int start, int end) {
	std::unordered_set<int>visitedStmts;
	std::unordered_set<int>stmtsAfter;
	DFSRecursiveNext(pkb, start, visitedStmts,stmtsAfter);
	if (stmtsAfter.find(end) == stmtsAfter.end()) {
		return false;
	}
	return true;
}

std::unordered_set<std::pair<int, int>, intPairhash> RunTimeDesignExtractor::getNextStarPairs(PKB* pkb) {
	int stmtNum = pkb->getTotalStmNo();
	std::unordered_set<std::pair<int, int>, intPairhash> allNextStarPairs;
	//for all Statement in statemnet list generate nextStar
	for (int i = 1; i <= stmtNum; i++) {
		std::unordered_set<int>visitedStmts;
		std::unordered_set<int>stmtsAfter;
		DFSRecursiveNext(pkb, i, visitedStmts,stmtsAfter);
		for (int statments : stmtsAfter) {
			allNextStarPairs.emplace(i, statments);
		}
	}
	return allNextStarPairs;
}

void RunTimeDesignExtractor::DFSRecursiveNext(PKB* pkb, int statments,
											  std::unordered_set<int> &visitedStmts,
											  std::unordered_set<int> &stmtsAfter) {
	//Marks statment as visited
	visitedStmts.insert(statments);
	//Get neighbouring statments.
	std::unordered_set<int> adjacentStmts = pkb->getNext(statments);

	//For each neighbouring statments
	for (int stmts : adjacentStmts) {
		
		std::unordered_set<int>::const_iterator visited = stmtsAfter.find(stmts);
		if (visited == stmtsAfter.end()) {
			stmtsAfter.insert(stmts);
		}
		//Check if neighbouring statments has been visited
		std::unordered_set<int>::const_iterator exist = visitedStmts.find(stmts);

		//If neighbouring statments has not been visited, visit it.
		if (exist == visitedStmts.end()) {
			stmtsAfter.insert(stmts);
			DFSRecursiveNext(pkb, stmts, visitedStmts,stmtsAfter);
		}
	}
}

void RunTimeDesignExtractor::DFSRecursivePrevious(PKB* pkb, int statments,
												  std::unordered_set<int> &visitedStmts,
												  std::unordered_set<int> &stmtsBefore) {
	//Marks statment as visited
	visitedStmts.insert(statments);
	//Get neighbouring statments.
	std::unordered_set<int> adjacentStmts = pkb->getPrev(statments);

	//For each neighbouring statments
	for (int stmts : adjacentStmts) {
		std::unordered_set<int>::const_iterator visited = stmtsBefore.find(stmts);

		if (visited == stmtsBefore.end()) {
			stmtsBefore.insert(stmts);
		}
		//Check if neighbouring statments has been visited
		std::unordered_set<int>::const_iterator exist = visitedStmts.find(stmts);

		//If neighbouring statments has not been visited, visit it.
		if (exist == visitedStmts.end()) {
			DFSRecursivePrevious(pkb, stmts, visitedStmts,stmtsBefore);
		}
	}
}