#include "OptimizerUtility.h"

/* Checking to test if the parts of such that clause contains a synonym */
int OptimizerUtility::getSuchThatEntityType(std::string str) {
	int res;
	if (str.find("_") == std::string::npos) {
		if (str.find('"') != std::string::npos) {
			res = 0; //varName
		}
		else if (std::all_of(str.begin(), str.end(), isdigit)) {
			res = 0; //stmtNum
		}
		else {
			res = 1; //synonym
		}
	}
	else {
		res = 2; // wildcard
	}
	return res;
}

/* Checking to test if the parts of pattern clause contains a synonym */
int OptimizerUtility::getPatternEntityType1(std::string str) {
	int res = 3;
	if (str.find("_") == std::string::npos) {
		if (str.find('"') == std::string::npos) {
			res = 1; // v
		}
		else {
			res = 0; // ""
		}
	}
	else {
		res = 2; // _
	}
	return res;
}

/* Checking to test if the parts of pattern clause contains a synonym */
int OptimizerUtility::getPatternEntityType2(std::string str) {
	int res = 3;
	if (str.find('"') != std::string::npos) {
		if (str.find("_") != std::string::npos) {
			res = 1; // _""_
		}
		else {
			res = 0; // ""
		}
	}
	else if (str.find("_") != std::string::npos) {
		res = 2; // _
	}
	return res;
}

/* Checking to test if the parts of with clause contains a synonym */
bool OptimizerUtility::isWithSynonym(std::string str) {
	bool res = false;
	if (str.find('.') != std::string::npos) {
		res = true;
	}
	return res;
}

// Used for getting PQ
int OptimizerUtility::getSuchThatPriority(std::string relation, int synNum) {
	//Priority determined by the relation and number of synonyms
	//if 1 syn and 1 wildcard -> 2 syn
	int res = 50;
	if (relation.compare("Follows") == 0) {
		if (synNum == 1) {
			res = 1;
		}
		else {
			res = 25;
		}
	}
	else if (relation.compare("Follows*") == 0) {
		if (synNum == 1) {
			res = 17;
		}
		else {
			res = 32;
		}
	}
	else if (relation.compare("Parent") == 0) {
		if (synNum == 1) {
			res = 2;
		}
		else {
			res = 24;
		}
	}
	else if (relation.compare("Parent*") == 0) {
		if (synNum == 1) {
			res = 14;
		}
		else {
			res = 31;
		}
	}
	else if (relation.compare("Modifies") == 0) {
		if (synNum == 1) {
			res = 7;
		}
		else {
			res = 27;
		}
	}
	else if (relation.compare("Uses") == 0) {
		if (synNum == 1) {
			res = 8;
		}
		else {
			res = 28;
		}
	}
	else if (relation.compare("Next") == 0) {
		if (synNum == 1) {
			res = 3;
		}
		else {
			res = 26;
		}
	}
	else if (relation.compare("Next*") == 0) {
		if (synNum == 1) {
			res = 36;
		}
		else {
			res = 39;
		}
	}
	else if (relation.compare("Calls") == 0) {
		if (synNum == 1) {
			res = 9;
		}
		else {
			res = 23;
		}
	}
	else if (relation.compare("Calls*") == 0) {
		if (synNum == 1) {
			res = 13;
		}
		else {
			res = 29;
		}
	}
	else if (relation.compare("Affects") == 0) {
		if (synNum == 1) {
			res = 37;
		}
		else {
			res = 40;
		}
	}
	else if (relation.compare("Affects*") == 0) {
		if (synNum == 1) {
			res = 38;
		}
		else {
			res = 41;
		}
	}
	return 50 - res;
}

int OptimizerUtility::getWithPriority(int synNum) {
	int res = 50;
	if (synNum == 1) {
		res = 4;
	}
	else {
		res = 18;
	}
	return 50 - res;
}

int OptimizerUtility::getPatternPriority(std::string synType, int t1, int t2) {
	int res = 50;
	if (synType.compare("assign") == 0) {
		if (t1 == 0 && t2 == 0) {
			res = 5; //a "", "" -> t1 = 0, t2 = 0
		}
		else if (t1 == 0 && t2 == 1) {
			res = 6; //a "", _""_ -> t1 = 0, t2 = 1
		}
		else if (t1 == 0 && t2 == 2) {
			res = 12; //a "" _ -> t1 = 0, t2 = 2
		}
		else if (t1 == 2 && t2 == 0) {
			res = 15; //a _, "" -> t1 = 2, t2 = 0
		}
		else if (t1 == 2 && t2 == 1) {
			res = 16; //a _, _""_ -> t1 = 2, t2 = 1
		}
		else if (t1 == 1 && t2 == 0) {
			res = 19; //a v, "" -> t1 = 1, t2 = 0
		}
		else if (t1 == 1 && t2 == 1) {
			res = 20; //a v, _""_ -> t1 = 1, t2 = 1
		}
		else if (t1 == 1 && t2 == 2) {
			res = 21; //a v, _ -> t1 = 1, t2 = 2
		}
		else if (t1 == 2 && t2 == 2) {
			res = 35; //a _,_ -> t1 = 2, t2 = 2
		}
	}
	else if (synType.compare("if") == 0) {
		if (t1 == 0) {
			res = 10; //ifs ""
		}
		else if (t1 == 1) {
			res = 21; //ifs v
		}
		else if (t1 == 2) {
			res = 33; //ifs _
		}
	}
	else if (synType.compare("while") == 0) {
		if (t1 == 0) {
			res = 11; //w ""
		}
		else if (t1 == 1) {
			res = 22; //w v
		}
		else if (t1 == 2) {
			res = 34; //w _
		}
	}
	return 50 - res;
}

int OptimizerUtility::getSynonymPriority(std::string type) {
	int res = 12;
	if (type.compare("assign") == 0) {
		res = 9;
	} else if (type.compare("prog_line") == 0) {
		res = 11;
	} else if (type.compare("read") == 0) {
		res = 2;
	} else if (type.compare("print") == 0) {
		res = 4;
	} else if (type.compare("stmt") == 0) {
		res = 10;
	} else if (type.compare("if") == 0) {
		res = 6;
	} else if (type.compare("while") == 0) {
		res = 7;
	} else if (type.compare("variable") == 0) {
		res = 8;
	} else if (type.compare("call") == 0) {
		res = 3;
	} else if (type.compare("constant") == 0) {
		res = 5;
	} else if (type.compare("procedure") == 0) {
		res = 1;
	}
	return 12 - res;
}

