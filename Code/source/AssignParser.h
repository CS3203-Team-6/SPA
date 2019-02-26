#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <regex>
#include <vector>
#include <sstream>

using namespace std;

class AssignParser
{
public:
	std::string parseLeft(std::string statement);
	std::string parseRight(std::string statement);
	string getLeft(string statement);
	vector<string> getRightVariable(string statement);
	vector<string> getRightConstant(string statement);
	string getPostfixExpression(string statement);
	AssignParser();
	~AssignParser();
};
