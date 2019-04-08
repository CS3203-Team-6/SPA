#include "stdafx.h"
#include "CppUnitTest.h"
#include "QueryParser.h" 

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(QueryParserTest)
	{
	public:

		TEST_METHOD(splitClauses_success)
		{
			std::vector<std::string> actual =
				QueryParser::splitClauses("assign a; while w;Select a such that Follows(a,w)");
			std::vector<std::string> expected{ "assign a", "while w",
				"Select a such that Follows(a,w)" };
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(splitDeclarations_success)
		{
			std::vector<std::string> input = { "assign a, a1, a2", "while w",
				"Select a such that Follows(a,w)" };

			std::unordered_map<std::string, std::string> actual =
				QueryParser::splitDeclarations(input);

			std::unordered_map<std::string, std::string> expected{ {"a", "assign"},
			{"a1", "assign"}, {"a2", "assign"}, {"w", "while"}};

			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(splitSelectParameter_success)
		{
			std::vector<std::string> actual = QueryParser::splitSelectParameter("Select a");
			std::vector<std::string> expected{ "a" };
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(splitSelectParameter_boolean_success)
		{
			std::vector<std::string> actual = QueryParser::splitSelectParameter("Select BOOLEAN");
			std::vector<std::string> expected{ "BOOLEAN" };
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(splitSelectParameter_synonymWithAttribute_success)
		{
			std::vector<std::string> actual = QueryParser::splitSelectParameter("Select p.procName");
			std::vector<std::string> expected{ "p.procName" };
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(splitSelectParameter_tuplesOfSynonymAttribute_success)
		{
			std::vector<std::string> actual =
				QueryParser::splitSelectParameter("Select <p.procName, v.varName, r.stmt#>");
			std::vector<std::string> expected{ "p.procName", "v.varName", "r.stmt#" };
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(splitSelectParameter_tupleWithSpace_success)
		{
			std::vector<std::string> actual = QueryParser::splitSelectParameter("Select <p, q, r>");
			std::vector<std::string> expected{ "p", "q", "r" };
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(splitSelectParameter_tupleWithNoSpace_success)
		{
			std::vector<std::string> actual = QueryParser::splitSelectParameter("Select <p,q,r>");
			std::vector<std::string> expected{ "p", "q", "r" };
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(splitSuchThatCondition_success)
		{
			std::vector<std::string> input = { "such that Parent(a, _)" };
			std::vector<std::pair<std::string, std::pair<std::string, std::string>>> actual =
				QueryParser::splitSuchThatCondition(input);
			std::vector<std::pair<std::string, std::pair<std::string, std::string>>>
				expected{ {"Parent", {"a", "_"}} };
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(splitSuchThatCondition_multipleWhitespaces_success)
		{
			std::vector<std::string> input = { "such that    Parent    (   a  , _  )  " };
			std::vector<std::pair<std::string, std::pair<std::string, std::string>>> actual =
				QueryParser::splitSuchThatCondition(input);
			std::vector<std::pair<std::string, std::pair<std::string, std::string>>>
				expected{ {"Parent", {"a", "_"}} };
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(splitSuchThatCondition_multipleClauses_success)
		{
			std::vector<std::string> input = { "such that Parent(a, _)",
				"such that Follows(10, 11)", "and Modifies(3, \"sum\"",
				"and Uses(5, \"a\")", "such that Next*(1, 10)", "and Affects*(4, 10)" };

			std::vector<std::pair<std::string, std::pair<std::string, std::string>>> actual =
				QueryParser::splitSuchThatCondition(input);
			std::vector<std::pair<std::string, std::pair<std::string, std::string>>>
				expected{ {"Parent", {"a", "_"}}, {"Follows", {"10", "11"}},
					{"Modifies", {"3", "\"sum\""}}, {"Uses", {"5", "\"a\""}},
					{"Next*", {"1", "10"}}, {"Affects*", {"4", "10"}} };
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(splitSuchThatCondition_multipleClauses_multipleWhitespaces_success)
		{
			std::vector<std::string> input = { "such that    Parent(a, _)", "such that Follows    (10, 11)", "and Modifies(   3, \"sum\"", "and Uses(5   , \"a\")", "such that Next*(1,10    )", "and Affects*(4, 10)    " };
			std::vector<std::pair<std::string, std::pair<std::string, std::string>>> actual =
				QueryParser::splitSuchThatCondition(input);
			std::vector<std::pair<std::string, std::pair<std::string, std::string>>>
				expected{ {"Parent", {"a", "_"}}, {"Follows", {"10", "11"}},
					{"Modifies", {"3", "\"sum\""}}, {"Uses", {"5", "\"a\""}},
					{"Next*", {"1", "10"}}, {"Affects*", {"4", "10"}} };
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(splitPatternCondition_success)
		{
			std::vector<std::string> input = { "pattern a(_, \"x*y+z\")" };
			std::vector<std::pair<std::string, std::pair<std::string, std::string>>> actual =
				QueryParser::splitPatternCondition(input);
			std::vector<std::pair<std::string, std::pair<std::string, std::string>>>
				expected{ {"a", {"_", "\"x*y+z\""}} };
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(splitPatternCondition_multipleWhitespaces_success)
		{
			std::vector<std::string> input = { "pattern   a  ( _ , \"x*y+z\" ) " };
			std::vector<std::pair<std::string, std::pair<std::string, std::string>>> actual =
				QueryParser::splitPatternCondition(input);
			std::vector<std::pair<std::string, std::pair<std::string, std::string>>>
				expected{ {"a", {"_", "\"x*y+z\""}} };
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(splitPatternCondition_multipleClauses_success)
		{
			std::vector<std::string> input = { "pattern a(_, \"x\")", "pattern a1(_, \"y\")", "and ifs(\"x\", _, _)", "and while1(\"y\", _)", "pattern ifs(_, _, _)" };
			std::vector<std::pair<std::string, std::pair<std::string, std::string>>> actual =
				QueryParser::splitPatternCondition(input);
			std::vector<std::pair<std::string, std::pair<std::string, std::string>>>
				expected{ {"a", {"_", "\"x\""}}, {"a1", {"_", "\"y\""}}, {"ifs", {"\"x\"", "_,_"}},
					{"while1", {"\"y\"", "_"}}, {"ifs", {"_", "_,_"}} };
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(splitPatternCondition_multipleClauses_multipleWhitespaces_success)
		{
			std::vector<std::string> input = { "pattern     a(_, \"x\")", "pattern a1    (_, \"y\")", "and ifs(   \"x\", _, _)", "and while1(\"y\"    , _)", "pattern ifs(_, _, _    )" };
			std::vector<std::pair<std::string, std::pair<std::string, std::string>>> actual =
				QueryParser::splitPatternCondition(input);
			std::vector<std::pair<std::string, std::pair<std::string, std::string>>>
				expected{ {"a", {"_", "\"x\""}}, {"a1", {"_", "\"y\""}}, {"ifs", {"\"x\"", "_,_"}},
					{"while1", {"\"y\"", "_"}}, {"ifs", {"_", "_,_"}} };
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(splitWithCondition_success)
		{
			std::vector<std::string> input = { "with a.stmt# = 20" };
			std::vector<std::pair<std::string, std::string>> actual =
				QueryParser::splitWithCondition(input);
			std::vector<std::pair<std::string, std::string>> expected{ {"a.stmt#", "20"} };
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(splitWithCondition_multipleWhitespaces_success)
		{
			std::vector<std::string> input = { "with    a.stmt#    =   20   " };
			std::vector<std::pair<std::string, std::string>> actual =
				QueryParser::splitWithCondition(input);
			std::vector<std::pair<std::string, std::string>> expected{ {"a.stmt#", "20"} };
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(splitWithCondition_multipleClauses_success)
		{
			std::vector<std::string> input = { "with a.stmt# = 20", "and s.stmt# = 5", "with p.procName = \"proc\"", "and p1.procName = \"proc1\"", "and c.value = 9" };
			std::vector<std::pair<std::string, std::string>> actual =
				QueryParser::splitWithCondition(input);
			std::vector<std::pair<std::string, std::string>> expected{ {"a.stmt#", "20"},
				{"s.stmt#", "5"}, {"p.procName", "\"proc\""}, {"p1.procName", "\"proc1\""} ,
				{"c.value", "9"} };
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(splitWithCondition_multipleClauses_multipleWhitespaces_success)
		{
			std::vector<std::string> input = { "with      a.stmt# = 20", "and s   .stmt# = 5", "with p.    procName = \"proc\"", "and p1.procName     = \"proc1\"", "and c.value =     9" };
			std::vector<std::pair<std::string, std::string>> actual =
				QueryParser::splitWithCondition(input);
			std::vector<std::pair<std::string, std::string>> expected{ {"a.stmt#", "20"},
				{"s.stmt#", "5"}, {"p.procName", "\"proc\""}, {"p1.procName", "\"proc1\""} ,
				{"c.value", "9"} };
			Assert::AreEqual(actual == expected, true);
		}

	};
}