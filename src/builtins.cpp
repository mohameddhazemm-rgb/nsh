#include "builtins.hpp"
#include "unistd.h"
#include <iostream>

bool Builtins::handle(const std::vector<std::string> &tokens)
{
    if (tokens[0] == "exit")
        exit(EXIT_SUCCESS);
    else if (tokens[0] == "cd")
    {
        size_t argc = tokens.size();
        const char *path = argc == 1 ? "~" : tokens[1].c_str();

        if (argc > 2)
            std::cerr << "cd: too many arguments" << std::endl;
        else
        {
            int status = chdir(path);

            if (status != 0)
                std::cerr << "cd: failed to change directory: " << tokens[1] << std::endl;
        }

        return true;
    }

    return false;
}
