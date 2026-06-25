#include <algorithm>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <print>
#include <random>
#include <stdexcept>
#include <thread>
#include <optional>
#include <future>


void sleep() {
    for (int i = 0; i < 10; ++i) {
        using namespace std::chrono_literals;
        std::cout << i << std::endl;
        std::this_thread::sleep_for(500ms);
    }
}

void init_vec(std::vector<int>& res, size_t size) {
    std::random_device rd{};
    std::mt19937 mt{rd()};
    std::uniform_int_distribution<> dist{1, 10};

    for (size_t i = 0; i < size; ++i) {
        res.emplace_back(dist(mt));
    }

}

void createThread() {
    std::vector<int> vec;
    // std::thread thread{init_vec, std::ref(vec), 10};
    std::thread thread{
        [&vec]() { init_vec(vec, 10); }
    };

    thread.join();

    for (auto i : vec) {
        std::cout << i << " ";
    }
    std::endl(std::cout);
}

class Queue {

    std::vector<int> data;
    std::mutex mutex;
    std::condition_variable cv;
    bool open = true;

    int unsafePoll() {
        int res = data.front();
        data.erase(data.begin());
        return res;
    }

    bool unsafeClosedAndEmpty() {
        return (not open) and data.empty();
    }

public:

    Queue() = default;
    Queue(std::initializer_list<int> data) : data{std::move(data)} {}

    void close() {
        {
            std::lock_guard lock{mutex};
            open = false;
        }
        cv.notify_all();
    }

    void append(int num) {
        {
            std::lock_guard lock{mutex};
            data.emplace_back(num);
        }
        cv.notify_one();
    }

    std::optional<int> tryPoll() {
        std::lock_guard lock{mutex};

        if (data.empty()) {
            return std::nullopt;
        }
        
        return unsafePoll();
    }

    int waitPoll() {
        {
            std::lock_guard lock{mutex};
            if (not data.empty()) {
                return unsafePoll();
            }
        }

        std::unique_lock lock{mutex};
        cv.wait(lock);

        if (unsafeClosedAndEmpty()) {
            throw std::runtime_error{"Queue closed"};
        }

        return unsafePoll();
    }

    std::optional<int> waitPoll(auto duration) {
        {
            std::lock_guard lock{mutex};
            if (not data.empty()) {
                return unsafePoll();
            }
        }

        std::unique_lock lock{mutex};
        auto res = cv.wait_for(lock, duration);

        if (res == std::cv_status::timeout) {
            return std::nullopt;
        }

        if (unsafeClosedAndEmpty()) {
            throw std::nullopt;
        }


        return unsafePoll();
    }
};

void readData(Queue& q) {
    while (true) {
        int num = 0;
        std::cin >> num;
        if (not std::cin) {
            q.close();
            return;
        }
        q.append(num);
    }
}

void processData(Queue& q) {
    while (true) {
        try {
            auto val = q.waitPoll();
            std::cout << val*2 << std::endl;
        } catch (const std::runtime_error&) {
            return;
        }
    }
}

void testQueue() {

    Queue q;

    std::jthread t1 {
        [&q]() { readData(q); }
    };
    std::jthread t2 {
        [&q]() { processData(q); }
    };

}

void computeValue(std::future<size_t>& size,
        std::promise<std::vector<int>>& res) {

    std::vector<int> vec;
    vec.reserve(1'000'000);

    size.wait();
    size_t s = size.get();

    for (size_t i = 0; i < s; ++i) {
        vec.emplace_back(i*i);
    }

    res.set_value(std::move(vec));

    std::println("computeValue finished");
}

void futures() {

    std::promise<size_t> sizep;
    auto sizef = sizep.get_future();
    std::promise<std::vector<int>> vecp;
    auto vecf = vecp.get_future();

    std::jthread thread {
        [&sizef, &vecp]() {
            computeValue(sizef, vecp);
        } 
    };

    size_t size;
    std::cout << "size: " << std::endl;
    std::cin >> size;
    sizep.set_value(size);

    std::cout << "Processing" << std::endl;

    vecf.wait();
    auto res = vecf.get();

    for (size_t i = 0; i < std::min(10ul, res.size()); ++i) {
        std::cout << res[i] << " ";
    }
    std::endl(std::cout);

}

int main() {
    // sleep();
    // createThread();
    // testQueue();
    futures();
}
