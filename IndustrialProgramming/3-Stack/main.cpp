#include <iostream>
#include "HardStack.hpp"

int main() {
    int error = 0;
    ConstHardStack<int> q (5, error);
    q.Push(10, error);
    q.Push(5, error);
    q.Push(3, error);
    std::cout << q.getDump();
    return 0;
}