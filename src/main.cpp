#include <iostream>
#include <string>
#include <functional>
#include <map>

#include "utilities.h"

using Args = std::vector<std::string>;

std::map<std::string, std::function<void(const Args& arguments)>> availableCommands;

// splits user input on " " and finds the command and arguments
// since split doesn't split on just all spaces, an extra check is added to 
// ignore empty strings from split result
void extractCommandsAndArgs(const std::string& input, std::string& command, Args& args) {
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
void handleCommand(const std::string rawCommand) {
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

int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  availableCommands.emplace("exit", [](const Args& arguments) { std::exit(EXIT_SUCCESS); });
  availableCommands.emplace("echo", [](const Args& arguments) { 
    for (int i = 0; i < arguments.size(); i++) {
      std::cout << arguments.at(i);
      if (i < arguments.size() - 1) {
        std::cout << " ";
      }
    }
    std::cout << "\n";
  });

  while (true) {
    std::string command = {};

    std::cout << "$ ";
    std::getline(std::cin, command);

    handleCommand(command);    
  }
  
  
  return 0;
}
