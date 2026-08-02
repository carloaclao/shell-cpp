#include <iostream>
#include <string>
#include <unordered_set>

int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  std::unordered_set<std::string> availableCommands{};
  while (true) {
    std::string command = "";

    std::cout << "$ ";
    std::cin >> command;

    if (!availableCommands.contains(command)) {
      std::cout << command << ": command not found" << std::endl;
    }
  }
  
  
  return 0;
}
