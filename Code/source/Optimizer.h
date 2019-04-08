#pragma once

#include <vector>
#include <string>
#include <queue>
#include <functional>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <map>
#include "OptimizerUtility.h"

/** Usage Guide
  This class optimises the evaluator by chaining the clauses together such that every other clause will have at least 1 clause evaluated
  
  // To use:
  Optimizer op = Optimizer(st, w, p, s);
  op.groupClause();
  
  // To get the groups of trivial/non-trivial clauses:
  std::vector<std::vector<std::pair<std::string, std::pair<std::string, std::string>>>> trivial = op.getTrivial();
  std::vector<std::vector<std::pair<std::string, std::pair<std::string, std::string>>>> nontrivial = op.getNonTrivial();
*/

class Optimizer {
private:
	// these variables will be used as reference and will NOT be altered after creation
	std::vector<std::pair<std::string, std::pair<std::string, std::string>>> suchThatClauses;
	std::vector<std::pair<std::string, std::pair<std::string, std::string>>> withClauses;
	std::vector<std::pair<std::string, std::pair<std::string, std::string>>> patternClauses;
	int psize; //patternClauseSize
	int wsize; //withClauseSize
	std::unordered_set<std::string> selectClause;
	std::unordered_map<std::string, std::string> declarations;
	std::unordered_map<std::string, std::priority_queue<std::pair<int,int>>> synMap;
	std::unordered_map<int, std::vector<std::string>> clMap;
	// these variables are used for storage of results
	std::vector<std::vector<std::pair<std::string, std::pair<std::string, std::string>>>> trivial;
	std::vector<std::vector<std::pair<std::string, std::pair<std::string, std::string>>>> nontrivial;
	std::vector<std::pair<std::string, std::pair<std::string, std::string>>> graph;
	// sets are used for bookkeeping to prevent infinite calls
	std::unordered_set<std::string> synSet;
	std::set<std::pair<int,int>, std::greater<std::pair<int, int>>> clSet;
	// processing functions - refer to .cpp for more details
	void groupByClauseType(int t);
	void createMaps(std::vector<std::string> synLst, std::pair<int,int> cl);
	bool mapClauses(std::pair<int,int> cl, bool trivial);
	bool mapSynonym(std::string syn, bool trivial);
	std::pair<std::string, std::pair<std::string, std::string>> getClause(int cl);
public:
	//Constructor
	Optimizer(std::vector<std::pair<std::string, std::pair<std::string, std::string>>> st,
		std::vector<std::pair<std::string, std::pair<std::string, std::string>>> w,
		std::vector<std::pair<std::string, std::pair<std::string, std::string>>> p,
		std::unordered_set<std::string> s, std::unordered_map<std::string, std::string> d);
	//Statement to process
	void groupClause();
	//Getter Functions
	std::vector<std::vector<std::pair<std::string, std::pair<std::string, std::string>>>> getTrivial();
	std::vector<std::vector<std::pair<std::string, std::pair<std::string, std::string>>>> getNonTrivial();
	//Priority and Syn Extractor (For Unit Testing -- this should otherwise be kept private)
	std::pair<int, std::vector<std::string>> extractSuchThatSyn(int index);
	std::pair<int, std::vector<std::string>> extractWithSyn(int index);
	std::pair<int, std::vector<std::string>> extractPatternSyn(int index);
};
