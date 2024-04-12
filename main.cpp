#include <iostream>
#include <sstream>
#include <stdexcept>
#include <tuple>


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

template<typename... T>
std::tuple<T...> parse(const std::string& str) {
    std::tuple<T...> tuple;
    std::stringstream ss { str };

    parse_item<0, T...>(tuple, ss);

    return tuple;
}

template<int idx>
void printTuple([[maybe_unused]] const std::tuple<>& tuple, [[maybe_unused]] std::ostream& os) { }


int main() {

    auto tuple = parse<int, double, std::string, bool>("666,6.9,pseudosacrosanct perversion,true");
    std::get<0>(tuple);

    std::cout
        << std::get<0>(tuple) << ", "
        << std::get<1>(tuple) << ", "
        << std::get<2>(tuple) << ", "
        << std::get<3>(tuple) << std::endl;

}
