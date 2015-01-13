
#include <chrono>
#include <cstring>
#include <iostream>
#include <openssl/pem.h>
#include <openssl/rsa.h>

// More here: http://stackoverflow.com/a/12661380/321937
//
// Also, WARNING: this program leaks memory like a sieve made of tissue paper. This warning is just
// to prove that I'm lazy, not incompetent. :)

RSA* generate( const int bits ){
    using namespace std::chrono;

    auto start = high_resolution_clock::now();
    RSA* key = RSA_generate_key( bits, 104651, nullptr, nullptr );
    auto end = high_resolution_clock::now();
    std::cout
        << "Generating " << bits << " bit key took "
        << duration_cast< milliseconds >( end - start ).count() << " milliseconds."
        << std::endl
    ;

    return key;
}

void writePrivateKey( EVP_PKEY* evpKey, char* rawBuffer, const std::size_t buffSize ){
    std::memset( rawBuffer, 0, buffSize );
    BIO* buff = BIO_new( BIO_s_mem() );
    PEM_write_bio_PrivateKey( buff, evpKey, nullptr, nullptr, 0, nullptr, nullptr );
    BIO_read( buff, rawBuffer, buffSize );
    BIO_free( buff );
}

void writePublicKey( EVP_PKEY* evpKey, char* rawBuffer, const std::size_t buffSize ){
    std::memset( rawBuffer, 0, buffSize );
    BIO* buff = BIO_new( BIO_s_mem() );
    PEM_write_bio_PUBKEY( buff, evpKey );
    BIO_read( buff, rawBuffer, buffSize );
    BIO_free( buff );
}

EVP_PKEY* readPublicKey( const char* rawBuffer ){
    EVP_PKEY* evpKey = nullptr;
    BIO* buff = BIO_new_mem_buf( (void*)rawBuffer, std::strlen( rawBuffer ) );
    PEM_read_bio_PUBKEY( buff, &evpKey, nullptr, nullptr );
    BIO_free( buff );
    return evpKey;
}

int main( void ){
    // Test various key length generation times.
    generate( 256 );
    generate( 512 );
    generate( 1024 );
    generate( 2048 );
    generate( 4096 );
    generate( 8192 );

    // Generate a new key for some testing.
    RSA* rsaKey = generate( 512 );
    EVP_PKEY* evpKey = EVP_PKEY_new();
    EVP_PKEY_assign_RSA( evpKey, rsaKey );

    // Allocate a buffer.
    constexpr std::size_t bufferSize = 102400;
    char rawBuffer[ bufferSize ] = { 0 };

    // Try writing the private key half.
    writePrivateKey( evpKey, rawBuffer, bufferSize );
    std::cout << rawBuffer << std::endl;

    // Try writing the public key half.
    writePublicKey( evpKey, rawBuffer, bufferSize );
    std::cout << rawBuffer << std::endl;

    // Now lets try reading in a public key half from memory.
    char rawBuff2[ bufferSize ] = { 0 };
    EVP_PKEY* evpKey2 = readPublicKey( rawBuffer );
    writePublicKey( evpKey2, rawBuff2, bufferSize );
    std::cout << "strcmp: " << std::strcmp( rawBuffer, rawBuff2 ) << std::endl;
    std::cout << rawBuff2 << std::endl;
}
