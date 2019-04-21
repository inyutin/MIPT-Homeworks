#include <iostream>

#include "GlobalVariables.h"
#include "CodeToByte.h"

static void showHelp() {
    std::cout <<"Usage: [switches] [programfile]\n"
                "  -c| Compile programfile\n"
                "  -e| Execute programfile\n"
                "  -h| Display this message\n";
}

int main(int argc, char* argv[]) {
    if (argc <= 1) {
        showHelp();
        return 0;
    }

    if (strcmp(argv[1], "-h") == 0 || argc <= 2) {
        showHelp();
        return 0;
    }

    if (strcmp(argv[1], "-c") == 0) {
        const std::string input = argv[2];
        Compile(input);
        return 0;
    }

    if (strcmp(argv[1], "-e") == 0) {
        const std::string input = argv[2];
        Execute(input);
        return 0;
    }

    return 0;
}