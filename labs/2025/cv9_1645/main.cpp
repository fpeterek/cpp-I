#include <algorithm>
#include <iostream>
#include <memory>
#include <print>
#include <string>
#include <cmath>
#include <type_traits>

#include <random>
#include <chrono>
#include <condition_variable>
#include <mutex>
#include <thread>
#include <optional>
#include <atomic>

void sleep() {
    for (int i = 0; i < 10; ++i) {
        using namespace std::chrono_literals;
        std::cout << i << std::endl;
        std::this_thread::sleep_for(500ms);
    }
}

void initVector(std::vector<int>& vec, size_t size) {
    std::random_device rd{};
    std::mt19937 mt{rd()};
    std::uniform_int_distribution<> dist{1, 10};

    for (size_t i = 0; i < size; ++i) {
        vec.emplace_back(dist(mt));
    }
}

void createThread() {
    std::vector<int> vec;

    // std::thread thread(initVector, std::ref(vec), 10);
    std::thread thread{
        [&vec]() { initVector(vec, 10); }
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

    int unsafePoll() {
        int res = data.front();
        data.erase(data.begin());
        return res;
    }

public:

    Queue() = default;
    Queue(std::initializer_list<int> data) : data{std::move(data)} {}

    void enqueue(int num) {
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

        return unsafePoll();
    }

};

void readData(Queue& q) {
    while (true) {
        int num = 0;
        std::cin >> num;
        if (not std::cin) {
            return;
        }
        q.enqueue(num);
    }
}

void processData(Queue& q) {
    while (true) {
        int data = q.waitPoll();
        std::cout << data*2 << std::endl;
        if (data == -1) {
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

int main() {
    // sleep();
    // createThread();
    testQueue();
}
