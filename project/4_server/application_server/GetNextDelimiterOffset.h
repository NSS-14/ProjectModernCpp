#pragma once

#include <string>

class GetNextDelimiterOffset
{
public:
	GetNextDelimiterOffset(const GetNextDelimiterOffset&) = delete;
	GetNextDelimiterOffset(GetNextDelimiterOffset&&) = delete;
	GetNextDelimiterOffset& operator =(const GetNextDelimiterOffset&) = delete;
	GetNextDelimiterOffset& operator =(GetNextDelimiterOffset&&) = delete;
	bool operator ==(const GetNextDelimiterOffset&) = delete;
	bool operator ==(GetNextDelimiterOffset&&) = delete;

public:
	GetNextDelimiterOffset(const std::string& inputString, const std::string& delimiter);

public:
	size_t operator ()(size_t currentWordOffset);

private:
	const std::string& m_inputString;
	const std::string& m_delimiter;
};
