#include <print>
#include <iostream>
#include <stdexcept>
#include <variant>
#include <vector>
#include <string>



enum class Enum {
    ABCD = 1,
    EFGH = 2
};

enum struct E2 {
    ABCD = 1,
};


enum {
    AAA = 1,
    BBB = 2
};


struct Bitfields {
    char a : 4;
    char b : 4;
};

class Class {
    int a{};

public:
    explicit Class(int a) : a{a} {
        if (a == 5) {
            throw std::runtime_error("error");
        }
    }

    Class(const Class& other) : a{other.a} { }

    Class& operator=(const Class& other);
};

Class& Class::operator=(const Class& other) {
    a = other.a;
    return *this;
}

void fn(const Class& c) {

}


class A { 

};

class B : public A {

    mutable int member;

    void fn() const;

    friend class AAAA;

    friend void fun(B& b);
};


struct C : A {
    int a;


    static int staticA;
};

int C::staticA = 5;


int main() {

    fn(Class { 25 });

    std::println("Hello, World");

    std::vector<int> vec { 1, 2, 3, 4, 5, 6 };

    Enum e = Enum::ABCD;

    std::variant<std::string, int, float> var { "abcd" };

    var = 25;

    if (std::holds_alternative<int>(var)) {
        std::get<int>(var);
    }

    {
        std::ifstream is{"file.txt"};
    }

    if (1 == 1) {
        std::vector<int> v1 = { 1, 2, 3 };

        std::println("");
        std::println("");
        std::println("");
    }

}
