#include <array>
#include <memory>
#include <numeric>
#include <print>
#include <type_traits>
#include <vector>
#include <iostream>

#include "tuple.hpp"

void print(const auto& item) {
    std::cout << item << std::endl;
}

template<typename T>
T sum(T a, T b, T c) {
    return a + b + c;
}

template<typename T>
class ContainerWrapper {
    T container;
    using iter = decltype(container.begin());
};

auto get_iter(auto& iterable) -> decltype(iterable.begin()) {
    return iterable.begin();
}

template<typename... T>
auto better_sum(T... args) {
    return (... + args);
}

template<typename T, typename Alloc = std::allocator<T>>
class Vec {
    Alloc allocator;
    T* begin_ptr = nullptr;
    T* end_ptr = nullptr;
    size_t reserved = 0;

    void reallocate_if_needed() {
        auto sz = size();
        if (sz == reserved) {
            reserved *= 2;
            T* new_ptr = allocator.allocate(reserved);

            std::uninitialized_move(begin_ptr, end_ptr, new_ptr);

            allocator.deallocate(begin_ptr, sz);

            begin_ptr = new_ptr;
            end_ptr = begin_ptr + sz; 
        }
    }

    void push_back_unsafe(T t) {
        new(end_ptr) T{std::move(t)};
        ++end_ptr;
    }


public:
    
    Vec(Alloc allocator = Alloc{}) :
        allocator{std::move(allocator)} {
    }

    Vec(size_t reserved, Alloc allocator = Alloc{}) :
        reserved{reserved},
        allocator{std::move(allocator)} {

        begin_ptr = allocator.allocate(reserved);
        end_ptr = begin_ptr;
    }

    Vec(std::initializer_list<T> list, Alloc alloc = Alloc{}) :
        reserved{list.size()},
        allocator{std::move(alloc)} {

        begin_ptr = allocator.allocate(reserved);
        end_ptr = begin_ptr;

        for (const T& t : list) {
            push_back_unsafe(t);
        }

    }

    ~Vec() {
        for (auto ptr = begin_ptr; ptr < end_ptr; ++ptr) {
            ptr->~T();            
        }

        allocator.deallocate(begin_ptr, reserved);
    }

    size_t size() {
        return static_cast<size_t>(end_ptr - begin_ptr);
    }

    void push_back(T t);

    template<typename Arg>
    void push_back_converted(const Arg& arg);

    template<typename... Args>
    void emplace_back(Args&& ...args) {
        push_back(T { std::forward(args)... });
    }

};

template<typename T, typename Alloc>
void Vec<T, Alloc>::push_back(T t) {
    reallocate_if_needed();
    push_back_unsafe(std::move(t));
}

template<typename T, typename Alloc>
template<typename Arg>
void Vec<T, Alloc>::push_back_converted(const Arg& arg) {
    push_back(T{arg});
}


int main() {
    // print(10);
    // print("pseudosacrosanct perversion");
    // print(better_sum(1, 2, 3, 4, 5, 6));
    // print(better_sum(1));
    //
    // std::array<int, 4> array;
    //
    // std::vector<int> vec { 1, 2, 3, 4 };
    // vec.emplace_back(1);
    // vec.push_back(1);
    // vec.begin();

    std::println("sum = {}", fp::sum(1, 2, 3, 4, 5));

    std::println("sum = {}",
            fp::sum(
                std::string { "1. string" },
                std::string { ", " },
                std::string { "2. string" },
                std::string { ", " },
                std::string { "3. string" }
                ));

    fp::tuple<std::string, std::string, std::string> tuple {
        std::string {"str 1"},
        std::string {"str 2"},
        std::string {"str 3"}
    };

    fp::get<0>(tuple);

    std::tuple<int, float, std::string> st;
    std::get<0>(st);
}
