
#include <boost/archive/iterators/binary_from_base64.hpp>
#include <boost/archive/iterators/insert_linebreaks.hpp>
#include <boost/archive/iterators/remove_whitespace.hpp>
#include <boost/archive/iterators/transform_width.hpp>
#include <iostream>
#include <openssl/engine.h>
#include <openssl/err.h>
#include <openssl/pem.h>
#include <openssl/rsa.h>
#include <stdexcept>
#include <string>

// ---------------------------------------------------------------------------------------------- //

// static const std::string data =
//     "nKEjRFsc5igF0stdIqVcb7Ep0QoUwUkFfC2uQ01LoqZ5+rFTIZGBb+WP1EWCTyr2R9WNxzKoTAIJ\n"
//     "ajGM4MJ+sVGPyrD9wyjWiSBEMGaP9+YgKC/++TlZm1AmwL7t3XoDvXfhtrXFJBC0O8zuh7O3LZmp\n"
//     "SZ6LQR806xuf9qUj1U4HzfT//tNUPc2ay0fJ+l+YaVUVQ5IcMxO8Oh4f13peVuXeUJRb2XDonuCe\n"
//     "Ms48moSejCtFpJCWmBoMWk5GkWzGHqFXmS9OYssyVCi6SfkI6lywhwbP1g+OTJy0LI6dvsq+ryJ5\n"
//     "wcF0xYiNXAdP03OIT2B2DMO2d1BxKBjjSFURiQ==\n"
// ;

static const std::string data =
    "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\n"
    "3eTvNtFVFzdArkUUdY1e/2EjQr7dHtc=\n"
;

// ---------------------------------------------------------------------------------------------- //

// static const char* key =
//     "-----BEGIN RSA PRIVATE KEY-----\n"
//     "MIIEogIBAAKCAQEA3A4+JvZx3JaG2iL/Bo5H3aaBW5AtzLTst2JBb2bpJoQmtpON\n"
//     "DpfDvAOY5TqnPIUtwohVtHzoHgXTDBxkKEtWhfXZ7xuz17jnT/iyLp88iuC1sXZn\n"
//     "+9tAXbuw/EvZNxV2Z2l2lH6WrgkpT7Y/dhWp0uQ4ygZT/4lg0mPa30WlcEDPfUrO\n"
//     "FBXfBQHLKAlx+V4zIQd8kznGEOJfpY3PDuBn4fx52qLwDXw/VN3ptcEbpMz/o/FW\n"
//     "6wwZHdw3qorJHScSq++wP+8rq73LlOPYAEHOtNUARhmAlys+WvEd8e7iUJgeeIM2\n"
//     "Fb2rFRBSln9+iyYNtvSTZNJFQuXdFsEod7osQQIDAQABAoIBACsuMEv8+ALNFtEf\n"
//     "JOPob3my/pc1B4evyPODOEvf6chBDketVHSUmzf9jLcOd1qM15ngntC34eRiSDOM\n"
//     "LqspfWT0BTvCN9wQ0HME3hH/9SVbSsvz7BujvlGmTuhzCcChWYaZT0d8P50O9yhJ\n"
//     "HtLRFW3okqwx6bGvB2zBY2WEiR4pfFBxaX7FcEP5zf++dbbVvWfadyEyMvpw/mMO\n"
//     "SasP7sIWa5C/zAzkJ8z+hhdZQHqnKsGjxEGmmOsReZOlbGDQGgfhH4xiJkcwH/9o\n"
//     "T9eCgVeUNo+Kmt+hvyD9JjqwjNmh3C7cUZU0afGFEjU8vhNxlnnim1Cr6IXkPH0F\n"
//     "k9LyVAECgYEA4yIFcyhA6VF6G12yzlHXhROb4BrZ+mrzacuIkAFCPmt7PfrjNqKY\n"
//     "gJDl62Mx+bgD6tOBAnFs8vLkCgOTB4zUDDwJR38utNQdBECiOPemwTTCfPo/lCLX\n"
//     "AJbXPtR4durr6zazdm0P+gBAKzG1B3PutKkV+Qjd7muvLlf/zXNDKHECgYEA+AX0\n"
//     "0F31TrIimdy5G8kPUIn1i4Q2TnZRNAoFoUklkiHCLSjb5W/7TQkeO4SoTEnL63jM\n"
//     "lGMcMQsmln+SjM5scvf/vexu35q+0Gp3Tfolgba+s/P/7+nYhqZQq5mLnAFywNGv\n"
//     "456vdeFQcCBpkreruVVaUayhvhSrm17sVk8cqNECgYAT/YKNSBWuc0VT80MxdXKK\n"
//     "e9dUOgjFSXRy8z7prbKu27bVFae5v0W8ywPxW6GiYNe3azYHiQQVvdrkJrxiBbsD\n"
//     "+KS4bjEmKSUEosukR+edSY90gHHoD8jsfFolZX3UuYhaA054NdEaJ7t+EbnlFgcE\n"
//     "T31nlagyu7eHVKdm5aYbgQKBgEPubbU8ZtndFHkL6W9gzF8EykoiNjWbVnZoYE5i\n"
//     "mHXD5Uk1LbVTzIK3Ap7OoMH0XHPsLCxgUPJl2XXTDzKdvzpmDpfYUC3SVn0DDBZB\n"
//     "c1Bp2yWyqODvLa0DlIjLj191Thbd180CQ5CH2946InBa+h/S7t/75nkHdFEj2Jtz\n"
//     "I6hhAoGAG7jwHhAQylXq48RJfNoQXOjyFvyZCBOwZgNBoSsRpM7lpyaHjVkDwgFe\n"
//     "dRiY5bUXxGZVAi1+w66xjg5Vm+jMhZt6RDYALpq22iI6LAdsxw1h6N6jCrIJm/6n\n"
//     "NrHO5KcXgyzuei+NRI63Vbdi2vgJ9Qu5RdVYsMBcdNIlRWOYc+A=\n"
//     "-----END RSA PRIVATE KEY-----"
// ;

