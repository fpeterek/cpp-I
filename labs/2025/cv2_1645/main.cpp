#include <iostream>
#include <vector>


enum class Enum {
    Val1 = 0,
    Val2 = 1
};

int Val1 = 0;

enum struct Enum2 {
    Val1 = 0,
    Val2 = 1,
};

Enum getVal() {
    return Enum::Val1;
}

struct Bitfield {
    short a : 6;
    short b : 4;
    short c : 3;
};

union Union {
    float a;
    int b;
    double c;
};

struct Alignment {
    int32_t b;
    int16_t a;
    int8_t c;
};

class String  {
    std::vector<char> data;

    mutable int num;

public:

    static int a;

    size_t size() const;
    char& operator[](size_t idx);
    char operator[](size_t idx) const;

    void append(char c);

    String();
    explicit String(const std::vector<char>& data);
    String(const String& str);
};

void String::append(char c) {
    data.emplace_back(c);
}

int String::a = 0;

size_t String::size() const {
    return data.size();
}

char& String::operator[](size_t idx) {
    ++num;
    return data.at(idx);
}

char String::operator[](size_t idx) const {
    ++num;
    return data.at(idx);
}



String::String() {
    data.reserve(64);
}

String::String(const std::vector<char>& data) : data(data) { }

String::String(const String& str) : String(str.data) { }

void printString(const String& str) {
    for (size_t i = 0; i < str.size(); ++i) {
        std::cout << str[i];
    }
    std::endl(std::cout);
}

std::ostream& operator<<(std::ostream& os, const String& str) {
    for (size_t i = 0; i < str.size(); ++i) {
        std::cout << str[i];
    }
    return os;
}

std::istream& operator>>(std::istream& is, String& str) {
    while (is) {
        if (std::isspace(is.peek())) {
            break;
        }
        char c = 0;
        is >> c;
        str.append(c);
    }
    return is;
}

int main() {

    String str;
    std::cout << "$ ";
    std::cin >> str;
    std::cout << str << std::endl;
    {
        std::vector<int> vec { 1, 2, 3, 4 };
    }

    struct {
        int a;
        int b;
        int c;
    } S { 5, 10, 15 };

    printString(String { { 'v', 's', 'b' } });

    std::cout << sizeof(Bitfield) << std::endl;

    Union u;
    u.a = 5.5;
    // ...
    u.c = 10.6;

    std::cout << sizeof(Alignment) << std::endl;
}
