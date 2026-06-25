#include <print>
#include <functional>
#include <vector>


class Parent {

public:
    virtual void mem_fn() {
        std::println("Parent member fn");
    }
};

class Child : public Parent {

public:
    void mem_fn() override {
        std::println("Child member fn");
    }
};


void call_mem_fn(Parent& p) {
    p.mem_fn();
}


namespace {
    class C {
    public:
        virtual void fn() {

        }
    };
}

void call_fn(C& c) {
    c.fn();
}

void cast_to_parent(Parent& p) {

}

void cast_to_child(Parent& p) {
    Child& c_ref = dynamic_cast<Child&>(p);

    Child* c_ptr = dynamic_cast<Child*>(&p);
}

struct C1 { int a = 5; };
struct C2 { int a = 10; };
struct C3 { };

struct Inheritance : public C1, public C2, public C3 {
    void print_a() {
        std::println("a={}", C1::a);
        std::println("a={}", C2::a);
    }
};

class ConsParent {
    public:
        ConsParent(int x, int y) {}
};

class Constructors : public ConsParent {

    std::vector<int> vec;
    std::string str;
    int a;

public:
    Constructors() : ConsParent(1, 2), vec{}, str{"abcd"}, a{} {
    }

    Constructors(int x, int y) : Constructors() {
        a = x+y;
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

    {
        S s;
    }

    S* s1 = new S;

    delete s1;

    S* s_arr = new S[10];

    delete[] s_arr;

    std::vector<S> s_vec;
    s_vec.reserve(10);

    S* s_uninit = static_cast<S*>(operator new[](sizeof(S) * 5));

    new(s_uninit+0) S{};
    new(s_uninit+1) S{};
    new(s_uninit+2) S{};

    s_uninit[0].~S();
    s_uninit[1].~S();
    s_uninit[2].~S();

    operator delete[](s_uninit);

    std::allocator<S> alloc;


    S* s_alloc = alloc.allocate(10);

    alloc.deallocate(s_alloc, 10);


    Inheritance inh;
    inh.print_a();

    Parent p;
    call_mem_fn(p);


    Child c;
    call_mem_fn(c);

    cast_to_parent(c);

    Parent& c_ref = c;
    cast_to_child(c_ref);

    Parent c_copy = c;

    const auto vector = []() -> auto {
        std::vector<int> vector;

        for (size_t i = 0; i < 1000; ++i) {
            vector.emplace_back(i*i);
        }

        return vector;
    }();

}

