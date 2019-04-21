#include "Equation.hpp"

static const double ZERO = 0.0000001;

bool IsZero(const std::complex<double>& num) {
    return (abs(num.real()) <= ZERO && abs(num.imag()) <= ZERO);
}

std::complex<double> Equation::SolveLinear(const std::complex<double>& b, const std::complex<double>& c) {
    // Сделать assert

    if (IsZero(b)) {
        throw std::invalid_argument("Wrong arguments, b != 0");
    }
    return -c/b;
}

std::vector<std::complex<double>>
Equation::SolveQuadratic (const std::complex<double>& a, const std::complex<double>& b, const std::complex<double>& c) {
    if (IsZero(a)) {
        if (IsZero(b)) {
            throw std::invalid_argument("Wrong arguments, a != 0 || b != 0");
        }
        return {SolveLinear(b, c)};
    }
    // Перевод на линейное уравнение
    const std::complex<double> D = (b*b-4.0*a*c);
    if (D.real() >= 0 && D.imag() == 0) {
        if (IsZero(D)) {
            return {-b/(2.0*a)};
        }
        const std::complex<double> sqrtD = sqrt(D);
        return {(-b+sqrtD)/(2.0*a), (-b-sqrtD)/(2.0*a)};
    }
    const std::complex<double> sqrtD = sqrt(abs(D));
    const std::complex<double> i (0.0, 1.0);
    return {(-b+i*sqrtD)/(2.0*a), (-b-i*sqrtD)/(2.0*a)};
}

