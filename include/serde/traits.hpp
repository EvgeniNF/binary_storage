#pragma once

#include <type_traits>
#include <vector>
#include <iostream>
#include <sstream>

#include <refl.hpp>

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
static auto constexpr isString = [] () constexpr -> bool {
    return std::is_same_v<T, std::string>;
}();

template<class T>
static auto constexpr isVector = [] () constexpr -> bool {
    return hasContainerTraits<T> and not isString<T>;
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

template<class T>
static auto constexpr isReflectable = [] () constexpr -> bool {
    return not isNumeric<T> and not isVector<T> and not isString<T> and refl::is_reflectable<T>();
}();

template<class T>
static auto constexpr isSerializeble = [] () constexpr -> bool {
    return isNumeric<T> or isVector<T> or isReflectable<T> or isString<T>;
}();

template<class F, class... Args>
struct ReturnType {
    using t = decltype(std::declval<F>()(std::declval<Args>()...));
};

template<class F, class... Args>
using ReturnType_t = typename ReturnType<F, Args...>::t;

} // namespace binary_storage::serde::traits

