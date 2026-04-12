#include "executer.hpp"

#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <iostream>

#include "builtins.hpp"

void Executer::execute(const std::vector<std::string>& tokens) {
  if (tokens.empty()) return;

  std::vector<std::string> args = tokens;

  // -------------------------
  // Detect background (&)
  // -------------------------
  bool background = false;
  if (args.back() == "&") {
    background = true;
    args.pop_back();
  }

  if (args.empty()) return;

  // -------------------------
  // Detect output redirection (>)
  // -------------------------
  std::string outputFile;
  for (size_t i = 0; i < args.size(); ++i) {
    if (args[i] == ">") {
      if (i + 1 >= args.size()) {
        std::cerr << "syntax error: no output file specified" << std::endl;
        return;
      }

      outputFile = args[i + 1];

      // Remove '>' and filename
      args.erase(args.begin() + i, args.begin() + i + 2);
      break;
    }
  }

  if (args.empty()) return;

  // -------------------------
  // Builtins (no redirection support for now)
  // -------------------------
  if (Builtins::handle(args)) {
    return;
  }

  // Prepare argv
  std::vector<const char*> argv;
  for (const std::string& token : args) {
    argv.push_back(token.c_str());
  }
  argv.push_back(nullptr);

  pid_t pid = fork();

  if (pid < 0) {
    std::cerr << args[0] << ": failed to execute command" << std::endl;
  } 
  else if (pid == 0) {
    // -------------------------
    // CHILD PROCESS
    // -------------------------

    // Handle output redirection
    if (!outputFile.empty()) {
      int fd = open(outputFile.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);

      if (fd < 0) {
        std::cerr << "failed to open file: " << outputFile << std::endl;
        std::exit(EXIT_FAILURE);
      }

      dup2(fd, STDOUT_FILENO); // redirect stdout → file
      close(fd);
    }

    int status = execvp(argv[0], const_cast<char* const*>(argv.data()));

    if (status != 0) {
      std::string msg = "failed to execute command";

      if (errno == ENOENT) {
        msg = "command not found";
      }

      std::cerr << args[0] << ": " << msg << std::endl;
    }

    std::exit(EXIT_FAILURE);
  } 
  else {
    // -------------------------
    // PARENT PROCESS
    // -------------------------
    if (!background) {
      waitpid(pid, nullptr, 0);
    } else {
      std::cout << "[background pid " << pid << "]" << std::endl;
    }
  }
}
