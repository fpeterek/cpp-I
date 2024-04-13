#include <ostream>
#include <utility>


namespace fp {

namespace {

template<int idx, typename... Ts>
struct tuple_impl {

    tuple_impl() = default;

    void print(std::ostream& os) const;

};

template<int idx>
struct tuple_impl<idx> {

    tuple_impl() = default;

    void print([[maybe_unused]] std::ostream& os) const { }
};

template<int idx, typename T, typename... Ts>
struct tuple_impl<idx, T, Ts...> : public tuple_impl<idx+1, Ts...> {

    T item;

    tuple_impl() = default;

    tuple_impl(T&& item, Ts&&... args) :
        tuple_impl<idx + 1, Ts...>(std::forward<Ts>(args)...),
        item(item) { }

    void print(std::ostream& os) const {
        os << "[" << idx << "]: " << item;
        
        if constexpr (sizeof...(Ts) > 0) {
            os << ", ";
        }

        tuple_impl<idx+1, Ts...>::print(os);
    }

};

template<int idx, typename T, typename... Ts>
T& get_impl(tuple_impl<idx, T, Ts...>& t) {
    return t.item;
}

} // namespace

template<typename... Ts>
class tuple : protected tuple_impl<0, Ts...> {

    void print(std::ostream& os) const {
        os << "fp::tuple { ";
        fp::tuple_impl<0, Ts...>::print(os);
        os << " }";
    }

public:

    template<int idx>
    auto& get() {
        tuple_impl<0, Ts...>& t = *this;
        return get_impl<idx>(t);
    }

    tuple() = default;
    tuple(Ts&&... args) : tuple_impl<0, Ts...>(std::forward<Ts>(args)...) { }

    template<typename... Args>
    friend std::ostream& operator<<(std::ostream& os, const tuple<Args...>& tuple);
};

template<typename... Ts>
std::ostream& operator<<(std::ostream& os, const tuple<Ts...>& tuple) {
    tuple.print(os);
    return os;
}


} // namespace fp


namespace std {
    template<int idx, typename... Ts>
    auto& get(fp::tuple<Ts...>& tuple) {
        return tuple.template get<idx>();
    }
}
