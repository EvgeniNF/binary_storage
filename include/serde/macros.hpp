#pragma once

#include <type_traits>
#include <tuple>
#include <vector>

namespace binary_storage::serde {
                                        
#define CREATE_HAS_TRAIT(trait)                                                                                 \
    namespace details {                                                                                         \
        template<class, class = std::void_t<>>                                                                  \
        struct has_##trait : std::false_type {};                                                                \
                                                                                                                \
        template<class T>                                                                                       \
        struct has_##trait<T, std::void_t<decltype(std::declval<typename T::trait>())>> : std::true_type {};    \
    }                                                                                                           \
    template<class T>                                                                                           \
    static auto constexpr has_##trait##_v = details::has_##trait<T>::value  


#define CREATE_HAS_SYMBOL(symbol)                                                   \
    namespace details {                                                             \
        template<class T>                                                           \
        struct has_##symbol {                                                       \
            using true_type = char;                                                 \
            using false_type = long;                                                \
                                                                                    \
            template<class U>                                                       \
            static false_type test(...);                                            \
                                                                                    \
            template<class U>                                                       \
            static true_type test(decltype(&U::symbol));                            \
                                                                                    \
            static auto constexpr value = sizeof(test<T>(0)) == sizeof(true_type);  \
        };                                                                          \
    }                                                                               \
    template<class T>                                                               \
    static auto constexpr has_##symbol##_v = details::has_##symbol<T>::value


#define CREATE_TYPE_FIELD(type, field)                                 \
    CREATE_HAS_SYMBOL(field);                                          \
    struct type##Fields {                                              \  
        static_assert(has_##field##_v<type>, "Field not found");       \
        using type##_##field = decltype(std::declval<type>().field);   \
        using type##_##field##_ptr = type##_##field*;                  \
        using type##_##field##_const_ptr = type##_##field##_ptr const; \
    }

struct Test {
    int a;
    double b;
    std::vector<int> c;
};

template<class T, class... Args>
std::tuple<Args...> to_tuple(T const& obj) {

}

CREATE_HAS_TRAIT(value_type);
CREATE_HAS_TRAIT(size_type);
CREATE_HAS_TRAIT(allocator_type);
CREATE_HAS_TRAIT(iterator);
CREATE_HAS_TRAIT(const_iterator);

} // namespace binary_storage::serde
