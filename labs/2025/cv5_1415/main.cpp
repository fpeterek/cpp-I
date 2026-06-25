#include <iostream>
#include <memory>
#include <print>
#include <vector>

std::vector<int> createVec() {
    std::vector<int> vec;
    vec.reserve(10);
    for (int i = 0; i < 10; ++i) {
        vec.emplace_back(i);
    }

    return vec;
}

void createVec(std::vector<int>& vec) {
    vec.reserve(10);
    for (int i = 0; i < 10; ++i) {
        vec.emplace_back(i);
    }
}

class C {

    std::vector<int> vec;
    int* array = nullptr;
    size_t size = 0;

public:

    class Iter {
        int* data;

        friend class C;

        Iter(int* ptr) : data(ptr) { }

    public:

        int& operator*() {
            return *data;
        }

        Iter& operator++() {
            ++data;
            return *this;
        }

        Iter operator++(int) {
            Iter copy{data};
            ++data;
            return copy;
        }

        bool operator==(const Iter& other) {
            return data == other.data;
        }

        bool operator!=(const Iter& other) {
            return not (*this == other);
        }

    };

    auto begin() {
        return Iter(array);
    }

    auto end() {
        return Iter(array+size);
    }

    C() = default;
    C(std::vector<int> data);
    C(const C& other);
    C(C&& other);

    ~C();

    C& operator=(C&& other);

};

C::~C() {
    delete[] array;
}

C::C(std::vector<int> data) :
        vec{std::move(data)} {
    array = new int[vec.size()];

    for (size_t i = 0; i < vec.size(); ++i) {
        array[i] = vec[i]*2;
    }

    size = vec.size();
}

C::C(C&& other) :
        vec{std::move(other.vec)},
        array{other.array},
        size{other.size} {
    other.array = nullptr;
    size = 0;
}


class Tree {

    Tree* parent;
    uint64_t num;
    std::shared_ptr<std::vector<int>> data;
    std::unique_ptr<Tree> left;
    std::unique_ptr<Tree> right;

public:

    Tree(Tree* parent,
         std::shared_ptr<std::vector<int>> data,
         std::unique_ptr<Tree> left,
         std::unique_ptr<Tree> right,
         uint64_t num);

    void setLeft(std::unique_ptr<Tree> value);
    void setRight(std::unique_ptr<Tree> value);

};

void Tree::setLeft(std::unique_ptr<Tree> value) {
    left = std::move(value);
    left->parent = this;
}

void Tree::setRight(std::unique_ptr<Tree> value) {
    right = std::move(value);
    right->parent = this;
}

Tree::Tree(Tree* parent,
           std::shared_ptr<std::vector<int>> data,
           std::unique_ptr<Tree> left,
           std::unique_ptr<Tree> right,
           uint64_t num) :
        parent{parent},
        data{std::move(data)},
        num{num},
        left{std::move(left)},
        right{std::move(right)} { }



int main() {

    std::vector<int> vec = createVec();
    
    C c1{vec };
    C c2{std::move(vec)};
    C c3{ std::vector{ 1, 2, 3 } };
    C c4{ createVec() };

    for (auto i : c1) {
        std::cout << i << std::endl;
    }

    auto data = std::make_shared<std::vector<int>>(createVec());
    
    auto left =
        std::make_unique<Tree>(
                nullptr,
                data,
                nullptr,
                nullptr,
                1
        );

    auto right =
        std::make_unique<Tree>(
                nullptr,
                nullptr,
                nullptr,
                nullptr,
                2
        );

    auto tree =
        std::make_unique<Tree>(
                nullptr,
                data,
                std::move(left),
                std::move(right),
                0
        );

    tree->setLeft(std::move(left));
    tree->setRight(std::move(right));


    const std::vector<int> v2 = [] {
        std::vector<int> v;
        for (size_t i = 0; i < 1'000'000; ++i) {
            v.emplace_back(i);
        }
        return v;
    }();


    for (int i : v2) {
        
    }

    for (auto iter = v2.begin(); iter != v2.end(); ++iter) {

        int i = *iter;

        // ...

    }

    for (auto iter = v2.rbegin(); iter != v2.rend(); ++iter) {

    }


}
