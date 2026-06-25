#include <print>


class string {

    char* str = nullptr;
    size_t size = 0;

    union {
        size_t reserved;
        char buffer[16];
    } data;

    string() {
        str = data.buffer;
    }

    char* get() {
        return str;
    }
};


int main() {
    std::println("{}", sizeof(std::string));
}
