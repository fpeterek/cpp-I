#include <format>
#include <print>
#include <type_traits>
#include <variant>
#include <array>


struct S1 {};
struct S2 {};
struct S3 {};
struct S4 {};
struct S5 {};
struct S6 {};

using Var = std::variant<std::monostate, S1, S2, S3, S4, S5, S6>;


struct PrinterStruct {

    void operator()(const std::monostate&) {
        std::println("PrinterStruct: std::monostate");
    }

    void operator()(const S1&) {
        std::println("PrinterStruct: S1");
    }

    void operator()(const S2&) {
        std::println("PrinterStruct: S2");
    }

    void operator()(const S3&) {
        std::println("PrinterStruct: S3");
    }

    void operator()(const S4&) {
        std::println("PrinterStruct: S4");
    }

    void operator()(const S5&) {
        std::println("PrinterStruct: S5");
    }

    void operator()(const S6&) {
        std::println("PrinterStruct: S6");
    }


    void process(const Var& variant) {
        std::visit(*this, variant);
    }

};

class PrinterClass {

    struct Visitor {

        PrinterClass& p;

        void operator()(const std::monostate& v) { p.printMonostate(v); }
        void operator()(const S1& v) { p.printS1(v); }
        void operator()(const S2& v) { p.printS2(v); }
        void operator()(const S3& v) { p.printS3(v); }
        void operator()(const S4& v) { p.printS4(v); }
        void operator()(const S5& v) { p.printS5(v); }
        void operator()(const S6& v) { p.printS6(v); }


        void process(const Var& variant) {
            std::visit(*this, variant);
        }
    };

    void printMonostate(const std::monostate&) {
        std::println("PrinterClass: std::monostate");
    }

    void printS1(const S1&) {
        std::println("PrinterClass: S1");
    }

    void printS2(const S2&) {
        std::println("PrinterClass: S2");
    }

    void printS3(const S3&) {
        std::println("PrinterClass: S3");
    }

    void printS4(const S4&) {
        std::println("PrinterClass: S4");
    }

    void printS5(const S5&) {
        std::println("PrinterClass: S5");
    }

    void printS6(const S6&) {
        std::println("PrinterClass: S6");
    }

public:
    void process(const Var& variant) {
        std::visit(Visitor { *this }, variant);
    }

};

class TemplatePrinterSafe {

    template<typename T>
    constexpr static inline bool unsupported_type = false;

public:

    void operator()(const auto& v) {
        using T = std::decay_t<decltype(v)>;

        if constexpr (std::is_same_v<T, std::monostate>) {
            std::println("TemplatePrinterSafe: std::monostate");
        }
        else if constexpr (std::is_same_v<T, S1>) {
            std::println("TemplatePrinterSafe: S1");
        }
        else if constexpr (std::is_same_v<T, S2>) {
            std::println("TemplatePrinterSafe: S2");
        }
        else if constexpr (std::is_same_v<T, S3>) {
            std::println("TemplatePrinterSafe: S3");
        }
        else if constexpr (std::is_same_v<T, S4>) {
            std::println("TemplatePrinterSafe: S4");
        }
        else if constexpr (std::is_same_v<T, S5>) {
            std::println("TemplatePrinterSafe: S5");
        }
        // When we comment the following branch out, we get an error message
        else if constexpr (std::is_same_v<T, S6>) {
            std::println("TemplatePrinterSafe: S6");
        }
        else {
            static_assert(unsupported_type<T>, "Missing branch for type");
        }
    }

    void process(const Var& variant) {
        std::visit(*this, variant);
    }
};

struct TemplatePrinterUnsafe {

    void operator()(const auto& v) {
        using T = std::decay_t<decltype(v)>;

        if constexpr (std::is_same_v<T, std::monostate>) {
            std::println("TemplatePrinterUnsafe: std::monostate");
        }
        else if constexpr (std::is_same_v<T, S1>) {
            std::println("TemplatePrinterUnsafe: S1");
        }
        else if constexpr (std::is_same_v<T, S2>) {
            std::println("TemplatePrinterUnsafe: S2");
        }
        else if constexpr (std::is_same_v<T, S3>) {
            std::println("TemplatePrinterUnsafe: S3");
        }
        else if constexpr (std::is_same_v<T, S4>) {
            std::println("TemplatePrinterUnsafe: S4");
        }
        else if constexpr (std::is_same_v<T, S5>) {
            std::println("TemplatePrinterUnsafe: S5");
        }
        // No branch for S6 but the code compiles
    }

    void process(const Var& variant) {
        std::visit(*this, variant);
    }
};


int main() {
    std::array variants {
        Var { S3 { } },
        Var { S6 { } },
        Var {  },
        Var { S4 { } },
        Var { S1 { } }
    };

    PrinterStruct ps;

    for (auto& variant : variants) {
        ps.process(variant);
    }

    PrinterClass pc;

    for (auto& variant : variants) {
        pc.process(variant);
    }

    TemplatePrinterSafe tps;

    for (auto& variant : variants) {
        tps.process(variant);
    }

    TemplatePrinterUnsafe tpu;

    for (auto& variant : variants) {
        tpu.process(variant);
    }
}
