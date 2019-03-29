#pragma once
#include "PKB.h"

/* Design Extractor performs Design Entitiy Extraction for the following relations
 * 1) Follow*
 * 2) Parent*
 * 3) While/IfStatement Modify variables in Containing Statement
 * 4) While/IfStatemnt Using variables in Containing Statement
 * 5) Procedure Modify variables in statement list.
 * 6) Procedure Using variables in statement list.
 *
 * IMPORTANT NOTE: Call DesignExtractor only after populating PKB with the basic relations
 * 
 * To use DesignExtractor:
 * 1) call extractDesigns() with the intended target PKB as the argument.
 * 
 */
class DesignExtractor {
public:
	static void extractDesigns(PKB storage);

private:
	static PKB pkb;
	static void verifyCalledProceduresPresence();
	static void verifyAbsenceOfCyclicity();
	static void processFollowStar();
	static void processParentStar();
	static vector<string> topologicalSortProcedures();
	static void DFSRecursive(std::string procedure, unordered_set<std::string>& visitedProcedures, vector<std::string>& sortedProcedures);
	static void processAdvancedUsesAndModifies(std::vector<std::string> sortedProcedures);
	static void processUsesCalls(std::string procedure);
	static void processModifiesCalls(std::string procedure);
	static void processUsesProcedures(std::string procedure);
	static void processUsesContainers(std::string procedure);
	static void processModifiesProcedures(std::string procedure);
	static void processModifiesContainers(std::string procedure);
};