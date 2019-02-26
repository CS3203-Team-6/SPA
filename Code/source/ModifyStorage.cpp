#include "ModifyStorage.h"

ModifyStorage::ModifyStorage()
{
}

bool ModifyStorage::addModifies(int stm, string variable)
{
	if (!stmVarPairs.emplace(pair<int, string>(stm, variable)).second)
	{
		return false;
	}
	varList_Stm.emplace(stm, variable);
	stmLists.emplace(variable, unordered_set<int>{ stm });
	stmLists.emplace("", unordered_set<int>{ stm });
	return true;
}

bool ModifyStorage::addModifies(string procedure, string variable)
{
	if (!procVarPairs.emplace(pair<string, string>(procedure, variable)).second)
	{
		return false;
	}
	varList_Proc.emplace(procedure, variable);
	procLists.emplace(variable, unordered_set<string>{ procedure });
	procLists.emplace("", unordered_set<string>{ procedure });
	return true;
}

bool ModifyStorage::containsStmVarPair(pair<int, string> pair)
{
	return stmVarPairs.find(pair) != stmVarPairs.end();
}

bool ModifyStorage::containsProcVarPair(pair<string, string> pair)
{
	return procVarPairs.find(pair) != procVarPairs.end();
}

string ModifyStorage::getVarModifiedBy(int stm)
{
	return varList_Stm.find(stm)->second;
}

string ModifyStorage::getVarModifiedBy(string proc)
{
	return varList_Proc.find(proc)->second;
}

unordered_set<int> ModifyStorage::getStmModifying(string variable)
{
	return stmLists.find(variable)->second;
}

unordered_set<string> ModifyStorage::getProcModifying(string variable)
{
	return procLists.find(variable)->second;
}

unordered_set<pair<int, string>> ModifyStorage::getStmVarPairs()
{
	return stmVarPairs;
}

unordered_set<pair<string, string>> ModifyStorage::getProcVarPairs()
{
	return procVarPairs;
}