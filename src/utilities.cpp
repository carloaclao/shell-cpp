#include <vector>
#include "utilities.h"

// Given a string and a sperator, returns a vector of strings that are split by seperator
std::vector<std::string> split(char separator, const std::string& input) {
  std::vector<std::string> result = {};

  if (input == "") {
    result.emplace_back("");
    return result;
  }

  int start = 0;
  int curr = 0;
  for(curr = 0; curr < input.length(); ++curr) {
    if (input.at(curr) == separator) {
      result.emplace_back(input.substr(start, curr - start));
      start = curr + 1;
    } 
  }

  if (start < curr) 
    result.emplace_back(input.substr(start, curr - start));

  return result;
}
