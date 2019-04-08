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
			std::string result = QueryValidator::initialValidation("assign a; Select a");
			Assert::AreEqual(result.compare(""), 0);
		}

		TEST_METHOD(initialValidation_lengthZero)
		{
			std::string result = QueryValidator::initialValidation("");
			Assert::AreEqual(result.compare("invalid query"), 0);
		}

		TEST_METHOD(initialValidation_noSelectStatement)
		{
			std::string result = QueryValidator::initialValidation("assign a;");
			Assert::AreEqual(result.compare("invalid query"), 0);
		}

		TEST_METHOD(initialValidation_noVariableDeclaration)
		{
			std::string result = QueryValidator::initialValidation("Select a");
			Assert::AreEqual(result.compare("None"), 0);
		}

		TEST_METHOD(validateClauses_success)
		{
			std::vector<std::string> clauses{ "assign a", "Select a" };
			std::string result = QueryValidator::validateClauses(clauses);
			Assert::AreEqual(result.compare(""), 0);
		}

		TEST_METHOD(validateClauses_noSpaceInDeclaration)
		{
			std::vector<std::string> clauses{ "assign", "Select a" };
			std::string result = QueryValidator::validateClauses(clauses);
			Assert::AreEqual(result.compare("invalid query"), 0);
		}

		TEST_METHOD(validateClauses_noSelectStatement)
		{
			std::vector<std::string> clauses{ "assign a", "while w" };
			std::string result = QueryValidator::validateClauses(clauses);
			Assert::AreEqual(result.compare("invalid query"), 0);
		}

		TEST_METHOD(validateDeclarations_success)
		{
			std::unordered_map<std::string, std::string> declarations{ {"a", "assign"},
				{"a1", "assign"}, {"a2", "assign"}, {"w", "while"} };
			std::string actual = QueryValidator::validateDeclarations(declarations);
			std::string expected = "";
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(validateDeclarations_invalidType)
		{
			std::unordered_map<std::string, std::string> declarations{ {"a", "assign"},
				{"h", "hello"} };
			std::string actual = QueryValidator::validateDeclarations(declarations);
			std::string expected = "invalid query";
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(validateDeclarations_invalidName)
		{
			std::unordered_map<std::string, std::string> declarations{ {"a12_", "assign"},
				{"h", "hello"} };
			std::string actual = QueryValidator::validateDeclarations(declarations);
			std::string expected = "invalid query";
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(validateSelectedVar_success)
		{
			std::vector<std::string> selectedVar = { "a" };
			std::unordered_map<std::string, std::string> declarationsMap = { {"a", "assign"},
				{"s", "stmt"} };
			std::string actual = QueryValidator::validateSelectedVar(selectedVar, declarationsMap);
			std::string expected = "";
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(validateSelectedVar_selectBoolean_success)
		{
			std::vector<std::string> selectedVar = { "BOOLEAN" };
			std::unordered_map<std::string, std::string> declarationsMap = { {"a", "assign"},
				{"s", "stmt"} };
			std::string actual = QueryValidator::validateSelectedVar(selectedVar, declarationsMap);
			std::string expected = "";
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(validateSelectedVar_selectTuple_success)
		{
			std::vector<std::string> selectedVar = { "a", "s" };
			std::unordered_map<std::string, std::string> declarationsMap = { {"a", "assign"},
				{"s", "stmt"} };
			std::string actual = QueryValidator::validateSelectedVar(selectedVar, declarationsMap);
			std::string expected = "";
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(validateSelectedVar_varNotDeclared)
		{
			std::vector<std::string> selectedVar = { "b" };
			std::unordered_map<std::string, std::string> declarationsMap = { {"a", "assign"},
				{"s", "stmt"} };
			std::string actual = QueryValidator::validateSelectedVar(selectedVar, declarationsMap);
			std::string expected = "selected variable not found";
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(validateSelectedVar_invalidVarName)
		{
			std::vector<std::string> selectedVar = { "hello_1" };
			std::unordered_map<std::string, std::string> declarationsMap = { {"a", "assign"},
				{"s", "stmt"} };
			std::string actual = QueryValidator::validateSelectedVar(selectedVar, declarationsMap);
			std::string expected = "invalid variable name";
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(validateSelectedVar_selectTupleBoolean)
		{
			std::vector<std::string> selectedVar = { "a", "BOOLEAN" };
			std::unordered_map<std::string, std::string> declarationsMap = { {"a", "assign"},
				{"s", "stmt"} };
			std::string actual = QueryValidator::validateSelectedVar(selectedVar, declarationsMap);
			std::string expected = "too many selected variable for boolean";
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(validateSelectedVar_selectAttributeSynonym_typeProcName_success)
		{
			std::vector<std::string> selectedVar = { "p.procName", "c.procName" };
			std::unordered_map<std::string, std::string> declarationsMap = { {"p", "procedure"},
				{"c", "call"}, {"v", "variable"}, {"r", "read"}, {"pr", "print"} };
			std::string actual = QueryValidator::validateSelectedVar(selectedVar, declarationsMap);
			std::string expected = "";
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(validateSelectedVar_selectAttributeSynonym_typeVarName_success)
		{
			std::vector<std::string> selectedVar = { "v.varName", "r.varName", "pr.varName" };
			std::unordered_map<std::string, std::string> declarationsMap = { {"p", "procedure"},
				{"c", "call"}, {"v", "variable"}, {"r", "read"}, {"pr", "print"} };
			std::string actual = QueryValidator::validateSelectedVar(selectedVar, declarationsMap);
			std::string expected = "";
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(validateSelectedVar_selectAttributeSynonym_typeValue_success)
		{
			std::vector<std::string> selectedVar = { "c.value", "c1.value" };
			std::unordered_map<std::string, std::string> declarationsMap = { {"c", "constant"},
				{"c1", "constant"} };
			std::string actual = QueryValidator::validateSelectedVar(selectedVar, declarationsMap);
			std::string expected = "";
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(validateSelectedVar_selectAttributeSynonym_typeStmtNumber_success)
		{
			std::vector<std::string> selectedVar = { "s.stmt#", "r.stmt#", "p.stmt#", "c.stmt#",
					"w.stmt#", "i.stmt#", "a.stmt#" };
			std::unordered_map<std::string, std::string> declarationsMap = { {"s", "stmt"},
				{"r", "read"}, {"p", "print"}, {"c", "call"}, {"w", "while"}, {"i", "if"},
				{"a", "assign"} };
			std::string actual = QueryValidator::validateSelectedVar(selectedVar, declarationsMap);
			std::string expected = "";
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(validateSelectedVar_selectAttributeSynonym_invalidAttribute)
		{
			std::vector<std::string> selectedVar = { "s.stmt#", "r.stmt#", "p.procName" };
			std::unordered_map<std::string, std::string> declarationsMap = { {"s", "stmt"},
				{"r", "read"}, {"p", "print"}, {"c", "call"}, {"w", "while"}, {"i", "if"},
				{"a", "assign"} };
			std::string actual = QueryValidator::validateSelectedVar(selectedVar, declarationsMap);
			std::string expected = "invalid synonym attributes";
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(validateSuchThatParam_success)
		{
			std::vector<std::pair<std::string, std::pair<std::string, std::string>>>
				param{ {"Parent", { "ifs", "_" }} };
			std::unordered_map<std::string, std::string> declarationsMap = { {"ifs", "if"},
				{"s", "stmt"} };
			std::string actual = QueryValidator::validateSuchThatParam(param, declarationsMap);
			std::string expected = "";
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(validateSuchThatParam_invalidRelName)
		{
			std::vector<std::pair<std::string, std::pair<std::string, std::string>>>
				param{ {"IfWhile", { "a", "_" }} };
			std::unordered_map<std::string, std::string> declarationsMap = { {"a", "assign"},
				{"s", "stmt"} };
			std::string actual = QueryValidator::validateSuchThatParam(param, declarationsMap);
			std::string expected = "invalid query";
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(validateSuchThatParam_invalidParamType)
		{
			std::vector<std::pair<std::string, std::pair<std::string, std::string>>>
				param{ {"Parent", { "a", "_" }} };
			std::unordered_map<std::string, std::string> declarationsMap = { {"a", "assign"},
				{"s", "stmt"} };
			std::string actual = QueryValidator::validateSuchThatParam(param, declarationsMap);
			std::string expected = "semantic error";
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(validateSuchThatParam_multipleClauses_success)
		{
			std::vector<std::pair<std::string, std::pair<std::string, std::string>>>
				param{ {"Parent", { "a", "_" }}, { "Follows", {"10", "11"} },
					{ "Modifies", {"3", "\"sum\""} }, { "Uses", {"5", "\"a\""} },
					{ "Next*", {"1", "10"} }, { "Affects*", {"4", "10"} } };
			std::unordered_map<std::string, std::string> declarationsMap = { {"a", "if"},
				{"s", "stmt"} };
			std::string actual = QueryValidator::validateSuchThatParam(param, declarationsMap);
			std::string expected = "";
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(validatePatternParam_success)
		{
			std::vector<std::pair<std::string, std::pair<std::string, std::string>>>
				param{ {"a", {"_", "\"x*y+z\""}} };
			std::unordered_map<std::string, std::string> declarationsMap = { {"a", "assign"},
				{"s", "stmt"} };
			std::string actual = QueryValidator::validatePatternParam(param, declarationsMap);
			std::string expected = "";
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(validatePatternParam_invalidStmtName)
		{
			std::vector<std::pair<std::string, std::pair<std::string, std::string>>>
				param{ {"a_a", {"_", "'x*y+z'"}} };
			std::unordered_map<std::string, std::string> declarationsMap = { {"a", "assign"},
				{"s", "stmt"} };
			std::string actual = QueryValidator::validatePatternParam(param, declarationsMap);
			std::string expected = "invalid synonym";
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(validatePatternParam_stmtNotDeclared)
		{
			std::vector<std::pair<std::string, std::pair<std::string, std::string>>>
				param{ {"b", {"_", "'x*y+z'"}} };
			std::unordered_map<std::string, std::string> declarationsMap = { {"a", "assign"},
				{"s", "stmt"} };
			std::string actual = QueryValidator::validatePatternParam(param, declarationsMap);
			std::string expected = "statement not found";
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(validatePatternParam_stmtTypeNotAssign)
		{
			std::vector<std::pair<std::string, std::pair<std::string, std::string>>> param{ {"s", {"_", "'x*y+z'"}} };
			std::unordered_map<std::string, std::string> declarationsMap = { {"a", "assign"},
				{"s", "stmt"} };
			std::string actual = QueryValidator::validatePatternParam(param, declarationsMap);
			std::string expected = "statement type is invalid";
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(validatePatternParam_multipleClauses_success)
		{
			std::vector<std::pair<std::string, std::pair<std::string, std::string>>>
				param{ {"a", {"_", "\"x\""}}, {"a1", {"_", "\"y\""}}, {"ifs", {"\"x\"", "_,_"}},
					{"while1", {"\"y\"", "_"}}, {"ifs", {"_", "_,_"}} };
			std::unordered_map<std::string, std::string> declarationsMap = { {"a", "assign"},
				{"a1", "assign"}, {"ifs", "if"}, {"while1", "while"} };
			std::string actual = QueryValidator::validatePatternParam(param, declarationsMap);
			std::string expected = "";
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(validateWithParam_multipleClauses_intNoAttrValue_success)
		{
			std::vector<std::pair<std::string, std::string>>
				param{ {"2", "5"}, {"pl", "5"}, {"9", "pl"} };
			std::unordered_map<std::string, std::string> declarationsMap = { {"pl", "prog_line"} };
			std::string actual = QueryValidator::validateWithParam(param, declarationsMap);
			std::string expected = "";
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(validateWithParam_multipleClauses_attrStmtNo_success)
		{
			std::vector<std::pair<std::string, std::string>>
				param{ {"s.stmt#", "1"}, {"2", "s.stmt#"},
					{"r.stmt#", "1"}, {"2", "r.stmt#"}, {"p.stmt#", "1"}, {"2", "p.stmt#"}, 
					{"w.stmt#", "1"}, {"2", "w.stmt#"}, {"ifs.stmt#", "1"}, {"2", "ifs.stmt#"}, 
					{"a.stmt#", "1"}, {"2", "a.stmt#"}, {"c.stmt#", "1"}, {"2", "c.stmt#"}, 
					{"s.stmt#", "r.stmt#"}, {"r.stmt#", "p.stmt#"}, {"p.stmt#", "w.stmt#"},
					{"w.stmt#", "ifs.stmt#"}, {"ifs.stmt#", "a.stmt#"}, {"a.stmt#", "c.stmt#"},
					{"c.stmt#", "s.stmt#"} };
			std::unordered_map<std::string, std::string> declarationsMap = { {"s", "stmt"},
				{"r", "read"}, {"p", "print"}, {"w", "while"}, {"ifs", "if"}, {"a", "assign"},
				{"c", "call"} };
			std::string actual = QueryValidator::validateWithParam(param, declarationsMap);
			std::string expected = "";
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(validateWithParam_multipleClauses_attrValue_success)
		{
			std::vector<std::pair<std::string, std::string>> param{ {"c.value", "1"},
				{"2", "c.value"}, {"c1.value", "3"}, {"4", "c1.value"}, {"c.value", "c1.value"} };
			std::unordered_map<std::string, std::string> declarationsMap = { {"c", "constant"},
				{"c1", "constant"} };
			std::string actual = QueryValidator::validateWithParam(param, declarationsMap);
			std::string expected = "";
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(validateWithParam_multipleClauses_strNoAttrValue_success)
		{
			std::vector<std::pair<std::string, std::string>> param{ {"\"hello\"", "\"hello\""},
				{"\"asd4\"", "\"asd9\""}, {"\"h3ll0\"", "\"hell0\""} };
			std::unordered_map<std::string, std::string> declarationsMap = { {"s", "stmt"},
				{"p", "procedure"} };
			std::string actual = QueryValidator::validateWithParam(param, declarationsMap);
			std::string expected = "";
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(validateWithParam_multipleClauses_attrProcName_success)
		{
			std::vector<std::pair<std::string, std::string>> param{ {"c.procName", "\"hello0\""},
				{"\"hello1\"", "c.procName"}, {"p.procName", "\"hello2\""},
				{"\"hello3\"", "p.procName"}, {"c.procName", "p.procName"} };
			std::unordered_map<std::string, std::string> declarationsMap = { {"c", "call"},
				{"p", "procedure"} };
			std::string actual = QueryValidator::validateWithParam(param, declarationsMap);
			std::string expected = "";
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(validateWithParam_multipleClauses_attrVarName_success)
		{
			std::vector<std::pair<std::string, std::string>> param{ {"v.varName", "\"add0\""},
				{"\"add1\"", "v.varName"}, {"r.varName", "\"add2\""}, {"\"add3\"", "r.varName"},
				{"p.varName", "\"add4\""}, {"\"add5\"", "p.varName"}, {"v.varName", "r.varName"},
				{"r.varName", "p.varName"}, {"p.varName", "v.varName"} };
			std::unordered_map<std::string, std::string> declarationsMap = { {"v", "variable"}, {"r", "read"}, {"p", "print"} };
			std::string actual = QueryValidator::validateWithParam(param, declarationsMap);
			std::string expected = "";
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(validateWithParam_singleClauses_differentAttrType)
		{
			std::vector<std::pair<std::string, std::string>> param{ {"v.varName", "s.stmt#"} };
			std::unordered_map<std::string, std::string> declarationsMap = { {"v", "variable"},
				{"s", "stmt"} };
			std::string actual = QueryValidator::validateWithParam(param, declarationsMap);
			std::string expected = "semantic error";
			Assert::AreEqual(actual == expected, true);
		}

		TEST_METHOD(validateWithParam_singleClauses_differentType)
		{
			std::vector<std::pair<std::string, std::string>> param{ {"5", "\"hello\""} };
			std::unordered_map<std::string, std::string> declarationsMap = { {"v", "variable"},
				{"s", "stmt"} };
			std::string actual = QueryValidator::validateWithParam(param, declarationsMap);
			std::string expected = "semantic error";
			Assert::AreEqual(actual == expected, true);
		}

	};
}