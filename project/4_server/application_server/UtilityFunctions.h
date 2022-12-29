#pragma once
#include <unordered_map>
#include <string>
#include <vector>

class GetNextDelimiterOffset
{
public:
	GetNextDelimiterOffset(const std::string& inputString, const std::string& delimiter);

public:
	size_t operator ()(size_t currentWordOffset);

private:
	const std::string& m_inputString;
	const std::string& m_delimiter;

public:
	GetNextDelimiterOffset(const GetNextDelimiterOffset&) = delete;
	GetNextDelimiterOffset(GetNextDelimiterOffset&&) = delete;
	GetNextDelimiterOffset& operator =(const GetNextDelimiterOffset&) = delete;
	GetNextDelimiterOffset& operator =(GetNextDelimiterOffset&&) = delete;
};
std::vector<std::string> Split(const std::string& inputString, const std::string& delimiter);


std::unordered_map<std::string, std::string> ParseUrlArguments(const std::string& urlArguments);