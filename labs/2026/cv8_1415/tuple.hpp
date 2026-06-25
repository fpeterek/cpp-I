#ifndef FP_TUPLE_HPP
#define FP_TUPLE_HPP

#include <cstddef>
#include <utility>


namespace fp {

template<size_t idx, typename... Ts>
struct tuple_impl;

template<size_t idx, typename T, typename... Ts>
struct tuple_impl<idx, T, Ts...> :
    tuple_impl<idx+1, Ts...> {

    T elem;

    tuple_impl(T&& t, Ts&&... ts) :
        tuple_impl<idx+1, Ts...>{std::forward<Ts>(ts)...},
        elem { t }
        { }
};

template<size_t idx>
struct tuple_impl<idx> {

};

template<typename... Ts>
class tuple : tuple_impl<0, Ts...> {

    template<size_t idx, typename... Args>
    friend auto& get(tuple<Args...>& t);

public:
    tuple(Ts&&... ts) :
        tuple_impl<0, Ts...>{std::forward<Ts>(ts)...}
        { }
};

template<size_t idx, typename... Ts>
auto& get_impl(tuple_impl<idx, Ts...>& tuple) {
    return tuple.elem;
}

template<size_t idx, typename... Ts>
auto& get(tuple<Ts...>& t) {
    return get_impl<idx>(t);
}

template<typename RT>
inline RT sum_type() {
    return RT{};
}

template<typename RT, typename T, typename... Ts>
inline RT sum_type(T fst, Ts... rest) {
    return fst + sum_type<RT>(rest...);
}

template<typename T, typename... Ts>
inline T sum(T fst, Ts... rest) {
    return sum_type<T>(fst, rest...);
}

}


#endif // FP_TUPLE_HPP
