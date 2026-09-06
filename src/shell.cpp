#include <iostream>
#include <filesystem>

#include "utilities.h"
#include "shell.h"

namespace fs = std::filesystem;

// splits user input on " " and finds the command and arguments
// since split doesn't split on just all spaces, an extra check is added to 
// ignore empty strings from split result
void Shell::extractCommandsAndArgs(const std::string& input, std::string& command, Args& args) {
  std::vector<std::string> commandAndArguments = split(' ', input); 
  command = commandAndArguments.at(0);
  if (commandAndArguments.size() > 1) {
    for (int i = 1; i < commandAndArguments.size(); i++) {
      if (commandAndArguments.at(i) == "") {
        continue;
      } else {
        args.emplace_back(commandAndArguments.at(i));
      }
    }
  }
}

// Parses raw command and correctly passes the arguments into the correct command handler
void Shell::handleCommand(const std::string rawCommand) {
  std::string command {};
  Args arguments {};

  extractCommandsAndArgs(rawCommand, command, arguments);

  auto it = availableCommands.find(command);
  if (it != availableCommands.end()) {
    it->second(arguments);
  } else {
    std::cout << command << ": command not found" << std::endl;
  }
}

// adds a command to the available and types registry
void Shell::addCommand(std::string command, std::string type, std::function<void(const Args& arguments)> handler) {
  availableCommands.emplace(command, handler);
  commandTypes.emplace(command, type);
}

// checks to see if the command is found in a path and has executable permissions
// if it is found and has permissions returns true
// other wise returns false
bool Shell::isCommandInPath(const std::string& command, std::string& commandPath) {
  std::string pathVar = getEnvVar("PATH");
  std::vector<std::string> paths = split(':', pathVar);
  
  for (auto path : paths) {
    if (fs::exists(path)) {
      for (const auto& entry : fs::directory_iterator(path)) {
        // skip file thats not command
        std::string filename = entry.path().filename();
        if (command != entry.path().filename()) continue;

        fs::file_status status = fs::status(path);
        fs::perms permissions = status.permissions();
        // checking for execution permissions
        if (((permissions & fs::perms::owner_exec) != fs::perms::none) ||
            ((permissions & fs::perms::group_exec) != fs::perms::none) || 
            ((permissions & fs::perms::others_exec) != fs::perms::none) 
        ) {
          commandPath = entry.path();
          return true;
        } else {
          return false;
        }
      }
    }
  }

  return false;
}

void Shell::registerCommands() {
  addCommand("echo", "builtin", [](const Args& arguments) { 
    for (int i = 0; i < arguments.size(); i++) {
      std::cout << arguments.at(i);
      if (i < arguments.size() - 1) {
        std::cout << " ";
      }
    }
    std::cout << "\n";
  });

  addCommand("exit", "builtin", [this](const Args& arguments) { 
    isRunning = false;
  });

  addCommand("type", "builtin", [this](const Args& arguments) {
    std::string commandToCheck = arguments.at(0);
    auto type = commandTypes.find(commandToCheck); 
    if (type != commandTypes.end()) {
      std::cout << commandToCheck << " is a shell " << type->second << std::endl;
    } else {
      std::string commandPath {};
      if (isCommandInPath(commandToCheck, commandPath)) {
        std::cout << commandToCheck << " is " << commandPath << std::endl;
      } else {
        std::cout << commandToCheck << ": not found" << std::endl;
      }
    }
  });
}

void Shell::run() {
  isRunning = true;
  registerCommands();

  while (isRunning) {
    std::string command = {};

    std::cout << "$ ";
    std::getline(std::cin, command);

    handleCommand(command);    
  }
}
