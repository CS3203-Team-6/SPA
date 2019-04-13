#pragma once

#include <unordered_map>
#include <unordered_set>
#include <utility>

#include "Hasher.h"

/*
	A structure to contain:	the procedure calling another (caller),
							a list of procedures it is calling (callee),
							a list of procedures that is directly/indirectly calling (callAnc),
							a list of procedures being directly/indirectly called (callDesc)
	If such statement or list does not exist, it'll be set to 0 or {} respectively
*/
struct cRelationships
{
	std::unordered_set<std::string> caller;
	std::unordered_set<std::string> callees;
	std::unordered_set<std::string> callAnc;
	std::unordered_set<std::string> callDesc;
};

/*
	A storage class to store the Call & Call* relationships of statements
*/
class CallStorage {
public:
	CallStorage();

	/*
		Adds the call relation into the various lists in the storage
		Returns false if the pair already exist
	*/
	void addCall(std::string caller, std::string callee, int stm);

	/*
		Sets "callAnc" of callee
		Each callAnc - callee pair is entered into callStarPairList
		If callee already has a list of callAnc, it is not replaced and it return false
	*/
	bool setCallAnc(std::string callee, std::unordered_set<std::string> callers);

	/*
		Sets "callDesc" of caller
		Each caller - callDesc pair is entered into callStarPairList
		If caller already has a list of callDesc, it is not replaced and it return false
	*/
	bool setCallDesc(std::string caller, std::unordered_set<std::string> callees);

	// returns true if callTable is empty
	bool isEmpty();

	// returns true if call* pair is found
	bool hasCallStarPair(std::pair<std::string, std::string> pair);

	// returns true if the specified procedure calls another
	bool isCaller(std::string procedure);

	// returns true if the specified procedure is called by another
	bool isCallee(std::string procedure);

	/*
		return the procedure calling the procedure specified
		return "" if 'procedure' is not found
	*/
	std::unordered_set<std::string> getCaller(std::string procedure);

	/*
		return the procedure called by the procedure specified
		return {} if 'procedure' is not found
	*/
	std::unordered_set<std::string> getCallee(std::string procedure);

	/*
		return a list of procedures that is directly/indirectly calling the procedure specified
		return an empty set if 'procedure' is not found
	*/
	std::unordered_set<std::string> getCallAnc(std::string procedure);

	/*
		return a list of procedures that is directly/indirectly called by the procedure specified
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
	std::unordered_set< std::pair<std::string, std::string>, strPairhash> getCallStarPairs();

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

	// empty the entire storage
	void clear();

private:
	static std::unordered_map<std::string, cRelationships> callTable;
	static std::unordered_set< std::pair<std::string, std::string>, strPairhash> callPairList;
	static std::unordered_set< std::pair<std::string, std::string>, strPairhash> callStarPairList;
	static std::unordered_set<std::string> callerList;
	static std::unordered_set<std::string> calleeList;
	static std::unordered_map<int, std::string> stmToProcMap;
	static std::unordered_map<std::string, std::unordered_set<int>> procToStmMap;
	static std::unordered_set< std::pair<int, std::string>, intStringhash> stmProcCallPairList;
};