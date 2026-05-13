#include <print>


#include "variant.hpp"


struct S {

    size_t x;

    S() { std::println("S()"); }
    S(const S& other) : x{other.x} { std::println("S(const S&)"); }
    S(S&& other) : x{other.x} { std::println("S(S&&)"); }
    S(size_t x) : x{x} { std::println("S(size_t)"); }
    ~S() { std::println("~S()"); }

    S& operator=(const S& other) {
        x = other.x;
        std::println("S::operator=(const S& other)");
        return *this;
    }
    S& operator=(S&& other) {
        x = other.x;
        std::println("S::operator=(S&& other)");
        return *this;
    }
};


int main() {

    size_t sz = fp::max_size<int, std::string, float, size_t>;

    std::println("fp::max_size<int, std::string, float, size_t> = {} bytes", sz);


    fp::variant<int, float, std::string, S> var;
    std::println();
    std::println("fp::get<int>(var) = {}", fp::get<int>(var));
    std::println();

    var = 3.14159f;
    std::println();
    std::println("fp::get<float>(var) = {}", fp::get<float>(var));
    std::println();

    var = std::string("jebat javascript");
    std::println();
    std::println("fp::get<std::string>(var) = {}", fp::get<std::string>(var));
    std::println();

    var = S{69};
    std::println();
    std::println("fp::get<S>(var).x = {}", fp::get<S>(var).x);
    std::println();

    fp::get<S>(var).x = 1987;
    std::println();
    std::println("assigning fp::get<S>(var).x = 1987;");
    std::println("fp::get<S>(var).x = {}", fp::get<S>(var).x);
    std::println();

    var = 1899;
    std::println();
    std::println("fp::get<int>(var) = {}", fp::get<int>(var));
    std::println();

}
