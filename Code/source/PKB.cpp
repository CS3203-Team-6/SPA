#include "PKB.h"

std::unordered_set<std::string> PKB::procList;
std::unordered_map<std::string, std::vector<int>> PKB::procStmMap;
std::vector<std::string> PKB::stmProcList;
std::vector<stmType> PKB::stmTypeList;
std::unordered_set<std::string> PKB::varList;
std::unordered_set<std::string> PKB::constList;
std::unordered_set<int> PKB::readStmList;
std::unordered_set<int> PKB::printStmList;
std::unordered_set<int> PKB::assignStmList;
std::unordered_set<int> PKB::ifStmList;
std::unordered_set<int> PKB::whileStmList;
std::unordered_set<int> PKB::callStmList;
std::unordered_set< std::pair<int, std::string>, intStringhash > PKB::readPairList;
std::unordered_set< std::pair<int, std::string>, intStringhash > PKB::printPairList;
std::unordered_map<int, std::vector<int>> PKB::whileBlockStmLists;
std::unordered_map<int, std::vector<int>> PKB::ifBlockStmLists;
std::unordered_map<int, std::vector<int>> PKB::elseBlockStmLists;

std::unordered_set< std::pair<int, int>, intPairhash> PKB::affectPairCache;
std::unordered_set< std::pair<int, int>, intPairhash> PKB::affectStarPairCache;

FollowStorage PKB::fStore;
ParentStorage PKB::pStore;
UseStorage PKB::uStore;
ModifyStorage PKB::mStore;
CallStorage PKB::cStore;
NextStorage PKB::nStore;
CtrlVarStorage PKB::cvStore;
std::unordered_map<int, std::pair<std::string, std::string> > PKB::patternList;

PKB::PKB()
{
}

bool PKB::addProc(std::string name)
{
	return procList.emplace(name).second;
}

void PKB::addStatement(int stmNo, stmType type, std::string procedure)
{
	stmTypeList.push_back(type);
	stmProcList.push_back(procedure);
	if (!procStmMap.emplace(procedure, std::vector<int>{stmNo}).second)
	{
		procStmMap.find(procedure)->second.push_back(stmNo);
	}

	switch (type)
	{
		case read:
			readStmList.emplace(stmNo);
			break;
		case print:
			printStmList.emplace(stmNo);
			break;
		case assign:
			assignStmList.emplace(stmNo);
			break;
		case whileStm:
			whileStmList.emplace(stmNo);
			break;
		case ifStm:
			ifStmList.emplace(stmNo);
			break;
		case call:
			callStmList.emplace(stmNo);
			break;
		default:
			break;
	}
}

void PKB::addVariable(std::string name)
{
	varList.emplace(name);
}

void PKB::addConstant(std::string value)
{
	constList.emplace(value);
}

void PKB::addWhileLastStm(int whileStm, int lastStm)
{//unused
	//whileLastStmList.emplace(whileStm, lastStm);
}

void PKB::addThenLastStm(int ifStm, int thenStm)
{/*unused
	if (!ifLastStmList.emplace(ifStm, std::pair<int, int>{thenStm, 0}).second)
	{
		ifLastStmList.at(ifStm).first = thenStm;
	}*/
}

void PKB::addElseLastStm(int ifStm, int elseStm)
{/*unused
	if (!ifLastStmList.emplace(ifStm, std::pair<int, int>{0, elseStm}).second)
	{
		ifLastStmList.at(ifStm).second = elseStm;
	}*/
}

void PKB::addIfContainerStatement(int ifStm, int stmToAdd)
{
	if (!ifBlockStmLists.emplace(ifStm, std::vector<int>{stmToAdd}).second)
	{
		ifBlockStmLists.at(ifStm).push_back(stmToAdd);
	}
}

void PKB::addElseContainerStatement(int ifStm, int stmToAdd)
{
	if (!elseBlockStmLists.emplace(ifStm, std::vector<int>{stmToAdd}).second)
	{
		elseBlockStmLists.at(ifStm).push_back(stmToAdd);
	}
}

