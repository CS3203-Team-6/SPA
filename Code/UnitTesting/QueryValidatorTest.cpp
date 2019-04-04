#include "stdafx.h"
#include "CppUnitTest.h"
#include "QueryValidator.h" 

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(QueryValidatorTest)
	{
	public:

		TEST_METHOD(initialValidation_success)
		{
			string result = QueryValidator::initialValidation("assign a; Select a");
			Assert::AreEqual(result.compare(""), 0);
		}

		TEST_METHOD(initialValidation_lengthZero)
		{
			string result = QueryValidator::initialValidation("");
			Assert::AreEqual(result.compare("invalid query"), 0);
		}

		TEST_METHOD(initialValidation_noSelectStatement)
		{
			string result = QueryValidator::initialValidation("assign a;");
			Assert::AreEqual(result.compare("invalid query"), 0);
		}

		TEST_METHOD(initialValidation_noVariableDeclaration)
		{
			string result = QueryValidator::initialValidation("Select a");
			Assert::AreEqual(result.compare("None"), 0);
		}

		TEST_METHOD(validateClauses_success)
		{
			vector<string> clauses{ "assign a", "Select a" };
			string result = QueryValidator::validateClauses(clauses);
			Assert::AreEqual(result.compare(""), 0);
		}

		TEST_METHOD(validateClauses_noSpaceInDeclaration)
		{
			vector<string> clauses{ "assign", "Select a" };
			string result = QueryValidator::validateClauses(clauses);
			Assert::AreEqual(result.compare("invalid query"), 0);
		}

		TEST_METHOD(validateClauses_noSelectStatement)
		{
			vector<string> clauses{ "assign a", "while w" };
			string result = QueryValidator::validateClauses(clauses);
			Assert::AreEqual(result.compare("invalid query"), 0);
		}

		TEST_METHOD(validateDeclarations_success)
		{
			unordered_map<string, string> declarations{ {"a", "assign"}, {"a1", "assign"}, {"a2", "assign"}, {"w", "while"} };
			string actual = QueryValidator::validateDeclarations(declarations);
			string expected = "";
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(validateDeclarations_invalidType)
		{
			unordered_map<string, string> declarations{ {"a", "assign"}, {"h", "hello"} };
			string actual = QueryValidator::validateDeclarations(declarations);
			string expected = "invalid query";
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(validateDeclarations_invalidName)
		{
			unordered_map<string, string> declarations{ {"a12_", "assign"}, {"h", "hello"} };
			string actual = QueryValidator::validateDeclarations(declarations);
			string expected = "invalid query";
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(validateSelectedVar_success)
		{
			vector<string> selectedVar = { "a" };
			unordered_map<string, string> declarationsMap = { {"a", "assign"}, {"s", "stmt"} };
			string actual = QueryValidator::validateSelectedVar(selectedVar, declarationsMap);
			string expected = "";
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(validateSelectedVar_selectBoolean_success)
		{
			vector<string> selectedVar = { "BOOLEAN" };
			unordered_map<string, string> declarationsMap = { {"a", "assign"}, {"s", "stmt"} };
			string actual = QueryValidator::validateSelectedVar(selectedVar, declarationsMap);
			string expected = "";
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(validateSelectedVar_selectTuple_success)
		{
			vector<string> selectedVar = { "a", "s" };
			unordered_map<string, string> declarationsMap = { {"a", "assign"}, {"s", "stmt"} };
			string actual = QueryValidator::validateSelectedVar(selectedVar, declarationsMap);
			string expected = "";
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(validateSelectedVar_varNotDeclared)
		{
			vector<string> selectedVar = { "b" };
			unordered_map<string, string> declarationsMap = { {"a", "assign"}, {"s", "stmt"} };
			string actual = QueryValidator::validateSelectedVar(selectedVar, declarationsMap);
			string expected = "selected variable not found";
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(validateSelectedVar_invalidVarName)
		{
			vector<string> selectedVar = { "hello_1" };
			unordered_map<string, string> declarationsMap = { {"a", "assign"}, {"s", "stmt"} };
			string actual = QueryValidator::validateSelectedVar(selectedVar, declarationsMap);
			string expected = "invalid variable name";
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(validateSelectedVar_selectTupleBoolean)
		{
			vector<string> selectedVar = { "a", "BOOLEAN" };
			unordered_map<string, string> declarationsMap = { {"a", "assign"}, {"s", "stmt"} };
			string actual = QueryValidator::validateSelectedVar(selectedVar, declarationsMap);
			string expected = "too many selected variable for boolean";
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(validateSelectedVar_selectAttributeSynonym_typeProcName_success)
		{
			vector<string> selectedVar = { "p.procName", "c.procName" };
			unordered_map<string, string> declarationsMap = { {"p", "procedure"}, {"c", "call"}, {"v", "variable"}, {"r", "read"}, {"pr", "print"} };
			string actual = QueryValidator::validateSelectedVar(selectedVar, declarationsMap);
			string expected = "";
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(validateSelectedVar_selectAttributeSynonym_typeVarName_success)
		{
			vector<string> selectedVar = { "v.varName", "r.varName", "pr.varName" };
			unordered_map<string, string> declarationsMap = { {"p", "procedure"}, {"c", "call"}, {"v", "variable"}, {"r", "read"}, {"pr", "print"} };
			string actual = QueryValidator::validateSelectedVar(selectedVar, declarationsMap);
			string expected = "";
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(validateSelectedVar_selectAttributeSynonym_typeValue_success)
		{
			vector<string> selectedVar = { "c.value", "c1.value" };
			unordered_map<string, string> declarationsMap = { {"c", "constant"}, {"c1", "constant"} };
			string actual = QueryValidator::validateSelectedVar(selectedVar, declarationsMap);
			string expected = "";
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(validateSelectedVar_selectAttributeSynonym_typeStmtNumber_success)
		{
			vector<string> selectedVar = { "s.stmt#", "r.stmt#", "p.stmt#", "c.stmt#", "w.stmt#", "i.stmt#", "a.stmt#" };
			unordered_map<string, string> declarationsMap = { {"s", "stmt"}, {"r", "read"}, {"p", "print"}, {"c", "call"}, {"w", "while"}, {"i", "if"}, {"a", "assign"} };
			string actual = QueryValidator::validateSelectedVar(selectedVar, declarationsMap);
			string expected = "";
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(validateSelectedVar_selectAttributeSynonym_invalidAttribute)
		{
			vector<string> selectedVar = { "s.stmt#", "r.stmt#", "p.procName" };
			unordered_map<string, string> declarationsMap = { {"s", "stmt"}, {"r", "read"}, {"p", "print"}, {"c", "call"}, {"w", "while"}, {"i", "if"}, {"a", "assign"} };
			string actual = QueryValidator::validateSelectedVar(selectedVar, declarationsMap);
			string expected = "invalid synonym attributes";
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(validateSuchThatParam_success)
		{
			vector<pair<string, pair<string, string>>> param{ {"Parent", { "ifs", "_" }} };
			unordered_map<string, string> declarationsMap = { {"ifs", "if"}, {"s", "stmt"} };
			string actual = QueryValidator::validateSuchThatParam(param, declarationsMap);
			string expected = "";
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(validateSuchThatParam_invalidRelName)
		{
			vector<pair<string, pair<string, string>>> param{ {"IfWhile", { "a", "_" }} };
			unordered_map<string, string> declarationsMap = { {"a", "assign"}, {"s", "stmt"} };
			string actual = QueryValidator::validateSuchThatParam(param, declarationsMap);
			string expected = "invalid query";
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(validateSuchThatParam_invalidParamType)
		{
			vector<pair<string, pair<string, string>>> param{ {"Parent", { "a", "_" }} };
			unordered_map<string, string> declarationsMap = { {"a", "assign"}, {"s", "stmt"} };
			string actual = QueryValidator::validateSuchThatParam(param, declarationsMap);
			string expected = "semantic error";
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(validateSuchThatParam_multipleClauses_success)
		{
			vector<pair<string, pair<string, string>>> param{ {"Parent", { "a", "_" }}, { "Follows", {"10", "11"} }, { "Modifies", {"3", "\"sum\""} }, { "Uses", {"5", "\"a\""} }, { "Next*", {"1", "10"} }, { "Affects*", {"4", "10"} } };
			unordered_map<string, string> declarationsMap = { {"a", "if"}, {"s", "stmt"} };
			string actual = QueryValidator::validateSuchThatParam(param, declarationsMap);
			string expected = "";
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(validatePatternParam_success)
		{
			vector<pair<string, pair<string, string>>> param{ {"a", {"_", "\"x*y+z\""}} };
			unordered_map<string, string> declarationsMap = { {"a", "assign"}, {"s", "stmt"} };
			string actual = QueryValidator::validatePatternParam(param, declarationsMap);
			string expected = "";
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(validatePatternParam_invalidStmtName)
		{
			vector<pair<string, pair<string, string>>> param{ {"a_a", {"_", "'x*y+z'"}} };
			unordered_map<string, string> declarationsMap = { {"a", "assign"}, {"s", "stmt"} };
			string actual = QueryValidator::validatePatternParam(param, declarationsMap);
			string expected = "invalid synonym";
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(validatePatternParam_stmtNotDeclared)
		{
			vector<pair<string, pair<string, string>>> param{ {"b", {"_", "'x*y+z'"}} };
			unordered_map<string, string> declarationsMap = { {"a", "assign"}, {"s", "stmt"} };
			string actual = QueryValidator::validatePatternParam(param, declarationsMap);
			string expected = "statement not found";
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(validatePatternParam_stmtTypeNotAssign)
		{
			vector<pair<string, pair<string, string>>> param{ {"s", {"_", "'x*y+z'"}} };
			unordered_map<string, string> declarationsMap = { {"a", "assign"}, {"s", "stmt"} };
			string actual = QueryValidator::validatePatternParam(param, declarationsMap);
			string expected = "statement type is invalid";
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(validatePatternParam_multipleClauses_success)
		{
			vector<pair<string, pair<string, string>>> param{ {"a", {"_", "\"x\""}}, {"a1", {"_", "\"y\""}}, {"ifs", {"\"x\"", "_,_"}}, {"while1", {"\"y\"", "_"}}, {"ifs", {"_", "_,_"}} };
			unordered_map<string, string> declarationsMap = { {"a", "assign"}, {"a1", "assign"}, {"ifs", "if"}, {"while1", "while"} };
			string actual = QueryValidator::validatePatternParam(param, declarationsMap);
			string expected = "";
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(validateWithParam_multipleClauses_intNoAttrValue_success)
		{
			vector<pair<string, string>> param{ {"2", "5"}, {"pl", "5"}, {"9", "pl"} };
			unordered_map<string, string> declarationsMap = { {"pl", "prog_line"} };
			string actual = QueryValidator::validateWithParam(param, declarationsMap);
			string expected = "";
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(validateWithParam_multipleClauses_attrStmtNo_success)
		{
			vector<pair<string, string>> param{ {"s.stmt#", "1"}, {"2", "s.stmt#"}, 
			{"r.stmt#", "1"}, {"2", "r.stmt#"}, {"p.stmt#", "1"}, {"2", "p.stmt#"}, 
			{"w.stmt#", "1"}, {"2", "w.stmt#"}, {"ifs.stmt#", "1"}, {"2", "ifs.stmt#"}, 
			{"a.stmt#", "1"}, {"2", "a.stmt#"}, {"c.stmt#", "1"}, {"2", "c.stmt#"}, 
			{"s.stmt#", "r.stmt#"}, {"r.stmt#", "p.stmt#"}, {"p.stmt#", "w.stmt#"},
			{"w.stmt#", "ifs.stmt#"}, {"ifs.stmt#", "a.stmt#"}, {"a.stmt#", "c.stmt#"},
			{"c.stmt#", "s.stmt#"} };
			unordered_map<string, string> declarationsMap = { {"s", "stmt"}, {"r", "read"}, 
			{"p", "print"}, {"w", "while"}, {"ifs", "if"}, {"a", "assign"}, {"c", "call"} };
			string actual = QueryValidator::validateWithParam(param, declarationsMap);
			string expected = "";
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(validateWithParam_multipleClauses_attrValue_success)
		{
			vector<pair<string, string>> param{ {"c.value", "1"}, {"2", "c.value"}, {"c1.value", "3"}, {"4", "c1.value"}, {"c.value", "c1.value"} };
			unordered_map<string, string> declarationsMap = { {"c", "constant"}, {"c1", "constant"} };
			string actual = QueryValidator::validateWithParam(param, declarationsMap);
			string expected = "";
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(validateWithParam_multipleClauses_strNoAttrValue_success)
		{
			vector<pair<string, string>> param{ {"\"hello\"", "\"hello\""}, {"\"asd4\"", "\"asd9\""}, {"\"h3ll0\"", "\"hell0\""} };
			unordered_map<string, string> declarationsMap = { {"s", "stmt"}, {"p", "procedure"} };
			string actual = QueryValidator::validateWithParam(param, declarationsMap);
			string expected = "";
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(validateWithParam_multipleClauses_attrProcName_success)
		{
			vector<pair<string, string>> param{ {"c.procName", "\"hello0\""}, {"\"hello1\"", "c.procName"}, {"p.procName", "\"hello2\""}, {"\"hello3\"", "p.procName"}, {"c.procName", "p.procName"} };
			unordered_map<string, string> declarationsMap = { {"c", "call"}, {"p", "procedure"} };
			string actual = QueryValidator::validateWithParam(param, declarationsMap);
			string expected = "";
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(validateWithParam_multipleClauses_attrVarName_success)
		{
			vector<pair<string, string>> param{ {"v.varName", "\"add0\""}, {"\"add1\"", "v.varName"}, 
			{"r.varName", "\"add2\""}, {"\"add3\"", "r.varName"}, {"p.varName", "\"add4\""}, {"\"add5\"", "p.varName"},
			{"v.varName", "r.varName"}, {"r.varName", "p.varName"}, {"p.varName", "v.varName"} };
			unordered_map<string, string> declarationsMap = { {"v", "variable"}, {"r", "read"}, {"p", "print"} };
			string actual = QueryValidator::validateWithParam(param, declarationsMap);
			string expected = "";
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(validateWithParam_singleClauses_differentAttrType)
		{
			vector<pair<string, string>> param{ {"v.varName", "s.stmt#"} };
			unordered_map<string, string> declarationsMap = { {"v", "variable"}, {"s", "stmt"} };
			string actual = QueryValidator::validateWithParam(param, declarationsMap);
			string expected = "semantic error";
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(validateWithParam_singleClauses_differentType)
		{
			vector<pair<string, string>> param{ {"5", "\"hello\""} };
			unordered_map<string, string> declarationsMap = { {"v", "variable"}, {"s", "stmt"} };
			string actual = QueryValidator::validateWithParam(param, declarationsMap);
			string expected = "semantic error";
			Assert::AreEqual(actual == expected, true);
		}

	};
}