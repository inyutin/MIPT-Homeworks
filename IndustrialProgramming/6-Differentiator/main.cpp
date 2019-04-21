#include <iostream>
#include <Derivative.hpp>

int main() {
    // Дифференцируем многочлен
    AST("3*x*x + 2*x + x + 5").derivative("answer/1.tex", 3);
}


