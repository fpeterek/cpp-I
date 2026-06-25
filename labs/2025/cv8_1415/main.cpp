#include <algorithm>
#include <iostream>
#include <memory>
#include <print>
#include <string>
#include <cmath>
#include <type_traits>


template<typename T>
T max(const T& t1, const T& t2) {
    if (t1 > t2) {
        return t1;
    }
    return t2;
}

auto max2(const auto& t1, const auto& t2) {
    return max(t1, t2);
}

template<typename T, typename Alloc = std::allocator<T>>
class Vector {
    
    Alloc allocator;
    T* begin;
    T* end;
    T* end_reserved;

    void realloc();

public:

    size_t size() {
        return end - begin;
    }

    size_t reserved();

    Vector();

    ~Vector() {

        if constexpr (not std::is_trivially_destructible_v<T>) {
            for (auto* it = begin; it != end; ++it) {
                it->~T();
            }
        }

        allocator.deallocate(begin, reserved());
    }


    T& operator[](size_t idx);

    void push_back(const T& t);
    void push_back(T&& t);

    void pop_back();

    template<typename... Args>
    void emplace_back(Args&&... args);


};

template<typename T, typename Alloc>
template<typename... Args>
void Vector<T, Alloc>::emplace_back(Args&&... args) {
    realloc();
    new(end) T(std::forward<Args>(args)...);
    ++end;
}

template<typename T, typename Alloc>
void Vector<T, Alloc>::pop_back() {
    --end;
    end->~T();
}

template<typename T, typename Alloc>
void Vector<T, Alloc>::push_back(const T& t) {
    realloc();
    new(end) T(t);
    ++end;
}

template<typename T, typename Alloc>
void Vector<T, Alloc>::push_back(T&& t) {
    realloc();
    new(end) T(std::move(t));
    ++end;
}

template<typename T, typename Alloc>
void Vector<T, Alloc>::realloc() {
    if (size() < reserved()) {
        return;
    }

    const auto oldSize = size();
    const auto newSize = std::max(16ul, oldSize*2);

    T* newData = allocator.allocate(newSize);

    if constexpr (
            std::is_trivially_constructible_v<T> and
            std::is_trivially_assignable_v<T, T>) {
        std::copy(begin, end, newData);
    } else if constexpr (std::is_move_constructible_v<T>) {
        for (size_t i = 0; i < oldSize; ++i) {
            new(newData+i) T{std::move(begin[i])};
        }
    } else {
        for (size_t i = 0; i < oldSize; ++i) {
            new(newData+i) T{begin[i]};
        }
    }

    allocator.deallocate(begin, oldSize);
    begin = newData; 
    end = newData + oldSize;
    end_reserved = newData + newSize;
}

template<typename T, typename Alloc>
T& Vector<T, Alloc>::operator[](size_t idx) {
    return begin[idx];
}

template<typename T, typename Alloc>
Vector<T, Alloc>::Vector() :
    allocator{},
    begin{nullptr},
    end{nullptr},
    end_reserved{nullptr} {  }

template<typename T, typename Alloc>
size_t Vector<T, Alloc>::reserved() {
    return end_reserved - begin;
}

struct S {
    int val;

    S(const S& other) : val{other.val} {
        std::println("S(const S&)");
    }

    S(int a, float b, double c) : val(a+b+c) {
        std::println("S(int, float, double)");
    }

    ~S() {
        std::println("~S()");
    }
};

template<typename... Args>
auto sum(Args&&... args) {
    return (... + args);  
}

template<typename... Args>
auto sum2();

template<typename Arg, typename... Args>
auto sum2(Arg a1, Args... args) {
    return a1 + sum2(args...);
}

template<>
auto sum2() {
    return 0;
}

template<size_t Idx, typename... Args>
struct TupleImpl;


template<size_t Idx, typename Arg, typename... Args>
struct TupleImpl<Idx, Arg, Args...> : TupleImpl<Idx+1, Args...> {

    Arg item;

    TupleImpl(const Arg& item, Args&&... args) :
        TupleImpl<Idx+1, Args...>(std::forward<Args>(args)...),
        item{item} {  }

};

template<size_t Idx>
struct TupleImpl<Idx> {
    TupleImpl() = default;
};


template<size_t Idx, typename T, typename... Ts>
T& get_impl(TupleImpl<Idx, T, Ts...>& tuple) {
    return tuple.item;
}

template<typename... Args>
class Tuple : TupleImpl<0, Args...> {
public:
    Tuple(Args&&... args) :
        TupleImpl<0, Args...>(std::forward<Args>(args)...) { }

    template<size_t Idx>
    auto& get() {
        TupleImpl<0, Args...>& parent = *this;
        return get_impl<Idx>(parent);
    }
};

namespace std {
    template<size_t Idx, typename... Args>
    auto& get(Tuple<Args...>& t) {
        return t.template get<Idx>();
    }

    template<typename... Args>
    struct tuple_size<Tuple<Args...>> {
        static constexpr int value = sizeof...(Args);
    };

    template<size_t Idx, typename... Args>
    class tuple_element<Idx, Tuple<Args...>> {

        static Tuple<Args...> get_tuple();

        public:
        using type = std::remove_reference_t<decltype(get_tuple().template get<Idx>())>;
    };
}

template<typename T>
T max_val();

template<>
uint8_t max_val<uint8_t>() {
    return 255;
}

consteval int max(int a, int b) {
    if (a > b) {
        return a;
    }
    return b;
}

int main() {

    static constexpr int m = max(5, max(10, 15));

    std::pair pair { 1, 2 };
    auto [x, y] = pair;

    Tuple<int, std::string, bool> tuple(0, "Haskell", true);

    std::cout << std::get<0>(tuple) << std::endl;
    std::cout << std::get<1>(tuple) << std::endl;
    std::cout << std::boolalpha << std::get<2>(tuple) << std::endl;

    auto& [i, s, b] = tuple;

    std::cout << sum(1, 2, 3, 4, 5, 6) << std::endl;
    std::cout << sum2(1, 2, 3, 4, 5, 6) << std::endl;

    Vector<S> vec;

    vec.push_back(S{1, 0.f, 0.0});
    vec.push_back(S{1, 1.f, 0.0});
    vec.emplace_back(1, 1.f, 1.1);

    for (size_t i = 0; i < vec.size(); ++i) {
        std::cout << vec[i].val << std::endl;
    }

    Vector<int> v;
    v.push_back(1);
}

