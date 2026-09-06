#pragma once

#include <functional>
#include <map>
#include <string>

using Args = std::vector<std::string>;

class Shell {
private:
    bool isRunning = false;
    std::map<std::string, std::function<void(const Args& arguments)>> availableCommands;
    std::map<std::string, std::string> commandTypes;

    bool isCommandInPath(const std::string& command, std::string& commandPath);
    void registerCommands();
    void addCommand(std::string command, std::string type, std::function<void(const Args& arguments)> handler);
    void handleCommand(const std::string rawCommand); 
    void extractCommandsAndArgs(const std::string& input, std::string& command, Args& args);

public:
    Shell shell();
    void run();
};
