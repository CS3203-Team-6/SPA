#include "TestWrapper.h"
#include "PKB.h"
#include "Preprocessor.h"
#include "Parser.h"
#include "QueryParser.h"
#include "DesignExtractor.h"

// implementation code of WrapperFactory - do NOT modify the next 5 lines
AbstractWrapper* WrapperFactory::wrapper = 0;
AbstractWrapper* WrapperFactory::createWrapper() {
  if (wrapper == 0) wrapper = new TestWrapper;
  return wrapper;
}
// Do not modify the following line
volatile bool TestWrapper::GlobalStop = false;

// a default constructor
TestWrapper::TestWrapper() {
	
}

// method for parsing the SIMPLE source
void TestWrapper::parse(std::string filename) {
	PKB pkb = PKB();
	Parser parser = Parser();
	DesignExtractor de = DesignExtractor();
	
	// Opens the file and read into contents.
	std::ifstream t(filename);
	std::string contents;

	t.seekg(0, std::ios::end);
	contents.reserve(t.tellg());
	t.seekg(0, std::ios::beg);

	contents.assign((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());

	contents = contents + '\0';

	try {
		//Preprocesses source into a list of Statements
		Preprocessor preprocessor = Preprocessor(contents);
		preprocessor.process();
		std::vector<Statement> procList = preprocessor.getProcLst();

		//Parses the list of procedures.
		parser.parse(procList, 0, "");

		//Perform post-parsing Design Extraction.
		de.extractDesigns();
	} catch (std::string exception) {
		std::cout << exception << std::flush;
		exit(0);
	}

}

// method to evaluating a query
void TestWrapper::evaluate(std::string query, std::list<std::string>& results){
// call your evaluator to evaluate the query here
  // ...code to evaluate query...

	std::unordered_set<std::string> qp = QueryParser::parse(query);
	if (qp.count("error") == 0 && qp.size() != 0) {
		for (std::unordered_set<std::string>::iterator it = qp.begin(); it != qp.end(); ++it) {
			std::string pointer = *it;
			results.push_back(pointer);
		}
	}

  // store the answers to the query in the results list (it is initially empty)
  // each result must be a string.
}