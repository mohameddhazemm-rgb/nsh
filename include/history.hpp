#ifndef HISTORY_HPP
#define HISTORY_HPP

#include <vector>
#include <string>

class History {
public:
    static void addCommand(const std::string& command);
    static void display();
    static void clear();

private:
    static std::vector<std::string> history;
    static const size_t MAX_HISTORY = 100;  // Max number of commands to store
};

#endif

