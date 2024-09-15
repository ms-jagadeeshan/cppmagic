#pragma once

#include <algorithm>
#include <string>

#include "MemoryAlias.h"

namespace cmg
{

namespace utils
{

// Join a vector of strings with a delimiter.
std::string join(const cmgVector<std::string>& strings, const std::string delimiter = ",");
// Reads a file and returns it as a vector of strings.
cmgVector<std::string> readLinesFromFile(const std::string& filename);

template <typename T1, typename T2>
std::vector<std::tuple<T1, T2>> zip(const std::vector<T1>& a, const std::vector<T2>& b)
{
    std::vector<std::tuple<T1, T2>> result;
    size_t minSize = std::min(a.size(), b.size());

    for (size_t i = 0; i < minSize; ++i)
    {
        result.push_back(std::make_tuple(a[i], b[i]));
    }

    return result;
}

template <typename T, typename S>
bool contains(const T& container, const S& element)
{
    auto itr = std::find(std::begin(container), std::end(container), element);
    return itr != std::end(container);
}

} // namespace utils
} // namespace cmg
