#pragma once

#include<stdio.h>
#include<vector>

#include "PKB.h"
#include "Statement.h"
#include "AssignParser.h"
#include "CallParser.h"
#include "ReadParser.h"
#include "PrintParser.h"
#include "WhileParser.h"
#include "IfParser.h"
#include "ElseParser.h"
#include "ProcedureParser.h"

class Parser
{

public:

	Parser();

	int parse(std::vector<Statement> stmtLst, int parent, std::string procedure);

	void populateNextEntity(int prevStmtLine, Statement & stmt, int currStmtLine, int parent, std::string procedure);

private:

	void populateDesignEntities(Statement stmt, std::string procedure);

	void extractAssignEntity(std::string &stmtString, int stmtLine);

	void extractCallEntity(std::string & stmtString, int stmtLine, std::string procedure);

	void extractReadEntity(std::string & stmtString, int stmtLine);

	void extractPrintEntity(std::string & stmtString, int stmtLine);

	void extractWhileEntity(std::string & stmtString, int stmtLine,
							std::vector<Statement> stmtLst, std::string procedure);

	void extractIfEntity(std::string & stmtString, int stmtLine,
						 std::vector<Statement> stmtLst, std::string procedure);

	void extractElseEntity(std::string & stmtString, int stmtLine,
						   std::vector<Statement> stmtLst, std::string procedure);

	void extractProcedureEntity(std::string & stmtString, std::vector<Statement> stmtLst);

	void populateStmtList(Statement stmt, std::string procedure);

	PKB pkb;
};