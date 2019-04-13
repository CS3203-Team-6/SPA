#include "UseStorage.h"

std::unordered_set<std::pair<int, std::string>, intStringhash> UseStorage::stmVarPairList;
std::unordered_set<std::pair<std::string, std::string>, strPairhash> UseStorage::procVarPairList;
std::unordered_map<int, std::unordered_set<std::string> > UseStorage::stmToVarMap;
std::unordered_map<std::string, std::unordered_set<std::string> > UseStorage::procToVarMap;
std::unordered_map<std::string, std::unordered_set<int> > UseStorage::varToStmMap;
std::unordered_map<std::string, std::unordered_set<std::string> > UseStorage::varToProcMap;

UseStorage::UseStorage()
{
}

bool UseStorage::addUsesStm(int stm, std::string variable)
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

bool UseStorage::addUsesProc(std::string procedure, std::string variable)
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

bool UseStorage::containsStmVarPair(std::pair<int, std::string> pair)
{
	return stmVarPairList.find(pair) != stmVarPairList.end();
}

bool UseStorage::containsProcVarPair(std::pair<std::string, std::string> pair)
{
	return procVarPairList.find(pair) != procVarPairList.end();
}

std::unordered_set<std::string> UseStorage::getVarUsedByStm(int stm)
{
	if (stmToVarMap.find(stm) != stmToVarMap.end())
	{
		return stmToVarMap.at(stm);
	}
	return {};
}

std::unordered_set<std::string> UseStorage::getVarUsedByProc(std::string procedure)
{
	if (procToVarMap.find(procedure) != procToVarMap.end())
	{
		return procToVarMap.at(procedure);
	}
	return {};
}

std::unordered_set<int> UseStorage::getStmUsing(std::string variable)
{
	if (varToStmMap.find(variable) != varToStmMap.end())
	{
		return varToStmMap.at(variable);
	}
	return {};
}

std::unordered_set<std::string> UseStorage::getProcUsing(std::string variable)
{
	if (varToProcMap.find(variable) != varToProcMap.end())
	{
		return varToProcMap.at(variable);
	}
	return {};
}

std::unordered_set<std::pair<int, std::string>, intStringhash> UseStorage::getStmVarPairs()
{
	return stmVarPairList;
}

std::unordered_set<std::pair<std::string, std::string>, strPairhash> UseStorage::getProcVarPairs()
{
	return procVarPairList;
}

void UseStorage::clear()
{
	stmVarPairList.clear();
	procVarPairList.clear();
	stmToVarMap.clear();
	procToVarMap.clear();
	varToStmMap.clear();
	varToProcMap.clear();
}
