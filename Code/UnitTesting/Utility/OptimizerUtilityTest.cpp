#include "stdafx.h"
#include "CppUnitTest.h"
#include "OptimizerUtility.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(OptimizerUtilityTest)
	{
	public:
		TEST_METHOD(testSuchThatEntityType1)
		{
			std::string str = "a";
			int actual = OptimizerUtility::getSuchThatEntityType(str);
			Assert::AreEqual(actual, 1);
		}
		TEST_METHOD(testSuchThatEntityType2)
		{
			int actual = OptimizerUtility::getSuchThatEntityType("_");
			Assert::AreEqual(actual, 2);
		}
		TEST_METHOD(testSuchThatEntityType0String)
		{
			std::string s = { '"', 'v', 'a', 'r', '"' };
			int actual = OptimizerUtility::getSuchThatEntityType(s);
			Assert::AreEqual(actual, 0);
		}
		TEST_METHOD(testSuchThatEntityType0Num)
		{
			std::string str = "14";
			int actual = OptimizerUtility::getSuchThatEntityType(str);
			Assert::AreEqual(actual, 0);
		}
		TEST_METHOD(testWithSynonymTrue)
		{
			bool actual = OptimizerUtility::isWithSynonym("a.varName");
			Assert::IsTrue(actual);
		}
		TEST_METHOD(testWithSynonymFalseNum)
		{
			bool actual = OptimizerUtility::isWithSynonym("14");
			Assert::IsTrue(!actual);
		}
		TEST_METHOD(testWithSynonymFalseName)
		{
			bool actual = OptimizerUtility::isWithSynonym("procedure");
			Assert::IsTrue(!actual);
		}
		TEST_METHOD(testPatternEntityTypeFirst0)
		{
			std::string str = { '"', 's', '"' };
			int actual = OptimizerUtility::getPatternEntityType1(str);
			Assert::AreEqual(actual, 0);
		}
		TEST_METHOD(testPatternEntityTypeFirst1)
		{
			int actual = OptimizerUtility::getPatternEntityType1("v");
			Assert::AreEqual(actual, 1);
		}
		TEST_METHOD(testPatternEntityTypeFirst2)
		{
			int actual = OptimizerUtility::getPatternEntityType1("_");
			Assert::AreEqual(actual, 2);
		}
		TEST_METHOD(testPatternEntityTypeSecond0)
		{
			std::string str = { '"',  's', '"' };
			int actual = OptimizerUtility::getPatternEntityType2(str);
			Assert::AreEqual(actual, 0);
		}
		TEST_METHOD(testPatternEntityTypeSecond1)
		{
			std::string str = { '_', '"',  's', '"', '_' };
			int actual = OptimizerUtility::getPatternEntityType2(str);
			Assert::AreEqual(actual, 1);
		}
		TEST_METHOD(testPatternEntityTypeSecond2)
		{
			int actual = OptimizerUtility::getPatternEntityType2("_");
			Assert::AreEqual(actual, 2);
		}
		TEST_METHOD(testWithPriority1)
		{
			int actual = OptimizerUtility::getWithPriority(1);
			Assert::AreEqual(actual, 46);
		}
		TEST_METHOD(testWithPriority2)
		{
			int actual = OptimizerUtility::getWithPriority(2);
			Assert::AreEqual(actual, 32);
		}
		TEST_METHOD(testPatternPriorityA11)
		{
			int actual = OptimizerUtility::getPatternPriority("assign", 1, 1);
			Assert::AreEqual(actual, 30);
		}
		TEST_METHOD(testPatternPriorityA02)
		{
			int actual = OptimizerUtility::getPatternPriority("assign", 0, 2);
			Assert::AreEqual(actual, 38);
		}
		TEST_METHOD(testPatternPriorityIfs20)
		{
			int actual = OptimizerUtility::getPatternPriority("if", 2, 0);
			Assert::AreEqual(actual, 17);
		}
		TEST_METHOD(testSuchThatPriorityFollowStar1)
		{
			int actual = OptimizerUtility::getSuchThatPriority("Follows*", 1);
			Assert::AreEqual(actual, 33);
		}
		TEST_METHOD(testSuchThatPriorityNextStar2)
		{
			int actual = OptimizerUtility::getSuchThatPriority("Next*", 2);
			Assert::AreEqual(actual, 11);
		}
		TEST_METHOD(testSynonymtPriorityRead)
		{
			int actual = OptimizerUtility::getSynonymPriority("read");
			Assert::AreEqual(actual, 10);
		}
		TEST_METHOD(testSynonymPriorityProg)
		{
			int actual = OptimizerUtility::getSynonymPriority("prog_line");
			Assert::AreEqual(actual, 1);
		}
		TEST_METHOD(testNoSynSTPriority1)
		{
			int actual = OptimizerUtility::getNoSynSuchThatPriority("Follows", 2);
			Assert::AreEqual(actual, 6);
		}
		TEST_METHOD(testNoSynSTPriority2)
		{
			int actual = OptimizerUtility::getNoSynSuchThatPriority("Affects", 0);
			Assert::AreEqual(actual, 4);
		}
	};
}