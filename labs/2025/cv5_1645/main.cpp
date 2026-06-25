#include <algorithm>
#include <iostream>
#include <memory>
#include <print>
#include <vector>


std::vector<int> createVec() {
    std::vector<int> v;

    for (int i = 0; i < 10; ++i) {
        v.emplace_back(i);
    }

    return v;
}

void createVec(std::vector<int>& v) {
    for (int i = 0; i < 10; ++i) {
        v.emplace_back(i);
    }
}

class C {

    std::vector<int> data;
    int* array;
    size_t size;

public:

    class Iter {
        friend class C;
        int* ptr;

        Iter(int* ptr) : ptr{ptr} { };
    public:
        bool operator==(const Iter& iter) {
            return ptr == iter.ptr;
        }

        bool operator!=(const Iter& iter) {
            return ptr != iter.ptr;
        }

        int& operator*() {
            return *ptr;
        }

        Iter& operator++() {
            ++ptr;
            return *this;
        }

        Iter operator++(int) {
            Iter copy{ptr};
            ++ptr;
            return copy;
        }
    };

    auto begin() {
        std::cout << size << std::endl;
        return Iter(array);
    }

    auto end() {
        return Iter(array+size);
    }

    C(std::vector<int> data);
    C(const C& other);
    C(C&& other);

    ~C();
};

C::C(C&& other) :
        data(std::move(other.data)),
        array{other.array},
        size{other.size} {
    other.size = 0;
    other.array = nullptr;
}

C::C(std::vector<int> data) :
        data{std::move(data)} {
    array = new int[this->data.size()];
    size = this->data.size();
    std::copy(this->data.begin(), this->data.end(), array);
}

C::~C() {
    delete[] array;
}

class Tree {

    Tree* parent;
    std::unique_ptr<Tree> left;
    std::unique_ptr<Tree> right;
    std::shared_ptr<std::string> data;

public:

    Tree(
         Tree* parent,
         std::unique_ptr<Tree> left,
         std::unique_ptr<Tree> right,
         std::shared_ptr<std::string> data);


    void setLeft(std::unique_ptr<Tree> left);
    void setRight(std::unique_ptr<Tree> right);
};

Tree::Tree(
         Tree* parent,
         std::unique_ptr<Tree> left,
         std::unique_ptr<Tree> right,
         std::shared_ptr<std::string> data) :
        parent{parent},
        left{std::move(left)},
        right{std::move(right)},
        data{std::move(data)} { }

void Tree::setLeft(std::unique_ptr<Tree> left) {
    this->left = std::move(left);
    if (this->left != nullptr) {
        this->left->parent = this;
    }
}

void Tree::setRight(std::unique_ptr<Tree> right) {
    this->right = std::move(right);
    if (this->right != nullptr) {
        this->right->parent = this;
    }
}

int main() {

    std::vector<int> v = createVec();

    C c1{v};
    C c2{createVec()};
    C c3{std::move(v)};

    for (auto i : c3) {
        std::cout << i << std::endl;
    }

    std::vector<int> vec { 1, 2, 3, 4 };
    // ...
    
    vec = createVec();

    auto data = std::make_shared<std::string>("aaaaaa");

    auto left = std::make_unique<Tree>(
        nullptr,
        nullptr,
        nullptr,
        data
    );

    auto right = std::make_unique<Tree>(
        nullptr,
        nullptr,
        nullptr,
        nullptr
    );

    auto tree = std::make_unique<Tree>(
        nullptr,
        nullptr,
        nullptr,
        data
    );

    tree->setLeft(std::move(left));
    tree->setRight(std::move(right));
    

    for (auto i : vec) {

    }

    for (auto iter = vec.begin(); iter != vec.end(); ++iter) {
        auto i = *iter;
        // ...
    }

}
