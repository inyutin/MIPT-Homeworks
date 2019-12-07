#include "typelist.h"
#include <iostream>

int main() {
    std::cout << getIndexOf<int, typelist<int, NullTypeList> > () << std::endl;

    std::cout << getIndexOf<int, typelist<double, NullTypeList> > () << std::endl;

    std::cout << getIndexOf<int, typelist<double, typelist<int, NullTypeList> > > () << std::endl;
    return 0;
}