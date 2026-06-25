#include <iostream>
#include <print>
#include <variant>
#include <vector>
#include <cstring>
#include <array>

struct AircraftId {
    int id;
};

struct RadarId {
    int id;
};

void fn1(AircraftId aircraftId, RadarId radarId) {

}

class Animal {

protected:
    int attr = 0;

public:
    Animal() = default;
    virtual void makeNoise() = 0;
};

class C1 { 
protected:
    int attr = 5;
};

class Dog : public Animal, public C1 {

public:
    void makeNoise() final;
};

void Dog::makeNoise() {
    Animal::attr;
    C1::attr;
}


class String {
    std::vector<char> data;

public:
    String(const std::vector<char>& data);
};

String::String(const std::vector<char>& data) :
    data(data) { }

class UTF8String {
    char* data;
    size_t length;
    size_t reserved;

public:
    UTF8String(const char* str, size_t len);
    UTF8String(const char* str);
    UTF8String(const std::string& str);

    UTF8String(std::string_view sv);
};

UTF8String::UTF8String(std::string_view sv) :
    data(new char[sv.length()]),
    length(sv.length()),
    reserved(sv.length()) {
    // copy data
}

UTF8String::UTF8String(const char* str, size_t len) :
    data(new char[len]),
    length(len),
    reserved(len) {
    (void)str;
    // copy data
}

UTF8String::UTF8String(const char* str) :
    UTF8String(str, strlen(str)) { }

UTF8String::UTF8String(const std::string& str) :
    UTF8String(str.c_str(), str.size()) { }

template<size_t arrSize>
void acceptArray(std::array<int, arrSize>& arr) {
}

class JsonObject { };
class JsonInt { };
class JsonString { };

using JsonElement = std::variant<JsonObject, JsonInt, JsonString>;

int main() {
    RadarId radar { 25 };
    AircraftId aircraft { 77 };

    std::vector<int> vec;

    fn1(aircraft, radar);

    Dog dog{};
    dog.makeNoise();

    Animal& animal = dog;
    animal.makeNoise();

    Dog& dogref = dynamic_cast<Dog&>(animal);

    std::array<int, 4> arr = { 1, 2, 3, 4 };

    arr.size();
    acceptArray(arr);

    JsonElement je = JsonObject{};

    if (std::holds_alternative<JsonObject>(je)) {
        JsonObject& object = std::get<JsonObject>(je);
        // ...
    } else if (std::holds_alternative<JsonString>(je)) {
        JsonString& str = std::get<JsonString>(je);
        // ...
    }

}
