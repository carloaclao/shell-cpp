#pragma once

#include "vector"
#include "string"

template <typename T>
std::string join(const T& enumerable, const char& separator) {
    std::string result {};
    for (const std::string& item : enumerable) {
    result += item + separator;
    }

    if (result.length() > 0)
        result.erase(result.length() - 1);

    return result;
} 

std::vector<std::string> split(char separator, const std::string& input);
std::string getEnvVar(const std::string&);
