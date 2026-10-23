#include "Calculator.h"
#include <string>

int main(int argc, char* argv[]) {
    std::string filename = "tests.txt";
    if (argc > 1) {
        filename = argv[1];
    }

    Calculator::processFile(filename);

    return 0;
}
