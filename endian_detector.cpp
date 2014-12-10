
#include <algorithm>
#include <iostream>

bool isBigEndian( void ){
    union {
        short num;
        char bytes[];
    } endianDetector;

    endianDetector.num = 1;
    return endianDetector.bytes[ 0 ] == 0;
}

template< typename IntType >
IntType swapEndian( const IntType i ){
    union {
        IntType num;
        char bytes[];
    } byteSeparator;
    byteSeparator.num = i;
    std::reverse( (char*)byteSeparator.bytes, (char*)byteSeparator.bytes + sizeof( IntType ) );
    return byteSeparator.num;
}

template< typename IntType >
IntType hostToNetwork( const IntType i ){
    return isBigEndian() ? i : swapEndian( i );
}

template< typename IntType >
IntType networkToHost( const IntType i ){
    return hostToNetwork( i );
}

int main( void ){
    if( isBigEndian() ){
        std::cout << "Big-endian" << std::endl;
    }
    else {
        std::cout << "Little-endian" << std::endl;
    }

    std::cout << "htonl( 1u )           " << htonl( 1u ) << std::endl;
    std::cout << "hostToNetwork( 1u )   " << hostToNetwork( 1u ) << std::endl;
    std::cout << "htonll( 1ul )         " << htonll( 1ul ) << std::endl;
    std::cout << "hostToNetwork( 1ul )  " << hostToNetwork( 1ul ) << std::endl;
    std::cout << std::endl;
    std::cout << "ntohl( 1u )           " << ntohl( 1u ) << std::endl;
    std::cout << "networkToHost( 1u )   " << networkToHost( 1u ) << std::endl;
    std::cout << "ntohll( 1ul )         " << ntohll( 1ul ) << std::endl;
    std::cout << "networkToHost( 1ul )  " << networkToHost( 1ul ) << std::endl;


    return 0;
}
