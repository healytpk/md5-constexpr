#include <random>           // random_device
#include <type_traits>      // is_constant_evaluated
#include "md5.h"

consteval __uint128_t uuid(char const *const name) noexcept
{
    // The following line shouldn't be needed
    // but I'm a fan of belt and braces
    static_assert( std::is_constant_evaluated() );
    md5::details::Context c;
    c.append( name, md5::details::const_strlen(name) );
    c.append( "This is my salt!", sizeof "This is my salt!" - 1u );
    return c.final();
}

__uint128_t uuid(void) noexcept(false)
{
    std::random_device rd;  // might throw
    // rd() yields an unsigned integer type of implementation-defined width
    __uint128_t retval = 0u;
    static_assert( 0u == (sizeof(__uint128_t) % sizeof(rd())) );
    constexpr unsigned how_many_uints = sizeof(__uint128_t) / sizeof(rd());
    for ( unsigned n = 0u; n < how_many_uints; ++n )
    {
        retval <<= 128u / how_many_uints;
        retval  |= rd();  // might throw
    }
    return retval;
}

// ======================= Here's some test code =========================

#include <iostream>    // cout, endl
#include <ios>         // hex

int main(int const argc, char **const argv)
{
    switch ( argc )
    {
    case (int)uuid("I like chocolate!"):
        break;
    }

    constexpr auto monkey = uuid("frog");
    std::cout << std::hex << (uint64_t)(monkey >> 64u) << std::endl;

    // constexpr auto b = uuid();   This line will fail to compiler

    // Be aware though that the following line compiles
    // successfully even though argc isn't known
    // until runtime. This is because the argument
    // to the function is known at compile time.
    if ( argc & 1 ) return (int)uuid("abc");
}
