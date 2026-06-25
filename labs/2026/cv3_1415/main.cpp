#include <print>
#include <functional>

struct AircraftID {
    uint32_t id{};
};

struct AirlineID {
    uint32_t id{};
};

bool ownsAircraft(AircraftID ac, AirlineID al) {
    return false;
}

void apply(int x, int y, auto fn) {
    fn(x, y);
    fn(y, x);
}

int main() {

    AircraftID al { 5 };
    AirlineID ac { 5 };

    ownsAircraft(al, ac);

    int z = 5;

    auto fn = [&z](int x, int y) -> auto {
        std::println("{} + {} + {} = {}", z, x, y, x+y+z);
    };

    std::function<void(int, int)> function = fn;

    z = 10;

    function(1, 2);

    class Lambda {
        int& z;
    public:

        Lambda(int& z) : z{z} { }
        
        auto operator()(int x, int y) {
            std::println("{} + {} + {} = {}", z, x, y, x+y+z);
        }
    };

    Lambda fn2 = Lambda{z};

    function = fn2;

    z = 13;

    function(1, 2);

    apply(4, 8, fn);
    apply(4, 8, fn2);
    apply(4, 8, [](int x, int y) {
            std::println("{}", x * y);
        });

    auto mutating = [called = 0]() mutable {
        ++called;
        std::println("Lambda was called {} times", called);
    };

    for (int i = 0; i < 10; ++i) {
        mutating();
    }

    z = 0;

    auto f1 = [&z](int x, int y) -> auto {
        std::println("{} + {} + {} = {}", z, x, y, x+y+z);
    };

    std::function<void(int, int)> func = f1;

    z = 10;

    func(6, 3);

    class Lambda1 {
        int& z;
    public:

        Lambda1(int& z) : z{z} { }
        
        auto operator()(int x, int y) const {
            std::println("{} + {} = {}",  x, y, x+y);
        }
    };

    Lambda1 f2 = Lambda1{z};

    func = f2;

    func(3, 6);

    auto f3 = [](int, int) -> void {
        std::println("noop");
    };

    apply(3, 6, f1);
    apply(3, 6, f2);
    apply(3, 6, f3);


    auto mutFn = [called = 0] mutable -> void {
        ++called;
        std::println("Lambda was called {}x times", called);
    };

    for (int i = 0; i < 10; ++i) {
        mutFn();
    }
}

