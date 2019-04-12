#include "DesignExtractor.h"
#include "Statement.h"

PKB DesignExtractor::pkb;

/* 
 * The main public function that calls other function to populate different design entities in the PKB.
 */
void DesignExtractor::extractDesigns() {
	pkb = PKB();

	//Verification
	verifyCalledProceduresPresence();

	//Statement-Statement Relationships
	processFollowStar();
	processParentStar();

	//Statement/Procedure - Variable Relationships
	std::vector<std::string> sortedProcedures = topologicalSortProcedures();
	processAdvancedUsesAndModifies(sortedProcedures);
	processCallsStar(sortedProcedures);
	extractNextEntity();
}

/*
 * Checks if the procedures found in Call Statements are found inside the procedure list.
 * Iterates through the list of Call Statements and check if it exist in procedure list.
 */
void DesignExtractor::verifyCalledProceduresPresence() {
	std::unordered_set<std::string> calledProcedures = pkb.getAllCallees();
	std::unordered_set<std::string> procedureList = pkb.getProcList();
	for (std::string calledProcedure : calledProcedures) {
		bool present = false;
		for (std::string procedure : procedureList) {
			if (calledProcedure == procedure) {
				present = true;
				break;
			}
		}
		if (!present) {
			throw "A procedure that was called does not exist : " + calledProcedure;
		}
	}
}

/*
 * Processes the Follow* Design Entitiy in the SPA requirement.
 */
void DesignExtractor::processFollowStar() {

	int stmtNum = pkb.getTotalStmNo();

	//Process stmt list s where Follow*(index, s) is true
	for (int i = stmtNum; i >= 1; i--) {
		int currStmt = i;
		int directFollowStm = pkb.getFollower(currStmt);
		//Has a Next Stm
		if (directFollowStm != 0) {
			std::unordered_set<int> followedByStar = pkb.getAllFollowing(directFollowStm);
			followedByStar.insert(directFollowStm);
			pkb.setAllFollowing(currStmt, followedByStar);
		}
	}

	//Process stmt list s where Follow*(s, index) is true
	for (int i = 1; i <= stmtNum; i++) {
		int currStmt = i;
		int directPrvStm = pkb.getStmFollowedBy(currStmt);

		//If have Prev Stm
		if (directPrvStm != 0) {
			std::unordered_set<int>  followStar = pkb.getAllFollowedBy(directPrvStm);
			followStar.insert(directPrvStm);
			pkb.setAllFollowedBy(currStmt, followStar);
		}
	}

}

/*
 * Processes the Parent* Design Entitiy in the SPA requirement.
 */
void DesignExtractor::processParentStar() {

	int stmtNum = pkb.getTotalStmNo();

	//Process stmt list s where Parent*(s, index) is true.
	for (int i = 1; i <= stmtNum; i++) {
		int currStmt = i;
		int directParentStm = pkb.getParent(i);

		//If have parent,
		if (directParentStm != 0) {
			std::unordered_set<int> ancestors = pkb.getAncestors(directParentStm);
			ancestors.insert(directParentStm);
			pkb.setAncestors(i, ancestors);
		}
	}

	//Process stmt list s where Parent*(index, s) is true.
	for (int i = stmtNum; i >= 1; i--) {
		int currStmt = i;
		std::unordered_set<int> directChildStm = pkb.getChildren(i);

		std::unordered_set<int> allDescendents;
		//Has a Child Stm
		if (!directChildStm.empty()) {
			for (int childStm : directChildStm) {
				std::unordered_set<int> descendents = pkb.getDescendants(childStm);
				for (int descendent : descendents) {
					allDescendents.insert(descendent);
				}
				allDescendents.insert(childStm);
			}
			pkb.setDescendants(i, allDescendents);
		}
	}
}

/*
 * This function performs a topological sort on the unorderedList of procedures.
 * It returns a vector of procedures in order such that procedures on a lower index will never call
 * a procedure with a higher index.
 * For example if TestA calls TestB, and TestB calls TestC, and Test A calls TestC.
 * A valid toposort result would return {TestC, TestB, TestA} in a vector.
 */
std::vector<std::string> DesignExtractor::topologicalSortProcedures() {

	std::unordered_set<std::string> procList = pkb.getProcList();
	int procListSize = procList.size();

	std::unordered_set<std::string> visitedProcedures;
	std::vector<std::string> sortedProcedures;
	std::unordered_set<std::string> pathVisitedProcedure;

	for (std::string procedure : procList) {
		//Try finding if the procedure has already been visited.
		std::unordered_set<std::string>::const_iterator exist = visitedProcedures.find(procedure);

		//If we have not visited this procedure, visit it.
		if (exist == visitedProcedures.end()) {
			DFSRecursive(procedure, visitedProcedures, sortedProcedures, pathVisitedProcedure);
		}
	}
	return sortedProcedures;
}

