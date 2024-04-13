#include <iostream>
#include <sstream>
#include <stdexcept>
#include <tuple>

#include "tuple.hpp"


template<typename T, std::enable_if_t<std::is_same_v<T, int>, bool> = true>
int load_item(const std::string& str) {
    return std::stoi(str);
}

template<typename T, std::enable_if_t<std::is_same_v<T, double>, bool> = true>
double load_item(const std::string& str) {
    return std::stod(str);
}

template<typename T, std::enable_if_t<std::is_same_v<T, std::string>, bool> = true>
std::string load_item(const std::string& str) {
    return str;
}

template<typename T, std::enable_if_t<std::is_same_v<T, bool>, bool> = true>
bool load_item(const std::string& str) {
    if (str == "true") {
        return true;
    } else if (str == "false") {
        return false;
    }
    throw std::runtime_error("Invalid value for type bool");
}

template<int idx>
void parse_item([[maybe_unused]] auto tuple, [[maybe_unused]] std::stringstream& ss) { }

template<int idx, typename T, typename... Ts>
void parse_item(auto& tuple, std::stringstream& ss) {

    std::string item;
    std::getline(ss, item, ',');

    std::get<idx>(tuple) = load_item<T>(item);

    parse_item<idx+1, Ts...>(tuple, ss);
}

template<template<typename...> typename TupleType, typename... T>
TupleType<T...> parse(const std::string& str) {
    TupleType<T...> tuple;
    std::stringstream ss { str };

    parse_item<0, T...>(tuple, ss);

    return tuple;
}

int main() {

    auto tuple = parse<std::tuple, int, double, std::string, bool>("666,6.9,pseudosacrosanct perversion,true");
    std::get<0>(tuple);

    std::cout
        << std::boolalpha
        << std::get<0>(tuple) << ", "
        << std::get<1>(tuple) << ", "
        << std::get<2>(tuple) << ", "
        << std::get<3>(tuple) << std::endl;

    auto parsed = parse<fp::tuple, int, double, std::string, bool>("123,4.56,string,false");

    fp::tuple manuallyConstructed { 1, 2.4, std::string("str"), false };

    fp::tuple<int, std::string, bool> empty;
    std::cout << parsed << std::endl;
    std::cout << manuallyConstructed << std::endl;

    std::cout << "parsed.get<2>(): " << parsed.get<2>() << std::endl;
    std::cout << "std::get<1>(parsed): " << std::get<1>(parsed) << std::endl;

}
