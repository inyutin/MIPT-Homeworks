#ifndef SOLVEQUADRATICEQUATION_HPP_
#define SOLVEQUADRATICEQUATION_HPP_

#include <vector>
#include <complex>
#include <assert.h>
#include <stdexcept>

//-------------------------------------------------------
//! Solve different equations
//! @note handle exceptions
//! @note equation must contain unknown variable (otherwise it does not make sense)
//-------------------------------------------------------
namespace Equation {
    //-------------------------------------------------------
    //! Solve linear(bx+c=0) equation
    //! @param [in] b b-coefficient
    //! @param [in] c c-coefficient
    //!
    //! @return root
    //!
    //! @code SolveLinear(1.0, 1.0)
    //! @code SolveQuadratic(std::complex<double> (1.0, 1.0), 0.0)
    //-------------------------------------------------------
    std::complex<double> SolveLinear(const std::complex<double>& b, const std::complex<double>& c);

    //-------------------------------------------------------
    //! Solve square(ax2+bx+c=0) equation
    //! @param [in] a a-coefficient
    //! @param [in] b b-coefficient
    //! @param [in] c c-coefficient
    //!
    //! @return vector with roots
    //!
    //! @code SolveQuadratic(1.0, 1.0, 1.0)
    //! @code SolveQuadratic(std::complex<double> (1.0, 1.0), 0.0, 0.0)
    //-------------------------------------------------------
    std::vector<std::complex<double>>
    SolveQuadratic(const std::complex<double>& a, const std::complex<double>& b, const std::complex<double>& c);
}

#endif