#include <new>
#include <numeric>
#include <print>
#include <iostream>
#include <stdexcept>
#include <type_traits>
#include <vector>
#include <cmath>
#include <stdexcept>

#include "tuple.hpp"


void* alloc() {
    void* p = nullptr;
    while (not (p = malloc(1024))) {
        auto handler = std::get_new_handler();

        if (handler == nullptr) {
            throw std::bad_alloc();
        }
        handler();
    }

    return p;
}

void print(const auto& item) {
    std::cout << item << std::endl;
}

template<typename T>
T sum(T a, T b, T c) {
    return a + b + c;
}

template<typename T>
auto get_iter(T& t) -> decltype(t.begin()) {
    return t.begin();
}

template<typename... T>
auto better_sum(T... ts) {
    return (... + ts);
}

template<
    typename T,
    std::enable_if_t<
        not std::is_trivially_destructible_v<T>,
        bool> = true>
void destruct(T* begin_ptr, T* end_ptr) {
    for (T* ptr = begin_ptr; ptr != end_ptr; ++ptr) {
        ptr->~T();
    }
}

template<
    typename T,
    std::enable_if_t<
        std::is_trivially_destructible_v<T>,
        bool> = true>
void destruct([[maybe_unused]] T* begin_ptr, T*) {
    // no-op
}

template<typename T, typename Alloc = std::allocator<T>>
class Vec {

    Alloc allocator;
    T* begin_ptr = nullptr;
    T* end_ptr = nullptr;
    size_t reserved = 0;

public:

    Vec(Alloc alloc = Alloc{}) :
        allocator{std::move(alloc)} {
    }

    Vec(size_t reserved, Alloc alloc = Alloc{}) :
        allocator{std::move(alloc)},
        reserved{reserved} {
        
        begin_ptr = allocator.allocate(reserved);
        end_ptr = begin_ptr;
    }

    Vec(std::initializer_list<T> list,
        Alloc alloc = Alloc{}) :
        allocator{std::move(alloc)},
        reserved{list.size()} {

        begin_ptr = allocator.allocate(reserved);
        end_ptr = begin_ptr;

        for (T t : list) {
            push_back_unsafe(std::move(t));
        }
    }

    ~Vec() {
        destruct(begin_ptr, end_ptr);

        allocator.deallocate(begin_ptr, reserved);
    }

    void push_back_unsafe(T t) {
        new(end_ptr) T{std::move(t)};
        ++end_ptr;
    }

    size_t size() const {
        return static_cast<size_t>(end_ptr - begin_ptr);
    }

    void reallocate_if_needed() {
        size_t sz = size();

        if (sz == reserved) {
            reserved = std::max(32ul, reserved*2);

            T* new_ptr = allocator.allocate(reserved);

            std::uninitialized_move(
                    begin_ptr, end_ptr,
                    new_ptr);
            
            allocator.deallocate(begin_ptr, sz);
            begin_ptr = new_ptr;
            end_ptr = begin_ptr + sz;
        }
    }

    void push_back(T t) {
        reallocate_if_needed();
        push_back_unsafe(std::move(t));
    }

    template<typename Arg>
    void push_back_converted(const Arg& arg);

    T& operator[](size_t idx);
    const T& operator[](size_t idx) const;
    
    auto& at(this auto& self, size_t idx) {
        if (self.size() < idx) {
            throw std::runtime_error("Index out of bounds");
        }
        return self.begin_ptr[idx];
    }

    template<typename... Args>
    void emplace_back(Args&&... args) {
        push_back(T { std::forward(args)... });
    }
};

template<typename T, typename Alloc>
T& Vec<T, Alloc>::operator[](size_t idx) {
    return begin_ptr[idx];
}

template<typename T, typename Alloc>
const T& Vec<T, Alloc>::operator[](size_t idx) const {
    return begin_ptr[idx];
}

template<typename T, typename Alloc>
template<typename Arg>
void Vec<T, Alloc>::push_back_converted(const Arg& arg) {
    push_back(static_cast<T>(arg));
}


struct StructuredBindings {
    int x;
    float y;
    char z;
};

template<size_t idx>
auto& get(StructuredBindings&);

template<>
auto& get<0>(StructuredBindings& sb) {
    return sb.x;
}

template<>
auto& get<1>(StructuredBindings& sb) {
    return sb.y;
}

template<>
auto& get<2>(StructuredBindings& sb) {
    return sb.z;
}

namespace std {
    template<>
    struct tuple_size<StructuredBindings> {
        static constexpr inline size_t value = 3;
    };

    template<>
    struct tuple_element<0, StructuredBindings> {
        using type = int;
    };

    template<>
    struct tuple_element<1, StructuredBindings> {
        using type = float;
    };

    template<>
    struct tuple_element<2, StructuredBindings> {
        using type = char;
    };
}


int main() {

    // print(fp::argCount(1, 2, 3, 4, 5));
    // print(fp::sum(1, 2, 3, 4, 5));
    //
    // print(25);
    // print("go drill your deserts");
    // print(std::string {"go dig your graves"});
    // print(better_sum(1, 2, 3, 4, 5, 6));
    //
    // std::vector<bool> vec { true, false, true, false };
    //
    // for (auto&& b : vec) {
    //     b = not b;
    // }
    //
    // std::vector<int> v {1, 2, 3, 4};
    // v.push_back(5);
    // v.emplace_back(5);
    //
    // auto i = v.begin().operator*();
    // decltype(auto) j = v.begin().operator*();
    //
    // for (decltype(auto) i : v) {
    //     i += 1;
    // }
    //
    //
    // Vec<int> vec1 { 1, 2, 3, 4, 5, 6 };
    //
    // print(vec1.at(2));
    //
    // const Vec<int> vec2 { 1, 2, 3, 4, 5, 6 };
    //
    // print(vec2.at(3));
    //
    // std::array<int, 4> array;

    std::println("sum = {}", fp::sum(1, 2, 3, 4, 5));
    std::println("sum = {}",
            fp::sum(
                std::string { "string 1" },
                std::string { ", " },
                std::string { "string 2" }
            )
            );
    std::println();

    fp::tuple<int, float, std::string> tuple {
        1,
        3.14f,
        std::string { "string" }
    };

    std::println("1st: {}", fp::get<0>(tuple));
    std::println("2nd: {}", fp::get<1>(tuple));
    std::println("3rd: {}", fp::get<2>(tuple));

    fp::get<0>(tuple) += 1898;
    fp::get<1>(tuple) = 2.71f;
    fp::get<2>(tuple) += std::string { " appended string" };

    std::println("1st: {}", fp::get<0>(tuple));
    std::println("2nd: {}", fp::get<1>(tuple));
    std::println("3rd: {}", fp::get<2>(tuple));

    std::tuple<int, float, char> st;
    std::get<1>(st);

    auto& [i, f, c] = st;

    StructuredBindings sb{
        .x = 26,
        .y = 3.8,
        .z = 'a'
    };

    auto& [a, b, c2] = sb;
    std::println("{}, {}, {}", a, b, c2);
}

template<typename T>
class Wrapper {

    T val;

public:
    Wrapper(T val) : val{val} { }

    T add(T other) {
        return val + other;
    }

    T concat(T other) {
        return val.append(other);
    }

};
