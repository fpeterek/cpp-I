#include <algorithm>
#include <iostream>
#include <memory>
#include <print>
#include <string>
#include <cmath>
#include <type_traits>
#include <vector>


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
        // if constexpr (not std::is_trivially_destructible_v<T>) {
            for (auto* it = begin; it != end; ++it) {
                it->~T();
            }
        // }

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
            std::is_trivially_assignable_v<T, T> and
            std::is_trivially_constructible_v<T>
            ) {
        std::copy(begin, end, newData);
    }
    else if constexpr (std::is_move_constructible_v<T>) {
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

    // S(const S& other) : val{other.val} {
    //     std::println("S(const S&)");
    // }

    S(int a, float b, double c) : val(a+b+c) {
        std::println("S(int, float, double)");
    }

    // ~S() {
    //     std::println("~S()");
    // }
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

    std::vector<S> vector;
    vector.push_back(S{1, 2, 3,});
    vector.emplace_back(S{1, 2, 3,});
    std::allocator<S> alloc;

    Vector<int> vi;
    vi.emplace_back(25);
}


// uninitialized_algorithms.h

template <class _Alloc, class _Tp>
_LIBCPP_HIDE_FROM_ABI _LIBCPP_CONSTEXPR_SINCE_CXX14 void
__uninitialized_allocator_relocate(_Alloc& __alloc, _Tp* __first, _Tp* __last, _Tp* __result) {
  static_assert(__is_cpp17_move_insertable<_Alloc>::value,
                "The specified type does not meet the requirements of Cpp17MoveInsertable");
  if (__libcpp_is_constant_evaluated() || !__libcpp_is_trivially_relocatable<_Tp>::value ||
      !__allocator_has_trivial_move_construct<_Alloc, _Tp>::value ||
      !__allocator_has_trivial_destroy<_Alloc, _Tp>::value) {
    auto __destruct_first = __result;
    auto __guard =
        std::__make_exception_guard(_AllocatorDestroyRangeReverse<_Alloc, _Tp*>(__alloc, __destruct_first, __result));
    auto __iter = __first;
    while (__iter != __last) {
#ifndef _LIBCPP_HAS_NO_EXCEPTIONS
      allocator_traits<_Alloc>::construct(__alloc, __result, std::move_if_noexcept(*__iter));
#else
      allocator_traits<_Alloc>::construct(__alloc, __result, std::move(*__iter));
#endif
      ++__iter;
      ++__result;
    }
    __guard.__complete();
    std::__allocator_destroy(__alloc, __first, __last);
  } else {
    __builtin_memcpy(const_cast<__remove_const_t<_Tp>*>(__result), __first, sizeof(_Tp) * (__last - __first));
  }
}


// allocator_traits.h

template <class _Tp, __enable_if_t<__has_destroy<allocator_type, _Tp*>::value, int> = 0>
_LIBCPP_HIDE_FROM_ABI _LIBCPP_CONSTEXPR_SINCE_CXX20 static void destroy(allocator_type& __a, _Tp* __p) {
_LIBCPP_SUPPRESS_DEPRECATED_PUSH
__a.destroy(__p);
_LIBCPP_SUPPRESS_DEPRECATED_POP
}
template <class _Tp, class = void, __enable_if_t<!__has_destroy<allocator_type, _Tp*>::value, int> = 0>
_LIBCPP_HIDE_FROM_ABI _LIBCPP_CONSTEXPR_SINCE_CXX20 static void destroy(allocator_type&, _Tp* __p) {
std::__destroy_at(__p);
}


// construct_at.h

template <class _Tp, __enable_if_t<!is_array<_Tp>::value, int> = 0>
_LIBCPP_HIDE_FROM_ABI _LIBCPP_CONSTEXPR_SINCE_CXX20 void __destroy_at(_Tp* __loc) {
  _LIBCPP_ASSERT_NON_NULL(__loc != nullptr, "null pointer given to destroy_at");
  __loc->~_Tp();
}


// allocator.h
_LIBCPP_DEPRECATED_IN_CXX17 _LIBCPP_HIDE_FROM_ABI void destroy(pointer __p) { __p->~_Tp(); }
