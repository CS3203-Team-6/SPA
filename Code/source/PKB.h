#pragma once

#include <string>
#include <unordered_set>
#include <vector>

#include "FollowStorage.h"
#include "ParentStorage.h"
#include "UseStorage.h"
#include "ModifyStorage.h"
#include "CallStorage.h"
#include "NextStorage.h"
#include "CtrlVarStorage.h"
#include "RunTimeDesignExtractor.h"
#include "Hasher.h"

enum stmType {read, print, assign, whileStm, ifStm, call};

/*
	Accepts relationship, pattern and other general data from Parser and DesignExtractor and
	stores them here or into their respective Storage classes.
	Reply to queries made by the QueryEvaluator.
	Forwards Next*, Affects and Affects* queries to RunTimeDesignExtractor.
*/
class PKB {
public:
	PKB();

/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//General adder Methods			/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
	// add a procedure to procList
	bool addProc(std::string procName);

	/*
		Pre-cond: statements added in must be added in numerical order; no jumps or reversing
		add statement to its respective StmLists and set stmTypeList[stmNo] to type
	*/
	void addStatement(int stmNo, stmType type, std::string procedure);

	// add variable to varList
	void addVariable(std::string name);

	// add constant to constList
	void addConstant(std::string value);

	// add the last statement of a while loop
	void addWhileLastStm(int whileStm, int lastStm);

	// add the last statement for an if then block
	void addThenLastStm(int ifStm, int thenStm);

	// add the last statement for an if else block
	void addElseLastStm(int ifStm, int elseStm);

/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//Follows adder & setter Methods	/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*
		add the follows relation in FollowStorage
		Returns false if
			1) the pair is already stored
			2) the followed statement has another follower stored
			3) the follower is following another statement
			4) stm2 <= stm1 or stm1, stm2 <= 0
	*/
	bool addFollow(int stm1, int stm2);

	/*
		Sets the list of followers of 'stm' to be 'stmList' in FollowStorage
		Every FollowStar pair is stored as well
		If stm already has a list of followers, it is not replaced and it return false
	*/
	bool setAllFollowing(int stm, std::unordered_set<int> stmList);

	/*
		Sets the list of followed of 'stm' to be 'stmList' in FollowStorage
		Every FollowStar pair is stored as well
		If stm already has a list of followed, it is not replaced and it return false
	*/
	bool setAllFollowedBy(int stm, std::unordered_set<int> stmList);

/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//Parent adder & setter Methods		/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*
		Adds the parent relation into ParentStorage
		Returns false if	
			1) the pair is already stored
			2) the child already has another parent
			3) stm2 <= stm1 or stm1, stm2 <= 0
	*/
	bool addParent(int stm1, int stm2);

	/*
		Sets the list of ancestors of 'stm' in ParentStorage
		Each Parent* pair is stored as well
		If stm already has a list of ancestors, it is not replaced and it return false
	*/
	bool setAncestors(int stm, std::unordered_set<int> stmList);

	/*
		Sets the list of descendants of 'stm' in ParentStorage
		Each Parent* pair is stored as well
		If stm already has a list of descendants, it is not replaced and it return false
	*/
	bool setDescendants(int stm, std::unordered_set<int> stmList);

/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//Uses adder Methods			/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*
		add the Uses relation for a Statement in UseStorage
		Returns false if
			1) the pair is already stored
			2) stm <= 0 and variable is an empty string
	*/
	bool addUsesStm(int stm, std::string variable);

	/*
		add the Uses relation for a Procedure in UseStorage
		Returns false if
			1) the pair is already stored
			2) procedure or variable is an empty string
	*/
	bool addUsesProc(std::string procedure, std::string variable);
/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//Modifies adder Methods			/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*
		add the Modifies relation for a Statement in ModifyStorage
		Returns false if
			1) the pair is already stored
			2) stm <= 0 and variable is an empty string
	*/
	bool addModifiesStm(int stm, std::string variable);

	/*
		add the Modifies relation for a Procedure in ModifyStorage
		Returns false if
			1) the pair is already stored
			2) procedure or variable is an empty string
	*/
	bool addModifiesProc(std::string procedure, std::string variable);

