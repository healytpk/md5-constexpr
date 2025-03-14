/*
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 * and associated documentation files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge, publish, distribute,
 * sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:

 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 * BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef HEADER_INCLUSION_GUARD_b451fcab_dc35_4960_ae62_0de12c893981
#define HEADER_INCLUSION_GUARD_b451fcab_dc35_4960_ae62_0de12c893981

#include <climits>          // CHAR_BIT
#include <cstddef>          // size_t
#include <cstdint>          // int16_t, uint32_t
#include <array>            // array

namespace md5 {
    using Digest = __uint128_t;

    namespace details {
        using std::array, std::uint32_t, std::size_t;
        using Array = array< char unsigned, 128u / CHAR_BIT >;

        constexpr __uint128_t make_uint128(Array const &arr)
        {
            __uint128_t n = 0u;
            for ( auto const c : arr )
            {
                n <<= CHAR_BIT;
                n |= c;
            }
            return n;
        }

        constexpr uint32_t block_C = 64u;
        constexpr uint32_t block_L = block_C / 4u;

        constexpr size_t const_strlen(char const *const str)
        {
            return (*str == '\0') ? 0u : const_strlen(str + 1) + 1u;
        }

        constexpr Digest make_digest(array<uint32_t, 4u> const &input) noexcept
        {
            Array digest{};
            for ( size_t i = 0u; i < input.size(); ++i )
            {
                digest[i * 4u + 0u] = (input[i] >> 0u ) & 0xff;
                digest[i * 4u + 1u] = (input[i] >> 8u ) & 0xff;
                digest[i * 4u + 2u] = (input[i] >> 16u) & 0xff;
                digest[i * 4u + 3u] = (input[i] >> 24u) & 0xff;
            }
            return make_uint128(digest);
        }

        constexpr uint32_t f(uint32_t const b, uint32_t const c, uint32_t const d) noexcept
        {
            return (b & c) | (~b & d);
        }

        constexpr uint32_t g(uint32_t const b, uint32_t const c, uint32_t const d) noexcept
        {
            return (b & d) | (c & ~d);
        }

        constexpr uint32_t h(uint32_t const b, uint32_t const c, uint32_t const d) noexcept
        {
            return b ^ c ^ d;
        }

        constexpr uint32_t i(uint32_t const b, uint32_t const c, uint32_t const d) noexcept
        {
            return c ^ (b | ~d);
        }

        typedef uint32_t (*Fn)(uint32_t, uint32_t, uint32_t);
        constexpr Fn block_F[4u] = { f, g, h, i };

        constexpr uint32_t block_G[block_C] = {
            0u, 1u,  2u,  3u,  4u,  5u,  6u,  7u,  8u,  9u, 10u, 11u, 12u, 13u, 14u, 15u,
            1u, 6u, 11u,  0u,  5u, 10u, 15u,  4u,  9u, 14u,  3u,  8u, 13u,  2u,  7u, 12u,
            5u, 8u, 11u, 14u,  1u,  4u,  7u, 10u, 13u,  0u,  3u,  6u,  9u, 12u, 15u,  2u,
            0u, 7u, 14u,  5u, 12u, 3u,  10u,  1u,  8u, 15u,  6u, 13u,  4u, 11u,  2u,  9u
        };

        constexpr uint32_t block_K[block_C] = {
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

        constexpr uint32_t block_S[block_L] = {
            7u, 12u, 17u, 22u,
            5u,  9u, 14u, 20u,
            4u, 11u, 16u, 23u,
            6u, 10u, 15u, 21u
        };

        constexpr char padding[block_C] = {
      (char)0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 0x80 = -128 two's complement
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        };

        constexpr uint32_t rotate(uint32_t const x, uint32_t const n) noexcept
        {
            return (x << n) | (x >> (32u - n));
        }

        constexpr uint32_t t(Fn const arg_func, uint32_t const a, uint32_t const b, uint32_t const c, uint32_t const d, uint32_t const x, uint32_t const s, uint32_t const ac) noexcept
        {
            return b + rotate(a + arg_func(b, c, d) + x + ac, s);
        }

        constexpr uint32_t to_uint32(char unsigned const *const data) noexcept
        {
            return
                (static_cast<uint32_t>(data[3]) << 24u) |
                (static_cast<uint32_t>(data[2]) << 16u) |
                (static_cast<uint32_t>(data[1]) << 8u ) |
                (static_cast<uint32_t>(data[0]) << 0u );
        }

        struct Context {
            array<char unsigned, block_C> buffer;
            array<uint32_t, 4u> state;
            uint32_t nl, nh;

            constexpr Context() noexcept
                : buffer()
                , state{ 0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476 }
                , nl(0)
                , nh(0)
            {}

            constexpr void append(char const *const data, size_t const len) noexcept
            {
                array<uint32_t, block_L> input{};
                auto k = (nl >> 3u) & 0x3f;
                auto length = static_cast<uint32_t>(len);
                nl += length << 3u;
                if ( nl < (length << 3u) )
                {
                    nh += 1u;
                }
                nh += length >> 29u;
                for (auto ptr = data; ptr != data + len; ++ptr)
                {
                    buffer[k++] = static_cast<char unsigned>(static_cast<int16_t>(*ptr) + UCHAR_MAX + 1);
                    if (k == 0x40)
                    {
                        auto j = 0;
                        for (auto i = 0; i < block_L; ++i)
                        {
                            input[i] = to_uint32(&buffer[j]);
                            j += 4;
                        }
                        transform(input);
                        k = 0;
                    }
                }
            }

            constexpr void transform(const array<uint32_t, block_L>& input) noexcept
            {
                auto a = state[0], b = state[1], c = state[2], d = state[3];
                for ( uint32_t r = 0u; r < 4u; ++r )
                {
                    const auto g = block_G + r * block_L;
                    const auto s = block_S + r * 4;
                    const auto k = block_K + r * block_L;

                    for (auto i = 0; i < input.size(); ++i)
                    {
                        const auto new_b = t(block_F[r], a, b, c, d, input[g[i]], s[i % 4], k[i]);
                        a = d;
                        d = c;
                        c = b;
                        b = new_b;
                    }
                }
                state[0] += a;
                state[1] += b;
                state[2] += c;
                state[3] += d;
            }

            constexpr Digest final() noexcept
            {
                array<uint32_t, block_L> input{};
                unsigned const k = ((nl >> 3u) & 0x3f);
                input[14] = nl;
                input[15] = nh;

                append(padding, k < 56 ? 56 - k : 120 - k);

                unsigned j = 0u;
                for ( unsigned i = 0u; i < 14u; ++i )
                {
                    input[i] = to_uint32(&buffer[j]);
                    j += 4u;
                }

                transform(input);
                return make_digest(state);
            }
        };
    }  // close namespace 'details'

    template <size_t N>
    constexpr Digest compute( char const (&data)[N] ) noexcept
    {
        details::Context c;
        // Don't hash the null-terminator
        c.append(data, N - 1u);
        return c.final();
    }

    constexpr Digest compute(char const *const s) noexcept
    {
        details::Context c;
        // Don't hash the null-terminator
        c.append(s, details::const_strlen(s));
        return c.final();
    }
}

#endif  // HEADER_INCLUSION_GUARD
