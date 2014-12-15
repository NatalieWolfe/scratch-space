
#include <condition_variable>
#include <future>
#include <iomanip>
#include <iostream>
#include <mutex>

int main( void ){
    std::mutex mute;
    std::condition_variable cv;

    std::cout << "(int)std::future_status::deferred : " << (int)std::future_status::deferred    << std::endl;
    std::cout << "(int)std::future_status::ready    : " << (int)std::future_status::ready       << std::endl;
    std::cout << "(int)std::future_status::timeout  : " << (int)std::future_status::timeout     << std::endl;
    std::cout << std::endl;

    std::future< bool > future = std::async( std::launch::async, [&](){
        std::cout << "Async: Started." << std::endl;
        std::unique_lock< std::mutex > lock( mute );
        cv.wait( lock );
        std::cout << "Async: Wait completed." << std::endl;
        std::this_thread::sleep_for( std::chrono::seconds( 5 ) );
        std::cout << "Async: Sleep completed." << std::endl;
        return true;
    });
    std::this_thread::sleep_for( std::chrono::milliseconds( 200 ) );

    std::cout << "future.valid()                : " << std::boolalpha << future.valid() << std::endl;
    std::cout << "(int)future.wait_for( 1ns )   : " << (int)future.wait_for( std::chrono::nanoseconds( 1 ) ) << std::endl;
    std::cout << std::endl;

    std::cout << "Signaling async task and waiting..."  << std::endl;
    std::cout << "std::chrono::system_clock::now() : "  << std::chrono::system_clock::now().time_since_epoch().count() << std::endl;
    cv.notify_all();
    auto waitRes = future.wait_for( std::chrono::seconds( 10 ) );
    std::cout << "(int)future.wait_for( 10s )       : " << (int)waitRes << std::endl;
    std::cout << "std::chrono::system_clock::now()  : " << std::chrono::system_clock::now().time_since_epoch().count() << std::endl;
    std::cout << std::endl;

    std::cout << "future.valid()    : " << std::boolalpha << future.valid() << std::endl;
    std::cout << "future.get()      : " << std::boolalpha << future.get()   << std::endl;
}
