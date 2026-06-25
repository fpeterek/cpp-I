#include <algorithm>
#include <iostream>
#include <iterator>
#include <memory>
#include <print>
#include <string>
#include <unordered_map>
#include <vector>
#include <sstream>
#include <fstream>

struct S {
    int number;
};


std::ostream& operator<<(std::ostream& os, const S& s) {
    os << "S{" << s.number << "}";
    return os;
}

std::istream& operator>>(std::istream& is, S& s) {
    is >> s.number;
    return is;
}

struct S2 {
    int n1;
    int n2;
};

namespace std {
    template<unsigned long idx>
    auto& get(S2& s) {
        if constexpr (idx == 0) {
            return s.n1;
        }
        if constexpr (idx == 1) {
            return s.n2;
        }
    }
}

int main() {
    S s{5};

    std::cout << s << std::endl;

    std::stringstream ss;
    ss << s << std::endl;

    std::ofstream os{"file.txt"};
    os << s << std::endl;

    std::ifstream is{"input.txt"};

    int num = 0;
    is >> num;
    if (is.fail())  {
        is.clear();
    }

    std::string line;

    while (std::getline(is, line)) {

    }

    std::ifstream bis{"file.bin", std::ios::binary};

    std::unordered_map<int, int> map {
        {1, 2},
        {2, 3},
        {3, 4}
    };

    for (const auto& [key, val] : map) {

    }

    std::pair<std::string, int> pair { "abcd", 123 };

    const auto& [str, number] = pair;

    std::tuple<int, int, double, int> tuple {1, 2, 3.14159, 4};

    const auto [n1, n2, n3, n4] = tuple;

    std::get<0>(tuple);
    std::get<1>(tuple);
    std::get<2>(tuple);
    std::get<3>(tuple);


    std::vector<int> data { 1, 2, 3, 4 };

    std::vector<int> res;

    std::copy_if(
            data.begin(), data.end(),
            std::back_inserter(res),
            [](int x) { return x%2 == 0; }
            )
}