/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//Calls adder & setter Methods	/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*
		Adds the call relation at statement 'stmNo' into CallStorage
		Returns false if
			1) the pair is already stored
			2) proc1 or proc2 == ""
			3) stmNo <= 0
	*/
	void addCall(std::string proc1, std::string proc2, int stmNo);

	/*
		Sets the list of call ancestors of 'procedure' in CallStorage
		Each Call* pair is stored as well
		If 'procedure' already has a list of call ancestors, it is not replaced and it return false
	*/
	bool setCallAnc(std::string procedure, std::unordered_set<std::string> procList);

	/*
		Sets the list of call descendants of 'procedure' in CallStorage
		Each Call* pair is stored as well
		If 'procedure' already has a list of call descendants, it is not replaced and it return false
	*/
	bool setCallDesc(std::string procedure, std::unordered_set<std::string> procList);

/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//Next adder Methods		/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*
		add the next relation in NextStorage
		Returns false if
			1) the pair is already stored
			2) line1, line2 <= 0
	*/
	bool addNext(int line1, int line2);

/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//Pattern adder Methods			/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*
		add a pattern for an assign statement to patternList
		returns false if 'stm' already exist in the list with another pattern
	*/
	bool addAssignPattern(int stm, std::string variable, std::string expr);

	// add an if statement containu=ing the specified control variable to patternStorage
	void addIfControlVariable(int stm, std::string variable);

	// add an while statement containu=ing the specified control variable to patternStorage
	void addWhileControlVariable(int stm, std::string variable);

/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//General Getter Methods	/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
	// returns the stored list of procedures
	std::unordered_set<std::string> getProcList();

	// returns an ordered list of statements that are in 'procedure'
	std::vector<int> getStmList(std::string procedure);

	// returns the total number of statements in the entire program
	int getTotalStmNo();

	// return the statement type of stm
	stmType getStmType(int stm);

	// returns the stored list of read statements
	std::unordered_set<int> getReadStms();

	// returns the stored list of print statements
	std::unordered_set<int> getPrintStms();

	// returns the stored list of assign statements
	std::unordered_set<int> getAssignStms();

	// returns the stored list of if statements
	std::unordered_set<int> getIfStms();

	// returns the stored list of while statements
	std::unordered_set<int> getWhileStms();

	// returns the stored list of call statements
	std::unordered_set<int> getCallStms();

	// returns the stored list of variables
	std::unordered_set<std::string> getVariables();

	// returns the stored list of constants
	std::unordered_set<std::string> getConstants();

	// returns the stored list of <read stm, variable> pairs
	std::unordered_set< std::pair<int, std::string>, intStringhash > getReadPairs();

	// returns the stored list of <print stm, variable> pairs
	std::unordered_set< std::pair<int, std::string>, intStringhash > getPrintPairs();

	// returns the last statement of the specified while loop
	int getWhileLastStm(int whileStm);

	// returns the last statements of the specified if stm's then and else block
	std::pair<int, int> getIfLastStms(int ifStm);

/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//Follows Getter Methods	/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
	// checks if there is at least one follows relation
	bool hasFollowRelation();

	// checks if the relation Follows*(stm1, stm2) exist
	bool hasFollowStarPair(int stm1, int stm2);

	/*
		return the statement followed by 'stm'
		return 0 if 'stm' is not found
	*/
	int getStmFollowedBy(int stm);

	/*
		return the statement following 'stm'
		return 0 if 'stm' is not found
	*/
	int getFollower(int stm);

	/*
		return a list of statements that is directly/indirectly following 'stm'
		return an empty set if 'stm' is not found
	*/
	std::unordered_set<int> getAllFollowing(int stm);

	/*
		return a list of statements that is directly/indirectly followed by 'stm'
		return an empty set if 'stm' is not found
	*/
	std::unordered_set<int> getAllFollowedBy(int stm);

	// returns a list of all statements that follows another
	std::unordered_set<int> getAllFollowers();

	// returns a list of all statements that is followed by another
	std::unordered_set<int> getAllFollowed();

	// returns a list of all follows pairs
	std::unordered_set< std::pair<int, int>, intPairhash> getFollowPairs();

	// returns a list of all follows* pairs
	std::unordered_set< std::pair<int, int>, intPairhash> getFollowStarPairs();

