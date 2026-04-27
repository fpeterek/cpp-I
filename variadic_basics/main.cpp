#include <print>


template<typename RT>
RT sum_type() {
    return RT{};
}

template<typename RT, typename T, typename... Ts>
RT sum_type(const T& fst, const Ts& ...rest) {
    return fst + sum_type<RT>(rest...);
}

template<typename T, typename... Ts>
auto sum(const T& t, const Ts&... ts) {
    return sum_type<T>(t, ts...);
}

namespace better {
    template<typename... Ts>
    auto sum(const Ts&... ts) {
        return (ts + ...);
    }
}


int main() {
    std::println("sum of ints = {}", sum(1, 2, 3, 4, 5));

    std::println("sum of strings = {}",
        sum(std::string { "string 1" },
            std::string { ", " },
            std::string { "string 2" },
            std::string { ", " },
            std::string { "string 2" }
        )
    );
}

