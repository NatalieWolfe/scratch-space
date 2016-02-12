
#include <chrono>
#include <iostream>
#include <list>
#include <thread>

void makeThreads(const int count){
    using namespace std::chrono;
    std::list<std::thread> threads;

    auto start = high_resolution_clock::now();
    for (int i = 0; i < count; ++i) {
        threads.emplace_back([](){ std::this_thread::sleep_for(milliseconds( 1 )); });
    }
    for (auto& thread : threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);

    std::cout << count << " threads in " << duration.count() << "ms" << std::endl;
}

int main(){
    makeThreads(10);
    makeThreads(100);
    makeThreads(1000);
    makeThreads(10000);
    makeThreads(100000);

    return 0;
}
