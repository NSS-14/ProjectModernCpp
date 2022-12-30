#include "UtilityFunctions.h"
#include "GetNextDelimiterOffset.h"

std::vector<std::string> Split(const std::string& inputString, const std::string& delimiter)
{
	std::vector<std::string> result;
	GetNextDelimiterOffset nextWordEnd(inputString, delimiter);

	size_t wordBegin = 0;

	std::string::const_iterator itBegin = inputString.begin();
	std::string::const_iterator itEnd = inputString.end();

	for (size_t wordEnd = nextWordEnd(wordBegin); wordEnd != std::string::npos; wordEnd = nextWordEnd(wordBegin))
	{
		result.emplace_back(itBegin + wordBegin, itBegin + wordEnd);
		wordBegin += wordEnd + delimiter.size();
	}
	if (wordBegin != inputString.size())
		result.emplace_back(itBegin + wordBegin, itEnd);
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