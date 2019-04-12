#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>

#include "PKB.h"
#include "QueryUtility.h"
#include "SuchThatEvaluator.h"

class WithEvaluator
{
public:
	static std::string isTrivial(std::string left, std::string right);

	static std::unordered_map<std::string, std::vector<std::string>>
		evaluate(std::unordered_map<std::string, std::string> declarations,
			std::string firstArgument, std::string secondArgument);

private:
	static std::unordered_map<std::string, std::vector<std::string>> evaluatePair(
		std::unordered_map<std::string, std::string> declarations,
		std::string attr);

};