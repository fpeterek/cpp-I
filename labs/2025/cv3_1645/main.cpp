#include <format>
#include <print>
#include <functional>


void printRes(int x, int y, const auto& fn) {
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
            return x + y;
        }
    };
    Lambda lambda;

    auto fn = [](int x, int y) {
        return x + y;
    };

    std::println("{}", typeid(fn).name());

    auto fn2 = [](int x, int y) {
        return x * y;
    };

    std::function<int(int, int)> fn_var = fn;
    fn_var = fn2;
    fn_var = lambda;

    std::string str = "aaa";
    int num = 0;

    auto capture = [&str, num, calls = 0] mutable {
        ++calls;
        std::println("({}) {} {}", calls, str, num);
    };

    for (int i = 0; i < 5; ++i) {
        capture();
    }

    const std::vector<int> vec = [] {
        std::vector<int> v;
        for (int i = 0; i < 50'000; ++i) {
            v.emplace_back(i*i);
        }
        return v;
    }();

    printRes(5, 10, fn);
    printRes(5, 10, fn2);
    printRes(5, 10, lambda);

    printRes3(5, 10, fn);
    printRes3(5, 10, fn2);
    printRes3(5, 10, lambda);

    struct S {
        S() {
            std::println("S()");
        }
        ~S() {
            std::println("~S()");
        }
    };

    std::println("new/delete");

    S* arr1 = new S[5];
    delete[] arr1;

    std::println("malloc/free");

    S* arr2 = static_cast<S*>(malloc(sizeof(S) * 5));
    free(arr2);

    std::println("operator new");

    S* mem1 = static_cast<S*>(operator new[](sizeof(S) * 5));

    new(mem1) S{};
    mem1->~S();

    operator delete[](mem1);

    std::println("std::allocator");

    std::allocator<S> alloc;
    S* mem2 = alloc.allocate(5);

    new(mem2) S{};
    mem2->~S();

    alloc.deallocate(mem2, 5);

    struct String {
        union {
            struct {
                size_t reserved;
            } reserved;
            char local[16];
        } data;

        char* str;
        size_t size;

        String() {
            str = data.local;
        }

        void alloc() {
            data.reserved.reserved = 512;
            str = new char[512];
        }

        char* c_str() {
            return str;
        }
    };

    const char* c_str = "abcd";
    std::string cpp_str = c_str;
    std::string_view sv = cpp_str;
}
