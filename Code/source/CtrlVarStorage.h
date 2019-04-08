#pragma once
#include <unordered_set>
#include <unordered_map>

#include "Hasher.h"

class CtrlVarStorage {

public:
	CtrlVarStorage();
	void addIfControlVariable(int stm, std::string variable);
	void addWhileControlVariable(int stm, std::string variable);

	//Call for if(_,_)
	std::unordered_set<int> getAllIfWithControls();

	//Call for w(_,_)
	std::unordered_set<int> getAllWhileWithControls();

	//Call for if('s',_)
	std::unordered_set<int> getIfStmWithControlVariable(std::string variable);

	//Call for w('s',_)
	std::unordered_set <int> getWhileStmWithControlVariable(std::string variable);

	//Call for if(s,_)
	std::unordered_set<std::pair<int, std::string>, intStringhash> getIfStmControlVariablePair();

	//Call for w(s,_)
	std::unordered_set<std::pair<int, std::string>, intStringhash> getWhileStmControlVariablePair();

private:
	static std::unordered_map<std::string, std::unordered_set<int>> ifVariableToStm;
	static std::unordered_map<std::string, std::unordered_set<int>> whileVariableToStm;
};