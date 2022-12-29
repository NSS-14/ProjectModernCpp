#include "UtilityFunctions.h"

GetNextDelimiterOffset::GetNextDelimiterOffset(const std::string& inputString, const std::string& delimiter)
	: m_inputString(inputString)
	, m_delimiter(delimiter)
{
	/* EMPTY */
}
size_t GetNextDelimiterOffset::operator()(size_t currentWordOffset)
{
	return m_inputString.find(m_delimiter, currentWordOffset);
}
std::vector<std::string> Split(const std::string& inputString, const std::string& delimiter)
{
	std::vector<std::string> result;

	GetNextDelimiterOffset nextDelimiterOffsetOf(inputString, delimiter);
	size_t currentWordOffset = 0;
	const std::string::const_iterator& beginIt(inputString.begin());
	for (size_t currentDelimiterOffset = nextDelimiterOffsetOf(currentWordOffset);
		currentDelimiterOffset != std::string::npos;
		currentDelimiterOffset = nextDelimiterOffsetOf(currentWordOffset))
	{
		result.emplace_back(beginIt + currentWordOffset, beginIt + currentDelimiterOffset);
		currentWordOffset += currentDelimiterOffset + delimiter.size();
	}
	if (currentWordOffset != inputString.size())
		result.emplace_back(inputString.begin() + currentWordOffset, inputString.end());
	return result;
}

std::unordered_map<std::string, std::string> ParseUrlArguments(const std::string& urlArguments)
{
	if (urlArguments == "") {
		return {};
	}

	std::unordered_map<std::string, std::string> result;
	for (const std::string& keyValueString : Split(urlArguments, "&")) {
		std::vector<std::string> keyValueVector = Split(keyValueString, "=");
		if (keyValueVector.size() == 2)
			result[keyValueVector[0]] = keyValueVector[1];
	}
	return result;
}