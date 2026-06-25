#include <algorithm>
#include <array>
#include <functional>
#include <iterator>
#include <print>
#include <unordered_set>
#include <vector>


int sum(int low, int high) {
    
    int res = 0;

    for (int i = low; i < high; ++i) {
        res += i;
    }

    return res;
}

struct Value {
    std::string id;
    int x;
    int y;
    int z;

    bool operator==(const Value& other) const {
        return id == other.id and
            x == other.x and
            y == other.y and
            z == other.z;
    }
};

namespace std {
    template<>
    struct hash<Value> {
        size_t operator()(const Value& val) const {
            return std::hash<std::string>{}
                .operator()(val.id);
        }
    };
}

int main() {
    std::array<int, 5> arr;
    std::less<int> l{};

    std::unordered_set<Value> values;

    values.emplace(Value {"id1", 1, 2, 3});
    values.emplace(Value {"id1", 1, 2, 3});
    values.emplace(Value {"id3", 1, 2, 3});
    values.emplace(Value {"id4", 1, 2, 3});

    std::println("{}", values.size());

    std::hash<int> h{};

    std::vector<int> nums { 1, 2, 3 };

    std::vector<int> results;

    std::transform(
            nums.begin(), nums.end(),
            std::back_inserter(results),
            [](int x) { return x*x; }
            );
}

