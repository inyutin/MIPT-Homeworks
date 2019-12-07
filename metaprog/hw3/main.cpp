#include "GenScatterHierarchy.h"
#include <iostream>


typedef GenScatterHierarchy<Typelist<int, Typelist<double, NullTypeList>>> widgetInfo;


int main() {
    widgetInfo obj;
    auto intName = Field<1, widgetInfo> (obj);
    auto doubleName = Field<1, widgetInfo> (obj);
    std::cout << "int: " << intName;
    std::cout << "double: " << doubleName;
    return 0;
}
