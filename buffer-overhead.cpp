
#include <algorithm>
#include <array>
#include <chrono>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <openssl/aes.h>
#include <random>
#include <sstream>
#include <vector>

typedef std::uint8_t byte;

typedef std::vector<byte> Buffer;

template<typename Source, typename Dest>
void encrypt(const Source& source, Dest&& dest, const std::size_t& size){
    byte iv[ 16 ] = { 0 };
    AES_KEY aesKey;

    AES_set_encrypt_key((byte*)"0123456789abcdef0123456789abcdef", 32 * 8, &aesKey);
    AES_cbc_encrypt((byte*)source, dest, size, &aesKey, iv, AES_ENCRYPT);
}

class DoubleBuffer {
public:
    DoubleBuffer(const Buffer& buffer):
        m_block_id(0),
        m_primary_buffer(buffer)
    {}

    const byte& operator[](const std::size_t& idx) const {
        return m_second_buffer[_adjust_index(idx)];
    }

    const std::size_t size(void) const {
        return m_primary_buffer.size();
    }

private:
    mutable std::size_t m_block_id;
    mutable std::array<byte, 1024> m_second_buffer;
    const Buffer& m_primary_buffer;

    std::size_t _adjust_index(const std::size_t& idx) const {
        // Figure out which block we're in.
        const std::size_t block_id      = idx / 1024;
        const std::size_t block_start   = block_id * 1024;

        // If the desired block is not the one currently loaded, then load the desired block.
        if( block_id != m_block_id ){
            m_block_id = block_id;
            const std::size_t block_size = std::min(m_primary_buffer.size() - block_start, 1024ul);
            encrypt(m_primary_buffer.data() + block_start, m_second_buffer.data(), block_size);
        }
        return idx - block_start;
    }
};

template<typename Container, typename Sequence>
void random_read(const Container& data, Sequence& sequence){
    std::stringstream str;
    for (std::size_t i = 0; i < data.size(); ++i) {
        str << (char)data[sequence()];
    }
}

template<typename Sequence>
void generate_random(Sequence& sequence, const std::size_t count){
    for (std::size_t i = 0; i < count; ++i) {
        sequence();
    }
}

template<typename Container>
void sequential_read(const Container& data){
    std::stringstream str;
    for (std::size_t i = 0; i < data.size(); ++i) {
        str << (char)data[i];
    }
}

void run_test(const std::size_t size){
    // Create our random sequence generator.
    const std::size_t seed = std::random_device()();
    std::mt19937_64 generator(seed);
    std::uniform_int_distribution<std::size_t> sequence(0, size);
    auto&& gen_func = [&](){ return sequence(generator); };

    // Generate our data.
    Buffer data;
    data.reserve(size);
    for (std::size_t i = 0; i < size; ++i) {
        data.push_back(gen_func());
    }

    // Set up the timer.
    using namespace std::chrono;
    typedef high_resolution_clock clock;
    typedef clock::time_point time_point;
    time_point start;
    time_point end;
    nanoseconds diff;

    // Figure out the random number overhead.
    std::cout << "Determining overhead of generating " << size << " random numbers..." << std::endl;
    start = clock::now();
    generate_random(gen_func, size);
    end = clock::now();
    auto rand_overhead = end - start;
    std::cout << " - " << duration_cast<nanoseconds>(rand_overhead).count() << "ns" << std::endl;

    // Time sequential reads of a plain buffer.
    std::cout << "Sequential read of " << size << " bytes from plain buffer:" << std::endl;
    start = clock::now();
    sequential_read(data);
    end = clock::now();
    diff = duration_cast<nanoseconds>(end - start);
    std::cout
        << " - Total:    " << diff.count() << "ns\n"
        << " - Per-byte: " << (diff.count() / size) << "ns" << std::endl;

    // Time random reads of a plain buffer.
    std::cout << "Random read of " << size << " bytes from plain buffer:" << std::endl;
    start = clock::now();
    random_read(data, gen_func);
    end = clock::now();
    diff = duration_cast<nanoseconds>((end - start) - rand_overhead);
    std::cout
        << " - Total:    " << diff.count() << "ns\n"
        << " - Per-byte: " << (diff.count() / size) << "ns" << std::endl;

    // Time sequential read from double buffer.
    std::cout << "Sequential read of " << size << " bytes from double buffer:" << std::endl;
    start = clock::now();
    sequential_read(DoubleBuffer(data));
    end = clock::now();
    diff = duration_cast<nanoseconds>(end - start);
    std::cout
        << " - Total:    " << diff.count() << "ns\n"
        << " - Per-byte: " << (diff.count() / size) << "ns" << std::endl;

    // Time random reads of a double buffer.
    std::cout << "Random read of " << size << " bytes from double buffer:" << std::endl;
    start = clock::now();
    random_read(DoubleBuffer(data), gen_func);
    end = clock::now();
    diff = duration_cast<nanoseconds>((end - start) - rand_overhead);
    std::cout
        << " - Total:    " << diff.count() << "ns\n"
        << " - Per-byte: " << (diff.count() / size) << "ns" << std::endl;

    std::cout << std::endl;
}

int main(void){
    run_test(100);
    run_test(1000);
    run_test(10000);
    run_test(100000);
    run_test(1000000);
}
