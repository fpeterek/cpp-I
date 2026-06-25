#include <fstream>
#include <iostream>
#include <print>
#include <random>
#include <variant>


enum struct Enum {
    ABCD = 1,
    EFGH = 2
};


enum class Enum2 {
    ABCD = 5
};

struct Bitfields {
    int a : 4;
    int b : 4;
};

struct S {
    int a;
    int b;
    int c;
};

class C {
    int a;
    int b;
    int c;
};

struct HttpRequest { };

struct HttpHandler {
    virtual void handle(HttpRequest& req) = 0;
};

struct HttpServer {

    HttpHandler& handler;
    std::vector<int> vec;

    HttpServer(HttpHandler& handler) : handler{handler} {

    }

    void listen();
};



struct Handler : HttpHandler {
    void handle(HttpRequest& req) override;
};


class IntWrapper {
    int number;

public:
    explicit IntWrapper(int a) : number{a} { }

    IntWrapper(const IntWrapper& other)
        : number{other.number} { }

    IntWrapper& operator=(const IntWrapper& other);
};

IntWrapper& IntWrapper::operator=(const IntWrapper& other) {
    number = other.number;
    return *this;
}


class A {
    static int a;
};

int A::a = 5;

class B : public A {

    friend class Class;
    
    friend int fn(B& b);

    mutable int a{};

    void fn() const {
        a = 25;
    }

};

void fn(IntWrapper w) {

}

int main() {

    fn(IntWrapper { 25 });

    Handler handler;
    HttpServer server { handler };
    server.listen();

    std::vector<int> vec { 1, 2, 3, 4, 5, 6 };

    std::println("Hello, World!");
    
    Enum e = Enum::ABCD;

    std::variant<std::string, int, float> var { "abcd" };

    var = 25;

    std::get<int>(var);



    std::vector<int> v1 { 1, 2, 3, 4, 5 };

    {
        std::ifstream is{"abcd.txt"};
    }

    if (1 == 2) {
        std::vector<int> v2 { 1, 2, 3, 4, 5 };

        std::println("neco se deje");
        fn(IntWrapper{10});
        std::println("neco se stalo");
    }
}
