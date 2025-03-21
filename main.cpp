#include <iostream>    // cout, endl
#include <ios>         // hex
#include <iomanip>
#include "md5.hpp"

int main(int const argc, char **const argv)
{
    //std::cout << std::hex << (uint64_t)(md5::to_uint128(md5::compute("frog")) >> 64u) << std::endl;

/*
    switch ( argc )
    {
    case (int)uuid("I like chocolate!"):
        break;
    }
*/

    constexpr auto monkey = uuid("frog");
    for ( auto const &b : monkey ) std::cout << std::hex << std::setfill('0') << std::setw(2) << (unsigned)b;
    std::cout << std::endl;

    // constexpr auto b = uuid();   This line will fail to compiler

    // Be aware though that the following line compiles
    // successfully even though argc isn't known
    // until runtime. This is because the argument
    // to the function is known at compile time.
    //if ( argc & 1 ) return (int)uuid("abc");
}
