#include <iostream>
#include <vector>


enum class Enum {
    Val1 = 0,
    Val2 = 1
};

int Val1 = 0;

enum class E2 {
    Val3,
    Val4
};

Enum getVal() {
    return Enum::Val1;
};

struct Struct {
    short a : 6;
    short b : 3;
    short c : 5;
};

union Union {
    int a;
    float b;
};

struct Align {
    int32_t b;
    int16_t a;
    int8_t c;
};


class String {
    std::vector<char> data;

    mutable int a = 0;

public:

    static const String empty;

    size_t size() const;

    char& operator[](size_t idx);
    char operator[](size_t idx) const;

    void append(char c);

    String() {
        data.reserve(64);
    }
    explicit String(const std::vector<char>& data);
    String(const String& str);
};

void String::append(char c) {
    data.emplace_back(c);
}

const String String::empty{};

char& String::operator[](size_t idx) {
    return data.at(idx);
}

char String::operator[](size_t idx) const {
    return data.at(idx);
}

size_t String::size() const {
    return data.size();
}

String::String(const String& str) : data(str.data) { }

String::String(const std::vector<char>& data) : data(data) { }

void printString(const String& str) {
    for (size_t i = 0; i < str.size(); ++i) {
        std::cout << str[i];
    }
    std::endl(std::cout);
}

std::ostream& operator<<(std::ostream& os, const String& str) {
    for (size_t i = 0; i < str.size(); ++i) {
        os << str[i];
    }
    return os;
}

std::istream& operator>>(std::istream& is, String& str) {
    while (is) {
        if (not isspace(is.peek())) {
            char c = 0;
            is >> c;
            str.append(c);
        } else {
            break;
        }
    }
    return is;
}

int main() {
    std::cout << sizeof(Struct) << std::endl;

    printString(String({ 'h', 'a', 's', 'k', 'e', 'l', 'l' }));

    String str;
    {
        String str2 { str };
    }

    struct S {
        int a;
        int b;
        int c;
    } myS { 10, 20, 30 };

    std::cout << "$ ";
    std::cin >> str;
    std::cout << str << std::endl;


    Union u;
    u.a = 10;
    u.b = 5.5;

    std::cout << sizeof(Align) << std::endl;
}
