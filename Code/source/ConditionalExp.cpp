#include <iostream>
#include <string>
#include <regex>
#include <sstream>
#include "ConditionalExp.h"
#include "LexicalToken.h"

using namespace ConditionalExp;

std::string str; 
std::regex formular("\\s*\\(*(!?[\\s\\(\!]*(?:([a-zA-Z]+[\\da-zA-Z]*)|(\\d+)))\\s*\\)*\\s*([\\+\\-\\*/%<>]|[<>!=]=)[\\(\\)\\s]*(([a-zA-Z]+[\\da-zA-Z]*)|(\\d+))[\\(\\s\\)]*(([\\(\\s\\)]*([\\+\\-\\*/%<>]|[<>!=]=)[\\(\\s\\)]*(([a-zA-Z]+[\\da-zA-Z]*)|(\\d+)))*[\\(\\s\\)]*)");


bool ConditionalExp::verifyConditionalExp(std::string statement) {
	std::vector<std::string> wordVector;
	str = statement;
	std::stringstream stringStream(str);
	std::string line;
	int count = 0;
	for (std::string::iterator it = str.begin(); it != str.end(); ++it) {
		if (*it == '(') {
			count++;
		}
		else if (*it == ')') {
			count--;
		}
		if (count < 0)
			return false;
	}
	if (count != 0) {
		return false;
	}
	while (std::getline(stringStream, line)) {
		std::size_t prev = 0, pos;
		while ((pos = line.find_first_of("&|", prev)) != std::string::npos)
		{
			if (pos > prev)
				wordVector.push_back(line.substr(prev, pos - prev));
			prev = pos + 1;
		}
		if (prev < line.length()) {
			wordVector.push_back(line.substr(prev, std::string::npos));
		}
	}
	//_________________________________________________________________
	//using Regex to check each seperated segments
	for (std::vector<std::string>::const_iterator i = wordVector.begin(); i != wordVector.end(); ++i) {
		std::string toCheck = *i;
		std::smatch wholeMatches;
		if (std::regex_match(toCheck, wholeMatches, formular)) {
		}
		else {
			return false;
		}
	}
	return true;
}

std::vector<std::string> ConditionalExp::getVariables() {
	std::vector<std::string> variables;
	std::vector<std::string> variableVector;
	std::stringstream stringStream2(str);
	std::string line2;
	std::string line3;
	std::locale loc;

	while (std::getline(stringStream2, line2))
	{
		std::size_t prev = 0, pos;
		while ((pos = line2.find_first_of(" +-*/()<>!=&|%", prev)) != std::string::npos)
		{
			if (pos > prev)
				variableVector.push_back(line2.substr(prev, pos - prev));
			prev = pos + 1;
		}
		if (prev < line2.length()) {
			variableVector.push_back(line2.substr(prev, std::string::npos));
		}
	}
	for (std::vector<std::string>::const_iterator i = variableVector.begin(); i != variableVector.end(); ++i) {
		line3 = *i;
		if (isdigit(line3[0], loc))
		{
			int year;
			std::stringstream(line3) >> year;
		}
		else {
			//do lexicaltoken check on the variable
			if (LexicalToken::verifyName(line3)) {
				//if variable is not already in the variable vector add it in
				if (find(variables.begin(), variables.end(), line3) == variables.end()) {
					variables.push_back(line3);
				}
				
			}
		}
	}
	return variables;
}

std::vector<std::string> ConditionalExp::getConstants() {
	std::vector<std::string> constants;
	std::vector<std::string> variables;
	std::vector<std::string> variableVector;
	std::stringstream stringStream2(str);
	std::string line2;
	std::string line3;
	std::locale loc;

	while (std::getline(stringStream2, line2))
	{
		std::size_t prev = 0, pos;
		while ((pos = line2.find_first_of(" +-*/()<>!=&|%", prev)) != std::string::npos)
		{
			if (pos > prev)
				variableVector.push_back(line2.substr(prev, pos - prev));
			prev = pos + 1;
		}
		if (prev < line2.length())
			variableVector.push_back(line2.substr(prev, std::string::npos));
	}
	for (std::vector<std::string>::const_iterator i = variableVector.begin(); i != variableVector.end(); ++i) {
		line3 = *i;
		if (isdigit(line3[0], loc))
		{
			int year;
			std::stringstream(line3) >> year;
			//if constant is not in the constant vector add it in
			if (find(constants.begin(), constants.end(), line3) == constants.end()) {
				constants.push_back(line3);
			}
		}
	}
	return constants;
}