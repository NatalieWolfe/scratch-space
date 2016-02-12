
#include <atomic>
#include <chrono>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <sstream>

using namespace std;
using namespace std::chrono;
typedef high_resolution_clock Clock;

volatile uint64_t final;

template <typename I>
milliseconds test(const int iterations) {
    I i(0);
    uint64_t t = 1;
    auto start = Clock::now();
    while (i < iterations) {
        t *= ++i;
    }
    auto end = Clock::now();
    final += t;
    return duration_cast<milliseconds>(end - start);
}

int main() {
    const int max_iterations = 1000000000;
    cout << "iterations | plain | atomic | volatile" << endl;
    for (int iterations = 1000; iterations <= max_iterations; iterations *= 10) {
        cout
            << setw(10) << iterations << " | " << flush
            << setw(5) << test<int>(iterations).count() << " | " << flush
            << setw(6) << test<atomic_int>(iterations).count() << " | " << flush
            << setw(8) << test<volatile int>(iterations).count() << flush
            << endl;
    }
    cout
        << "All times in milliseconds." << endl
        << "std::atomic is lock free: " << boolalpha << atomic_int().is_lock_free() << endl;
}
