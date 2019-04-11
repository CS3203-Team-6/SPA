#include "Optimizer.h"

/* 
* Constructor function
*/
Optimizer::Optimizer(
	std::vector<std::pair<std::string, std::pair<std::string, std::string>>> st,
	std::vector<std::pair<std::string, std::string>> w,
	std::vector<std::pair<std::string, std::pair<std::string, std::string>>> p, 
	std::vector<std::string> s, std::unordered_map<std::string, std::string> d) {
	suchThatClauses = st;
	withClauses = w;
	patternClauses = p;
	selectClause = s;
	declarations = d;
	//internal reference of clauses will follow: p, w, st
	psize = patternClauses.size();
	wsize = withClauses.size();
}

/*
* Main processing function
*/
void Optimizer::groupClause() {
	//extract synonyms and create the syn-cl and cl-syn map
	int clauseSize = psize + wsize + suchThatClauses.size();
	for (int i = 0; i < clauseSize; i++) {
		groupByClauseType(i);
	}
	//while loop to order non-synonym clauses
	std::pair<int,int> cl;
	while (!nonSynPQ.empty()) { //
		cl = nonSynPQ.top();
		nonsynonym.push_back(getClause(cl.second));
		nonSynPQ.pop();
	}
	bool isTrivial;
	//while loop to chain clauses (DFS)
	while (!clSet.empty()) {
		//function for the more efficient clause? get from PKB statistics or use pq
		cl = *clSet.begin(); //rn use the first in set
		isTrivial = mapClauses(cl, true);
		if (isTrivial) {
			trivial.push_back(graph);
		} else {
			nontrivial.push_back(graph);
		}
		graph.clear();
	}
}

/*
* Creates a hashmap of syn-clause and clause-syn, which will be used for graph traversal
*/
void Optimizer::groupByClauseType(int i) {
	std::pair<int,std::vector<std::string>> prioritySyn;
	//clauses are represented internally by int, in the order pattern, with, st
	if (i >= (wsize + psize)) {
		prioritySyn = extractSuchThatSyn(i - wsize - psize);
		if (prioritySyn.first != 0) {
			createMaps(prioritySyn.second, std::make_pair(prioritySyn.first, i));
		}
	} else if (i >= psize) {
		prioritySyn = extractWithSyn(i - psize);
		if (prioritySyn.first != 0) {
			createMaps(prioritySyn.second, std::make_pair(prioritySyn.first, i));
		}
	} else {
		prioritySyn = extractPatternSyn(i);
		if (prioritySyn.first != 0) {
			createMaps(prioritySyn.second, std::make_pair(prioritySyn.first, i));
		}
	}
}

/*
* Creates syn-cl and cl-syn maps 
*/
void Optimizer::createMaps(std::vector<std::string> synLst, std::pair<int,int> cl) {
	clMap[cl.second] = synLst;
	clSet.insert(cl);
	for (std::vector<std::string>::iterator it = synLst.begin(); it != synLst.end(); ++it) {
		if (synMap.find(*it) == synMap.end()) {
			synMap[*it] = std::priority_queue<std::pair<int,int>>();
			synSet.insert(*it);
		} 
		synMap[*it].push(cl);
	}
}

/* 
* Chains synonyms and clauses together
* Input: clause, isTrivial
* Output: isTrivial
* Bookeeping: as this is DFS, a synSet and clSet is used for bookkeeping
  i.e. once a clause/synonym is accessed, no other synonym/clause can access it
  As the for loop is implemented, the algorithm is guaranteed to access all synonyms of the clause and vice versa
* Note: The actual clause, rather than the internal reference, is added to the results graph
*/
bool Optimizer::mapClauses(std::pair<int,int> cl, bool isTrivial) {
	std::pair<std::pair<std::string, std::string>, std::pair<std::string, std::string>> clause;
	clause = getClause(cl.second);
	graph.push_back(clause);
	clSet.erase(cl); //clause removed from bookkeeping sets
	for (std::vector<std::string>::iterator it = clMap[cl.second].begin(); it != clMap[cl.second].end(); ++it) {
		if (synSet.find(*it) != synSet.end()) { //check if synonym is already (partially) processed
			isTrivial = mapSynonym(*it, isTrivial);
		}
	}
	return isTrivial;
}

/*
* Chains synonyms and clauses together
* Input: synonym, isTrivial
* Output: isTrivial
* Bookeeping: as this is DFS, a synSet and clSet is used for bookkeeping
  i.e. once a clause/synonym is accessed, no other synonym/clause can access it
  As the for loop is implemented, the algorithm is guaranteed to access all synonyms of the clause and vice versa
* Note: Group is assumed to be trivial until a select synonym matches
*/
bool Optimizer::mapSynonym(std::string syn, bool isTrivial) {
	//check if syn is part of select if group is trivial
	if (isTrivial && (std::find(selectClause.begin(), selectClause.end(), syn) != selectClause.end())) {
		isTrivial = false;
	}
	synSet.erase(syn); //synonym removed from bookkeeping sets
	while (!synMap[syn].empty()) {
		std::pair<int,int> cl = synMap[syn].top();
		synMap[syn].pop();
		if (clSet.find(cl) != clSet.end()) { //check if clause is already processed
			isTrivial = mapClauses(cl, isTrivial);
		}
	}
	return isTrivial;
}

