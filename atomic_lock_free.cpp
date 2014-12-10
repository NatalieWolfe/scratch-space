
#include <atomic>
#include <iostream>

int main( void ){
    std::atomic_char            atomic_char;
    std::atomic_schar           atomic_schar;
    std::atomic_uchar           atomic_uchar;
    std::atomic_short           atomic_short;
    std::atomic_ushort          atomic_ushort;
    std::atomic_int             atomic_int;
    std::atomic_uint            atomic_uint;
    std::atomic_long            atomic_long;
    std::atomic_ulong           atomic_ulong;
    std::atomic_llong           atomic_llong;
    std::atomic_ullong          atomic_ullong;
    std::atomic_char16_t        atomic_char16;
    std::atomic_char32_t        atomic_char32;
    std::atomic_wchar_t         atomic_wchar;
    std::atomic_int_least8_t    atomic_int_least8;
    std::atomic_uint_least8_t   atomic_uint_least8;
    std::atomic_int_least16_t   atomic_int_least16;
    std::atomic_uint_least16_t  atomic_uint_least16;
    std::atomic_int_least32_t   atomic_int_least32;
    std::atomic_uint_least32_t  atomic_uint_least32;
    std::atomic_int_least64_t   atomic_int_least64;
    std::atomic_uint_least64_t  atomic_uint_least64;
    std::atomic_int_fast8_t     atomic_int_fast8;
    std::atomic_uint_fast8_t    atomic_uint_fast8;
    std::atomic_int_fast16_t    atomic_int_fast16;
    std::atomic_uint_fast16_t   atomic_uint_fast16;
    std::atomic_int_fast32_t    atomic_int_fast32;
    std::atomic_uint_fast32_t   atomic_uint_fast32;
    std::atomic_int_fast64_t    atomic_int_fast64;
    std::atomic_uint_fast64_t   atomic_uint_fast64;
    std::atomic_intptr_t        atomic_intptr;
    std::atomic_uintptr_t       atomic_uintptr;
    std::atomic_size_t          atomic_size;
    std::atomic_ptrdiff_t       atomic_ptrdiff;
    std::atomic_intmax_t        atomic_intmax;
    std::atomic_uintmax_t       atomic_uintmax;

    std::cout << "std::atomic_char::is_lock_free:           " << std::boolalpha << atomic_char.is_lock_free()           << std::endl;
    std::cout << "std::atomic_schar::is_lock_free:          " << std::boolalpha << atomic_schar.is_lock_free()          << std::endl;
    std::cout << "std::atomic_uchar::is_lock_free:          " << std::boolalpha << atomic_uchar.is_lock_free()          << std::endl;
    std::cout << "std::atomic_short::is_lock_free:          " << std::boolalpha << atomic_short.is_lock_free()          << std::endl;
    std::cout << "std::atomic_ushort::is_lock_free:         " << std::boolalpha << atomic_ushort.is_lock_free()         << std::endl;
    std::cout << "std::atomic_int::is_lock_free:            " << std::boolalpha << atomic_int.is_lock_free()            << std::endl;
    std::cout << "std::atomic_uint::is_lock_free:           " << std::boolalpha << atomic_uint.is_lock_free()           << std::endl;
    std::cout << "std::atomic_long::is_lock_free:           " << std::boolalpha << atomic_long.is_lock_free()           << std::endl;
    std::cout << "std::atomic_ulong::is_lock_free:          " << std::boolalpha << atomic_ulong.is_lock_free()          << std::endl;
    std::cout << "std::atomic_llong::is_lock_free:          " << std::boolalpha << atomic_llong.is_lock_free()          << std::endl;
    std::cout << "std::atomic_ullong::is_lock_free:         " << std::boolalpha << atomic_ullong.is_lock_free()         << std::endl;
    std::cout << "std::atomic_char16_t::is_lock_free:       " << std::boolalpha << atomic_char16.is_lock_free()         << std::endl;
    std::cout << "std::atomic_char32_t::is_lock_free:       " << std::boolalpha << atomic_char32.is_lock_free()         << std::endl;
    std::cout << "std::atomic_wchar_t::is_lock_free:        " << std::boolalpha << atomic_wchar.is_lock_free()          << std::endl;
    std::cout << "std::atomic_int_least8_t::is_lock_free:   " << std::boolalpha << atomic_int_least8.is_lock_free()     << std::endl;
    std::cout << "std::atomic_uint_least8_t::is_lock_free:  " << std::boolalpha << atomic_uint_least8.is_lock_free()    << std::endl;
    std::cout << "std::atomic_int_least16_t::is_lock_free:  " << std::boolalpha << atomic_int_least16.is_lock_free()    << std::endl;
    std::cout << "std::atomic_uint_least16_t::is_lock_free: " << std::boolalpha << atomic_uint_least16.is_lock_free()   << std::endl;
    std::cout << "std::atomic_int_least32_t::is_lock_free:  " << std::boolalpha << atomic_int_least32.is_lock_free()    << std::endl;
    std::cout << "std::atomic_uint_least32_t::is_lock_free: " << std::boolalpha << atomic_uint_least32.is_lock_free()   << std::endl;
    std::cout << "std::atomic_int_least64_t::is_lock_free:  " << std::boolalpha << atomic_int_least64.is_lock_free()    << std::endl;
    std::cout << "std::atomic_uint_least64_t::is_lock_free: " << std::boolalpha << atomic_uint_least64.is_lock_free()   << std::endl;
    std::cout << "std::atomic_int_fast8_t::is_lock_free:    " << std::boolalpha << atomic_int_fast8.is_lock_free()      << std::endl;
    std::cout << "std::atomic_uint_fast8_t::is_lock_free:   " << std::boolalpha << atomic_uint_fast8.is_lock_free()     << std::endl;
    std::cout << "std::atomic_int_fast16_t::is_lock_free:   " << std::boolalpha << atomic_int_fast16.is_lock_free()     << std::endl;
    std::cout << "std::atomic_uint_fast16_t::is_lock_free:  " << std::boolalpha << atomic_uint_fast16.is_lock_free()    << std::endl;
    std::cout << "std::atomic_int_fast32_t::is_lock_free:   " << std::boolalpha << atomic_int_fast32.is_lock_free()     << std::endl;
    std::cout << "std::atomic_uint_fast32_t::is_lock_free:  " << std::boolalpha << atomic_uint_fast32.is_lock_free()    << std::endl;
    std::cout << "std::atomic_int_fast64_t::is_lock_free:   " << std::boolalpha << atomic_int_fast64.is_lock_free()     << std::endl;
    std::cout << "std::atomic_uint_fast64_t::is_lock_free:  " << std::boolalpha << atomic_uint_fast64.is_lock_free()    << std::endl;
    std::cout << "std::atomic_intptr_t::is_lock_free:       " << std::boolalpha << atomic_intptr.is_lock_free()         << std::endl;
    std::cout << "std::atomic_uintptr_t::is_lock_free:      " << std::boolalpha << atomic_uintptr.is_lock_free()        << std::endl;
    std::cout << "std::atomic_size_t::is_lock_free:         " << std::boolalpha << atomic_size.is_lock_free()           << std::endl;
    std::cout << "std::atomic_ptrdiff_t::is_lock_free:      " << std::boolalpha << atomic_ptrdiff.is_lock_free()        << std::endl;
    std::cout << "std::atomic_intmax_t::is_lock_free:       " << std::boolalpha << atomic_intmax.is_lock_free()         << std::endl;
    std::cout << "std::atomic_uintmax_t::is_lock_free:      " << std::boolalpha << atomic_uintmax.is_lock_free()        << std::endl;
}
