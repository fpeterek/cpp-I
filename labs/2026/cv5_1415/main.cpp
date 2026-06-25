#include <algorithm>
#include <iterator>
#include <memory>
#include <print>
#include <functional>
#include <string_view>
#include <vector>
#include <iostream>
#include <fstream>


class Vec {

    int* begin_ptr = nullptr;
    int* end_ptr = nullptr;
    size_t capacity = 0;

public:

    class iterator {

        int* ptr;

        friend class Vec;

        explicit iterator(int* ptr) : ptr{ptr} { }

    public:

        iterator& operator++() {
            ++ptr;
            return *this;
        }

        iterator operator++(int) {
            iterator copy { ptr };
            ++ptr;
            return copy;
        }

        iterator operator+(std::ptrdiff_t x) {
            return iterator { ptr + x };
        }

        iterator& operator+=(std::ptrdiff_t x) {
            ptr += x;
            return *this;
        }

        int& operator*() {
            return *ptr;
        }

        bool operator==(const iterator& other) const {
            return ptr == other.ptr;
        }

        bool operator!=(const iterator& other) const {
            return not this->operator==(other);
        }

    };

    iterator begin() {
        return iterator { begin_ptr };
    }
    
    iterator end() {
        return iterator { end_ptr };
    }

    Vec() noexcept = default;

    Vec(const Vec& orig) {

        if (orig.capacity != 0) {
            begin_ptr = new int[orig.capacity];
            const size_t size = orig.size();
            std::copy_n(orig.begin_ptr, size, begin_ptr);
            end_ptr = begin_ptr + size;

            capacity = orig.capacity;
        }

    }

    Vec(Vec&& tmp) {
        begin_ptr = tmp.begin_ptr;
        tmp.begin_ptr = nullptr;
        end_ptr = tmp.end_ptr;
        tmp.end_ptr = nullptr;
        capacity = tmp.capacity;
        tmp.capacity = 0;
    }

    Vec& operator=(const Vec& other) {

        if (capacity < other.capacity) {
            // reallocate
        }

        const size_t size = other.size();
        std::copy_n(other.begin_ptr, size, begin_ptr);
        end_ptr = begin_ptr + size;

        capacity = other.capacity;

        return *this;
    }

    Vec& operator=(Vec&& other) {
        delete[] begin_ptr;

        begin_ptr = other.begin_ptr;
        end_ptr = other.end_ptr;
        capacity = other.capacity;

        other.begin_ptr = nullptr;
        other.end_ptr = nullptr;
        other.capacity = 0;

        return *this;
    }

    ~Vec() {
        delete[] begin_ptr;
    }

    int& operator[](size_t index) {
        return begin_ptr[index];
    }
    
    void emplace_back(int item) {
        const size_t size = this->size();
        if (size == capacity) {
            capacity = std::max(
                    static_cast<size_t>(32), 
                    capacity*2);
            int* new_data = new int[capacity];
            std::copy(begin_ptr, end_ptr, new_data);
            delete[] begin_ptr;
            begin_ptr = new_data;
            end_ptr = begin_ptr + size;
        }

        *end_ptr = item;
        ++end_ptr;
    }

    size_t size() const {
        return static_cast<size_t>(end_ptr - begin_ptr);
    }

};


class VecWrapper {

    std::vector<int> vec;

public:

    VecWrapper(std::vector<int> vec) :
        vec{std::move(vec)} { }

    VecWrapper(VecWrapper&& other) :
        vec{std::move(other.vec)} { }

};


Vec createVec() {
    Vec vec;

    for (int i = 0; i < 10; ++i) {
        vec.emplace_back(i*i);
    }

    return vec;
}


Vec mutate(Vec vector) {
    vector[0] = 1899;
    return vector;
}


struct Tree {

    Tree* parent;

    std::unique_ptr<Tree> left;
    std::unique_ptr<Tree> right;
};

struct string_view {

    const char* data;
    size_t size;

};


void consume_string(std::string_view sv) {
    // ...
}

int main() {
    std::string str;

    str = "abcd";

    consume_string(str);
    consume_string("abcd");

    std::string str2 = str + "def";

    str2 = std::string { "Jo soc culer" };

    std::string_view sv = str2;


    auto vec_ptr
        = std::make_shared<Vec>();

    auto vec_ptr2
        = std::make_unique<Vec>();

    *vec_ptr;
    vec_ptr->size();

    Vec vec = createVec();
    Vec vec2 { createVec() };

    Vec vec3 = mutate(vec);
    Vec vec4 = mutate(createVec());


    void printSomething(std::ostream&);

    std::ifstream is { "abcd.txt" };
    std::ofstream os { "bcde.txt" };

    printSomething(os);


    Vec iterable = createVec();

    for (int i : iterable) {
        std::println("vec item: {}", i);
    }

    std::vector<int> vec_int { 1, 2, 3 };

    std::vector<int> results;

    std::transform(
            vec_int.begin(), vec_int.end(),
            std::back_inserter(results),
            [](int num) { return num*num; }
            );

}


void printSomething(std::ostream& os) {

}

