
#include <exception>
#include <iostream>
#include <string>
#include <openssl/aes.h>
#include <openssl/bio.h>
#include <openssl/engine.h>
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/rsa.h>

static const char* privateKey =
    "-----BEGIN RSA PRIVATE KEY-----\n"
    "MIIEowIBAAKCAQEAws5ZlcsFQv8oh+f5YDE/Dqro+tyQxcRpw8Ykjo/Vxq/x7rFg\n"
    "CZch7IUWfImTXEiYbePVApgcCFS/yMBJpaG9mWYbYDmpQEMrYEAdo7dB0A6NS/DF\n"
    "vdlTmhUxe2YBqeP7U+s5pZ1nekhVD1vCkJroP8Z8pwOZ4kDo1pWDcguL8j0c0a5J\n"
    "eO24sBtBxak3lDOlTdIrc6ulJ/cNrhzIhbmuQUTwImsmOH/SYHHKhMctPAU26CRa\n"
    "i8NmhIucNx+0LYhikaJXgfdyHD/a7RdSqMHyQWqRjvEyk7DJOEojSEF8OlES24qo\n"
    "yMTNRUIndrQc2u96oQToQh9sjg6S0g8TlWc0BwIDAQABAoIBAAYE7D21pTb5vV3d\n"
    "rBXtz537Z/GAaTTDKeVztON2zEs0WoxejLIfKlwtjJFSiuwaDiOvG4DWBF+5hqeE\n"
    "UYI9qicYQZZKkdE7ghaaRJAvdGgiWvlSujlwgqXLK9k9QKXoNnbUWNamM3FS1NYB\n"
    "ptRjBPQbhPSAJvwXt1oSCpq2gp98eBYOIFSXu2CAU9RzMcmf6fnC1ddqR+ZIr7Hy\n"
    "J8ud/VByVyW4qth+sUAKNMQaoDGA2laP7LyzHOhGL9B7j/+hhrdkf25onrEdBMf6\n"
    "4B1wRZ9Ljfa0UZek874XcaQgX3dBBSjujeECU99iVXKMRQXbP/W2wgrEL+Rt/Lq9\n"
    "DzzXVNkCgYEAz4e36LGCKIbGCsc4egCFf7kFcS4UM4Yi5EVDXjUKpJC5JlNMkP5x\n"
    "YCmGRrZWrm1texpAj/xjRDkLxusOQrxA/TpEcC3VQMv4iYip6RR1EMKKlYtK71o0\n"
    "VQdKFu4Zpe6bvULwmKzGEnzjSKABUbX203ORz06qAbsVyJvY2k+3xiMCgYEA8E3a\n"
    "xRC8WWRog+Kf/dIrWlbgwI+oeIOKVdRTMLJOlpevXRSMHf4QwQIE0seQMGkSYXzy\n"
    "q8LY6h2Y2mVN7/bWleWP3JhEcI6j3tstz6/Pl1eJOk50Sg5O5NNtDk2DCYr7sqWo\n"
    "wGTJZTujqgKaEcwQURcb5hBhO7fOi6stQDzj7s0CgYAJxA03egLx/UWchDt4IkYl\n"
    "+IL1DIIO2qURsOC2sjBKBjVwREM6H38eU1XqRfJWSvf37whSvkG9mCGhvrXxFgLI\n"
    "59EIpl9+qRqM/k1gloHbxAvZdbCVCt2jkrA92/6A/HV/toKz7I11mULoy/7D5Zgz\n"
    "4yBdbQo7Ap7Hze2qeE3hmQKBgQCpAVh3eIm/JQsMlnqHfSTYaHmxdFe8SmckbiwD\n"
    "96Ol7hB7mlPhVCpcVBsoUwppwbwXrFTP0D3WifJSxyTFPAk3azh+oOihcPB3cBcM\n"
    "Fk4d1obA5ySKYP3DmZ79jC44q/LRdgj1Ju0tsYAttZ6+HZLsNSB98c8wvNkbCczN\n"
    "fQWhkQKBgFzyQSZzKHKlu3Zfeyg+doPXlXlQVpRHqYUoOkzY83DE5ZujXE5H081P\n"
    "bui0fXQMsIFBt+/ItCwZsp9rBXPW0uWtA0jDIEkk5Ro4FuRyx/59/Zllxo3vpZyA\n"
    "v7lalsxJIywVo2Ni393HP/63bDKjb99mVzeEfpy7Q89Qh+upP8c6\n"
    "-----END RSA PRIVATE KEY-----"
;

