#include "GetNextDelimiterOffset.h"

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