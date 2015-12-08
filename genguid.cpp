
#include <chrono>
#include <cstdint>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <thread>

std::string generate_guid(void){
    // Create a seed sequence using the random device and a high-resolution timestamp. The high
    // resolution clock has nanosecond fidelity which should provide at least a few bits of entropy
    // from the least-signficant end.
    std::seed_seq seedSequence{
        (std::uint64_t)std::random_device()(),
        (std::uint64_t)std::chrono::high_resolution_clock::now().time_since_epoch().count()
    };

    // Initialize our random number generator.
    std::uniform_int_distribution<> dist( 0, 15 );
    std::mt19937 randGen( seedSequence );

    // Set up our GUID template and buffer.
    char guid[ 40 ] = { 0 };
    static const std::string guidTemplate   = "xxxxxxxx-xxxx-4xxx-yxxx-xxxxxxxxxxxx";
    static const std::string hexChars       = "0123456789ABCDEF";

    // Iterate over the template, generating our GUID.
    for( unsigned int i = 0; i < guidTemplate.size(); ++i ){
        int randNum     = dist( randGen );
        char character  = 0;

        switch( guidTemplate[ i ] ){
            case 'x'    : { character = hexChars[ randNum ];                    } break;
            case 'y'    : { character = hexChars[ (randNum & 0x03) | 0x08 ];    } break;
            default     : { character = guidTemplate[ i ];                      } break;
        }

        guid[ i ] = character;
    }

    // Return the generated GUID.
    return std::string( guid );
}

int main(void){
    std::stringstream s;
    for (int i = 0; i < 100000; ++i) {
        std::cout << '.' << std::flush;
        s << generate_guid() << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}
