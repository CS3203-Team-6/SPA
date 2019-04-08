#include "stdafx.h"
#include "CppUnitTest.h"
#include "Optimizer.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(OptimizerTest)
	{
	public:
		std::vector<std::pair<std::string, std::pair<std::string, std::string>>> empty;
		std::unordered_set<std::string> s;
		std::unordered_map<std::string, std::string> d;
		TEST_METHOD(testExtractSuchThat1)
		{
			std::pair<std::string, std::pair<std::string, std::string>> clause = 
				std::make_pair("Follows", std::make_pair("1", "s"));
			std::vector<std::pair<std::string, std::pair<std::string, std::string>>> st;
			d["s"] = "stmt";
			st.push_back(clause);
			Optimizer op = Optimizer(st, empty, empty, s, d);
			std::pair<int, std::vector<std::string>> result = op.extractSuchThatSyn(0);
			std::vector<std::string> vect{ "s" };
			std::pair<int, std::vector<std::string>> expected = std::make_pair(49, vect);
			Assert::IsTrue(result.first == expected.first && result.second == expected.second);
		}
		TEST_METHOD(testExtractSuchThat2)
		{
			std::pair<std::string, std::pair<std::string, std::string>> clause =
				std::make_pair("Parent*", std::make_pair("s", "a"));
			std::vector<std::pair<std::string, std::pair<std::string, std::string>>> st;
			d["a"] = "assign";
			st.push_back(clause);
			Optimizer op = Optimizer(st, empty, empty, s, d);
			std::pair<int, std::vector<std::string>> result = op.extractSuchThatSyn(0);
			std::vector<std::string> vect{ "a", "s" };
			std::pair<int, std::vector<std::string>> expected = std::make_pair(19, vect);
			Assert::IsTrue(result.first == expected.first && result.second == expected.second);
		}
		TEST_METHOD(testExtractSuchThatWildCard)
		{
			std::pair<std::string, std::pair<std::string, std::string>> clause =
				std::make_pair("Modifies", std::make_pair("p", "_"));
			std::vector<std::pair<std::string, std::pair<std::string, std::string>>> st;
			d["p"] = "procedure";
			st.push_back(clause);
			Optimizer op = Optimizer(st, empty, empty, s, d);
			std::pair<int, std::vector<std::string>> result = op.extractSuchThatSyn(0);
			std::vector<std::string> vect{ "p" };
			std::pair<int, std::vector<std::string>> expected = std::make_pair(23, vect);
			Assert::IsTrue(result.first == expected.first && result.second == expected.second);
		}
		TEST_METHOD(testExtractWithSynonym1)
		{
			std::pair<std::string, std::pair<std::string, std::string>> clause =
				std::make_pair("with", std::make_pair("a.stmtNum", "14"));
			std::vector<std::pair<std::string, std::pair<std::string, std::string>>> w;
			w.push_back(clause);
			Optimizer op = Optimizer(empty, w, empty, s, d);
			std::pair<int, std::vector<std::string>> result = op.extractWithSyn(0);
			std::vector<std::string> vect{ "a" };
			std::pair<int, std::vector<std::string>> expected = std::make_pair(46, vect);
			Assert::IsTrue(result.first == expected.first && result.second == expected.second);
		}
		TEST_METHOD(testExtractWithSynonym2)
		{
			std::pair<std::string, std::pair<std::string, std::string>> clause =
				std::make_pair("with", std::make_pair("pr.varName", "re.varName"));
			std::vector<std::pair<std::string, std::pair<std::string, std::string>>> w;
			w.push_back(clause);
			d["re"] = "read";
			d["pr"] = "print";
			Optimizer op = Optimizer(empty, w, empty, s, d);
			std::pair<int, std::vector<std::string>> result = op.extractWithSyn(0);
			std::vector<std::string> vect{ "re", "pr" };
			std::pair<int, std::vector<std::string>> expected = std::make_pair(32, vect);
			Assert::IsTrue(result.first == expected.first && result.second == expected.second);
		}
		TEST_METHOD(testExtractPatternSynonymAssign)
		{
			std::string str1 = { '"', 'v', 'a', 'r', '"' };
			std::string str2 = { '_', '"', 'a', '+', 'b', '"', '_' };
			std::pair<std::string, std::pair<std::string, std::string>> clause =
				std::make_pair("a", std::make_pair(str1, str2));
			std::vector<std::pair<std::string, std::pair<std::string, std::string>>> p;
			p.push_back(clause);
			d["a"] = "assign";
			Optimizer op = Optimizer(empty, empty, p, s, d);
			std::pair<int, std::vector<std::string>> result = op.extractPatternSyn(0);
			std::vector<std::string> vect{ "a" };
			std::pair<int, std::vector<std::string>> expected = std::make_pair(44, vect);
			Assert::IsTrue(result.first == expected.first && result.second == expected.second);
		}
		TEST_METHOD(testExtractPatternSynonymIf)
		{
			std::pair<std::string, std::pair<std::string, std::string>> clause =
				std::make_pair("ifs", std::make_pair("v", "_"));
			std::vector<std::pair<std::string, std::pair<std::string, std::string>>> p;
			p.push_back(clause);
			d["ifs"] = "if";
			d["v"] = "variable";
			Optimizer op = Optimizer(empty, empty, p, s, d);
			std::pair<int, std::vector<std::string>> result = op.extractPatternSyn(0);
			std::vector<std::string> vect{ "ifs", "v" };
			std::pair<int, std::vector<std::string>> expected = std::make_pair(29, vect);
			Assert::IsTrue(result.first == expected.first && result.second == expected.second);
		}
		TEST_METHOD(testExtractPatternSynonymWhile)
		{
			std::pair<std::string, std::pair<std::string, std::string>> clause =
				std::make_pair("w", std::make_pair("_", "_"));
			std::vector<std::pair<std::string, std::pair<std::string, std::string>>> p;
			d["w"] = "while";
			p.push_back(clause);
			Optimizer op = Optimizer(empty, empty, p, s, d);
			std::pair<int, std::vector<std::string>> result = op.extractPatternSyn(0);
			std::vector<std::string> vect{ "w" };
			std::pair<int, std::vector<std::string>> expected = std::make_pair(16, vect);
			Assert::IsTrue(result.first == expected.first && result.second == expected.second);
		}
		
		TEST_METHOD(testGraphChainingLogic1)
		{
			std::string str = { '"', 'a', '"' };
			std::pair<std::string, std::pair<std::string, std::string>> clause1 =
				std::make_pair("w", std::make_pair("v", "_"));
			std::pair<std::string, std::pair<std::string, std::string>> clause2 =
				std::make_pair("Follows", std::make_pair("w", "s"));
			std::pair<std::string, std::pair<std::string, std::string>> clause3 =
				std::make_pair("Modifies", std::make_pair("s", "v"));
			std::pair<std::string, std::pair<std::string, std::string>> clause4 =
				std::make_pair("Parent", std::make_pair("w", "ifs"));
			std::pair<std::string, std::pair<std::string, std::string>> clause5 =
				std::make_pair("with", std::make_pair("v.varName", str));
			std::pair<std::string, std::pair<std::string, std::string>> clause6 =
				std::make_pair("with", std::make_pair("s.stmt#", "15"));
			std::vector<std::pair<std::string, std::pair<std::string, std::string>>> p;
			std::vector<std::pair<std::string, std::pair<std::string, std::string>>> st;
			std::vector<std::pair<std::string, std::pair<std::string, std::string>>> w;
			p.push_back(clause1);
			st.push_back(clause2); st.push_back(clause3); st.push_back(clause4);
			w.push_back(clause5); w.push_back(clause6);
			s.insert("a");
			d["w"] = "while"; d["v"] = "variable"; d["ifs"] = "if"; d["s"] = "stmt";
			Optimizer op = Optimizer(st, w, p, s, d);
			op.groupClause();
			std::vector<std::vector<std::pair<std::string, std::pair<std::string, std::string>>>> result1 = op.getTrivial();
			std::vector<std::vector<std::pair<std::string, std::pair<std::string, std::string>>>> expected1 = { {clause6, clause2, clause1, clause5, clause3, clause4} };
			std::vector<std::vector<std::pair<std::string, std::pair<std::string, std::string>>>> result2 = op.getNonTrivial();
			std::vector<std::vector<std::pair<std::string, std::pair<std::string, std::string>>>> expected2 = { };
			Assert::IsTrue(result1 == expected1 && result2 == expected2);
		}
		TEST_METHOD(testGraphChainingLogic2)
		{
			std::string str1 = { '"', 'a', '"' };
			std::string str2 = { '"', 'p', 'r', 'o', 'c', 'e', 'd', 'u', 'r', 'e', '"' };
			std::pair<std::string, std::pair<std::string, std::string>> clause1 =
				std::make_pair("ifs", std::make_pair("_", "_"));
			std::pair<std::string, std::pair<std::string, std::string>> clause2 =
				std::make_pair("Follows", std::make_pair("a", "14"));
			std::pair<std::string, std::pair<std::string, std::string>> clause3 =
				std::make_pair("Modifies", std::make_pair("p", str1));
			std::pair<std::string, std::pair<std::string, std::string>> clause4 =
				std::make_pair("Call", std::make_pair("p", str2));
			std::pair<std::string, std::pair<std::string, std::string>> clause5 =
				std::make_pair("with", std::make_pair("v.varName", "q.procName"));
			std::pair<std::string, std::pair<std::string, std::string>> clause6 =
				std::make_pair("with", std::make_pair("q.procName", str2));
			std::vector<std::pair<std::string, std::pair<std::string, std::string>>> p;
			std::vector<std::pair<std::string, std::pair<std::string, std::string>>> st;
			std::vector<std::pair<std::string, std::pair<std::string, std::string>>> w;
			p.push_back(clause1);
			st.push_back(clause2); st.push_back(clause3); st.push_back(clause4);
			w.push_back(clause5); w.push_back(clause6);
			d["a"] = "assign"; d["v"] = "variable"; d["p"] = "procedure"; d["q"] = "procedure";
			d["ifs"] = "if";
			s.insert("a");
			Optimizer op = Optimizer(st, w, p, s, d);
			op.groupClause();
			std::vector<std::vector<std::pair<std::string, std::pair<std::string, std::string>>>> result1 = op.getTrivial();
			std::vector<std::vector<std::pair<std::string, std::pair<std::string, std::string>>>> expected1 = { { clause6, clause5 }, { clause3, clause4 }, {clause1} };
			std::vector<std::vector<std::pair<std::string, std::pair<std::string, std::string>>>> result2 = op.getNonTrivial();
			std::vector<std::vector<std::pair<std::string, std::pair<std::string, std::string>>>> expected2 = { {clause2} };
			Assert::IsTrue(result1 == expected1 && result2 == expected2);
		}
		TEST_METHOD(testTriviality)
		{
			std::string str1 = { '"', 'a', '"' };
			std::string str2 = { '_', '"', 'a', '+', 'b', '"', '_' };
			std::string str3 = { '"', 'p', 'r', 'o', 'c', 'e', 'd', 'u', 'r', 'e', '"' };
			std::pair<std::string, std::pair<std::string, std::string>> clause1 =
				std::make_pair("a", std::make_pair(str1, str2));
			std::pair<std::string, std::pair<std::string, std::string>> clause2 =
				std::make_pair("Next*", std::make_pair("s", "14"));
			std::pair<std::string, std::pair<std::string, std::string>> clause3 =
				std::make_pair("Uses", std::make_pair("ifs", "v"));
			std::pair<std::string, std::pair<std::string, std::string>> clause4 =
				std::make_pair("Call", std::make_pair("p", "q"));
			std::pair<std::string, std::pair<std::string, std::string>> clause5 =
				std::make_pair("with", std::make_pair("v.varName", str3));
			std::pair<std::string, std::pair<std::string, std::string>> clause6 =
				std::make_pair("with", std::make_pair("q.procName", str1));
			std::vector<std::pair<std::string, std::pair<std::string, std::string>>> p;
			std::vector<std::pair<std::string, std::pair<std::string, std::string>>> st;
			std::vector<std::pair<std::string, std::pair<std::string, std::string>>> w;
			p.push_back(clause1);
			st.push_back(clause2); st.push_back(clause3); st.push_back(clause4);
			w.push_back(clause5); w.push_back(clause6);
			s.insert("q"); s.insert("s");
			d["a"] = "assign"; d["v"] = "variable"; d["p"] = "procedure"; d["q"] = "procedure";
			d["ifs"] = "if"; d["s"] = "stmt"; 
			Optimizer op = Optimizer(st, w, p, s, d);
			op.groupClause();
			std::vector<std::vector<std::pair<std::string, std::pair<std::string, std::string>>>> result1 = op.getTrivial();
			std::vector<std::vector<std::pair<std::string, std::pair<std::string, std::string>>>> expected1 = { { clause5, clause3 }, {clause1} };
			std::vector<std::vector<std::pair<std::string, std::pair<std::string, std::string>>>> result2 = op.getNonTrivial();
			std::vector<std::vector<std::pair<std::string, std::pair<std::string, std::string>>>> expected2 = { { clause6, clause4 }, {clause2} };
			Assert::IsTrue(result1 == expected1 && result2 == expected2);
		}
	};
}