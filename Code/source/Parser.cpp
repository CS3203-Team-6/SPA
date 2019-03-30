#pragma once
#include "Parser.h"

Parser::Parser()
{
	this->pkb = PKB();
}

int Parser::parse(vector<Statement> stmtLst, int parent, string procedure) {
	int prevStmtLine = 0;

	for (size_t i = 0; i < stmtLst.size(); i++) {
		Statement stmt = stmtLst.at(i);

		//Add Statement Type into PKB.
		populateStmtList(stmt);

		int currStmtLine = stmt.getStmtNum();

		//Add Follow and Next relation if is not the very first line or if not an else statement
		if (prevStmtLine != 0 && stmt.getType() != 7) {
			pkb.addFollow(prevStmtLine, currStmtLine);
			pkb.addNext(prevStmtLine, currStmtLine);
		}

		//Add Next relation for IF statement type
		if (stmt.getType() == 6) {
			pkb.addNext(currStmtLine, stmt.getStmtLst().front().getStmtNum());
		}

		//Add Next relation for Else
		if (stmt.getType() == 7) {
			pkb.addNext(currStmtLine, currStmtLine + 1);
		}
		//Add Parent relation if parent is not 0.
		if (parent != 0) {
			pkb.addParent(parent, currStmtLine);
		}

		//Add Next relation for WHILE statment type
		if (stmt.getType() == 5) {
			pkb.addNext(currStmtLine, stmt.getStmtLst().front().getStmtNum());
			pkb.addNext(stmt.getStmtLst().back().getStmtNum(), currStmtLine);
		}

		//Add Procedure - Statement Relationship if Statment is in a Procedure.
		if (!procedure.empty()) {
			//TODO:: Add Procedure containing statement in pkb.
		}

		//Add VariableName, Constants, and Procedure name into PKB.
		populateDesignEntities(stmt, procedure);

		//Update previous statement line.
		prevStmtLine = currStmtLine;
	}
	
	return 0;
}

void Parser::populateDesignEntities(Statement stmt, std::string procedure) {
	std::string stmtString = stmt.getString();
	int stmtLine = stmt.getStmtNum();
	int stmtType = stmt.getType();

	// 1 "ASSIGN", 2 "CALL", 3 "READ", 4 "PRINT", 5 "WHILE", 6 "IF", 7 "ELSE", 8 "PROCEDURE"
	switch (stmtType) {
	case 1: {
		//Assign Statement
		extractAssignEntity(stmtString, stmtLine);
		break;
	}
	case 2:
		//Call Statement
		extractCallEntity(stmtString, stmtLine, procedure);
		break;
	case 3: {
		//ReadStatement
		extractReadEntity(stmtString, stmtLine);
		break;
	}
	case 4: {
		//PrintStatement
		extractPrintEntity(stmtString, stmtLine);
		break;
	}
	case 5: {
		//WhileStatement
		vector<Statement> whileStmtLst = stmt.getStmtLst();
		extractWhileEntity(stmtString, stmtLine, whileStmtLst, procedure);
		break;
	}
	case 6: {
		//IfStatement
		vector<Statement> ifStmtLst = stmt.getStmtLst();
		extractIfEntity(stmtString, stmtLine, ifStmtLst, procedure);
		break;
	}
	case 7: {
		//ElseStmt 
		vector<Statement> elseStmtLst = stmt.getStmtLst();
		extractElseEntity(stmtString, stmtLine, elseStmtLst, procedure);
		break;
	}
	case 8: {		
		//Procedure
		if (!procedure.empty()) {
			throw "Invalid Syntax, Procedure within Procedure.";
		}
		vector<Statement> procStmtLst = stmt.getStmtLst();
		extractProcedureEntity(stmtString, procStmtLst);
	}
	}
}