void PKB::addWhileContainerStatement(int whileStm, int stmToAdd)
{
	if (!whileBlockStmLists.emplace(whileStm, std::vector<int>{stmToAdd}).second)
	{
		whileBlockStmLists.at(whileStm).push_back(stmToAdd);
	}
}

bool PKB::addFollow(int stm1, int stm2)
{
	if (stm2 <= stm1 || stm1 <= 0 || stm2 <= 0)
	{
		return false;
	}
	return fStore.addFollow(stm1, stm2);
}

bool PKB::setAllFollowing(int stm, std::unordered_set<int> stmList)
{
	return fStore.setAllFollowing(stm, stmList);
}

bool PKB::setAllFollowedBy(int stm, std::unordered_set<int> stmList)
{
	return fStore.setAllFollowedBy(stm, stmList);
}

bool PKB::addParent(int stm1, int stm2)
{
	if (stm2 <= stm1 || stm1 <= 0 || stm2 <= 0)
	{
		return false;
	}
	return pStore.addParent_Child(stm1, stm2);
}

bool PKB::setAncestors(int stm, std::unordered_set<int> stmList)
{
	return pStore.setAncestors(stm, stmList);
}

bool PKB::setDescendants(int stm, std::unordered_set<int> stmList)
{
	return pStore.setDescendants(stm, stmList);
}

bool PKB::addUsesStm(int stm, std::string variable)
{
	if (stm <= 0 || variable == "")
	{
		return false;
	}
	// if the stm using 'variable' is a print stm
	if (printStmList.find(stm) != printStmList.end())
	{
		printPairList.emplace(std::pair<int, std::string>(stm, variable));
	}
	return uStore.addUsesStm(stm, variable);
}

bool PKB::addUsesProc(std::string procedure, std::string variable)
{
	if (procedure == "" || variable == "")
	{
		return false;
	}
	return uStore.addUsesProc(procedure, variable);
}

bool PKB::addModifiesStm(int stm, std::string variable)
{
	if (variable == "")
	{
		return false;
	}
	// if the stm modifying 'variable' is a read stm
	if (readStmList.find(stm) != readStmList.end())
	{
		readPairList.emplace(std::pair<int, std::string>(stm, variable));
	}
	return mStore.addModifiesStm(stm, variable);
}

bool PKB::addModifiesProc(std::string procedure, std::string variable)
{
	if (procedure == "" || variable == "")
	{
		return false;
	}
	return mStore.addModifiesProc(procedure, variable);
}

void PKB::addCall(std::string proc1, std::string proc2, int stmNo)
{
	/*
	if (proc1 == "" || proc2 == "" || stmNo <= 0)
	{
		return false;
	}
	*/
	cStore.addCall(proc1, proc2, stmNo);
}

bool PKB::setCallAnc(std::string proc, std::unordered_set<std::string> procList)
{
	return cStore.setCallAnc(proc, procList);
}

bool PKB::setCallDesc(std::string proc, std::unordered_set<std::string> procList)
{
	return cStore.setCallDesc(proc, procList);
}

bool PKB::addNext(int line1, int line2)
{
	if (line1 <= 0 || line2 <= 0)
	{
		return false;
	}
	return nStore.addNext(line1, line2);
}

bool PKB::addAssignPattern(int stm, std::string variable, std::string expr)
{
	return patternList.emplace(stm, std::pair<std::string, std::string>(variable, expr)).second;
}

std::unordered_set<std::string> PKB::getProcList()
{
	return procList;
}

std::vector<int> PKB::getStmList(std::string procedure)
{
	if (procStmMap.find(procedure) != procStmMap.end())
	{
		return procStmMap.at(procedure);
	}
	return {};
}

int PKB::getTotalStmNo()
{
	return stmTypeList.size();
}

stmType PKB::getStmType(int stm)
{
	if (stm <= 0 || stm > stmTypeList.size())
	{
		return nonExistant;
	}
	return stmTypeList.at(stm - 1);
}

