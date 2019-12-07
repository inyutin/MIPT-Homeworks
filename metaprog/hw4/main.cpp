#include <iostream>
#include "headers/functor.h"

int function(int) {
    return 0;
}
class Y {};
class X {
public:
    float process(Y& y) {
        return 1.1;
    }
};


int main() {
// обычные функции
    auto functor1 = make_functor(&function, 1);
    std::cout << functor1.call() << "\n";

    // функции-члены
    auto functor2 = make_functor(&X::process, Y());
    X class_x;
    std::cout << functor2.call(class_x) << "\n";

    // лямбды
    Y class_y;
    auto functor3 = make_functor([](Y*) {return 2;}, class_y);
    std::cout << functor3.call() << "\n";
}