void Parser::extractAssignEntity(std::string &stmtString, int stmtLine) {
	AssignParser ap;
	std::string modified = ap.getLeft(stmtString);
	std::vector<std::string> usedVariables = ap.getRightVariable(stmtString);
	std::vector<std::string> usedConstants = ap.getRightConstant(stmtString);
	std::string prefixExpression = ap.getPrefixExpression(stmtString);

	//Add Variable and Modify
	pkb.addVariable(modified);
	pkb.addModifiesStm(stmtLine, modified);

	//Add Constants
	for (string constant : usedConstants) {
		pkb.addConstant(constant);
	}

	//Add Variable and Uses.
	for (string variable : usedVariables) {
		pkb.addVariable(variable);
		pkb.addUsesStm(stmtLine, variable);
	}

	pkb.addAssignPattern(stmtLine, modified, prefixExpression);
}

void Parser::extractCallEntity(std::string &stmtString, int stmtLine, std::string procedure) {
	CallParser cp;
	string procedureName = cp.parseCallStmt(stmtString);
	
	//pkb.addCall(procedure, procedureName);
}

void Parser::extractReadEntity(std::string &stmtString, int stmtLine) {
	ReadParser rp;
	string modified = rp.parseReadStmt(stmtString);

	pkb.addModifiesStm(stmtLine, modified);
	pkb.addVariable(modified);
}

void Parser::extractPrintEntity(std::string &stmtString, int stmtLine) {
	PrintParser pp;
	string used = pp.parsePrintStmt(stmtString);

	pkb.addUsesStm(stmtLine, used);
	pkb.addVariable(used);
}

void Parser::extractWhileEntity(std::string &stmtString, int stmtLine, vector<Statement> stmtLst, std::string procedure) {
	WhileParser wp = WhileParser(stmtLine, stmtString, stmtLst, procedure);
	vector<string> constants = wp.getConstants();
	vector<string> variables = wp.getVariables();

	for (string variable : variables) {
		pkb.addVariable(variable);
		pkb.addUsesStm(stmtLine, variable);
	}

	for (string constant : constants) {
		pkb.addConstant(constant);
	}
	
	wp.parseStmtLst();
}

void Parser::extractIfEntity(std::string &stmtString, int stmtLine, vector<Statement> stmtLst, std::string procedure) {
	IfParser ip = IfParser(stmtLine, stmtString, stmtLst, procedure);
	vector<string> constants = ip.getConstants();
	vector<string> variables = ip.getVariables();

	for (string variable : variables) {
		pkb.addVariable(variable);
		pkb.addUsesStm(stmtLine, variable);
	}

	for (string constant : constants) {
		pkb.addConstant(constant);
	}

	ip.parseStmtLst();
}

void Parser::extractElseEntity(std::string &stmtString, int stmtLine, vector<Statement> stmtLst, std::string procedure) {
	ElseParser ep = ElseParser(stmtLine, stmtString, stmtLst, procedure);
	ep.parseStmtLst();
}

void Parser::extractProcedureEntity(std::string &stmtString, vector<Statement> stmtLst) {
	ProcedureParser pp;
	string procName = pp.parseProcName(stmtString);
	pkb.addProc(procName);

	pp.parseStmtLst(stmtLst, procName);
}


void Parser::populateStmtList(Statement stmt) {
	int stmtLine = stmt.getStmtNum();
	int stmtType = stmt.getType();

	// 1 "ASSIGN", 2 "CALL", 3 "READ", 4 "PRINT", 5 "WHILE", 6 "IF", 7 "ELSE", 8 "PROCEDURE"
	switch (stmtType) {
	case 1:
		//AssignStatement
		pkb.addStatement(stmtLine, stmType::assign);
		break;
	case 2:
		//CallStatement
		pkb.addStatement(stmtLine, stmType::call);
		break;
	case 3:
		//ReadStatement
		pkb.addStatement(stmtLine, stmType::read);
		break;
	case 4:
		//PrintStatement
		pkb.addStatement(stmtLine, stmType::print);
		break;
	case 5:
		//WhileStatement
		pkb.addStatement(stmtLine, stmType::whileStm);
		break;
	case 6:
		//IfStatemnt
		pkb.addStatement(stmtLine, stmType::ifStm);
		break;
	}
}