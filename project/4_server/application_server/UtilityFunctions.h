#pragma once

#include <unordered_map>
#include <string>
#include <vector>

std::vector<std::string> Split(const std::string& inputString, const std::string& delimiter);
std::unordered_map<std::string, std::string> ParseUrlArguments(const std::string& urlArguments);