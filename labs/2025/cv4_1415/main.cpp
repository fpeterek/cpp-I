#include <iostream>
#include <print>
#include <array>
#include <variant>


class Animal {

protected:
    Animal() = default;

public:

    virtual void makeNoise() = 0;
};

class Dog : public Animal {

public:

    void makeNoise() final;

};


class UTF8String {

    size_t reserved;
    size_t length;
    char* data;

public:

    // UTF8String(const char* str, size_t);
    // UTF8String(const char* str);
    // UTF8String(const std::string& str);
    UTF8String(std::string_view sv);
};

UTF8String::UTF8String(std::string_view sv) :
    reserved(sv.length()),
    length(sv.length()),
    data(new char[sv.length()]) {
    // copy data
}

// UTF8String::UTF8String(const char* str, size_t len) : 
//     reserved(len),
//     length(len),
//     data(new char[len]) {
//
//     }
//
// UTF8String::UTF8String(const char* str) :
//     UTF8String(str, strlen(str)) { }
//
// UTF8String::UTF8String(const std::string& str) :
//     UTF8String(str.c_str(), str.size()) { }


void Dog::makeNoise() {
    std::println("haf");
}


void makeNoise(Dog& dog) {
    dog.makeNoise();
}

void makeNoise(Animal& animal) {
    animal.makeNoise();
}

template<size_t arrSize>
void receiveArray(std::array<int, arrSize> arr) {

}

class Desktop { };
class Laptop { };
class Mobile { };

using Device = std::variant<Desktop, Laptop, Mobile>;

int main() {

    auto arr = std::array { 1, 2, 3, 4 };
    arr.size();
    receiveArray(arr);

    Dog dog{};
    dog.makeNoise();

    Animal& animal = dog;

    Dog& dogref = dynamic_cast<Dog&>(animal);

    Device dev = Laptop{};

    if (std::holds_alternative<Laptop>(dev)) {
        std::get<Laptop>(dev);
    } else if (std::holds_alternative<Desktop>(dev)) {
        std::get<Desktop>(dev);
    }



}
