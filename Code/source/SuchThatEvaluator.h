#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>

#include "FollowsEvaluator.h"
#include "FollowsStarEvaluator.h"
#include "ParentEvaluator.h"
#include "ParentStarEvaluator.h"
#include "UsesEvaluator.h"
#include "ModifiesEvaluator.h"
#include "CallsEvaluator.h"
#include "CallsStarEvaluator.h"
#include "NextEvaluator.h"
#include "NextStarEvaluator.h"
#include "AffectsEvaluator.h"
#include "AffectsStarEvaluator.h"

class SuchThatEvaluator
{
public:
	static std::string isTrivial(std::string relation, std::string firstArgument,
		std::string secondArgument);

	static std::unordered_map<std::string, std::vector<std::string>>
		evaluate(std::unordered_map<std::string, std::string> declarations,
			std::string relation, std::string firstArgument, std::string secondArgument);

	static std::unordered_map<std::string, std::vector<std::string>>
		filterType(std::string synonym, std::unordered_map<std::string, std::string> declarations,
			std::unordered_map<std::string, std::vector<std::string>> toBeFiltered);

	static std::unordered_map<std::string, std::vector<std::string>>
		filterType(std::string synonym1, std::string synonym2,
			std::unordered_map<std::string, std::string> declarations,
			std::unordered_map<std::string, std::vector<std::string>> toBeFiltered);
};