static const std::string fullMessage =
    "FHgzBLg5RC1cG2A3USTrXt16wbkP_QfxCdMaxd1OT8ErfU2UxFYTUO6cfXh5vUSeuXS50WOwiJmr8WczDoi01HROg4nqVn"
    "YVqoHKbk6Ju4UQ_3RJqGbMjSbTRpeOEqlYjaotUpIv55ogu8kzBKJ6ky3abV2iILdAMa-c8yjh6HppGBJnJuTZSmGJIrwm"
    "r14kVOXosD2N5XCA2jcmkGYRf-6FakUjLz9CQwOoehU8I4OC1cvgbVLcE7GZEEMTcwgfaO_jUsq1xz3H8r3NS0tyLqsRWa"
    "0dGI7RzjSf4meHm5iIwScnlw96iQf8q0xWXSoATjT4gynS5fy7xqbMLg_BRP0e0WeGQ9AW97Ng0BY8bo35iKqs9DspOOXd"
    "yXuADY9QD8zlHbEYzhnz_ENqDUHbALP5b5GLXEubmZDXDyPKqlTu4BCa69gilvZCA9L6fKYBtdyMh1q2CmISvqOlP6Mn5m"
    "5y1TEIltrE0ySbtLggXkw="
;

// ---------------------------------------------------------------------------------------------- //

std::string base64Decode( const std::string& encoded ){
    // Prepare our string, reserving an estimate of how large the result will be.
    std::string decoded;
    decoded.reserve( encoded.size() * 3 / 4 );

    // Loop through the string, decoding the characters in batches of 4.
    char chars[ 4 ] = { 0 };
    int padding = 0;
    for( std::size_t i = 0; i < encoded.size(); ++i ){
        // Don't bother decoding any padding.
        const char c = encoded[ i ];

        // Decode this character.
        chars[ i % 4 ] =
            std::isupper( c ) ? c - 'A' +  0 :
            std::islower( c ) ? c - 'a' + 26 :
            std::isdigit( c ) ? c - '0' + 52 :
            c == '-' ? 62 :
            c == '_' ? 63 : (++padding, 0)
        ;

        // Every 4 characters we decode 3 characters and append them to the output.
        if( i % 4 == 3 ){
            decoded.append( 1, (chars[ 0 ] << 2) + ((chars[ 1 ] & 0x30) >> 4) );
            if( padding <= 1 ){
                decoded.append( 1, ((chars[ 1 ] & 0x0f) << 4) + ((chars[ 2 ] & 0x3c) >> 2) );
            }
            if( padding == 0 ){
                decoded.append( 1, ((chars[ 2 ] & 0x03) << 6) + chars[ 3 ] );
            }
        }
    }

    return std::move( decoded );
}

// ---------------------------------------------------------------------------------------------- //

EVP_PKEY* loadKey( const std::string& priKeyStr ){
    // Create a buffer.
    EVP_PKEY* evpKey = nullptr;
    BIO* buff = BIO_new_mem_buf( (void*)priKeyStr.c_str(), priKeyStr.length() );
    if( !buff ){
        throw std::runtime_error( "Failed to load key buffer" );
    }

    // Read the data from the buffer.
    EVP_PKEY* readKey = PEM_read_bio_PrivateKey( buff, &evpKey, nullptr, nullptr );
    BIO_free( buff );
    if( readKey == nullptr ){
        throw std::runtime_error( "Failed to read key from buffer" );
    }

    return evpKey;
}

// ---------------------------------------------------------------------------------------------- //

std::string decryptKey( EVP_PKEY* key, const std::string& encrypted ){
    RSA* rsaKey = EVP_PKEY_get1_RSA( key );
    unsigned char buffer[ 1024 ] = { 0 };
    int size = RSA_private_decrypt(
        encrypted.size(),
        (unsigned char*)encrypted.c_str(),
        buffer,
        rsaKey,
        RSA_PKCS1_OAEP_PADDING
    );
    if( size <= 0 ){
        throw std::runtime_error( "Failed to decrypt data." );
    }

    return std::string( (const char*)buffer, size );
}

// ---------------------------------------------------------------------------------------------- //

std::string decryptLicense( const std::string& key, const std::string& message ){
    unsigned char buffer[ 10240 ] = { 0 };
    unsigned char iv[ 16 ] = { 0 };
    std::memcpy( iv, key.c_str() + 32, 16 );

    AES_KEY aesKey = { 0 };
    AES_set_decrypt_key( (unsigned char*)key.c_str(), 32 * 8, &aesKey );
    AES_cbc_encrypt((unsigned char*)message.c_str(), buffer, message.size(), &aesKey, iv, AES_DECRYPT );

    return std::string( (const char*)buffer, message.size() );
}

// ---------------------------------------------------------------------------------------------- //

int main( void ){
    EVP_PKEY* key = loadKey( privateKey );
    std::string decryptedKey = decryptKey( key, base64Decode( fullMessage ).substr( 0, 256 ) );
    // std::cout << "Decrypted key: '" << decryptedKey << "'" << std::endl;

    std::string decryptedLicense = decryptLicense( decryptedKey, base64Decode( fullMessage ).substr( 256 ) );
    std::cout << "Decrypted license: " << decryptedLicense << std::endl;

    return 0;
}