/*
 * A Recursive Function to perform Depth First Search of a procedure calling.
 * This DFSRecursive Function requires 4 inputs.
 * 1) procedure that is being visited.
 * 2) vistedProcedures: The list of procedures that have been visited in a DFS.
 * 3) sortedProcedures: The list of procedures that in a toposort order.
 * 4) pathVisitedProcedure: The list of procedures that have been called in the current DFSRecursive Stack.
 */
void DesignExtractor::DFSRecursive(std::string procedure,
								   std::unordered_set<std::string> &visitedProcedures,
								   std::vector<std::string> &sortedProcedures,
								   std::unordered_set<std::string> pathVisitedProcedure) {
	//Marks procedure as visited
	visitedProcedures.insert(procedure);

	//Add visited procedure to currentCallPath
	pathVisitedProcedure.insert(procedure);

	//Get neighbouring procedures.
	std::unordered_set<std::string> adjacentProcedures = pkb.getCallee(procedure);

	//For each neighbouring procedure
	for (std::string callee : adjacentProcedures) {

		//Check if neighbouring procedure is in currentCallPath
		std::unordered_set<std::string>::const_iterator currPath = pathVisitedProcedure.find(callee);

		//If neighbouring procedure is in currentCallPath, detect cyclic Calls.
		if (currPath != pathVisitedProcedure.end()) {
			throw "Cyclical call detected at Procedure : " + procedure;
		}

		//Check if neighbouring procedure has been visited
		std::unordered_set<std::string>::const_iterator exist = visitedProcedures.find(callee);

		//If neighbouring procedure has not been visited, visit it.
		if (exist == visitedProcedures.end()) {
			DFSRecursive(callee, visitedProcedures, sortedProcedures, pathVisitedProcedure);
		}
	}
	//Add procedure to the sortedProcedures.
	sortedProcedures.push_back(procedure);

	//Remove procedure from current Call path.
	pathVisitedProcedure.erase(procedure);
}

/* 
 * Performs population of Uses and Modifies for Calls,
 * If/While Container Statements and Procecedures starting from the leaf node procedures.
 */
void DesignExtractor::processAdvancedUsesAndModifies(std::vector<std::string> sortedProcedures) {

	for (std::string procedure : sortedProcedures) {
		//Populate Call Statement Uses/Modifies
		processUsesCalls(procedure);
		processModifiesCalls(procedure);

		//Populate Container Statement (While/If) Uses/Modifies
		processUsesContainers(procedure);
		processModifiesContainers(procedure);

		//Populate Procedure Uses/Modifies
		processUsesProcedures(procedure);
		processModifiesProcedures(procedure);
	}
}

/*
 * Populates the Uses of Call Statements in SIMPLE
 */
void DesignExtractor::processUsesCalls(std::string procedure) {
	std::vector<int> procedureStm = pkb.getStmList(procedure);

	for (size_t i = 0; i < procedureStm.size(); i++) {
		int currLine = procedureStm.at(i);
		stmType type = pkb.getStmType(currLine);

		if (type == stmType::call) {
			std::string procedure = pkb.getProcCalledBy(currLine);
			std::unordered_set<std::string> usedVars = pkb.getVarUsedByProc(procedure);

			for (std::string var : usedVars) {
				pkb.addUsesStm(currLine, var);
			}
		}
	}
}

/*
 * Populates the Uses of Modifies Statements in SIMPLE
 */
void DesignExtractor::processModifiesCalls(std::string procedure) {
	std::vector<int> procedureStm = pkb.getStmList(procedure);

	for (size_t i = 0; i < procedureStm.size(); i++) {
		int currLine = procedureStm.at(i);
		stmType type = pkb.getStmType(currLine);

		if (type == stmType::call) {
			std::string procedure = pkb.getProcCalledBy(currLine);
			std::unordered_set<std::string> modifiedVars = pkb.getVarModifiedByProc(procedure);

			for (std::string var : modifiedVars) {
				pkb.addModifiesStm(currLine, var);
			}
		}
	}
}

/*
 * Processes the the variables that are MODIFIED in Containing Statement so that 
 * the While/If Statement also MODIFY these variables.
 */
void DesignExtractor::processModifiesContainers(std::string procedure) {

	std::vector<int> procedureStm = pkb.getStmList(procedure);

	for (int i = procedureStm.size() - 1; i >= 0; i--) {
		int currLine = procedureStm.at(i);
		std::unordered_set<int> descendents = pkb.getDescendants(currLine);
		for (int descendent : descendents) {
			std::unordered_set<std::string> modifiedList = pkb.getVarModifiedByStm(descendent);
			for (std::string modifiedVar : modifiedList) {
				pkb.addModifiesStm(currLine, modifiedVar);
			}
		}
	}
}

/*
 * Processes the the variables that are USED in Containing Statement so that the While/If Statement
 * also USE these variables.
 */
void DesignExtractor::processUsesContainers(std::string procedure) {

	std::vector<int> procedureStm = pkb.getStmList(procedure);

	for (int i = procedureStm.size() - 1; i >= 0; i--) {
		int currLine = procedureStm.at(i);
		std::unordered_set<int> descendents = pkb.getDescendants(currLine);
		for (int descendent : descendents) {
			std::unordered_set<std::string> usedList = pkb.getVarUsedByStm(descendent);
			for (std::string usedVariable : usedList) {
				pkb.addUsesStm(currLine, usedVariable);
			}
		}
	}
}