/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//Parent Getter Methods		/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
	// checks if there is at least one Parent relationship
	bool hasParentRelation();

	// checks if 'stm' is a parent of another
	bool isParent(int stm);

	// checks if 'stm' is a child of another
	bool isChild(int stm);

	// checks if the relation Parent*(stm1, stm2) exist
	bool hasAncDescPair(int stm1, int stm2);

	/*
		return a statement that is the parent of 'stm'
		return 0 if 'stm' is not found
	*/
	int getParent(int stm);

	/*
		return a list of statements that is the children of 'stm'
		return an empty set if 'stm' is not found
	*/
	std::unordered_set<int> getChildren(int stm);

	/*
		return a list of statements that is the ancestors of 'stm'
		return an empty set if 'stm' is not found
	*/
	std::unordered_set<int> getAncestors(int stm);

	/*
		return a list of statements that is the descendants of 'stm'
		return an empty set if 'stm' is not found
	*/
	std::unordered_set<int> getDescendants(int stm);

	// returns a list of all statements that is the parent of another
	std::unordered_set<int> getAllParents();

	// returns a list of all statements that is the child of another
	std::unordered_set<int> getAllChildren();

	// returns a list of all parent pairs
	std::unordered_set< std::pair<int, int>, intPairhash> getParentChildPairs();

	// returns a list of all parent* pairs
	std::unordered_set< std::pair<int, int>, intPairhash> getAncDescPairs();

/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//Uses Getter Methods		/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
	// checks if the relation Uses(stm, variable) exist
	bool isStmUsing(int stm, std::string variable);

	// checks if the relation Uses(procedure, variable) exist
	bool isProcUsing(std::string procedure, std::string variable);

	/*
		return a list of variables that is used by 'stm'
		return an empty set if 'stm' is not found
	*/
	std::unordered_set<std::string> getVarUsedByStm(int stm);

	/*
		return a list of variables that is used by 'procedure'
		return an empty set if 'procedure' is not found
	*/
	std::unordered_set<std::string> getVarUsedByProc(std::string procedure);

	/*
		return a list of statements that is using 'variable'
		returns a list of all statements that used a variable if 'variable' == "" 
		return an empty set if 'variable' is not found
	*/
	std::unordered_set<int> getStmUsing(std::string variable);

	/*
		return a list of procedure that is using 'variable'
		returns a list of all procedures that used a variable if 'variable' == "" 
		return an empty set if 'variable' is not found
	*/
	std::unordered_set<std::string> getProcUsing(std::string variable);

	// returns a list of all Uses pairs for statements
	std::unordered_set< std::pair<int, std::string>, intStringhash> getStmVarUsePairs();

	// returns a list of all Uses pairs for procedures
	std::unordered_set< std::pair<std::string, std::string>, strPairhash> getProcVarUsePairs();

/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//Modifies Getter Methods	/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
	// checks if the relation Modifies(stm, variable) exist
	bool isStmModifying(int stm, std::string variable);

	// checks if the relation Modifies(procedure, variable) exist
	bool isProcModifying(std::string procedure, std::string variable);

	/*
		return a list of variables that is modified by 'stm'
		return an empty set if 'stm' is not found
	*/
	std::unordered_set<std::string> getVarModifiedByStm(int stm);

	/*
		return a list of variables that is modified by 'procedure'
		return an empty set if 'procedure' is not found
	*/
	std::unordered_set<std::string> getVarModifiedByProc(std::string procedure);

	/*
		return a list of statements that modifies 'variable'
		returns a list of all statements that modifies a variable if 'variable' == "" 
		return an empty set if 'variable' is not found
	*/
	std::unordered_set<int> getStmModifying(std::string variable);

	/*
		return a list of procedures that modifies 'variable'
		returns a list of all procedures that modifies a variable if 'variable' == "" 
		return an empty set if 'variable' is not found
	*/
	std::unordered_set<std::string> getProcModifying(std::string variable);

	// returns a list of all Modifies pairs for statements
	std::unordered_set< std::pair<int, std::string>, intStringhash> getStmVarModifyPairs();

	// returns a list of all Modifies pairs for procedures
	std::unordered_set< std::pair<std::string, std::string>, strPairhash> getProcVarModifyPairs();

