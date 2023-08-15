#pragma once

#include <type_traits>
#include <vector>
#include <iostream>
#include <sstream>

#include "macros.hpp"

namespace binary_storage::serde {

template<class T>
static auto constexpr hasContainerTraits = [] () constexpr -> bool {
    return has_value_type_v<T> and has_const_iterator_v<T> and has_iterator_v<T> and has_value_type_v<T>;
}();

template<class T>
static auto constexpr isNumeric = [] () constexpr -> bool {
    return std::is_fundamental_v<T> and std::is_arithmetic_v<T>;
}();

template<class T>
static auto constexpr isVector = [] () constexpr -> bool {
    return hasContainerTraits<T> and std::is_same_v<std::vector<T::value_type>, T>;
}();

template<class T>
static auto constexpr isOutStream = [] () constexpr -> bool {
    return std::is_base_of_v<std::ostream, T>;
}();

template<class T>
static auto constexpr isInStream = [] () constexpr -> bool {
    return std::is_base_of_v<std::istream, T>;
}();

template<class T>
static auto constexpr isStream = [] () constexpr -> bool {
    return isInStream<T> || isOutStream<T>;
}();

} // namespace binary_storage::serde::traits

