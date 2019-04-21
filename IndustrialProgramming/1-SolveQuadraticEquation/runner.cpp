/* Generated file, do not edit */

#ifndef CXXTEST_RUNNING
#define CXXTEST_RUNNING
#endif

#define _CXXTEST_HAVE_STD
#define _CXXTEST_HAVE_EH
#include <cxxtest/TestListener.h>
#include <cxxtest/TestTracker.h>
#include <cxxtest/TestRunner.h>
#include <cxxtest/RealDescriptions.h>
#include <cxxtest/TestMain.h>

bool suite_EquationTest_init = false;
#include "test/EquationsTest.hpp"

static EquationTest suite_EquationTest;

static CxxTest::List Tests_EquationTest = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_EquationTest( "./test/EquationsTest.hpp", 5, "EquationTest", suite_EquationTest, Tests_EquationTest );

static class TestDescription_suite_EquationTest_testRealSingleRoot : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_EquationTest_testRealSingleRoot() : CxxTest::RealTestDescription( Tests_EquationTest, suiteDescription_EquationTest, 8, "testRealSingleRoot" ) {}
 void runTest() { suite_EquationTest.testRealSingleRoot(); }
} testDescription_suite_EquationTest_testRealSingleRoot;

static class TestDescription_suite_EquationTest_testRealDoubleRoot : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_EquationTest_testRealDoubleRoot() : CxxTest::RealTestDescription( Tests_EquationTest, suiteDescription_EquationTest, 13, "testRealDoubleRoot" ) {}
 void runTest() { suite_EquationTest.testRealDoubleRoot(); }
} testDescription_suite_EquationTest_testRealDoubleRoot;

static class TestDescription_suite_EquationTest_testComplexDoubleRoot : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_EquationTest_testComplexDoubleRoot() : CxxTest::RealTestDescription( Tests_EquationTest, suiteDescription_EquationTest, 18, "testComplexDoubleRoot" ) {}
 void runTest() { suite_EquationTest.testComplexDoubleRoot(); }
} testDescription_suite_EquationTest_testComplexDoubleRoot;

static class TestDescription_suite_EquationTest_testLinearEquation : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_EquationTest_testLinearEquation() : CxxTest::RealTestDescription( Tests_EquationTest, suiteDescription_EquationTest, 24, "testLinearEquation" ) {}
 void runTest() { suite_EquationTest.testLinearEquation(); }
} testDescription_suite_EquationTest_testLinearEquation;

static class TestDescription_suite_EquationTest_testNullLinearEquation : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_EquationTest_testNullLinearEquation() : CxxTest::RealTestDescription( Tests_EquationTest, suiteDescription_EquationTest, 29, "testNullLinearEquation" ) {}
 void runTest() { suite_EquationTest.testNullLinearEquation(); }
} testDescription_suite_EquationTest_testNullLinearEquation;

static class TestDescription_suite_EquationTest_testSendAssert : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_EquationTest_testSendAssert() : CxxTest::RealTestDescription( Tests_EquationTest, suiteDescription_EquationTest, 34, "testSendAssert" ) {}
 void runTest() { suite_EquationTest.testSendAssert(); }
} testDescription_suite_EquationTest_testSendAssert;

static class TestDescription_suite_EquationTest_testNegative : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_EquationTest_testNegative() : CxxTest::RealTestDescription( Tests_EquationTest, suiteDescription_EquationTest, 38, "testNegative" ) {}
 void runTest() { suite_EquationTest.testNegative(); }
} testDescription_suite_EquationTest_testNegative;

#include <cxxtest/Root.cpp>
const char* CxxTest::RealWorldDescription::_worldName = "cxxtest";