/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//Call Getter Methods		/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
	// checks if there is at least one Call relationship
	bool hasCallRelation();

	// checks if 'procedure' calls another
	bool isCaller(std::string proc);

	// checks if 'procedure' is called by another
	bool isCallee(std::string procedure);

	// checks if the relation Call*(proc1, proc2) exist
	bool hasCallStarPair(std::string proc1, std::string proc2);

	/*
		return a list of procedure calling 'procedure'
		return {} if 'procedure' is not found
	*/
	std::unordered_set<std::string> getCaller(std::string procedure);

	/*
		return a list of procedure called by 'procedure'
		return {} if 'procedure' is not found
	*/
	std::unordered_set<std::string> getCallee(std::string procedure);

	/*
		return a list of procedures that is directly/indirectly calling 'procedure'
		return an empty set if 'procedure' is not found
	*/
	std::unordered_set<std::string> getCallAnc(std::string procedure);

	/*
		return a list of procedures that is directly/indirectly called by 'procedure'
		return an empty set if 'procedure' is not found
	*/
	std::unordered_set<std::string> getCallDesc(std::string procedure);

	// returns a list of all procedures that calls another
	std::unordered_set<std::string> getAllCallers();

	// returns a list of all procedures that is called by another
	std::unordered_set<std::string> getAllCallees();

	// returns a list of all call pairs
	std::unordered_set< std::pair<std::string, std::string>, strPairhash> getCallPairs();

	// returns a list of all call* pairs
	std::unordered_set<std::pair<std::string, std::string>, strPairhash> getCallStarPairs();

	/*
		returns the procedure called by 'stm'
		returns empty string if 'stm' is not found
	*/
	std::string getProcCalledBy(int stm);

	/*
		returns a list of statements that calls 'procedure'
		returns an empty set {} if 'procedure' is not found
	*/
	std::unordered_set<int> getStmCalling(std::string procedure);

	// returns a list of all call statement - procedure pairs
	std::unordered_set< std::pair<int, std::string>, intStringhash> getStmProcCallPairs();

/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//Next Getter Methods	/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
	// checks if there is at least one next relation
	bool hasNextRelation();

	// checks if the relation Next*(line1, line2) exist
	bool hasNextStarPair(int line1, int line2);

	/*
		return a list of program lines that can be executed after 'line'
		return {} if 'line' is not found
	*/
	std::unordered_set<int> getNext(int line);

	/*
		return a list of program lines that can be executed before 'line'
		return {} if 'line' is not found
	*/
	std::unordered_set<int> getPrev(int line);

	/*
		return a list of program lines that is directly/indirectly after 'line'
		return an empty set if 'line' is not found
	*/
	std::unordered_set<int> getAllLnAfter(int line);

	/*
		return a list of program lines that is directly/indirectly before 'line'
		return an empty set if 'line' is not found
	*/
	std::unordered_set<int> getAllLnBefore(int line);

	// returns a list of all program lines that is executed after another
	std::unordered_set<int> getAllNext();

	// returns a list of all program lines that is before after another
	std::unordered_set<int> getAllPrev();

	// returns a list of all next pairs
	std::unordered_set< std::pair<int, int>, intPairhash> getNextPairs();

	// returns a list of all next* pairs
	std::unordered_set< std::pair<int, int>, intPairhash> getNextStarPairs();