std::unordered_set<int> PKB::getReadStms()
{
	return readStmList;
}

std::unordered_set<int> PKB::getPrintStms()
{
	return printStmList;
}

std::unordered_set<int> PKB::getAssignStms()
{
	return assignStmList;
}

std::unordered_set<int> PKB::getIfStms()
{
	return ifStmList;
}

std::unordered_set<int> PKB::getWhileStms()
{
	return whileStmList;
}

std::unordered_set<int> PKB::getCallStms()
{
	return callStmList;
}

std::unordered_set<std::string> PKB::getVariables()
{
	return varList;
}

std::unordered_set<std::string> PKB::getConstants()
{
	return constList;
}

std::unordered_set<std::pair<int, std::string>, intStringhash> PKB::getReadPairs()
{
	return readPairList;
}

std::unordered_set<std::pair<int, std::string>, intStringhash> PKB::getPrintPairs()
{
	return printPairList;
}

int PKB::getWhileLastStm(int whileStm)
{
	if (whileBlockStmLists.find(whileStm) != whileBlockStmLists.end())
	{
		std::vector<int> whileBlock = whileBlockStmLists.at(whileStm);
		return whileBlock.at(whileBlock.size() - 1);
	}
	return 0;
}

std::pair<int, int> PKB::getIfLastStms(int ifStm)
{
	if (ifBlockStmLists.find(ifStm) != ifBlockStmLists.end())
	{
		std::vector<int> ifBlock = ifBlockStmLists.at(ifStm);
		std::vector<int> elseBlock = elseBlockStmLists.at(ifStm);

		int first = ifBlock.at(ifBlock.size() - 1);
		int second = elseBlock.at(elseBlock.size() - 1);

		return std::pair<int, int>(first, second);
	}
	return std::pair<int, int>(0, 0);
}

std::vector<int> PKB::getIfStmContainer(int ifStm)
{
	if (ifBlockStmLists.find(ifStm) != ifBlockStmLists.end())
	{
		return ifBlockStmLists.at(ifStm);
	}
	return {};
}

std::vector<int> PKB::getElseStmContainer(int ifStm)
{
	if (elseBlockStmLists.find(ifStm) != elseBlockStmLists.end())
	{
		return elseBlockStmLists.at(ifStm);
	}
	return {};
}

std::vector<int> PKB::getWhileStmContainer(int whileStm)
{
	if (whileBlockStmLists.find(whileStm) != whileBlockStmLists.end())
	{
		return whileBlockStmLists.at(whileStm);
	}
	return {};
}

std::string PKB::getProcOfStm(int stm)
{
	if (stm <= stmProcList.size() && stm > 0)
	{
		return stmProcList.at(stm - 1);
	}
	return "";
}

bool PKB::hasFollowRelation()
{
	return !fStore.isEmpty();
}

bool PKB::hasFollowStarPair(int stm1, int stm2)
{
	return fStore.hasFollowStarPair(std::pair<int, int>(stm1, stm2));
}

int PKB::getStmFollowedBy(int stm)
{
	return fStore.getStmFollowedBy(stm);
}

int PKB::getFollower(int stm)
{
	return fStore.getFollower(stm);
}

std::unordered_set<int> PKB::getAllFollowing(int stm)
{
	return fStore.getAllFollowing(stm);
}

std::unordered_set<int> PKB::getAllFollowedBy(int stm)
{
	return fStore.getAllFollowedBy(stm);
}

std::unordered_set<int> PKB::getAllFollowers()
{
	return fStore.getAllFollowers();
}

std::unordered_set<int> PKB::getAllFollowed()
{
	return fStore.getAllFollowed();
}

std::unordered_set<std::pair<int, int>, intPairhash> PKB::getFollowPairs()
{
	return fStore.getFollowPairs();
}

std::unordered_set<std::pair<int, int>, intPairhash> PKB::getFollowStarPairs()
{
	return fStore.getFollowStarPairs();
}

bool PKB::hasParentRelation()
{
	return !pStore.isEmpty();
}

