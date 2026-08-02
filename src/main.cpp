#include <iostream>
#include <string>
#include <unordered_set>

int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  std::string command = "";

  std::cout << "$ ";
  std::cin >> command;

  std::unordered_set<std::string> availableCommands{};
  if (!availableCommands.contains(command)) {
    std::cout << command << ": command not found" << std::endl;
  }
  
  return 0;
}
