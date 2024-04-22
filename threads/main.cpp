#include <cstdio>
#include <future>
#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <condition_variable>


class Queue {

    std::vector<int> data;
    std::mutex mutex;
    std::condition_variable var;

    int pollUnsafe() {
        auto front = data.front();
        data.erase(data.begin());
        return front;
    }

public:

    Queue() = default;
    Queue(std::initializer_list<int> initVals) : data(std::move(initVals)) { }

    void enqueue(const int val) {
        std::unique_lock lock { mutex };
        data.emplace_back(val);
        var.notify_one();
    }

    int poll() {

        // If queue is non-empty, poll immediately
        {
            std::lock_guard lock { mutex };
            if (data.size()) {
                return pollUnsafe(); 
            }
        }

        // Otherwise wait until a value is inserted
        std::unique_lock lock { mutex };
        var.wait(lock);
        return pollUnsafe();
    }
};

void print(const char* str, std::chrono::milliseconds delay) {
    while (*str) {
        std::cout << *str << std::endl;
        ++str;
        std::this_thread::sleep_for(delay);
    }
}

void runThreads() {
    using namespace std::chrono_literals;

    std::thread thread1 { print, "Hello, World", 500ms };
    std::thread thread2 { print, "Ahoj Svete", 500ms };

    thread1.join();
    thread2.join();
}

void doubleAndAppend(int number, std::string& result, std::mutex& mutex) {
    // Perform an independent but complicated computation without blocking other threads
    auto tmp = std::to_string(number*2);
    
    // Only lock when absolutely necessary, unlock as quick as possible
    std::lock_guard lock { mutex };
    result += tmp;
}

void useMutexToSynchronizeWrites() {

    std::string result;
    std::mutex mutex;

    // We have to use std::ref to create references, otherwise the compiler will try to bind
    // an r-value to a mutable reference
    std::thread thread1 { doubleAndAppend, 1'000'000, std::ref(result), std::ref(mutex) };
    // Or we can just use a lambda
    std::thread thread2 { [&result, &mutex]() { doubleAndAppend(50, result, mutex); } };

    // Don't forget to wait for the threads to finish, do not access the result too quickly
    std::cout << "result: " << result << std::endl;

    thread1.join();
    thread2.join();

    // The order in which the strings are concatenated is seemingly random and differs across
    // multiple runs
    // Just because a thread is started earlier doesn't mean it will also finish earlier,
    // as multiple factors come into play (hardware, OS scheduler, input size)
    std::cout << "result: " << result << std::endl;

}

void queuePrinter(Queue& q) {
    while (true) {
        const auto val = q.poll(); 
        std::cout << (val*2) << std::endl;
        if (not val) {
            return;
        }
    }
}

void queueReader(Queue& q) {
    while (true) {
        int input = 0;
        std::cin >> input;
        q.enqueue(input);

        if (not input) {
            return;
        }
    }
}


void useDifferentThreadsToReadAndProcess() {
    // Fill the queue with a list of initial values
    // The printer will first process all the initial values at once
    // Once it has depleted the entire queue, a call to poll will block and
    // cause the thread to wait for input
    Queue q { 1, 2, 3, 4, 5 };
    
    std::thread reader { [&q]() { queueReader(q); } };
    std::thread printer { [&q]() { queuePrinter(q); } };

    reader.join();
    printer.join();
}

void performComputation(
        std::future<size_t>& input,
        std::promise<std::vector<std::string>>& output) {

    // Perform setup whilst waiting for input
    const auto string = "fuck up some commas";
    std::vector<std::string> res;

    // Get input
    auto size = input.get();

    // Perform computation
    res.reserve(size);

    for (size_t i = 0; i < size; ++i) {
        res.emplace_back(string);
    }

    // Return value using a promise
    output.set_value(std::move(res));
}

void complicatedAlgorithm() {
    if (1+1 != 2) {
        std::puts("Something is very wrong");
    }
}

void usePromisesToPassValues() {

    std::promise<size_t> sizePromise;
    auto sizeFuture = sizePromise.get_future();
    std::promise<std::vector<std::string>> resultPromise;

    // Start the thread earlier so it can perform setup while waiting for input
    std::thread thread { [&sizeFuture, &resultPromise]() {
        performComputation(sizeFuture, resultPromise);
    }};

    // Get input
    size_t input = 0;
    std::cout << "size: ";
    std::cin >> input;

    // Pass the value to worker thread
    sizePromise.set_value(input);

    // Do other things while waiting for input
    std::cout << "Waiting for result" << std::endl;
    complicatedAlgorithm();

    // Get the output of worker thread
    auto res = resultPromise.get_future().get();

    std::cout << "(" << res.size() << "x) " << res.front() << std::endl;

    thread.join();
}

int main() {

    // runThreads();
    // useMutexToSynchronizeWrites();
    // useDifferentThreadsToReadAndProcess();
    usePromisesToPassValues();
}
