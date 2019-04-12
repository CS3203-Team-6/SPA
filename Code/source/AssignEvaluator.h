#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>

#include "PKB.h"
#include "QueryUtility.h"
#include "ExpressionUtil.h"


class AssignEvaluator
{
public:
	static std::unordered_map<std::string, std::vector<std::string>> evaluate(
		std::unordered_map<std::string, std::string> declarations,
		std::string patternSynonym, std::string firstArgument, std::string secondArgument);
};
