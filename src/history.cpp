#include "history.hpp"
#include <iostream>

std::vector<std::string> History::history;

void History::addCommand(const std::string& command) {
    if (history.size() >= MAX_HISTORY) {
        history.erase(history.begin());  // Remove the oldest command
    }
    history.push_back(command);
}

void History::display() {
    if (history.empty()) {
        std::cout << "No history available." << std::endl;
        return;
    }

    for (size_t i = 0; i < history.size(); ++i) {
        std::cout << i + 1 << " " << history[i] << std::endl;
    }
}

void History::clear() {
    history.clear();
}