bool PKB::isParent(int stm)
{
	return pStore.isParent(stm);
}

bool PKB::isChild(int stm)
{
	return pStore.isChild(stm);
}

bool PKB::hasAncDescPair(int stm1, int stm2)
{
	return pStore.hasAncDescPair(std::pair<int, int>(stm1, stm2));
}

int PKB::getParent(int stm)
{
	return pStore.getParent(stm);
}

std::unordered_set<int> PKB::getChildren(int stm)
{
	return pStore.getChildren(stm);
}

std::unordered_set<int> PKB::getAncestors(int stm)
{
	return pStore.getAncestors(stm);
}

std::unordered_set<int> PKB::getDescendants(int stm)
{
	return pStore.getDescendants(stm);
}

std::unordered_set<int> PKB::getAllParents()
{
	return pStore.getAllParent();
}

std::unordered_set<int> PKB::getAllChildren()
{
	return pStore.getAllChildren();
}

std::unordered_set<std::pair<int, int>, intPairhash> PKB::getParentChildPairs()
{
	return pStore.getParentChildPairs();
}

std::unordered_set<std::pair<int, int>, intPairhash> PKB::getAncDescPairs()
{
	return pStore.getAncDescPair();
}

bool PKB::isStmUsing(int stm, std::string variable)
{
	return uStore.containsStmVarPair(std::pair<int, std::string>(stm, variable) );
}

bool PKB::isProcUsing(std::string procedure, std::string variable)
{
	return uStore.containsProcVarPair(std::pair<std::string, std::string>(procedure, variable));
}

std::unordered_set<std::string> PKB::getVarUsedByStm(int stm)
{
	return uStore.getVarUsedByStm(stm);
}

std::unordered_set<std::string> PKB::getVarUsedByProc(std::string procedure)
{
	return uStore.getVarUsedByProc(procedure);
}

std::unordered_set<int> PKB::getStmUsing(std::string variable)
{
	return uStore.getStmUsing(variable);
}

std::unordered_set<std::string> PKB::getProcUsing(std::string variable)
{
	return uStore.getProcUsing(variable);
}

std::unordered_set<std::pair<int, std::string>, intStringhash> PKB::getStmVarUsePairs()
{
	return uStore.getStmVarPairs();
}

std::unordered_set<std::pair<std::string, std::string>, strPairhash> PKB::getProcVarUsePairs()
{
	return uStore.getProcVarPairs();
}

bool PKB::isStmModifying(int stm, std::string variable)
{
	return mStore.containsStmVarPair(std::pair<int, std::string>(stm, variable));
}

bool PKB::isProcModifying(std::string procedure, std::string variable)
{
	return mStore.containsProcVarPair(std::pair<std::string, std::string>(procedure, variable));
}

std::unordered_set<std::string> PKB::getVarModifiedByStm(int stm)
{
	return mStore.getVarModifiedByStm(stm);
}

std::unordered_set<std::string> PKB::getVarModifiedByProc(std::string procedure)
{
	return mStore.getVarModifiedByProc(procedure);
}

std::unordered_set<int> PKB::getStmModifying(std::string variable)
{
	return mStore.getStmModifying(variable);
}

std::unordered_set<std::string> PKB::getProcModifying(std::string variable)
{
	return mStore.getProcModifying(variable);
}

std::unordered_set<std::pair<int, std::string>, intStringhash> PKB::getStmVarModifyPairs()
{
	return mStore.getStmVarPairs();
}

std::unordered_set<std::pair<std::string, std::string>, strPairhash> PKB::getProcVarModifyPairs()
{
	return mStore.getProcVarPairs();
}

bool PKB::hasCallRelation()
{
	return !cStore.isEmpty();
}

bool PKB::isCaller(std::string procedure)
{
	return cStore.isCaller(procedure);
}

bool PKB::isCallee(std::string procedure)
{
	return cStore.isCallee(procedure);
}

bool PKB::hasCallStarPair(std::string proc1, std::string proc2)
{
	return cStore.hasCallStarPair(std::pair<std::string, std::string>(proc1, proc2));
}

