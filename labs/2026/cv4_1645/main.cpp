#include <print>
#include <functional>
#include <vector>


class Parent {

public:
    virtual void mem_fn() {
        std::println("Parent::mem_fn");
    }
};

class Child : public Parent {

public:
    void mem_fn() override {
        std::println("Child::mem_fn");
    }
};

void call_mem_fn(Parent& p) {
    p.mem_fn();
}

void subclass_to_parent(Parent& p) {

}

void parent_to_subclass(Parent& p) {
    Child& c = dynamic_cast<Child&>(p);

    Child* c_ptr = dynamic_cast<Child*>(&p);
}

struct C1 {
    int x = 0;
};

struct C2 {
    int x = 5;
};

struct C3 {};

struct MultInh : C1, C2, C3 {
    void print_x() {
        std::println("C1::x={}", C1::x);
        std::println("C2::x={}", C2::x);
    }
};

struct Interface {
    virtual void pure_virtual() = 0;
};

struct ParentCons {
    ParentCons() {
        std::println("Parent constructor");
    }
};

struct Constructors : ParentCons {

    std::vector<int> vec;
    std::string str;
    int x;

    Constructors() : ParentCons(), vec{}, str{"abcd"}, x{} {
    }

    Constructors(int a, int b) : Constructors() {
        x = a+b;
    }

};


struct S {
    S() {
        std::println("S::S()");
    }

    ~S() {
        std::println("S::~S()");
    }
};


int main() {

    int* malloced = (int*)malloc(sizeof(int) * 10);

    for (int i = 0; i < 10; ++i) {
        std::println("{}", malloced[i]);
    }

    {
        S s;
    }

    S* s_new = new S;
    delete s_new;

    S* s_arr = new S[10];
    delete[] s_arr;

    std::vector<S> vec;
    vec.reserve(10);

    S* s_uninit = static_cast<S*>(operator new(sizeof(S) * 5));

    new(s_uninit+0) S;
    new(s_uninit+1) S;
    new(s_uninit+2) S;

    s_uninit[0].~S();
    s_uninit[1].~S();
    s_uninit[2].~S();

    operator delete(s_uninit);

    std::allocator<S> alloc;

    S* s_alloc = alloc.allocate(10);

    alloc.deallocate(s_alloc, 10);

    MultInh mi;
    mi.print_x();

    Parent p;
    call_mem_fn(p);

    Child c;
    call_mem_fn(c);

    Parent s = c;

    subclass_to_parent(c);

    const std::vector<int> vector = []() {

        std::vector<int> vector;
        for (size_t i = 0; i < 1000; ++i) {
            vector.emplace_back(i*i);
        }

        return vector;
    }();
    
}

