#include "CtrlVarStorage.h"

std::unordered_map<std::string, std::unordered_set<int>> CtrlVarStorage::ifVariableToStm;
std::unordered_map<std::string, std::unordered_set<int>> CtrlVarStorage::whileVariableToStm;

CtrlVarStorage::CtrlVarStorage() {

}

void CtrlVarStorage::addIfControlVariable(int stm, std::string variable) {

	//if cannot find key
	if (ifVariableToStm.find(variable) == ifVariableToStm.end()) {
		std::unordered_set<int> stmList;
		stmList.insert(stm);
		ifVariableToStm[variable] = stmList;
	} else {
		std::unordered_set<int> stmList = ifVariableToStm[variable];
		stmList.insert(stm);
		ifVariableToStm[variable] = stmList;
	}
}

void CtrlVarStorage::addWhileControlVariable(int stm, std::string variable) {

	//If cannot find key
	if (whileVariableToStm.find(variable) == whileVariableToStm.end()) {
		std::unordered_set<int> stmList;
		stmList.insert(stm);
		whileVariableToStm[variable] = stmList;
	} else {
		std::unordered_set<int> stmList = whileVariableToStm[variable];
		stmList.insert(stm);
		whileVariableToStm[variable] = stmList;
	}
}

std::unordered_set<int> CtrlVarStorage::getAllIfWithControls() {
	std::unordered_set<int> result;

	for (std::pair<std::string, std::unordered_set<int>> pair : ifVariableToStm) {
		for (int stm : pair.second) {
			result.insert(stm);
		}
	}
	return result;

}

std::unordered_set<int> CtrlVarStorage::getAllWhileWithControls() {
	std::unordered_set<int> result;

	for (std::pair<std::string, std::unordered_set<int>> pair : whileVariableToStm) {
		for (int stm : pair.second) {
			result.insert(stm);
		}
	}
	return result;

}

std::unordered_set<int> CtrlVarStorage::getIfStmWithControlVariable(std::string variable) {

	return ifVariableToStm[variable];

}

std::unordered_set<int> CtrlVarStorage::getWhileStmWithControlVariable(std::string variable) {

	return whileVariableToStm[variable];

}

std::unordered_set<std::pair<int, std::string>, intStringhash> CtrlVarStorage::getIfStmControlVariablePair() {

	std::unordered_set<std::pair<int, std::string>, intStringhash> result;

	for (std::pair<std::string, std::unordered_set<int>> pair : ifVariableToStm) {
		for (int stm : pair.second) {
			result.insert(std::make_pair(stm, pair.first));
		}
	}
	return result;
}

std::unordered_set<std::pair<int, std::string>, intStringhash> CtrlVarStorage::getWhileStmControlVariablePair()
{
	std::unordered_set<std::pair<int, std::string>, intStringhash> result;

	for (std::pair<std::string, std::unordered_set<int>> pair : whileVariableToStm) {
		for (int stm : pair.second) {
			result.insert(std::make_pair(stm, pair.first));
		}
	}
	return result;
}

void CtrlVarStorage::clear()
{
	ifVariableToStm.clear();
	whileVariableToStm.clear();
}

