#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>

#include "PKB.h"
#include "QueryUtility.h"

class UsesEvaluator
{
public:
	static std::string isTrivial(std::string firstArgument, std::string secondArgument);
	static std::unordered_map<std::string, std::vector<std::string>> evaluate(
		std::unordered_map<std::string, std::string> declarations,
		std::string firstArgument, std::string secondArgument);
};
