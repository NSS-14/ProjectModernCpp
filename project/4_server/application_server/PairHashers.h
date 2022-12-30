#pragma once

#include <tuple>
#include <string>

namespace PairHashers
{
	struct HashStringableTypes
	{
		template<class T1, class T2>
		size_t operator()(const std::pair<T1, T2>& key) const
		{
			return std::hash<std::string>()
				(
					std::to_string(key.first) +
					"_" +
					std::to_string(key.second)
					);
		}
	};
}