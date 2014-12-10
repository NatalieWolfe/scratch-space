
#include <chrono>
#include <iostream>
#include <ratio>
#include <string>

template< std::intmax_t Num, std::intmax_t Denom >
std::string ratioStr( const std::ratio< Num, Denom >& ){
    return std::to_string( Num ) + ":" + std::to_string( Denom );
}

int main( void ){
    using namespace std::chrono;

    std::cout << "std::chrono::high_resolution_clock::period:   " << ratioStr( high_resolution_clock::period() ) << std::endl;
    std::cout << "std::chrono::steady_clock::period:            " << ratioStr( steady_clock::period() ) << std::endl;
    std::cout << "std::chrono::system_clock::period:            " << ratioStr( system_clock::period() ) << std::endl;

    return 0;
}
