#include "PKB.h"

std::unordered_set<std::string> PKB::procList;
std::unordered_map<std::string, std::vector<int>> PKB::procStmList;
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
std::unordered_map<int, int> PKB::whileLastStmList;
std::unordered_map<int, std::pair<int, int> > PKB::ifLastStmList;

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
	if (!procStmList.emplace(procedure, std::vector<int>{stmNo}).second)
	{
		procStmList.find(procedure)->second.push_back(stmNo);
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
{
	whileLastStmList.emplace(whileStm, lastStm);
}

void PKB::addThenLastStm(int ifStm, int thenStm)
{
	if (!ifLastStmList.emplace(ifStm, std::pair<int, int>{thenStm, 0}).second)
	{
		ifLastStmList.at(ifStm).first = thenStm;
	}
}

void PKB::addElseLastStm(int ifStm, int elseStm)
{
	if (!ifLastStmList.emplace(ifStm, std::pair<int, int>{0, elseStm}).second)
	{
		ifLastStmList.at(ifStm).second = elseStm;
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
	if (procStmList.find(procedure) != procStmList.end())
	{
		return procStmList.at(procedure);
	}
	return {};
}

int PKB::getTotalStmNo()
{
	return stmTypeList.size();
}

stmType PKB::getStmType(int stm)
{
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
	if (whileLastStmList.find(whileStm) != whileLastStmList.end())
	{
		return whileLastStmList.at(whileStm);
	}
	return 0;
}

std::pair<int, int> PKB::getIfLastStms(int ifStm)
{
	if (ifLastStmList.find(ifStm) != ifLastStmList.end())
	{
		return ifLastStmList.at(ifStm);
	}
	return std::pair<int, int>(0, 0);
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
	return RunTimeDesignExtractor::extractNextStarPair(this, line1, line2);
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
	return RunTimeDesignExtractor::extractNextStar(this, line);
}

std::unordered_set<int> PKB::getAllLnBefore(int line)
{
	return RunTimeDesignExtractor::extractPreviousStar(this, line);
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
	return RunTimeDesignExtractor::getNextStarPairs(this);
}

bool PKB::hasAffectsRelation()
{
	// call DE
	return false;
}

bool PKB::isAffector(int stm)
{
	// call DE
	return false;
}

bool PKB::isAffected(int stm)
{
	// call DE
	return false;
}

bool PKB::hasAffectStarPair(int stm1, int stm2)
{
	// call DE
	return false;
}

int PKB::getAffector(int stm)
{
	// call DE
	return 0;
}

int PKB::getAffected(int stm)
{
	// call DE
	return 0;
}

std::unordered_set<int> PKB::getAffectorStar(int stm)
{
	// call DE
	return std::unordered_set<int>();
}

std::unordered_set<int> PKB::getAffectedStar(int stm)
{
	// call DE
	return std::unordered_set<int>();
}

std::unordered_set<int> PKB::getAllAffectors()
{
	// call DE
	return std::unordered_set<int>();
}

std::unordered_set<int> PKB::getAllAffected()
{
	// call DE
	return std::unordered_set<int>();
}

std::unordered_set<std::pair<int, int>, intPairhash> PKB::getAffectPairs()
{
	// call DE
	return std::unordered_set<std::pair<int, int>, intPairhash>();
}

std::unordered_set<std::pair<int, int>, intPairhash> PKB::getAffectStarPairs()
{
	// call DE
	return std::unordered_set<std::pair<int, int>, intPairhash>();
}

std::unordered_set<int> PKB::findPattern(std::string variable, std::string expr, bool isExclusive)
{
	std::unordered_set<int> validStm;
	for each (const auto elem in patternList)
	{
		if (elem.second.first.compare(variable) == 0)
		{
			if (isExclusive && elem.second.second.compare(expr) == 0)
			{
				validStm.emplace(elem.first);
			}
			else if (!isExclusive && elem.second.second.find(expr) != std::string::npos)
			{
				validStm.emplace(elem.first);
			}
		}
	}
	return validStm;
}

std::unordered_set<int> PKB::findPattern(std::string expr, bool isExclusive)
{
	std::unordered_set<int> validStm;
	for each (const auto elem in patternList)
	{
		if (isExclusive && elem.second.second.compare(expr) == 0)
		{
			validStm.emplace(elem.first);
		}
		else if (!isExclusive && elem.second.second.find(expr) != std::string::npos)
		{
			validStm.emplace(elem.first);
		}
	}
	return validStm;
}

std::unordered_set<std::pair<int, std::string>, intStringhash>PKB::findPatternPairs(std::string expr, bool isExclusive)
{
	std::unordered_set<std::pair<int, std::string>, intStringhash> validPairs;
	for each (const auto elem in patternList)
	{
		if (isExclusive && elem.second.second.compare(expr) == 0)
		{
			validPairs.emplace(std::pair<int, std::string>(elem.first, elem.second.first));
		}
		else if (!isExclusive && elem.second.second.find(expr) != std::string::npos)
		{
			validPairs.emplace(std::pair<int, std::string>(elem.first, elem.second.first));
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

void PKB::erase()
{
	procList.erase(procList.begin(), procList.end());
	procStmList.erase(procStmList.begin(), procStmList.end());
	stmTypeList.erase(stmTypeList.begin(), stmTypeList.end());
	varList.erase(varList.begin(), varList.end());
	constList.erase(constList.begin(), constList.end());
	readStmList.erase(readStmList.begin(), readStmList.end());
	printStmList.erase(printStmList.begin(), printStmList.end());
	assignStmList.erase(assignStmList.begin(), assignStmList.end());
	ifStmList.erase(ifStmList.begin(), ifStmList.end());
	whileStmList.erase(whileStmList.begin(), whileStmList.end());
	callStmList.erase(callStmList.begin(), callStmList.end());
	patternList.erase(patternList.begin(), patternList.end());
	fStore.erase();
	pStore.erase();
	uStore.erase();
	mStore.erase();
	cStore.erase();
	nStore.erase();
}
