
#include <cctype>
#include <iostream>
#include <string>

std::string base64Encode( const std::string& str ) {
    // The base64 character set. We use `-` and `_` instead of `+` and `/`.
    const char b64Table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";

    // Base64 turns 3 8-bit chars into 4 6-bit characters, thus requiring 4*n/3 (or about 1.34x)
    // Also, this automatically adds padding symbols at the end of the string.
    const size_t length = str.size();
    std::string retval( (((length + 2) / 3) * 4), '=' );

    std::size_t outpos          = 0;
    int bitsCollected           = 0;
    unsigned int accumulator    = 0;

    // Work on every character of the source string...
    for( int i = 0; i < length; i++ ){
        accumulator = (accumulator << 8) | (str[i] & 0xffu);
        bitsCollected += 8;

        // ...and add groups of 6 bits to the resulting string.
        // Remember that 3 real bytes = 4 base64 bytes
        while( bitsCollected >= 6 ){
            bitsCollected -= 6;
            retval[ outpos++ ] = b64Table[ (accumulator >> bitsCollected) & 0x3fu ];
        }
    }

    // Sometimes, some bits are left over. Forcibly add these to the string.
    if( bitsCollected > 0 ){
        accumulator <<= 6 - bitsCollected;
        retval[ outpos++ ] = b64Table[ accumulator & 0x3fu ];
    }

    return std::move( retval );
}

// ------------------------------------------------------------------------------------------ //

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
            c == '/' ? 63 : (++padding, 0)
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

int main( void ){
    std::cout << "\"\"          \"" << base64Encode( ""       ) << "\"" << std::endl;
    std::cout << "\"Zg==\"      \"" << base64Encode( "f"      ) << "\"" << std::endl;
    std::cout << "\"Zm8=\"      \"" << base64Encode( "fo"     ) << "\"" << std::endl;
    std::cout << "\"Zm9v\"      \"" << base64Encode( "foo"    ) << "\"" << std::endl;
    std::cout << "\"Zm9vYg==\"  \"" << base64Encode( "foob"   ) << "\"" << std::endl;
    std::cout << "\"Zm9vYmE=\"  \"" << base64Encode( "fooba"  ) << "\"" << std::endl;
    std::cout << "\"Zm9vYmFy\"  \"" << base64Encode( "foobar" ) << "\"" << std::endl;

    std::cout << "\"\"          \"" << base64Decode( ""           ) << "\"" << std::endl;
    std::cout << "\"f\"         \"" << base64Decode( "Zg=="       ) << "\"" << std::endl;
    std::cout << "\"fo\"        \"" << base64Decode( "Zm8="       ) << "\"" << std::endl;
    std::cout << "\"foo\"       \"" << base64Decode( "Zm9v"       ) << "\"" << std::endl;
    std::cout << "\"foob\"      \"" << base64Decode( "Zm9vYg=="   ) << "\"" << std::endl;
    std::cout << "\"fooba\"     \"" << base64Decode( "Zm9vYmE="   ) << "\"" << std::endl;
    std::cout << "\"foobar\"    \"" << base64Decode( "Zm9vYmFy"   ) << "\"" << std::endl;
}
