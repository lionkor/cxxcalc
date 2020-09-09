#include <iostream>
#include "Calculator.h"
#include "version.h"

int main() {

    // main loop
    bool is_running = true;

    Calculator calc;

    std::cout << WELCOME_TEXT << std::flush;

    while (is_running) {
        // prompt
        std::string input;
        std::cout << "> ";
        std::getline(std::cin, input);

        if (input == "help") {
            std::cout << HELP_TEXT << std::endl;
            continue;
        } else if (input == "exit") {
            std::cout << GOODBYE_TEXT << std::endl;
            break;
        }

        std::string result = "invalid input";
        if (calc.parse(input)) {
            try {
                result = std::to_string(calc.execute());
            } catch (const std::exception& e) {
                dbg("execute failed: " << e.what() << std::endl);
            }
        }
        std::cout << " = " << result << std::endl;
    }
}
