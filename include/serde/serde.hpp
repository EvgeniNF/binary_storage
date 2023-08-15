#pragma once

#include "traits.hpp"


namespace binary_storage::serde {

template<class S, class T>
std::enable_if_t<isNumeric<T>, void> serializeImpl(S& stream, T const& value) noexcept {
    auto constexpr size = sizeof(T);
    auto data = reinterpret_cast<S::char_type const* const>(&value);

    for (uint16_t i = 0; i < size; ++i) {
        stream << data[i];
    }
}

template<class S, class T>
std::enable_if_t<isVector<T>, void> serializeImpl(S& stream, T const& value) noexcept {
    auto const size = value.size();
    serializeImpl(stream, size);

    for (auto const& data: value) {
        serializeImpl(stream, data);
    }
}

struct Test {
    int a;
    double b;
};

template<class S, class T>
static inline void serialize(S& stream, T const& value) noexcept {
    static_assert(isStream<S>, "S parameter must be a stream");
    static_assert(isOutStream<S>, "S parameter must be a output stream");
    serializeImpl(stream, value);
}


} // namespace binary_storage::serde