/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//Affects Getter Methods	 /////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
	// checks if there is at least one Affect relationship
	bool hasAffectsRelation();

	// checks if 'stm' affects another
	bool isAffector(int stm);

	// checks if 'stm' is affected by another
	bool isAffected(int stm);

	// checks if the relation Affects*(stm1, stm2) exist
	bool hasAffectStarPair(int stm1, int stm2);

	/*
		return the stm affecting 'stm'
		return 0 if 'stm' is not found
	*/
	int  getAffector(int stm);

	/*
		return the stm affected by 'stm'
		return 0 if 'stm' is not found
	*/
	int getAffected(int stm);

	/*
		return a list of statements that is directly/indirectly affecting 'stm'
		return an empty set if 'stm' is not found
	*/
	std::unordered_set<int> getAffectorStar(int stm);

	/*
		return a list of statements that is directly/indirectly affected by 'stm'
		return an empty set if 'stm' is not found
	*/
	std::unordered_set<int> getAffectedStar(int stm);

	// returns a list of all statements that affects another
	std::unordered_set<int> getAllAffectors();

	// returns a list of all statements that is affected by another
	std::unordered_set<int> getAllAffected();

	// returns a list of all affects pairs
	std::unordered_set< std::pair<int, int>, intPairhash> getAffectPairs();

	// returns a list of all affects* pairs
	std::unordered_set< std::pair<int, int>, intPairhash> getAffectStarPairs();

/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//Assign Pattern Getter Methods	/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*
		Search for assign statements with pattern matching
			- the specified variable ont the left side and
			- the specified expression on the right side
		If isExclusive is true, the function finds for an exact match to the specfied expression.
		If false, it finds for a matching substring instead.
	*/
	std::unordered_set<int> findPattern(std::string variable, std::string expr, bool isExclusive);

	/*
		Search for assign statements with pattern matching
			- the specified expression on the right side
		If isExclusive is true, the function finds for an exact match to the specfied expression.
		If false, it finds for a matching substring instead.
	*/
	std::unordered_set<int> findPattern(std::string expr, bool isExclusive);

	/*
		Search for pairs of an assign statement and left-hand side variable with
			the right side expression matching the specified expression
		If isExclusive is true, the function finds for an exact match to the specfied expression.
		If false, it finds for a matching substring instead.
	*/
	std::unordered_set<std::pair<int, std::string>, intStringhash> findPatternPairs(std::string expr, bool isExclusive);

/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//If & While Pattern Getter Methods	/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

	// return a list of all if statements with control variables
	std::unordered_set<int> getAllIfWithControls();

	// return a list of all while statements with control variables
	std::unordered_set<int> getAllWhileWithControls();

	// return a list of if statements that has 'variable' as a control variable
	std::unordered_set<int> getIfStmWithControlVariable(std::string variable);

	// return a list of while statements that has 'variable' as a control variable
	std::unordered_set <int> getWhileStmWithControlVariable(std::string variable);

	// return a list of all <  If Statement, Control Variable > pairs
	std::unordered_set<std::pair<int, std::string>, intStringhash> getIfStmControlVariablePair();

	// return a list of all <  While Statement, Control Variable > pairs
	std::unordered_set<std::pair<int, std::string>, intStringhash> getWhileStmControlVariablePair();

//////////////////////////////////////////////////////////////////////////////////////////////////
	//Erase Method		//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
	// Empty the entire PKB including the Storage obects
	void erase();

private:
	static std::unordered_set<std::string> procList;
	static std::unordered_map<std::string, std::vector<int>> procStmList;
	static std::vector<stmType> stmTypeList;
	static std::unordered_set<std::string> varList;
	static std::unordered_set<std::string> constList;
	static std::unordered_set<int> readStmList;
	static std::unordered_set<int> printStmList;
	static std::unordered_set<int> assignStmList;
	static std::unordered_set<int> ifStmList;
	static std::unordered_set<int> whileStmList;
	static std::unordered_set<int> callStmList;
	static std::unordered_set< std::pair<int, std::string>, intStringhash > readPairList;
	static std::unordered_set< std::pair<int, std::string>, intStringhash > printPairList;
	static std::unordered_map<int, int> whileLastStmList;
	static std::unordered_map<int, std::pair<int, int> > ifLastStmList;

	static FollowStorage fStore;
	static ParentStorage pStore;
	static UseStorage uStore;
	static ModifyStorage mStore;
	static CallStorage cStore;
	static NextStorage nStore;
	static CtrlVarStorage cvStore;
	static std::unordered_map<int, std::pair<std::string, std::string> > patternList;
};