#include <iostream>

#include "utilities.h"
#include "shell.h"

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

void Shell::addCommand(std::string command, std::string type, std::function<void(const Args& arguments)> handler) {
  availableCommands.emplace(command, handler);
  commandTypes.emplace(command, type);
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
      std::cout << commandToCheck << ": not found" << std::endl;
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
