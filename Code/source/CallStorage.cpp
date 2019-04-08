#include "CallStorage.h"

std::unordered_map<std::string, cRelationships> CallStorage::callTable;
std::unordered_set< std::pair<std::string, std::string>, strPairhash> CallStorage::callPairList;
std::unordered_set< std::pair<std::string, std::string>, strPairhash> CallStorage::callStarPairList;
std::unordered_set<std::string> CallStorage::callerList;
std::unordered_set<std::string> CallStorage::calleeList;
std::unordered_map<int, std::string> CallStorage::stmToProcMap;
std::unordered_map<std::string, std::unordered_set<int>> CallStorage::procToStmMap;
std::unordered_set< std::pair<int, std::string>, intStringhash> CallStorage::stmProcCallPairList;

CallStorage::CallStorage()
{
}

void CallStorage::addCall(std::string caller, std::string callee, int stm)
{
	// add Stm - Proc Pair
	stmProcCallPairList.emplace(std::pair<int, std::string>(stm, callee));
	stmToProcMap.emplace(stm, callee);

	// add Call Pair
	callPairList.emplace(std::pair<std::string, std::string>(caller, callee));

	// if callee exist in callTable
	if (!callTable.emplace(callee, cRelationships{ {caller}, {}, {}, {} }).second)
	{
		callTable.find(callee)->second.caller.emplace(caller);
	}

	// if callee exist in procToStmMap
	if (!procToStmMap.emplace(callee, std::unordered_set<int>{ stm }).second)
	{
		procToStmMap.find(callee)->second.emplace(stm);
	}

	// if caller exist in callTable
	if (!callTable.emplace(caller, cRelationships{ {} ,{callee}, {}, {} }).second)
	{
		callTable.find(caller)->second.callees.emplace(callee);
	}

	callerList.emplace(caller);
	calleeList.emplace(callee);
}

bool CallStorage::setCallAnc(std::string callee, std::unordered_set<std::string> callAnc)
{
	if (callTable.find(callee)->second.callAnc.size() != 0)
	{
		return false;
	}

	callTable.find(callee)->second.callAnc = callAnc;

	for (auto itr = callAnc.cbegin(); itr != callAnc.cend(); ++itr)
	{
		callStarPairList.emplace(std::pair<std::string, std::string>(*itr, callee));
	}
	return true;
}

bool CallStorage::setCallDesc(std::string caller, std::unordered_set<std::string> callDesc)
{
	if (callTable.find(caller)->second.callDesc.size() != 0)
	{
		return false;
	}

	callTable.find(caller)->second.callDesc = callDesc;

	for (auto itr = callDesc.cbegin(); itr != callDesc.cend(); ++itr)
	{
		callStarPairList.emplace(std::pair<std::string, std::string>(caller, *itr));
	}
	return true;
}

bool CallStorage::isEmpty()
{
	return callTable.size() == 0;
}

bool CallStorage::hasCallStarPair(std::pair<std::string, std::string> pair)
{
	return callStarPairList.find(pair) != callStarPairList.end();
}

bool CallStorage::isCaller(std::string procedure)
{
	return callerList.find(procedure) != callerList.end();
}

bool CallStorage::isCallee(std::string procedure)
{
	return calleeList.find(procedure) != calleeList.end();
}

std::unordered_set<std::string> CallStorage::getCaller(std::string procedure)
{
	if (callTable.find(procedure) != callTable.end())
	{
		return callTable.at(procedure).caller;
	}
	return {};
}

std::unordered_set<std::string> CallStorage::getCallee(std::string procedure)
{
	if (callTable.find(procedure) != callTable.end())
	{
		return callTable.at(procedure).callees;
	}
	return {};
}

std::unordered_set<std::string> CallStorage::getCallAnc(std::string procedure)
{
	if (callTable.find(procedure) != callTable.end())
	{
		return callTable.at(procedure).callAnc;
	}
	return {};
}

std::unordered_set<std::string> CallStorage::getCallDesc(std::string procedure)
{
	if (callTable.find(procedure) != callTable.end())
	{
		return callTable.at(procedure).callDesc;
	}
	return {};
}

std::unordered_set<std::string> CallStorage::getAllCallees()
{
	return calleeList;
}

std::unordered_set<std::string> CallStorage::getAllCallers()
{
	return callerList;
}

std::unordered_set<std::pair<std::string, std::string>, strPairhash> CallStorage::getCallPairs()
{
	return callPairList;
}

std::unordered_set<std::pair<std::string, std::string>, strPairhash> CallStorage::getCallStarPairs()
{
	return callStarPairList;
}

std::string CallStorage::getProcCalledBy(int stm)
{
	if (stmToProcMap.find(stm) != stmToProcMap.end())
	{
		return stmToProcMap.at(stm);
	}
	return "";
}

std::unordered_set<int> CallStorage::getStmCalling(std::string procedure)
{
	if (procToStmMap.find(procedure) != procToStmMap.end())
	{
		return procToStmMap.at(procedure);
	}
	return {};
}

std::unordered_set<std::pair<int, std::string>, intStringhash> CallStorage::getStmProcCallPairs()
{
	return stmProcCallPairList;
}

void CallStorage::erase()
{
	callTable.erase(callTable.begin(), callTable.end());
	callPairList.erase(callPairList.begin(), callPairList.end());
	callStarPairList.erase(callStarPairList.begin(), callStarPairList.end());
	callerList.erase(callerList.begin(), callerList.end());
	calleeList.erase(calleeList.begin(), calleeList.end());
	stmToProcMap.erase(stmToProcMap.begin(), stmToProcMap.end());
	procToStmMap.erase(procToStmMap.begin(), procToStmMap.end());
}
