#pragma once

#include "vector"
#include "string"

std::vector<std::string> split(char separator, const std::string& input);
std::string getEnvVar(const std::string&);
