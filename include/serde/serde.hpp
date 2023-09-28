#pragma once

#include <optional>

#include "traits.hpp"

#include <refl.hpp>


namespace binary_storage::serde {

template<class S, class T>
static void serialize(S& stream, T const& value) noexcept;

template<class T, class S>
static std::optional<T> deserialize(S& stream) noexcept;

template<class S, class T>
std::enable_if_t<isNumeric<T>, void> serializeImpl(S& stream, T const& value) noexcept {
    auto constexpr size = sizeof(T);
    auto data = reinterpret_cast<typename S::char_type const* const>(&value);

    for (uint16_t i = 0; i < size; ++i) {
        stream.put(data[i]);
    }
}

template<class S, class T>
std::enable_if_t<isVector<T>, void> serializeImpl(S& stream, T const& value) noexcept {
    auto const size = value.size();
    serialize(stream, size);

    for (auto const& data: value) {
        serialize(stream, data);
    }
}

template<class S, class T>
std::enable_if_t<isString<T>, void> serializeImpl(S& stream, T const& value) noexcept {
    auto const size = value.size();
    serialize(stream, size);
    stream << value;
}

template<class S, class T>
std::enable_if_t<isReflectable<T>, void> serializeImpl(S& stream, T const& value) noexcept {
    for_each(refl::reflect(value).members, [&] (auto member) {
        serialize(stream, member(value));
    });
}

template<class S, class T>
static inline void assertTypes() noexcept {
    static_assert(isStream<S>, "S parameter must be a stream");
    static_assert(isSerializeble<T>, "T parameter must be a serializeble");
}


template<class S, class T>
static void serialize(S& stream, T const& value) noexcept {
    assertTypes<S, T>();
    serializeImpl(stream, value);
}

template<class T, class S>
std::enable_if_t<isNumeric<T>, std::optional<T>> deserializeImpl(S& stream) noexcept {
    auto constexpr size = sizeof(T);
    
    T value {};
    auto data = reinterpret_cast<typename S::char_type* const>(&value);

    size_t i = 0;
    do {
        if (stream.eof()) {
            return std::nullopt;
        }
        data[i] = stream.get();
    } while (++i < size);
    
    return value;
}

template<class T, class S>
std::enable_if_t<isVector<T>, std::optional<T>> deserializeImpl(S& stream) noexcept {
    auto const size = deserialize<typename T::size_type>(stream);
    if (not size.has_value()) {
        return std::nullopt;
    }

    T value;
    value.resize(*size);
    for (auto& data: value) {
        auto el = deserialize<typename T::value_type>(stream);
        if (not el.has_value()) {
            return std::nullopt;
        }

        if constexpr (std::is_move_constructible_v<typename T::value_type>) {
            data = std::move(*el);
        } else {
            data = *el;
        }
    }
    return value;
}

template<class T, class S>
std::enable_if_t<isString<T>, std::optional<T>> deserializeImpl(S& stream) noexcept {
    auto const size = deserialize<typename T::size_type>(stream);
    if (not size.has_value()) {
        return std::nullopt;
    }

    T value;
    value.resize(size.value());
    for (auto& data: value) {
        auto el = deserialize<typename T::value_type>(stream);
        if (not el.has_value()) {
            return std::nullopt;
        }

        data = *el;
    }

    return value;
}

template<class T, class S>
std::enable_if_t<isReflectable<T>, std::optional<T>> deserializeImpl(S& stream) noexcept {
    T value {};
    bool error {false};

    for_each(refl::reflect(value).members, [&] (auto member) {
        if (error) {
            return;
        }

        auto data = deserialize<std::remove_reference_t<decltype(member(value))>>(stream);
        
        if (not data.has_value()) {
            error = true;
            return;
        }

        if constexpr (std::is_move_constructible_v<std::remove_reference_t<decltype(member(value))>>) {
            member(value) = std::move(data.value());
        } else {
            member(value) = data.value();       
        }
    });

    if (error) {
        return std::nullopt;
    }

    return value;
}

template<class T, class S>
static std::optional<T> deserialize(S& stream) noexcept {
    assertTypes<S, T>();
    return deserializeImpl<T>(stream);
}

} // namespace binary_storage::serde
