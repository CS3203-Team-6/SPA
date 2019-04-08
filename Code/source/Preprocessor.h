#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <algorithm>

#include "Statement.h"

/**** Usage guide
  Preprocessor will contain:
  (1) validate(...): the validator methods to validate statement types (used mainly for testing)
  (2) getProcLst: getter method for the procedure list
  (3) Preprocessor: constructor method to satrt the processing of source code

  Preprocessor:
  input: string representing source code
  output: the object

  getter methods: getProcLst
  input: null
  output: vector<Statement> representing the vector of procedures in the program  

  to use:
  (1) create preprocesser and get procedrue list: Preprocessor(string chunk).getProcLst();
*/

class Preprocessor {
private:
	std::string chunk;
	int size;
	int stopper;
	int stmtNum = 1;
	std::vector<Statement> procLst;
	std::stack<int> ifStmt;
	Statement processProc(int bookmark);
	std::vector<Statement> processLst(int bookmark);
public:
	//only validators, atring trimmer, getter and Processer is public
	static int validateSemicolon(std::string s);
	static int validateCurvedBrackets(std::string s);
	static int validateProc(std::string s);
	static std::string trim(std::string s);
	std::vector<Statement> getProcLst();
	Preprocessor(std::string chunk1);
	void process();
};

