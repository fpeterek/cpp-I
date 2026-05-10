#include <algorithm>
#include <print>
#include <stdexcept>

#include "vector.hpp"



struct Breakable {

    static int counter;

    Breakable() {
        std::println("Breakable()");
        ++counter;

        if (counter % 5 == 0) {
            throw std::runtime_error("Exception");
        }
    }

    Breakable(const Breakable&) : Breakable() {  }

    ~Breakable() {
        std::println("~Breakable()");
    }

};

int Breakable::counter = 0;


int main() {
    fp::vector<int> intvec { 1, 2, 3, 4, 5, 6 };

    for (int& i : intvec) {
        i *= i;
    }

    for (int i = 0; i < 50; ++i) {
        intvec.emplace_back(0);
    }

    std::ranges::for_each(intvec, [c = 0](const int i) mutable {
        std::print("{:3} ", i);
        ++c;
        if (c % 10 == 0) {
            std::println();
        }
    });

    std::println();


    try {
        fp::vector<Breakable> breakablevec { 10, Breakable{} };
    } catch (const std::runtime_error&) {
        std::println("Exception caught");
    }
}

