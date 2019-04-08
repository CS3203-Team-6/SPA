#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <regex>
#include <vector>
#include <sstream>
#include "StringUtil.h"

using namespace StringUtil;

//Splits the string based on the given limiter and returns a vector of substrings.
std::vector<std::string> StringUtil::split(std::string stmt, char delimiter)
{
	std::vector<std::string> v;
	std::string::size_type i = 0;
	//Find first occurence of delimiter.
	std::string::size_type j = stmt.find(delimiter);

	//Loop while there is still statement to parse.
	while (j != std::string::npos) {
		//Push result into vector.
		v.push_back(stmt.substr(i, j - i));
		//current j is ignore as it is the delimiter. We update i to after delimiter.
		i = ++j;
		//find the new j.
		j = stmt.find(delimiter, j);

		
	}

	if (j == std::string::npos)
		v.push_back(stmt.substr(i, stmt.length()));

	return v;
}

//Trims the front and back of the String with the given whitespace.
std::string StringUtil::trim(std::string& str, std::string& whitespace)
{
	int strStart = str.find_first_not_of(whitespace);
	int strEnd = str.find_last_not_of(whitespace);

	if (strStart == std::string::npos) {
		return "";
	}

	int strLength = strEnd - strStart + 1;
	return str.substr(strStart, strLength);

}

//Removes all the whitespace in the given string
std::string StringUtil::removeAllWhitespaces(std::string s) {
	s.erase(remove_if(s.begin(), s.end(), isspace), s.end());
	return s;
}