static const char* key =
    "-----BEGIN RSA PRIVATE KEY-----\n"
    "MIIEowIBAAKCAQEAyRAyIEc3BwEmhIqa8es+5Vu+0OVdgt/MJl1hUYhqh1OwaFsP\n"
    "p7PDguZG0u9b/fnAZfI2IXecb72KZnWyZ7f0lPmJefPmes2+0G0ACKt03/ijBAgi\n"
    "7sxsADbS9zKjOhkYAJZMM7BAYxSr0msmx/AFNXLrVFVY5NTunIuss3lafe7GSmzO\n"
    "/40pKXMd2DEgRw8V3Jnv52COWrZdBH7kf7RegZc4dG18DY4K7FsN6bFTIv6ekpkZ\n"
    "7PDqApCJvCYrZO48wSzrfNF7cjQXv5J24rOSvYxHzeTzHcNiJ3mMTC/N4Jp6J0yU\n"
    "Ws8lPYunOpRHP0UySUqN059nZsGreYIfxxCHlwIDAQABAoIBAC24sB1k9F90ZITp\n"
    "W/i9TQ0tAsRQFCapq6vhujqnMcpaPphMdM/m8gO3R825voqPm6/TF3oe0AUc7fcE\n"
    "trAdRdIGuLtxuiyc0tJkpwhRrP8tdCx7iS4ScFDQJsMt0a5pfGrNwsBSSSH+1l/A\n"
    "HA6FRznzSw5fZhqLTUNuA7TAWNKf7UafRz05GZwwBtFAr5GHM6FKanWVkXOLTqIj\n"
    "AAFB7PWXQ1ZEvdwjzz6qX6Id3eKd47Qz962FwfFlNZ1ZyprnRkIjTpdQc+9FxdzO\n"
    "kzQbUGgBDpVg4E2rk+TYDMN8cISWaN4zn9lLeDuhpc+297xHizHhybauS9v9EVq1\n"
    "FBItd1ECgYEA11fvYAd0gY2Px9AikamfVVPBQ9njkpKYvVYkVIeMjGPEJF3NjQhy\n"
    "AaM3x3PNcwqqZUtVVmHd0TnkYwwivDQCMZAIxxKpDDCUkEeRvcv3HcpBAhBRXdBZ\n"
    "agJFb2uBqfmTDb9kaujoSUDpS1jJhPkqag6ka6G/le5rDS42Vt6A+mkCgYEA7wYQ\n"
    "JqjMSQpdR+Ou1BzL20Eye01fLfS7ANSvAXm6f3LDwdUJ6w3OoHEVJmX9/Ernd0Vz\n"
    "EssAsQKt2f8XwaHijw73BV0i7vXf0rtQndVMFp8zdDdOq+qGD/sVrjoARFnLdx1C\n"
    "16VX+6BkScDU653PNYYYC9JDRKRZV/1NNuBBMf8CgYB3irQyrdDKPmiLtooKLIxr\n"
    "1LPb07AWZNTvfktqtVibcqKmwX/jA8YuDIphaXKqo0gWNSypaGi3xhh3t3vs2D5D\n"
    "fz6wLDa1sp42PdS09O44lldiKubQWmHYjGw5Pwfa8NXmctHu215fk+4uSpqL+7nE\n"
    "6F3z9fWh6Jl61qWJsbkNyQKBgBHIlbMKpAhbKQ2MNxm4+sHZW/QokYhNyQwKxuqJ\n"
    "MAMDeuUVAKPoZxMw1q4AsKDwwb/x/tnLGMYSuPSbMh1J+h1Cs8ZGY677xCddoGXd\n"
    "1MdqGN3wr33C+S5/M+G+lv5+39CA8NQyjRU/nbaPDkhpeV02GNvoTPDyeWM+iVwn\n"
    "2zHpAoGBALn4MeHcLz5mQqeZeqqxIOWAPnSBRVEYZ7qf8FURVgFYUWkx+hEQxs9W\n"
    "YBFU8p8H+DxqnLV+ZodVKBuj5ka7JQXllUV7XJ8MrjB3h0GH4qrp4EXiqnTcr9nI\n"
    "EaoGKJM29g222yGTdWg9mtwX7NmSwPNEkHJq5tmSrYWa51wMLI/+\n"
    "-----END RSA PRIVATE KEY-----"
