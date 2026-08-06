#include <iostream>
#include <string>
#include <functional>
#include <map>

std::map<std::string, std::function<void()>> availableCommands;

void handleCommand(const std::string command) {
  auto it = availableCommands.find(command);
  if (it != availableCommands.end()) {
    it->second();
  } else {
    std::cout << command << ": command not found" << std::endl;
  }
}

int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  availableCommands.emplace("exit", []() { std::exit(EXIT_SUCCESS); });

  while (true) {
    std::string command = "";

    std::cout << "$ ";
    std::cin >> command;

    handleCommand(command);    
  }
  
  
  return 0;
}
