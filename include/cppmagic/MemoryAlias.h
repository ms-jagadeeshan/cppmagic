#pragma once

#include <map>
#include <memory>
#include <unordered_map>
#include <vector>

template <typename T>
using cmgSP = std::shared_ptr<T>;

template <typename T>
using cmgUP = std::unique_ptr<T>;

template <typename T>
using cmgVector = std::vector<T>;

template <typename T>
using cmgSPVector = std::vector<std::shared_ptr<T>>;

template <typename T>
using cmgUPVector = std::vector<std::unique_ptr<T>>;

template <typename T, typename U>
using cmgMap = std::map<T, U>;

template <typename T, typename U>
using cmgUOMap = std::unordered_map<T, U>;

template <typename T, typename U>
using cmgSPMap = std::map<T, std::shared_ptr<U>>;