std::unordered_set<std::string> PKB::getCaller(std::string procedure)
{
	return cStore.getCaller(procedure);
}

std::unordered_set<std::string> PKB::getCallee(std::string procedure)
{
	return cStore.getCallee(procedure);
}

std::unordered_set<std::string> PKB::getCallAnc(std::string procedure)
{
	return cStore.getCallAnc(procedure);
}

std::unordered_set<std::string> PKB::getCallDesc(std::string procedure)
{
	return cStore.getCallDesc(procedure);
}

std::unordered_set<std::string> PKB::getAllCallers()
{
	return cStore.getAllCallers();
}

std::unordered_set<std::string> PKB::getAllCallees()
{
	return cStore.getAllCallees();
}

std::unordered_set<std::pair<std::string, std::string>, strPairhash> PKB::getCallPairs()
{
	return cStore.getCallPairs();
}

std::unordered_set<std::pair<std::string, std::string>, strPairhash> PKB::getCallStarPairs()
{
	return cStore.getCallStarPairs();
}

std::string PKB::getProcCalledBy(int stm)
{
	return cStore.getProcCalledBy(stm);
}

std::unordered_set<int> PKB::getStmCalling(std::string procedure)
{
	return cStore.getStmCalling(procedure);
}

std::unordered_set<std::pair<int, std::string>, intStringhash> PKB::getStmProcCallPairs()
{
	return cStore.getStmProcCallPairs();
}

bool PKB::hasNextRelation()
{
	return !nStore.isEmpty();
}

bool PKB::hasNextStarPair(int line1, int line2)
{
	return RunTimeDesignExtractor().extractNextStarPair(line1, line2);
}

std::unordered_set<int> PKB::getNext(int line)
{
	return nStore.getNext(line);
}

std::unordered_set<int> PKB::getPrev(int line)
{
	return nStore.getPrev(line);
}

std::unordered_set<int> PKB::getAllLnAfter(int line)
{
	return RunTimeDesignExtractor().extractNextStar(line);
}

std::unordered_set<int> PKB::getAllLnBefore(int line)
{
	return RunTimeDesignExtractor().extractPreviousStar(line);
}

std::unordered_set<int> PKB::getAllNext()
{
	return nStore.getAllNext();
}

std::unordered_set<int> PKB::getAllPrev()
{
	return nStore.getAllPrev();
}

std::unordered_set<std::pair<int, int>, intPairhash> PKB::getNextPairs()
{
	return nStore.getNextPairs();
}

std::unordered_set<std::pair<int, int>, intPairhash> PKB::getNextStarPairs()
{
	return RunTimeDesignExtractor().getNextStarPairs();
}

bool PKB::hasAffectsRelation()
{
	if (!affectPairCache.empty())
	{
		return true;
	}
	return RunTimeDesignExtractor().hasAffectsRelation();
}

bool PKB::isAffector(int stm)
{
	if (!affectPairCache.empty())
	{
		for (std::unordered_set<std::pair<int, int>, intPairhash>::iterator itr =
			 affectPairCache.cbegin(); itr != affectPairCache.cend(); itr++)
		{
			if (itr->first == stm)
			{
				return true;
			}
		}
	}
	return RunTimeDesignExtractor().isStatementAffectingAnother(stm);
}

bool PKB::isAffected(int stm)
{
	if (!affectPairCache.empty())
	{
		for (std::unordered_set<std::pair<int, int>, intPairhash>::iterator itr =
			 affectPairCache.cbegin(); itr != affectPairCache.cend(); itr++)
		{
			if (itr->second == stm)
			{
				return true;
			}
		}
	}
	return RunTimeDesignExtractor().isStatementAffectedByAnother(stm);
}

bool PKB::hasAffectPair(int stm1, int stm2)
{
	if (!affectPairCache.empty())
	{
		if (affectPairCache.find(std::pair<int, int>(stm1, stm2)) != affectPairCache.end())
		{
			return true;
		}
		return false;
	}
	return RunTimeDesignExtractor().isAffect(stm1, stm2);
}

