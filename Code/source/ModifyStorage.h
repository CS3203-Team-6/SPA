#pragma once

#include <string>
#include <unordered_set>
#include <unordered_map>
#include <utility>

#include "Hasher.h"

/*
	A storage class to store the Modifies relationships between Procedures & Statements with Variables
*/
class ModifyStorage
{
public:
	ModifyStorage();

	/*
		add the Modifies relation for a statement into the relevant lists and maps in the storage
		Returns false if the pair is already exist
	*/
	bool addModifiesStm(int stm, std::string variable);

	/*
		add the Modifies relation for a procedure into the relevant lists and maps in the storage
		Returns false if the pair is already exist
	*/
	bool addModifiesProc(std::string procedure, std::string variable);

	// returns true if the specified <statement, variable> pair is found
	bool containsStmVarPair(std::pair<int, std::string> pair);

	// returns true if the specified <procedure, variable> pair is found
	bool containsProcVarPair(std::pair<std::string, std::string> pair);

	/*
		return a list of variables that is modified by 'stm'
		return an empty set if 'stm' is not found
	*/
	std::unordered_set<std::string> getVarModifiedByStm(int stm);

	/*
		return a list of variables that is modified by 'procedure'
		return an empty set if 'procedure' is not found
	*/
	std::unordered_set<std::string> getVarModifiedByProc(std::string proc);

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
	std::unordered_set< std::pair<int, std::string>, intStringhash> getStmVarPairs();

	// returns a list of all Modifies pairs for procedures
	std::unordered_set< std::pair<std::string, std::string>, strPairhash> getProcVarPairs();

	// empty the entire storage
	void erase();

private:
	static std::unordered_set<std::pair<int, std::string>, intStringhash> stmVarPairList;
	static std::unordered_set<std::pair<std::string, std::string>, strPairhash> procVarPairList;
	static std::unordered_map<int, std::unordered_set<std::string>> stmToVarMap;
	static std::unordered_map<std::string, std::unordered_set<std::string>> procToVarMap;
	static std::unordered_map<std::string, std::unordered_set<int> > varToStmMap;
	static std::unordered_map<std::string, std::unordered_set<std::string> > varToProcMap;
};