
#include <iostream>
#include <openssl/aes.h>

int main( void ){
    std::cout << "AES_BLOCK_SIZE            " << AES_BLOCK_SIZE         << std::endl;
    std::cout << "sizeof( std::int32_t )    " << sizeof( std::int32_t ) << std::endl;

    return 0;
}
