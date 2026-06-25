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
    int x;
};

std::ostream& operator<<(std::ostream& os, const S& s) {
    os << "S{" << s.x << "}";
    return os;
}

std::istream& operator>>(std::istream& is, S& s) {
    is >> s.x;
    return is;
}

int main() {
    S s{5};

    std::cin >> s;
    std::cout << s << std::endl;

    std::ifstream is{"input.txt"};
    is >> s;

    std::ofstream os{"output.txt"};
    os << s << std::endl;

    std::stringstream ss;
    ss >> s;
    ss << s;

    ss << "abcd" << 123;
    
    std::string line;
    while (std::getline(is, line)) {

    }

    std::ifstream bis{"binary.in", std::ios::binary};
    bis.read();

    int number = 0;
    std::cin >> number;
    if (std::cin.fail()) {
        std::cin.clear();
        std::cout << "user is incompetent" << std::endl;
    }

    std::pair<int, double> pair;
    pair.first;
    pair.second;

    std::unordered_map<int, int> map {
        {0, 1},
        {1, 2}
    };

    for (const auto& [key, value] : map) {
    }

    auto [fst, snd] = pair;

    std::tuple<int, int, double, int> tuple = {
        1, 2, 3.14159, 4
    };

    std::get<0>(tuple);
    std::get<1>(tuple);
    std::get<2>(tuple);
    std::get<3>(tuple);

    auto [n1, n2, n3, n4] = tuple;

    std::vector<int> vec { 1, 2, 3, 4 };
    std::vector<int> res;

    std::copy_if(
            vec.begin(), vec.end(),
            std::back_inserter(res),
            [](int num) { return num%2 == 0; }
            );
}
