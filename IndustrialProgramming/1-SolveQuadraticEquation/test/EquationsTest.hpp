#include <cxxtest/TestSuite.h>
#include "../Equation.hpp"
#include "../Equation.cpp"

class EquationTest : public CxxTest::TestSuite {
public:
    // Quadratic Tests
    void testRealSingleRoot(void) {
        std::vector<std::complex<double>> answer {std::complex<double> (1.0, 0.0)};
        TS_ASSERT_EQUALS(Equation::SolveQuadratic(1.0, -2.0, 1.0), answer);
    }

    void testRealDoubleRoot(void) {
        std::vector<std::complex<double>> answer {std::complex<double> (1.0, 0.0), std::complex<double> (-1.0, 0.0)};
        TS_ASSERT_EQUALS(Equation::SolveQuadratic(1.0, 0.0, -1.0), answer);
    }

    void testComplexDoubleRoot(void) {
        std::vector<std::complex<double>> answer {std::complex<double> (0.0, 1.0), std::complex<double> (0.0, -1.0)};
        TS_ASSERT_EQUALS(Equation::SolveQuadratic(1.0, 0.0, 1.0), answer);
    }

    // Linear Tests
    void testLinearEquation(void) {
        std::complex<double>  answer (4.0, 0.0);
        TS_ASSERT_EQUALS(Equation::SolveLinear(1.0, -4.0), answer);
    }

    void testNullLinearEquation(void) {
        TS_ASSERT_THROWS_ANYTHING(Equation::SolveLinear(0.0, -4.0));
    }
    
    // Other Tests
    void testSendAssert(void) {
        TS_ASSERT_THROWS_ANYTHING(Equation::SolveQuadratic(0.0, 0.0, 1.0));
    }

    void testNegative(void) {
        std::vector<std::complex<double>> answer {std::complex<double> (4.0, 0.0)};
        TS_ASSERT_DIFFERS(Equation::SolveQuadratic(1.0, -25.0, 0.0), answer);
    }
};
