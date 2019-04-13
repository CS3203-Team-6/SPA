#include "ModifyStorage.h"

std::unordered_set<std::pair<int, std::string>, intStringhash> ModifyStorage::stmVarPairList;
std::unordered_set<std::pair<std::string, std::string>, strPairhash> ModifyStorage::procVarPairList;
std::unordered_map<int, std::unordered_set<std::string>> ModifyStorage::stmToVarMap;
std::unordered_map<std::string, std::unordered_set<std::string>> ModifyStorage::procToVarMap;
std::unordered_map<std::string, std::unordered_set<int> > ModifyStorage::varToStmMap;
std::unordered_map<std::string, std::unordered_set<std::string> > ModifyStorage::varToProcMap;

ModifyStorage::ModifyStorage()
{
}

bool ModifyStorage::addModifiesStm(int stm, std::string variable)
{
	if (!stmVarPairList.emplace(std::pair<int, std::string>(stm, variable)).second)
	{
		return false;
	}
	if (!stmToVarMap.emplace(stm, std::unordered_set<std::string>{ variable }).second)
	{
		stmToVarMap.at(stm).emplace(variable);
	}
	if (!varToStmMap.emplace(variable, std::unordered_set<int>{ stm }).second)
	{
		varToStmMap.at(variable).emplace(stm);
	}
	if (!varToStmMap.emplace("", std::unordered_set<int>{ stm }).second)
	{
		varToStmMap.at("").emplace(stm);
	}
	return true;
}

bool ModifyStorage::addModifiesProc(std::string procedure, std::string variable)
{
	if (!procVarPairList.emplace(std::pair<std::string, std::string>(procedure, variable)).second)
	{
		return false;
	}
	if (!procToVarMap.emplace(procedure, std::unordered_set<std::string>{ variable }).second)
	{
		procToVarMap.at(procedure).emplace(variable);
	}
	if (!varToProcMap.emplace(variable, std::unordered_set<std::string>{ procedure }).second)
	{
		varToProcMap.at(variable).emplace(procedure);
	}
	if (!varToProcMap.emplace("", std::unordered_set<std::string>{ procedure }).second)
	{
		varToProcMap.at("").emplace(procedure);
	}
	return true;
}

bool ModifyStorage::containsStmVarPair(std::pair<int, std::string> pair)
{
	return stmVarPairList.find(pair) != stmVarPairList.end();
}

bool ModifyStorage::containsProcVarPair(std::pair<std::string, std::string> pair)
{
	return procVarPairList.find(pair) != procVarPairList.end();
}

std::unordered_set<std::string> ModifyStorage::getVarModifiedByStm(int stm)
{
	if (stmToVarMap.find(stm) != stmToVarMap.end())
	{
		return stmToVarMap.at(stm);
	}
	return {};
}

std::unordered_set<std::string> ModifyStorage::getVarModifiedByProc(std::string proc)
{
	if (procToVarMap.find(proc) != procToVarMap.end())
	{
		return procToVarMap.at(proc);
	}
	return {};
}

std::unordered_set<int> ModifyStorage::getStmModifying(std::string variable)
{
	if (varToStmMap.find(variable) != varToStmMap.end())
	{
		return varToStmMap.at(variable);
	}
	return {};
}

std::unordered_set<std::string> ModifyStorage::getProcModifying(std::string variable)
{
	if (varToProcMap.find(variable) != varToProcMap.end())
	{
		return varToProcMap.at(variable);
	}
	return {};
}

std::unordered_set<std::pair<int, std::string>, intStringhash> ModifyStorage::getStmVarPairs()
{
	return stmVarPairList;
}

std::unordered_set<std::pair<std::string, std::string>, strPairhash> ModifyStorage::getProcVarPairs()
{
	return procVarPairList;
}

void ModifyStorage::clear()
{
	stmVarPairList.clear();
	procVarPairList.clear();
	stmToVarMap.clear();
	procToVarMap.clear();
	varToProcMap.clear();
	varToStmMap.clear();
}
