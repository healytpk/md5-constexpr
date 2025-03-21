#ifndef HEADER_INCLUSION_GUARD_64646654958285984464484672252288642515144
#define HEADER_INCLUSION_GUARD_64646654958285984464484672252288642515144

// Written for C++14 compilers and above

#include <climits>          // CHAR_BIT, UCHAR_MAX
#include <cstddef>          // size_t
#include <cstdint>          // int16_t
#include <cstring>          // strlen
#include <array>            // array
#include <random>           // random_device
#include <type_traits>      // is_constant_evaluated (c++20)

#ifdef __cpp_lib_string_view
#   include <string_view>  // c++17
#endif

namespace md5 {
    struct Digest {
        static constexpr unsigned count = (128u / CHAR_BIT) + !!(128u % CHAR_BIT);
        char unsigned b[count];
        constexpr char unsigned &operator[](unsigned const n) noexcept
        {
            return this->b[n];
        }
        constexpr char unsigned       *begin(void)       noexcept { return b; }
        constexpr char unsigned       *end  (void)       noexcept { return b + count; }
        constexpr char unsigned const *begin(void) const noexcept { return b; }
        constexpr char unsigned const *end  (void) const noexcept { return b + count; }
    };

    namespace details {

        using std::array;
        using std::size_t;
        using std::uint_fast32_t;

        // We can't use uint_fast32_t if it undergoes promotion,
        // so instead use long unsigned (which might be 64-Bit)
        typedef typename std::conditional<
            std::is_same< uint_fast32_t, decltype(uint_fast32_t() + uint_fast32_t()) >::value,
            uint_fast32_t,
            long unsigned >::type UIntType;

        constexpr UIntType constant_c = 64u;
        constexpr UIntType constant_L = constant_c / 4u;

        constexpr size_t const_strlen(char const *str) noexcept
        {
#ifdef __cpp_lib_is_constant_evaluated
            if constexpr ( false == std::is_constant_evaluated() )
            {
                // This is just an optimisation for runtime use
                return std::strlen(str);
            }
            else
#endif
            {
                return ('\0' == *str) ? 0u : const_strlen(++str) + 1u;
            }
        }

        constexpr Digest make_digest(array<UIntType, 4u> const &input) noexcept
        {
            Digest digest{};
            for ( unsigned i = 0u; i < input.size(); ++i )
            {
                digest[i * 4u + 0u] = (input[i] >> 0u ) & 0xff;
                digest[i * 4u + 1u] = (input[i] >> 8u ) & 0xff;
                digest[i * 4u + 2u] = (input[i] >> 16u) & 0xff;
                digest[i * 4u + 3u] = (input[i] >> 24u) & 0xff;
            }
            return digest;
        }

        constexpr UIntType f(UIntType const b, UIntType const c, UIntType const d) noexcept
        {
            return (b & c) | (~b & d);
        }

        constexpr UIntType g(UIntType const b, UIntType const c, UIntType const d) noexcept
        {
            return (b & d) | (c & ~d);
        }

        constexpr UIntType h(UIntType const b, UIntType const c, UIntType const d) noexcept
        {
            return b ^ c ^ d;
        }

        constexpr UIntType i(UIntType const b, UIntType const c, UIntType const d) noexcept
        {
            return c ^ (b | ~d);
        }

        typedef UIntType (*Fn)(UIntType, UIntType, UIntType);
        constexpr Fn array_of_funcptrs[4u] = { f, g, h, i };

        constexpr UIntType block_G[constant_c] = {
            0u, 1u,  2u,  3u,  4u,  5u,  6u,  7u,  8u,  9u, 10u, 11u, 12u, 13u, 14u, 15u,
            1u, 6u, 11u,  0u,  5u, 10u, 15u,  4u,  9u, 14u,  3u,  8u, 13u,  2u,  7u, 12u,
            5u, 8u, 11u, 14u,  1u,  4u,  7u, 10u, 13u,  0u,  3u,  6u,  9u, 12u, 15u,  2u,
            0u, 7u, 14u,  5u, 12u, 3u,  10u,  1u,  8u, 15u,  6u, 13u,  4u, 11u,  2u,  9u
        };

        constexpr UIntType block_K[constant_c] = {
            0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
            0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
            0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
            0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
            0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
            0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
            0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
            0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
            0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
            0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
            0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
            0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
            0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
            0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
            0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
            0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
        };

        constexpr UIntType block_S[constant_L] = {
            7u, 12u, 17u, 22u,
            5u,  9u, 14u, 20u,
            4u, 11u, 16u, 23u,
            6u, 10u, 15u, 21u
        };

        constexpr char padding[constant_c] = {
      (char)0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 0x80 = -128 two's complement
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        };

        constexpr UIntType rotate(UIntType x, UIntType const n) noexcept
        {
            x &= 0xfffffffful;  // The only change needed for 64-Bit
            return (x << n) | (x >> (32u - n));
        }

        constexpr UIntType t(Fn arg_func, UIntType a, UIntType b, UIntType c, UIntType d, UIntType x, UIntType s, UIntType ac) noexcept
        {
            return b + rotate(a + arg_func(b, c, d) + x + ac, s);
        }