;

// ---------------------------------------------------------------------------------------------- //

RSA* loadKey( const std::string& clientKey ){
    RSA* privateKey = nullptr;

    // Create another buffer for the client key.
    BIO* keyBuffer = BIO_new_mem_buf( (void*)clientKey.data(), clientKey.size() );
    if( !keyBuffer ){
        throw std::runtime_error( "Failed to create private key memory buffer." );
    }

    // Convert this memory buffer into a private key.
    PEM_read_bio_RSAPrivateKey( keyBuffer, &privateKey, nullptr, nullptr );
    if( !privateKey ){
        throw std::runtime_error( "Failed to read private key." );
    }

    // Free up the buffer.
    BIO_free( keyBuffer );
    return privateKey;
}

// ---------------------------------------------------------------------------------------------- //

std::streamsize read( RSA* key, std::string source, char* out ){
    // Attempt to decrypt the data.
    std::streamsize bytes = 0;
    char buffer[ 10240 ] = { 0 };
    std::size_t keySize = RSA_size( key );
    std::cout << "Decrypting " << source.size() << " bytes in " << keySize << " byte chunks." << std::endl;
    for( std::size_t i = 0; i < source.size() - 1; i += keySize ){
        std::cout << "Decrypting from offset " << i << std::endl;

        std::size_t decryptedSize = RSA_private_decrypt(
            keySize,
            (unsigned char*)source.c_str() + i,
            (unsigned char*)buffer,
            key,
            RSA_PKCS1_OAEP_PADDING
        );
        bytes += decryptedSize;
        // std::cout << "Got " << std::string( buffer, decryptedSize ) << std::endl;
        std::memcpy( out + i, buffer, decryptedSize );
    }
    return bytes;
}

// ---------------------------------------------------------------------------------------------- //

std::string fromBase64( const std::string& b64 ){
    using namespace boost::archive::iterators;
    typedef transform_width<
        binary_from_base64<
            remove_whitespace< const char* >
        >,
        8, 6
    > Base64Iterator;
    return std::string(
        Base64Iterator( &b64.at( 0 ) ),
        Base64Iterator( &b64.at( b64.size() - 2 ) )
    );
}

// ---------------------------------------------------------------------------------------------- //

int main( void ){
    ERR_load_crypto_strings();
    std::string decodedData = fromBase64( data );

    std::cout << "Decoded " << data.size() << " bytes into " << decodedData.size() << " bytes." << std::endl;

    RSA* privateKey = loadKey( key );
    char decrypted[ 1024 ] = { 0 };
    int bytes = read( privateKey, decodedData, decrypted );
    RSA_free( privateKey );

    std::size_t err;
    while( (err = ERR_get_error()) != 0 ){
        std::cerr << "Error " << err << ": " << ERR_error_string( err, nullptr ) << std::endl;
    }

    std::cout << "Decrypted " << bytes << " bytes: '" << (const char*)decrypted << "'." << std::endl;

    return 0;
}
