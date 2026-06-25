#ifndef FP_TUPLE_HPP
#define FP_TUPLE_HPP

#include <cstddef>
#include <cstdint>
#include <utility>

namespace fp {


template<size_t idx, typename... Ts>
struct tuple_impl;


template<size_t idx, typename T, typename... Ts>
struct tuple_impl<idx, T, Ts...>
    : tuple_impl<idx+1, Ts...> {

    T elem;

    tuple_impl() = default;

    tuple_impl(T&& t, Ts&&... ts) :
        tuple_impl<idx+1, Ts...>(std::forward<Ts>(ts)...),
        elem(std::forward<T>(t))
        { }
};

template<size_t idx>
struct tuple_impl<idx> {
    tuple_impl() = default;
};

template<typename... Ts>
class tuple : tuple_impl<0, Ts...> {

    template<size_t idx, typename... Args>
    friend auto& get(tuple<Args...>& t);

public:

    tuple() = default;
    tuple(Ts&&... ts) :
        tuple_impl<0, Ts...>(std::forward<Ts>(ts)...)
        { }
    
};

template<size_t idx,
         typename T,
         typename... Ts>
T& get_impl(tuple_impl<idx, T, Ts...>& t) {
    return t.elem;
}

template<size_t idx, typename... Args>
auto& get(tuple<Args...>& t)
{
    return get_impl<idx>(t);
}

template<template<typename...> class Tuple, typename... Args>
concept NonEmptyTuple = requires(Tuple<Args...> t) {
    get_impl<0>(t);
};

template<typename T, typename... Ts>
T sum_sizeof(T val, Ts... rest) {
    if (sizeof...(rest) > 0) {
        return val + sum(rest...);
    }
    return val;
}

template<typename RT>
RT sum_impl() {
    return RT{};
}

template<typename RT, typename T, typename... Ts>
RT sum_impl(T val, Ts... rest) {
    return val + sum_impl<RT>(rest...);
}

template<typename T, typename... Ts>
T sum(T val, Ts... rest) {
    return sum_impl<T>(val, rest...);
}


template<typename... Args>
std::size_t argCount(Args... args) {
    return sizeof...(args);
}


}

namespace std {

    template<typename... Ts>
    struct tuple_size<fp::tuple<Ts...>> {
        static constexpr inline size_t value
            = sizeof...(Ts);
    };


    template<size_t idx, typename... Ts>
    struct tuple_element<idx, fp::tuple<Ts...>> {
        // using type = decltype(fp::get<idx>);
    };
}

#endif  // FP_TUPLE_HPP
