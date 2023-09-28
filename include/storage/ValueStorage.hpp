#pragma once

#include <variant>
#include <fstream>
#include <chrono>
#include <optional>

#include "serde/traits.hpp"
#include "serde/serde.hpp"

namespace binary_storage::storage {

template<class T>
struct Value {
    using ValueType = T;
    using StorageType = std::variant<ValueType, std::string>;
    static_assert(std::is_copy_constructible_v<ValueType> or std::is_move_constructible_v<ValueType>, "Value type must be copy constructible or move constructible");

    StorageType storage;
    std::chrono::system_clock::time_point lastAccess;
};

template<class T>
using RemoveCRType = std::remove_const_t<std::remove_reference_t<T>>;

template<class T>
void updateData(T&& data, Value<std::remove_const_t<std::remove_reference_t<T>>>& value) {
    value.lastAccess = std::chrono::system_clock::now();
    value.storage = std::forward<T>(data);
}

template<class T>
void storeValue(Value<T>& value, std::string path) {
    value.lastAccess = std::chrono::system_clock::now();
    if (not std::holds_alternative<T>(value.storage)) {
        return;
    }

    std::ofstream stream(path);
    serde::serialize(stream, std::get<T>(value.storage));
    value.storage = std::move(path); 
}

template<class T>
bool isCashed(Value<T> const& value) noexcept {
    return std::holds_alternative<T>(value.storage);
}

template<class T>
T& getData(Value<T>& value) {
    if (std::holds_alternative<T>(value.storage)) {
        return std::get<T>(value.storage);
    }

    std::ifstream stream(std::get<std::string>(value.storage));
    auto data = serde::deserialize<T>(stream);
    if (not data.has_value()) {
        throw std::logic_error("Deserialize eror");
    }

    if constexpr (std::is_move_constructible_v<T>) {
        value.storage = std::move(data.value());
        return std::get<T>(value.storage);
    }

    value.storage = data.value();
    return std::get<T>(value.storage);
}

template<class T>
Value<RemoveCRType<T>> createFromData(T&& data) {
    return {std::forward<T>(data), std::chrono::system_clock::now()};
}

template<class T>
Value<T> createFormFile(std::string path) {
    return {std::move(path), std::chrono::system_clock::now()};
}

} // namespace binary_storage::storage
