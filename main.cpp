#include <cstdio>                         // puts
#include "factoryuuid.hpp"

int main(int const argc, char **const argv)
{
    using namespace FactoryUUID;

    constexpr Byte frog[] = {'f','r','o','g','\0'};

    constexpr auto monkey = uuid( frog );
    Byte str[64u];
    uuid_to_cstr(monkey, str);

    for ( Byte *p = str; '\0' != *p; ++p )
    {
        char two[2u];
        two[0] = *p;
        two[1] = '\0';
        printf("%s", two);
    }

    puts("");
}