        constexpr UIntType array_to_long_unsigned(char unsigned const *const data) noexcept
        {
            return
                (static_cast<UIntType>(data[3] & 0xFF) << 24u) |
                (static_cast<UIntType>(data[2] & 0xFF) << 16u) |
                (static_cast<UIntType>(data[1] & 0xFF) << 8u ) |
                (static_cast<UIntType>(data[0] & 0xFF) << 0u );
        }

        struct Context {
            char unsigned buffer[constant_c];
            array<UIntType, 4u> state;
            UIntType nl, nh;

            constexpr Context() noexcept
                : buffer()
                , state{ 0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476 }
                , nl(0u)
                , nh(0u)
            {}

            constexpr void append(char const *const data, size_t const len) noexcept
            {
                UIntType input[constant_L]{};
                UIntType k = (nl >> 3u) & 0x3f;
                UIntType const length = static_cast<UIntType>(len);
                nl += length << 3u;
                if ( nl < (length << 3u) ) ++nh;
                nh += length >> 29u;
                for ( char const *ptr = data; ptr != (data + len); ++ptr )
                {
                    buffer[k++] = static_cast<char unsigned>(static_cast<int16_t>(*ptr) + UCHAR_MAX + 1);
                    if ( 0x40 != k ) continue;
                    int j = 0;
                    for ( int i = 0; i < constant_L; ++i )
                    {
                        input[i] = array_to_long_unsigned(&buffer[j]);
                        j += 4;
                    }
                    transform(input);
                    k = 0;
                }
            }

            template <size_t N>
            constexpr Context &operator<<( char const (&data)[N] ) noexcept
            {
                this->append( data, N - 1u );
                return *this;
            }

            constexpr Context &operator<<( char const *const data ) noexcept
            {
                this->append( data, const_strlen(data) );
                return *this;
            }

#ifdef __cpp_lib_string_view
            constexpr Context &operator<<(std::string_view const s) noexcept
            {
                this->append(s.data(), s.size());
                return *this;
            }
#endif

            constexpr void transform(UIntType const (&input)[constant_L]) noexcept
            {
                UIntType a = std::get<0u>(state), b = std::get<1u>(state), c = std::get<2u>(state), d = std::get<3u>(state);
                for ( unsigned r = 0u; r < 4u; ++r )
                {
                    UIntType const *const pG = block_G + r * constant_L;
                    UIntType const *const pS = block_S + r * 4u;
                    UIntType const *const pK = block_K + r * constant_L;

                    for ( unsigned i = 0; i < constant_L; ++i )
                    {
                        auto const new_b = t(array_of_funcptrs[r], a, b, c, d, input[pG[i]], pS[i % 4u], pK[i]);
                        a = d;
                        d = c;
                        c = b;
                        b = new_b;
                    }
                }
                std::get<0u>(state) += a;
                std::get<1u>(state) += b;
                std::get<2u>(state) += c;
                std::get<3u>(state) += d;
            }

            constexpr Digest final(void) noexcept
            {
                UIntType input[constant_L]{};
                unsigned const k = (nl >> 3u) & 0x3f;
                input[14] = nl;
                input[15] = nh;

                append(padding, k < 56u ? 56u - k : 120u - k);

                unsigned j = 0u;
                for ( unsigned i = 0u; i < 14u; ++i )
                {
                    input[i] = array_to_long_unsigned(&buffer[j]);
                    j += 4u;
                }

                transform(input);
                return make_digest(state);
            }
        };
    }  // close namespace 'details'

    template <std::size_t N>
    constexpr Digest compute( char const (&s)[N] ) noexcept
    {
        return (details::Context() << s).final();
    }

    constexpr Digest compute(char const *const s) noexcept
    {
        return (details::Context() << s).final();
    }

#ifdef __cpp_lib_string_view
    constexpr Digest compute(std::string_view const s) noexcept
    {
        return (details::Context() << s).final();
    }
#endif //__cpp_lib_string_view

/*
    constexpr __uint128_t to_uint128(Digest const &arr)
    {
        __uint128_t n = 0u;
        
#if defined(__BYTE_ORDER__) && (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
        for ( unsigned i = 15; i < 16u; --i )
#else
        for ( unsigned i = 0u; i < 16u; ++i )
#endif
        {
            n <<= CHAR_BIT;
            n |= arr[i];
        }

        return n;
    }
*/
}

constexpr md5::Digest rand128_to_UUIDv4(md5::Digest arg) noexcept
{
    // Set to version 4 and IETF variant
    //arg &= ((__uint128_t)0xffffffffffff003f << 64u) | 0xff0fffffffffffff;
    //arg |= ((__uint128_t)0x0000000000000080 << 64u) | 0x0040000000000000;
    return arg;
}

#ifdef __cpp_consteval
    consteval  // c++(20)
#else
    constexpr
#endif
md5::Digest uuid(char const *const name) noexcept
{
    md5::Digest const digest = (md5::details::Context() << name << "This is my salt!").final();
#ifdef UUID_ONLY_122_BITS
    return rand128_to_UUIDv4(digest);
#endif
    return digest;
}

md5::Digest uuid(void) noexcept(false)
{
    std::random_device rd;  // might throw
    // rd() yields an unsigned integer type of implementation-defined width
    md5::Digest d;
    for ( auto &b : d ) b = static_cast< char unsigned >(  rd() & UCHAR_MAX  );
    return d;
}

#endif  // HEADER_INCLUSION_GUARD
