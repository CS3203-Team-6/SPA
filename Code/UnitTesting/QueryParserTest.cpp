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
			vector<string> actual = QueryParser::splitClauses("assign a; while w;Select a such that Follows(a,w)");
			vector<string> expected{ "assign a", "while w", "Select a such that Follows(a,w)" };
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(splitDeclarations_success)
		{
			vector<string> input = { "assign a, a1, a2", "while w", "Select a such that Follows(a,w)" };
			unordered_map<string, string> actual = QueryParser::splitDeclarations(input);
			unordered_map<string, string> expected{ {"a", "assign"}, {"a1", "assign"}, {"a2", "assign"}, {"w", "while"}};
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(splitSelectParameter_success)
		{
			vector<string> actual = QueryParser::splitSelectParameter("Select a");
			vector<string> expected{ "a" };
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(splitSelectParameter_boolean_success)
		{
			vector<string> actual = QueryParser::splitSelectParameter("Select BOOLEAN");
			vector<string> expected{ "BOOLEAN" };
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(splitSelectParameter_synonymWithAttribute_success)
		{
			vector<string> actual = QueryParser::splitSelectParameter("Select p.procName");
			vector<string> expected{ "p.procName" };
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(splitSelectParameter_tuplesOfSynonymAttribute_success)
		{
			vector<string> actual = QueryParser::splitSelectParameter("Select <p.procName, v.varName, r.stmt#>");
			vector<string> expected{ "p.procName", "v.varName", "r.stmt#" };
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(splitSelectParameter_tupleWithSpace_success)
		{
			vector<string> actual = QueryParser::splitSelectParameter("Select <p, q, r>");
			vector<string> expected{ "p", "q", "r" };
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(splitSelectParameter_tupleWithNoSpace_success)
		{
			vector<string> actual = QueryParser::splitSelectParameter("Select <p,q,r>");
			vector<string> expected{ "p", "q", "r" };
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(splitSuchThatCondition_success)
		{
			vector<string> input = { "such that Parent(a, _)" };
			vector<pair<string, pair<string, string>>> actual = QueryParser::splitSuchThatCondition(input);
			vector<pair<string, pair<string, string>>> expected{ {"Parent", {"a", "_"}} };
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(splitSuchThatCondition_multipleWhitespaces_success)
		{
			vector<string> input = { "such that    Parent    (   a  , _  )  " };
			vector<pair<string, pair<string, string>>> actual = QueryParser::splitSuchThatCondition(input);
			vector<pair<string, pair<string, string>>> expected{ {"Parent", {"a", "_"}} };
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(splitSuchThatCondition_multipleClauses_success)
		{
			vector<string> input = { "such that Parent(a, _)", "such that Follows(10, 11)", "and Modifies(3, \"sum\"", "and Uses(5, \"a\")", "such that Next*(1, 10)", "and Affects*(4, 10)" };
			vector<pair<string, pair<string, string>>> actual = QueryParser::splitSuchThatCondition(input);
			vector<pair<string, pair<string, string>>> expected{ {"Parent", {"a", "_"}}, {"Follows", {"10", "11"}}, {"Modifies", {"3", "\"sum\""}}, {"Uses", {"5", "\"a\""}}, {"Next*", {"1", "10"}}, {"Affects*", {"4", "10"}} };
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(splitSuchThatCondition_multipleClauses_multipleWhitespaces_success)
		{
			vector<string> input = { "such that    Parent(a, _)", "such that Follows    (10, 11)", "and Modifies(   3, \"sum\"", "and Uses(5   , \"a\")", "such that Next*(1,10    )", "and Affects*(4, 10)    " };
			vector<pair<string, pair<string, string>>> actual = QueryParser::splitSuchThatCondition(input);
			vector<pair<string, pair<string, string>>> expected{ {"Parent", {"a", "_"}}, {"Follows", {"10", "11"}}, {"Modifies", {"3", "\"sum\""}}, {"Uses", {"5", "\"a\""}}, {"Next*", {"1", "10"}}, {"Affects*", {"4", "10"}} };
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(splitPatternCondition_success)
		{
			vector<string> input = { "pattern a(_, \"x*y+z\")" };
			vector<pair<string, pair<string, string>>> actual = QueryParser::splitPatternCondition(input);
			vector<pair<string, pair<string, string>>> expected{ {"a", {"_", "\"x*y+z\""}} };
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(splitPatternCondition_multipleWhitespaces_success)
		{
			vector<string> input = { "pattern   a  ( _ , \"x*y+z\" ) " };
			vector<pair<string, pair<string, string>>> actual = QueryParser::splitPatternCondition(input);
			vector<pair<string, pair<string, string>>> expected{ {"a", {"_", "\"x*y+z\""}} };
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(splitPatternCondition_multipleClauses_success)
		{
			vector<string> input = { "pattern a(_, \"x\")", "pattern a1(_, \"y\")", "and ifs(\"x\", _, _)", "and while1(\"y\", _)", "pattern ifs(_, _, _)" };
			vector<pair<string, pair<string, string>>> actual = QueryParser::splitPatternCondition(input);
			vector<pair<string, pair<string, string>>> expected{ {"a", {"_", "\"x\""}}, {"a1", {"_", "\"y\""}}, {"ifs", {"\"x\"", "_,_"}}, {"while1", {"\"y\"", "_"}}, {"ifs", {"_", "_,_"}} };
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(splitPatternCondition_multipleClauses_multipleWhitespaces_success)
		{
			vector<string> input = { "pattern     a(_, \"x\")", "pattern a1    (_, \"y\")", "and ifs(   \"x\", _, _)", "and while1(\"y\"    , _)", "pattern ifs(_, _, _    )" };
			vector<pair<string, pair<string, string>>> actual = QueryParser::splitPatternCondition(input);
			vector<pair<string, pair<string, string>>> expected{ {"a", {"_", "\"x\""}}, {"a1", {"_", "\"y\""}}, {"ifs", {"\"x\"", "_,_"}}, {"while1", {"\"y\"", "_"}}, {"ifs", {"_", "_,_"}} };
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(splitWithCondition_success)
		{
			vector<string> input = { "with a.stmt# = 20" };
			vector<pair<string, string>> actual = QueryParser::splitWithCondition(input);
			vector<pair<string, string>> expected{ {"a.stmt#", "20"} };
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(splitWithCondition_multipleWhitespaces_success)
		{
			vector<string> input = { "with    a.stmt#    =   20   " };
			vector<pair<string, string>> actual = QueryParser::splitWithCondition(input);
			vector<pair<string, string>> expected{ {"a.stmt#", "20"} };
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(splitWithCondition_multipleClauses_success)
		{
			vector<string> input = { "with a.stmt# = 20", "and s.stmt# = 5", "with p.procName = \"proc\"", "and p1.procName = \"proc1\"", "and c.value = 9" };
			vector<pair<string, string>> actual = QueryParser::splitWithCondition(input);
			vector<pair<string, string>> expected{ {"a.stmt#", "20"}, {"s.stmt#", "5"}, {"p.procName", "\"proc\""}, {"p1.procName", "\"proc1\""} , {"c.value", "9"} };
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(splitWithCondition_multipleClauses_multipleWhitespaces_success)
		{
			vector<string> input = { "with      a.stmt# = 20", "and s   .stmt# = 5", "with p.    procName = \"proc\"", "and p1.procName     = \"proc1\"", "and c.value =     9" };
			vector<pair<string, string>> actual = QueryParser::splitWithCondition(input);
			vector<pair<string, string>> expected{ {"a.stmt#", "20"}, {"s.stmt#", "5"}, {"p.procName", "\"proc\""}, {"p1.procName", "\"proc1\""} , {"c.value", "9"} };
			Assert::AreEqual(actual == expected, true);
		}

	};
}