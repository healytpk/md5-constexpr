#include <cstdio>    // puts
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
    char str[32u + 1u];
    uuid_to_cstr(monkey, str);
    std::puts(str);

    // constexpr auto b = uuid();   This line will fail to compiler

    // Be aware though that the following line compiles
    // successfully even though argc isn't known
    // until runtime. This is because the argument
    // to the function is known at compile time.
    //if ( argc & 1 ) return (int)uuid("abc");
}
