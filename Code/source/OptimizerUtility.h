#include <string>
#include <algorithm>

namespace OptimizerUtility {
	int getSuchThatEntityType(std::string str);
	int getPatternEntityType1(std::string str);
	int getPatternEntityType2(std::string str);
	bool isWithSynonym(std::string str);
	int getSuchThatPriority(std::string relation, int synNum);
	int getWithPriority(int synNum);
	int getPatternPriority(std::string synType, int t1, int t2);
	int getSynonymPriority(std::string type);
}