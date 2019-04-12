#pragma once
#include "Hasher.h"
#include "PKB.h"

class PKB;

class RunTimeDesignExtractor {

public:

	std::unordered_set<int> extractNextStar(int stmt);
	std::unordered_set<int> extractPreviousStar(int stmt);
	bool extractNextStarPair(int start, int end);
	std::unordered_set<std::pair<int, int>, intPairhash> getNextStarPairs();
	void DFSRecursiveNext(int procedure, std::unordered_set<int> &visitedStmts, std::unordered_set<int>& stmtsAfter);
	void DFSRecursivePrevious(int procedure, std::unordered_set<int> &visitedStmts, std::unordered_set<int> &stmtsBefore);

	/*
	 * Check for the truthfulness of Relation Affects(INTEGER, INTEGER).
	 * Returns true if First Args Affects Second Args
	 */
	bool isAffect(int stmt, int stmt1);

	/*
	 * Check for the truthfulness of Relation Affects(INTEGER, _).
	 * Returns true if First Args Affects Something Else
	 */
	bool isStatementAffectingAnother(int stmt);

	/*
	 * Check for the truthfulness of Relation Affects(_, INTEGER).
	 * Returns true if Second Args is Affected By Something Else
	 */
	bool isStatementAffectedByAnother(int stmt);

	/*
	 * Check for the truthfulness of Relation Affects(_, _).
	 * Returns true if something affects something.
	 */
	bool hasAffectsRelation();

	/*
	 * Check for the the list of SYNONYM of Relation Affects(SYNONYM, INTEGER).
	 * Returns list of possible SYNONYM that affects the INTEGER (index)
	 */
	std::vector<int> getStatementsAffectingIndex(int stmt);

	/*
	 * Check for the the list of SYNONYM of Relation Affects(INTEGR, SYNONYM).
	 * Returns list of possible SYNONYM that is affected by the INTEGER (index)
	 */
	std::vector<int> getStatementsAffectedByIndex(int stmt);

	/*
	 * Check for the the list of SYNONYM of Relation Affects(SYNONYM, _).
	 * Returns list of possible SYNONYM that is affecting something else.
	 */
	std::vector<int> getAllStatementsAffectingAnother();

	/*
	 * Check for the the list of SYNONYM of Relation Affects(_, SYNONYM).
	 * Returns list of possible SYNONYM that is affected by something else.
	 */
	std::vector<int> getAllStatementsAffectingByAnother();

	/*
	 * Check for the the list of <SYNONYM1, SYNONYM2> pair of Relation Affects(SYNONYM1, SYNONYM2).
	 * Returns list of possible <SYNONYM1, SYNONYM2> pair.
	 * Where SYNONYM1 Affects SYNONYM2.
	 */
	std::unordered_set<std::pair<int, int>, intPairhash> getAffectsPair();

	/*
	 * Check for the truthfullness of Relation Affects*(INTEGER, INTEGER).
	 * Returns true if such a relation is found.
	 */
	bool isAffectStar(int start, int target);

	/*
	 * Check for the the list of SYNONYM of Relation Affects*(INTEGER, SYNONYM).
	 * Returns list of possible SYNONYM
	 */
	std::vector<int> getAllStatementsAffectedByIndexStar(int index);

	/*
	 * Check for the the list of SYNONYM of Relation Affects*(SYNONYM, INTEGER).
	 * Returns list of possible SYNONYM
	 */
	std::vector<int> getAllStatementsAffectingIndexStar(int index);

	/*
	 * Check for the the list of <SYNONYM1, SYNONYM2> pair of Relation Affects*(SYNONYM1, SYNONYM2).
	 * Returns list of possible <SYNONYM1, SYNONYM2> pair.
	 * Where SYNONYM1 Affects* SYNONYM2.
	 */
	std::unordered_set<std::pair<int, int>, intPairhash> getAffectsStarPair();

private:
	PKB* pkb;

