
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <vector>

namespace _details {
    template< std::size_t Pos, std::size_t FullBits >
    std::bitset< FullBits >& join( std::bitset< FullBits >& out ){
        return out;
    }

    template< std::size_t Pos, std::size_t FullBits, std::int64_t Bits, typename... MoreBits >
    std::bitset< FullBits >& join(
        std::bitset< FullBits >& out,
        const std::bitset< Bits >& bits,
        const MoreBits&... moreBits
    ){
        for( std::int64_t i = Bits; i > 0; --i ){
            std::bitset< FullBits > maskedBit = bits & (1 << (i - 1));
            out |= maskedBit << ((FullBits );
        }

        return join< Pos - 1, FullBits >( out, moreBits... );
    }
}

template< std::int64_t Bits, typename... MoreBits >
std::bitset< Bits * (sizeof...( MoreBits ) + 1) > join(
    const std::bitset< Bits >& bits,
    const MoreBits&... moreBits
){
    const std::size_t size = sizeof...( MoreBits ) + 1;
    std::bitset< Bits * size > data = 0;
    return _details::join< size - 1, size >( data, bits, moreBits... );
}

template<
    std::size_t Pieces,
    std::size_t Bits,
    template<class, class...> class Container,
    class... Rest
>
std::bitset< Pieces * Bits > join( const Container< std::bitset< Bits >, Rest... >& bits ){
    std::bitset< Pieces * Bits > value = 0;

    for( std::int64_t step = Bits; step > 0; --step ){
        std::bitset< Pieces * Bits > mask( 1 << (step - 1) );
        for( std::int64_t i = 0; i < Pieces; ++i ){
            // Load the bits into a bitset.
            std::bitset< Pieces * Bits > maskedBit( bits[ i ].to_ulong() );

            // Mask the bit and shift it into the right spot in the block.
            maskedBit = (maskedBit & mask) << (Pieces - step) >> i;

            // Shift the bit into the final position and add it to the output value.
            value |= maskedBit << (step * Pieces - Bits);
        }
    }

    return value;
}

int main( void ){
    std::size_t num     = 0xabcd; // 1010 1011 1100 1101
    std::size_t num0    = 0x000f; // 1111; First bit of each nibble.
    std::size_t num1    = 0x0003; // 0011; Second bit of each nibble.
    std::size_t num2    = 0x000c; // 1100; Third bit of each nibble.
    std::size_t num3    = 0x0005; // 0101; Fourth bit of each nibble.

    auto full = join< 4 >(
        std::vector< std::bitset< 4 > >{
            std::bitset< 4 >( num0 ),
            std::bitset< 4 >( num1 ),
            std::bitset< 4 >( num2 ),
            std::bitset< 4 >( num3 )
        }
    );

    std::cout
        << std::bitset< 16 >( num   ) << std::endl
        << std::bitset< 4 >( num0  ) << std::endl
        << std::bitset< 4 >( num1  ) << std::endl
        << std::bitset< 4 >( num2  ) << std::endl
        << std::bitset< 4 >( num3  ) << std::endl
        << std::bitset< 16 >( full  ) << std::endl
    ;

    return 0;
}
