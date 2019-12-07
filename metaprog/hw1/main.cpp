#include <iostream>
#include <vector>
#include <tuple>
#include "csv.h"

int main(){
    std::string fileName = "./samples/test.csv";
    try {
        csv<int, double, std::string> CSV;
        auto result = CSV.readFile(fileName);
        for (auto& elem: result) {
            std::cout
                << std::get<0>(elem) << " "
                << std::get<1>(elem) << " "
                << std::get<2>(elem) << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}