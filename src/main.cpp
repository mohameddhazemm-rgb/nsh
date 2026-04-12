#include <iostream>
#include "executer.hpp"
#include "parser.hpp"
#include "history.hpp"

int main() {
    while (true) {
        std::cout << "nsh> ";
        std::string input;
        std::getline(std::cin, input);

        if (input.empty()) {
            continue;
        }

        // Add command to history
        History::addCommand(input);

        std::vector<std::string> tokens = Parser::parseInput(input);
        Executer::execute(tokens);
    }
}
