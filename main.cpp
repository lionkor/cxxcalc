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
            try {
                result = std::to_string(calc.execute());
            } catch (const std::exception& e) {
                std::cout << "execute failed: " << e.what() << std::endl;
            }
        }
        std::cout << " = " << result << std::endl;
    }
}
