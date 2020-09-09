#include "Calculator.h"
#include <iostream>

int main() {

    // main loop
    bool is_running = true;

    Calculator calc;

    while (is_running) {
        // prompt
        std::string input;
        std::cout << "> ";
        std::getline(std::cin, input);
        std::string result = "invalid input";
        if (calc.parse(input)) {
            result = calc.execute();
        }
        std::cout << result << std::endl;
    }
}