	//For DFSing Affects(int, int)
	bool DFSRecursiveCheckAffectsPair(int start, int target, int current, std::unordered_set<int>& cfgPath, bool isStart);

	//For DFSing Affects(int , _)
	bool DFSRecursiveCheckAffecting(int start, int current, std::unordered_set<int>& cfgPath, bool isStart);

	//For DFSing Affects(_, int)
	bool DFSRecursiveCheckAffectedBy(int end, int current, std::unordered_set<int>& cfgPath, bool isStart, std::unordered_set<std::string> & relevantVar);

	//For DFSing Affects(synonym, int)
	void DFSRecursiveGetAffectingList(int end, int current, std::unordered_set<int>& cfgPath, bool isStart, std::vector<int>& affectedByList, std::unordered_set<std::string>& relevantVar);
	
	//For DFSing Affects(int, synonym)
	void DFSRecursiveGetAffectedByList(int start, int current, std::unordered_set<int>& cfgPath, bool isStart, std::vector<int>& affectedList);

	//For retrieving the Affects(SYNONYM, SYNONYM) pair of an particular procedure.
	std::unordered_set<std::pair<int, int>, intPairhash> getAffectsPairOfProc(std::string procedure);


	/* Helper function for extracting Affects Pair Relations.
	 * Calls on 
	 * (1) processWhile if it encounters a while statement.
	 * (2) processIf if it encounters a if statement.
	 * (3) processAssign if it encounters an assign statemnt.
	 * (4) processCallAndRead if it encounters a prog_line that affects something
	 */
	void extractAffectsPair(int start, std::unordered_map<std::string, std::unordered_set<int>>& lastModifiedTable, std::unordered_set<std::pair<int, int>, intPairhash>& affectsPair);

	//Performs Affects relation extraction for while statement block
	void processWhile(std::unordered_map<std::string, std::unordered_set<int>> & lastModifiedTable, int &i, std::unordered_set<std::pair<int, int>, intPairhash> & affectsPair);

	//Performs Affects relation extraction for if statement block
	void processIfStatement(std::unordered_map<std::string, std::unordered_set<int>> & lastModifiedTable, int &i, std::unordered_set<std::pair<int, int>, intPairhash> & affectsPair);

	//Performs Affects relation extraction for assign
	void processAssign(int &i, std::unordered_map<std::string, std::unordered_set<int>> & lastModifiedTable, std::unordered_set<std::pair<int, int>, intPairhash> & affectsPair);

	//Performs Affects relation lastModified update for modifying statements.
	void processCallAndRead(int &i, std::unordered_map<std::string, std::unordered_set<int>> & lastModifiedTable);

	//Perform Affects Table ytraversal for Affects*(INTEGER, INTEGER).
	bool DFSRecursiveStartReachableToEnd(int start, int end, std::unordered_set<int>& visitedPath, std::unordered_map<int, std::unordered_set<int>> adjacencyList, bool isStart);

	//Performs Affects Table traversal for Affects*(SYNONYM, SYNONYM).
	void DFSRecursiveReachability(int start, std::vector<int>& results, std::unordered_set<int>& visitedPath, std::unordered_map<int, std::unordered_set<int>> adjacencyList, bool isStart);

	/* Checks if 
	 * (1) stmt1 and stmt2 are assign statements
	 * (2) stmt1 Modifies something used by stmt2
	 */
	bool isAffectPossible(int stmt, int stmt1);

	/* Checks if
	 * (1) if current is a modifying statement
	 * (2) if current modifies what that start modified.
	 * Return true if (1) and (2) are true.
	 */
	bool isLastModifiedBroken(int current, int start);

	/*
	 * Returns true if an element in modifiedInStmt is found in usedInStmt1.
	 */
	bool contains(std::unordered_set<std::string>& modifiedInStmt, std::unordered_set<std::string>& usedInStmt1);

};