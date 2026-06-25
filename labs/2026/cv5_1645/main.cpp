#include <algorithm>
#include <memory>
#include <string_view>
#include <print>
#include <vector>


class StringWrapper {

    std::string str;

public:

    auto begin() {
        return str.begin();
    }

    auto end() {
        return str.end();
    }

    StringWrapper(std::string str) :
        str{std::move(str)} { }

    StringWrapper(StringWrapper&& other) :
        str{std::move(other.str)} { }
};

class Vector {

    int* begin_ptr = nullptr;
    int* end_ptr = nullptr;
    size_t capacity = 0;

public:

    class Iterator {

        int* ptr;

        Iterator(int* ptr) : ptr{ptr} { }

        friend Vector;
        
    public:

        Iterator& operator++() {
            ++ptr;
            return *this;
        }

        Iterator operator++(int) {
            Iterator copy{ptr};
            ++ptr;
            return copy;
        }

        Iterator& operator--() {
            --ptr;
            return *this;
        }

        Iterator operator+(int x) const {
            return { ptr + x };
        }

        bool operator==(const Iterator& other) const {
            return ptr == other.ptr;
        }

        bool operator!=(const Iterator& other) const {
            return not (*this == other);
        }

        int& operator*() {
            return *ptr;
        }

    };

    Iterator begin() {
        return { begin_ptr };
    }

    Iterator end() {
        return { end_ptr };
    }


    Vector(const Vector&);

    Vector(Vector&& vec) :
        begin_ptr{vec.begin_ptr},
        end_ptr{vec.end_ptr},
        capacity{vec.capacity}
    {
        vec.begin_ptr = nullptr;
        vec.end_ptr = nullptr;
        vec.capacity = 0;
    }

    ~Vector() {
        delete[] begin_ptr;
    }

    Vector& operator=(const Vector&);

    Vector& operator=(Vector&& other) {
        std::swap(capacity, other.capacity);
        std::swap(begin_ptr, other.begin_ptr);
        std::swap(end_ptr, other.end_ptr);

        return *this;
    }

    int pop_back();
    void emplace_back(int item);
    int operator[](size_t index);
};

std::string process_string(std::string str) {
    str += "Jo soc culer";

    std::string str2 = "Un dia de partit";

    return rand() % 2 ? str : str2;
}


int main() {

    std::string str = "abcd";

    str.operator+=("def");

    str = "abcdefg";

    std::string str2 = str + "defg";

    std::string processed = process_string(str);

    std::string processed2 = 
        process_string(std::string { "tmp" });

    StringWrapper wrapper { std::move(str) };
    
    std::shared_ptr<std::string> shared
        = std::make_shared<std::string>("abcd");

    std::unique_ptr<std::string> unique
        = std::make_unique<std::string>("abcd");

    std::vector<int> vec { 1, 2, 3 };

    for (size_t i = 0; i < vec.size(); ++i) {
        std::println("{}", vec[i]);
    }

    for (int i : vec) {
        std::println("{}", i);
    }

    for (auto it = vec.begin(); it != vec.end(); ++it) {
        auto elem = *it;

        //
        std::println("{}", elem);
    }

    void process_str(std::string_view);

    std::string cpp_str = "abcd";
    const char* c_str = "abcd";

    process_str(cpp_str);
    process_str(c_str);

    std::string_view sv;
}


void process_str(std::string_view sv) {
}