bool PKB::hasAffectStarPair(int stm1, int stm2)
{
	if (!affectStarPairCache.empty())
	{
		if (affectStarPairCache.find(std::pair<int, int>(stm1, stm2)) != affectStarPairCache.end())
		{
			return true;
		}
		return false;
	}
	return RunTimeDesignExtractor().isAffectStar(stm1, stm2);
}

std::vector<int> PKB::getAffector(int stm)
{
	if (!affectPairCache.empty())
	{
		std::vector<int> affectors;
		for (std::unordered_set<std::pair<int, int>, intPairhash>::iterator itr =
			 affectPairCache.cbegin(); itr != affectPairCache.cend(); itr++)
		{
			if (itr->second == stm)
			{
				affectors.push_back(itr->first);
			}
		}
		return affectors;
	}
	return RunTimeDesignExtractor().getStatementsAffectingIndex(stm);
}

std::vector<int> PKB::getAffected(int stm)
{
	if (!affectPairCache.empty())
	{
		std::vector<int> affected;
		for (std::unordered_set<std::pair<int, int>, intPairhash>::iterator itr =
			 affectPairCache.cbegin(); itr != affectPairCache.cend(); itr++)
		{
			if (itr->first == stm)
			{
				affected.push_back(itr->second);
			}
		}
		return affected;
	}
	return RunTimeDesignExtractor().getStatementsAffectedByIndex(stm);
}

std::vector<int> PKB::getAffectorStar(int stm)
{
	if (!affectStarPairCache.empty())
	{
		std::vector<int> affectors;
		for (std::unordered_set<std::pair<int, int>, intPairhash>::iterator itr =
			 affectStarPairCache.cbegin(); itr != affectStarPairCache.cend(); itr++)
		{
			if (itr->second == stm)
			{
				affectors.push_back(itr->first);
			}
		}
		return affectors;
	}
	return RunTimeDesignExtractor().getAllStatementsAffectingIndexStar(stm);
}

std::vector<int> PKB::getAffectedStar(int stm)
{
	if (!affectStarPairCache.empty())
	{
		std::vector<int> affected;
		for (std::unordered_set<std::pair<int, int>, intPairhash>::iterator itr =
			 affectStarPairCache.cbegin(); itr != affectStarPairCache.cend(); itr++)
		{
			if (itr->first == stm)
			{
				affected.push_back(itr->second);
			}
		}
		return affected;
	}
	return RunTimeDesignExtractor().getAllStatementsAffectedByIndexStar(stm);
}

std::vector<int> PKB::getAllAffectors()
{
	if (!affectPairCache.empty())
	{
		std::vector<int> affectors;
		for (std::unordered_set<std::pair<int, int>, intPairhash>::iterator itr =
			 affectPairCache.cbegin(); itr != affectPairCache.cend(); itr++)
		{
			affectors.push_back(itr->first);
		}
		return affectors;
	}
	return RunTimeDesignExtractor().getAllStatementsAffectingAnother();
}

std::vector<int> PKB::getAllAffected()
{
	if (!affectPairCache.empty())
	{
		std::vector<int> affected;
		for (std::unordered_set<std::pair<int, int>, intPairhash>::iterator itr =
			 affectPairCache.cbegin(); itr != affectPairCache.cend(); itr++)
		{
			affected.push_back(itr->second);
		}
		return affected;
	}
	return RunTimeDesignExtractor().getAllStatementsAffectingByAnother();
}

std::unordered_set<std::pair<int, int>, intPairhash> PKB::getAffectPairs()
{
	affectPairCache = RunTimeDesignExtractor().getAffectsPair();
	return affectPairCache;
}

std::unordered_set<std::pair<int, int>, intPairhash> PKB::getAffectStarPairs()
{
	affectStarPairCache =  RunTimeDesignExtractor().getAffectsStarPair();
	return affectStarPairCache;
}