/*
* To extract synonym from the such that clauses
* Input: index of such that clause to be examined
* Output: int (priority of clause), and vector<string> (vector of synonyms)
* If clause has no synonyms --> add to nonsynonym
* Else give priority and synLst for chaining
*/
std::pair<int, std::vector<std::string>> Optimizer::extractSuchThatSyn(int index) {
	std::vector<std::string> synLst;
	std::string r = suchThatClauses.at(index).first;
	std::string f = suchThatClauses.at(index).second.first;
	std::string s = suchThatClauses.at(index).second.second;
	int tmp1 = OptimizerUtility::getSuchThatEntityType(f);
	int tmp2 = OptimizerUtility::getSuchThatEntityType(s);
	if (tmp1 != 1 && tmp2 != 1) { //if nonsyn
		nonSynPQ.push(std::make_pair(OptimizerUtility::getNoSynSuchThatPriority(r, tmp1 + tmp2), index + wsize + psize));
		tmp1 = 0;
	} else {
		int synNum = 0;
		if (tmp1 > 0) {
			synNum++;
		}
		if (tmp2 > 0) {
			synNum++;
		}
		if (tmp1 == 1 && tmp2 == 1) {
			tmp1 = OptimizerUtility::getSynonymPriority(declarations[f]);
			tmp2 = OptimizerUtility::getSynonymPriority(declarations[s]);
			if (tmp1 < tmp2) {
				synLst.push_back(s);
				synLst.push_back(f);
			} else {
				synLst.push_back(f);
				synLst.push_back(s);
			}
		} else if (tmp1 == 1) {
			synLst.push_back(f);
		} else if (tmp2 == 1) {
			synLst.push_back(s);
		}
		tmp1 = OptimizerUtility::getSuchThatPriority(r, synNum);
	}
	return std::make_pair(tmp1, synLst);
}

/*
* To extract synonym from the pattern clauses
* Input: index of pattern clause to be examined
* Output: int (priority of clause), and vector<string> (vector of synonyms)
* Gives priority and synLst for chaining
*/
std::pair<int, std::vector<std::string>> Optimizer::extractPatternSyn(int index) {
	std::vector<std::string> synLst;
	std::string syn = patternClauses.at(index).first;
	std::string f = patternClauses.at(index).second.first;
	std::string s = patternClauses.at(index).second.second;
	int tmp1 = OptimizerUtility::getPatternEntityType1(f);
	//get from declaration --> syn type of syn
	std::string type = declarations[syn];
	int tmp2 = 0;
	if (type.compare("assign") == 0) {
		tmp2 = OptimizerUtility::getPatternEntityType2(s);
		if (tmp1 == 1) {
			synLst.push_back(f);
		}
		synLst.push_back(syn);
	} else {
		synLst.push_back(syn);
		if (tmp1 == 1) {	
			synLst.push_back(f);
		}
	}
	int p = OptimizerUtility::getPatternPriority(type, tmp1, tmp2);
	return std::make_pair(p, synLst);
}

/*
* To extract synonym from the with clauses
* Input: index of with clause to be examined
* Output: int (priority of clause), and vector<string> (vector of synonyms)
* If clause has no synonyms --> add to nonsynonym
* Else give priority and synLst for chaining
*/
std::pair<int, std::vector<std::string>> Optimizer::extractWithSyn(int index) {
	std::vector<std::string> synLst;
	std::string f = withClauses.at(index).first;
	std::string s = withClauses.at(index).second;
	std::string syn;
	int synNum = 0;
	int tmp = 0;
	bool tmp1 = OptimizerUtility::isWithSynonym(f);
	bool tmp2 = OptimizerUtility::isWithSynonym(s);
	if (tmp1) {
		syn = f.substr(0, f.find("."));
		tmp = OptimizerUtility::getSynonymPriority(declarations[syn]);
		synLst.push_back(syn);
		synNum++;
	} else if (isProg_line(f)) {
		tmp = 1;
		synLst.push_back(f);
		synNum++;
	}
	if (tmp2) {
		syn = s.substr(0, s.find("."));
		if (OptimizerUtility::getSynonymPriority(declarations[syn]) > tmp) {
			synLst.insert(synLst.begin(), syn);
		} else {
			synLst.push_back(syn);
		}
		synNum++;
	} else if (isProg_line(s)) {
		synLst.push_back(s);
		synNum++;
	}
	if (synNum == 0) {
		nonSynPQ.push(std::make_pair(9, index + psize));
	} else {
		tmp = OptimizerUtility::getWithPriority(synNum);
	}
	return std::make_pair(tmp, synLst);
}

/* 
* Checks whether synonym is prog_line for processing of with clause
*/
bool Optimizer::isProg_line(std::string str) {
	bool res = false;
	if (declarations.find(str) != declarations.end()) {
		res = declarations[str] == "prog_line";
	}
	return res;
}

/* Getter functions for retrieving clauses */
std::vector<std::vector<std::pair<std::pair<std::string, std::string>, std::pair<std::string, std::string>>>> Optimizer::getTrivial() {
	return trivial;
}
std::vector<std::vector<std::pair<std::pair<std::string, std::string>, std::pair<std::string, std::string>>>> Optimizer::getNonTrivial() {
	return nontrivial;
}
std::vector<std::pair<std::pair<std::string, std::string>, std::pair<std::string, std::string>>> Optimizer::getNonSynonym() {
	return nonsynonym;
}

std::pair<std::pair<std::string, std::string>, std::pair<std::string, std::string>> Optimizer::getClause(int cl) {
	std::pair<std::pair<std::string, std::string>, std::pair<std::string, std::string>> clause;
	int index;
	if (cl >= psize + wsize) {
		index = cl - psize - wsize;
		clause = std::make_pair(std::make_pair("st", suchThatClauses.at(index).first), suchThatClauses.at(index).second);
	} else if (cl >= psize) {
		clause = std::make_pair(std::make_pair("with", "with"), withClauses.at(cl - psize));
	} else {
		clause = std::make_pair(std::make_pair("pattern", patternClauses.at(cl).first), patternClauses.at(cl).second);
	}
	return clause;
}