/*
 * Processes the variables that are USED in a procedure,
 * by checking the use Relation for all Statements belonging to that procedure.
 */
void DesignExtractor::processUsesProcedures(std::string procedure)
{
	std::vector<int> procedureStm = pkb.getStmList(procedure);

	for (size_t i = 0; i < procedureStm.size(); i++) {
		int currLine = procedureStm.at(i);
		std::unordered_set<std::string> usedList = pkb.getVarUsedByStm(currLine);

		for (std::string usedVar : usedList) {
			pkb.addUsesProc(procedure, usedVar);
		}
	}

}

/*
 * Processes the variables that are MODIFIED in a procedure,
 * by checking the modifying Relation for all Statements belonging to that procedure.
 */
void DesignExtractor::processModifiesProcedures(std::string procedure)
{
	std::vector<int> procedureStm = pkb.getStmList(procedure);

	for (size_t i = 0; i < procedureStm.size(); i++) {
		int currLine = procedureStm.at(i);
		std::unordered_set<std::string> modifiedList = pkb.getVarModifiedByStm(currLine);

		for (std::string modifiedVar : modifiedList) {
			pkb.addModifiesProc(procedure, modifiedVar);
		}
	}
}

/*
 * Processes the Call* Relationship
 */
void DesignExtractor::processCallsStar(std::vector<std::string> sortedProcedures) {
	//Process procedure list s where Calls*(s, procedure) is true
	for (int i = sortedProcedures.size() - 1; i >= 0; i--) {
		std::string procedure = sortedProcedures.at(i);
		std::unordered_set<std::string> callerList = pkb.getCaller(procedure);

		std::unordered_set<std::string> allAncestorCallers;

		//If Someone called this procedure
		if (!callerList.empty()) {
			for (std::string caller : callerList) {
				std::unordered_set<std::string> ancestorCallersList = pkb.getCallAnc(caller);
				for (std::string ancestorCaller : ancestorCallersList) {
					allAncestorCallers.insert(ancestorCaller);
				}
				allAncestorCallers.insert(caller);
			}
			pkb.setCallAnc(procedure, allAncestorCallers);
		}
	}

	//Process procedure list s where Calls*(procedure, s) is true.
	for (size_t i = 0; i < sortedProcedures.size(); i++) {
		std::string procedure = sortedProcedures.at(i);
		std::unordered_set<std::string> calleeList = pkb.getCallee(procedure);

		std::unordered_set<std::string> allDescendentsCallee;

		//If this procedure called something.
		if (!calleeList.empty()) {
			for (std::string callee : calleeList) {
				std::unordered_set<std::string> descendentsCalleeList = pkb.getCallDesc(callee);
				for (std::string descendentCallee : descendentsCalleeList) {
					allDescendentsCallee.insert(descendentCallee);
				}
				allDescendentsCallee.insert(callee);
			}
			pkb.setCallDesc(procedure, allDescendentsCallee);
		}
	}
}

//get all if Statments and extrct Next entity
void DesignExtractor::extractNextEntity() {

	std::unordered_set<int> ifStmtList = pkb.getIfStms();
	for (int stmt : ifStmtList) {
		//pair<int,int> first int is last stmt of then block, second int is last stmt of else block
		std::pair<int, int> ifLastStmt = pkb.getIfLastStms(stmt);
		int parent = pkb.getParent(stmt);
		//if if statment have follower, next(last stmt of then block and else block, follower)
		if (pkb.getFollower(stmt) != 0) {
			pkb.addNext(ifLastStmt.first, pkb.getFollower(stmt));
			pkb.addNext(ifLastStmt.second, pkb.getFollower(stmt));
		}
		else {
			if (parent != 0) {
				//if if statment have a while statment as parent, 
				//next(last stmt of then block and else block, while statment)
				if (pkb.getStmType(parent) == whileStm) {
					pkb.addNext(ifLastStmt.first, parent);
					pkb.addNext(ifLastStmt.second, parent);
				}
				//if if statment have a if statment as parent, 
				if (pkb.getStmType(parent) == ifStm) {
					//next(last stmt of then block and else block, follower of parent)
					if (pkb.getFollower(parent) != 0) {
						pkb.addNext(ifLastStmt.first, pkb.getFollower(parent));
						pkb.addNext(ifLastStmt.second, pkb.getFollower(parent));
					}
					//if parent if statement have no follower 
					else {
						//check the parent of parent if it is a while statment
						//next(last stmt of then block and else block, while statment)
						if (pkb.getStmType(pkb.getParent(parent)) == whileStm) {
							pkb.addNext(ifLastStmt.first, pkb.getParent(parent));
							pkb.addNext(ifLastStmt.second, pkb.getParent(parent));
						}
					}
				}
			}
		}
	}
}