std::unordered_set<int> PKB::findPattern(std::string variable, std::string expr, bool isExclusive)
{
	std::unordered_set<int> validStm;
	for (std::unordered_map<int, std::pair<std::string, std::string>>::iterator itr =
		 patternList.begin(); itr != patternList.end(); itr++)
	{
		if (itr->second.first.compare(variable) == 0)
		{
			if (isExclusive && itr->second.second.compare(expr) == 0)
			{
				validStm.emplace(itr->first);
			}
			else if (!isExclusive && itr->second.second.find(expr) != std::string::npos)
			{
				validStm.emplace(itr->first);
			}
		}
	}
	return validStm;
}

std::unordered_set<int> PKB::findPattern(std::string expr, bool isExclusive)
{
	std::unordered_set<int> validStm;
	for (std::unordered_map<int, std::pair<std::string, std::string>>::iterator itr =
		 patternList.begin(); itr != patternList.end(); itr++)
	{
		if (isExclusive && itr->second.second.compare(expr) == 0)
		{
			validStm.emplace(itr->first);
		}
		else if (!isExclusive && itr->second.second.find(expr) != std::string::npos)
		{
			validStm.emplace(itr->first);
		}
	}
	return validStm;
}

std::unordered_set<std::pair<int, std::string>, intStringhash>PKB::findPatternPairs(std::string expr,
																					bool isExclusive)
{
	std::unordered_set<std::pair<int, std::string>, intStringhash> validPairs;
	for (std::unordered_map<int, std::pair<std::string, std::string>>::iterator itr =
		 patternList.begin(); itr != patternList.end(); itr++)
	{
		if (isExclusive && itr->second.second.compare(expr) == 0)
		{
			validPairs.emplace(std::pair<int, std::string>(itr->first, itr->second.first));
		}
		else if (!isExclusive && itr->second.second.find(expr) != std::string::npos)
		{
			validPairs.emplace(std::pair<int, std::string>(itr->first, itr->second.first));
		}
	}
	return validPairs;
}

void PKB::addIfControlVariable(int stm, std::string variable)
{
	cvStore.addIfControlVariable(stm, variable);
}

void PKB::addWhileControlVariable(int stm, std::string variable)
{
	cvStore.addWhileControlVariable(stm, variable);
}

std::unordered_set<int> PKB::getAllIfWithControls()
{
	return cvStore.getAllIfWithControls();
}

std::unordered_set<int> PKB::getAllWhileWithControls()
{
	return cvStore.getAllWhileWithControls();
}

std::unordered_set<int> PKB::getIfStmWithControlVariable(std::string variable)
{
	return cvStore.getIfStmWithControlVariable(variable);
}

std::unordered_set<int> PKB::getWhileStmWithControlVariable(std::string variable)
{
	return cvStore.getWhileStmWithControlVariable(variable);
}

std::unordered_set<std::pair<int, std::string>, intStringhash> PKB::getIfStmControlVariablePair()
{
	return cvStore.getIfStmControlVariablePair();
}

std::unordered_set<std::pair<int, std::string>, intStringhash> PKB::getWhileStmControlVariablePair()
{
	return cvStore.getWhileStmControlVariablePair();
}

void PKB::clear()
{
	procList.clear();
	procStmMap.clear();
	stmProcList.clear();
	stmTypeList.clear();
	varList.clear();
	constList.clear();
	readStmList.clear();
	printStmList.clear();
	assignStmList.clear();
	ifStmList.clear();
	whileStmList.clear();
	callStmList.clear();
	patternList.clear();
	readPairList.clear();
	printPairList.clear();
	whileBlockStmLists.clear();
	ifBlockStmLists.clear();
	elseBlockStmLists.clear();
	affectPairCache.clear();
	affectStarPairCache.clear();
	fStore.clear();
	pStore.clear();
	uStore.clear();
	mStore.clear();
	cStore.clear();
	nStore.clear();
}

void PKB::clearCache()
{
	affectPairCache.clear();
	affectStarPairCache.clear();
}
