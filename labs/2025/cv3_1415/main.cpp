#include <iostream>
#include <new>
#include <string_view>
#include <vector>
#include <format>
#include <print>
#include <functional>


void acceptStringC(const char* ptr, size_t size) {
}

void acceptStringCpp(std::string_view sv) {
}


void printRes(int x, int y, const auto& fn) {
    // Only in new compilers, replace with std::cout if needed
    // Requires C++23
    std::println("{} op {} = {}", x, y, fn(x, y));
}

template <typename T>
void printRes2(int x, int y, const T& fn) {
    std::println("{} op {} = {}", x, y, fn(x, y));
}

void printRes3(int x, int y, const std::function<int(int, int)>& fn) {
    std::println("{} op {} = {}", x, y, fn(x, y));
}


int main() {


    class Lambda {
    public:
        int operator()(int x, int y) const {
            return x+y;
        }
    };
    Lambda lambda;

    auto fn = [](int x, int y) -> int {
        return x+y;
    };
    std::println("{}", typeid(fn).name());

    auto fn2 = [](int x, int y) -> int {
        return x*y;
    };

    std::string str = "aaa";
    int num = 10;

    auto capture = [&str, num, calls = 0]() mutable -> void {
        ++calls;
        std::println("({}) {} {}", calls, str, num);
    };

    for (int i = 0; i < 10; ++i) {
        capture();
    }

    std::function<int(int, int)> fn_wrapper = fn;
    fn_wrapper = fn2;

    printRes(5, 10, lambda);
    printRes(5, 10, fn);
    printRes(5, 10, fn2);

    printRes3(5, 10, lambda);
    printRes3(5, 10, fn);
    printRes3(5, 10, fn2);

    struct S {
        S() {
            std::println("S()");
        }
        ~S() {
            std::println("~S()");
        }
    };

    std::println("new");
    S* arr1 = new S[5];
    std::println("malloc");
    S* arr2 = static_cast<S*>(malloc(sizeof(S) * 5));
    delete[] arr1;
    free(arr2);

    std::println("operator new");

    S* mem1 = static_cast<S*>(operator new[](sizeof(S) * 5));

    new(mem1+0) S{};
    mem1->~S();

    operator delete[](mem1);

    std::println("std::allocator");

    std::allocator<S> alloc;

    S* mem2 = alloc.allocate(5);

    new(mem2+0) S{};
    mem2->~S();

    alloc.deallocate(mem2, 5);

    struct String {
        union {
            struct {
                size_t reserved;
            } reserved;
            char local[16];
        } string_data;

        size_t size;
        char* data;

        String() {
            data = string_data.local;
        }

        void alloc_str() {
            data = new char[256];
        }

        char* c_str() {
            return data;
        }
    };

    std::string_view sv = "abcd";
    acceptStringCpp("abcd");

    std::println("{}", sizeof(std::string));
}
