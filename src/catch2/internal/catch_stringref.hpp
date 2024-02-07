
//              Copyright Catch2 Authors
// Distributed under the Boost Software License, Version 1.0.
//   (See accompanying file LICENSE.txt or copy at
//        https://www.boost.org/LICENSE_1_0.txt)

// SPDX-License-Identifier: BSL-1.0
#ifndef CATCH_STRINGREF_HPP_INCLUDED
#define CATCH_STRINGREF_HPP_INCLUDED

#include <catch2/internal/catch_stringrefbase.hpp>

#include <cstddef>
#include <string>
#include <iosfwd>
#include <cassert>

namespace Catch {

    /// A non-owning string class (similar to the forthcoming std::string_view)
    /// Note that, because a StringRef may be a substring of another string,
    /// it may not be null terminated.
    class StringRef : public StringRefBase {


    public: // construction
        constexpr StringRef() noexcept = default;

        StringRef( char const* rawChars ) noexcept;
        StringRef( StringRefBase base ) noexcept;

        constexpr StringRef( char const* rawChars, size_type size ) noexcept
        :   StringRefBase( rawChars, size)
        {}

        StringRef( std::string const& stdString ) noexcept;

        explicit operator std::string() const;

    public: // operators


        constexpr auto operator[] ( size_type index ) const noexcept -> char {
            assert(index < m_size);
            return m_start[index];
        }

        bool operator<(StringRef rhs) const noexcept;

    public: // named queries

        // Returns a substring of [start, start + length).
        // If start + length > size(), then the substring is [start, size()).
        // If start > size(), then the substring is empty.
        constexpr StringRef substr(size_type start, size_type length) const noexcept {
            if (start < m_size) {
                const auto shortened_size = m_size - start;
                return StringRef(m_start + start, (shortened_size < length) ? shortened_size : length);
            } else {
                return StringRef();
            }
        }




        friend std::string& operator += (std::string& lhs, StringRef sr);
        friend std::ostream& operator << (std::ostream& os, StringRef sr);
        friend std::string operator+(StringRef lhs, StringRef rhs);

        /**
         * Provides a three-way comparison with rhs
         *
         * Returns negative number if lhs < rhs, 0 if lhs == rhs, and a positive
         * number if lhs > rhs
         */
        int compare( StringRef rhs ) const;
    };


    constexpr auto operator ""_sr( char const* rawChars, std::size_t size ) noexcept -> StringRef {
        return StringRef( rawChars, size );
    }
} // namespace Catch

constexpr auto operator ""_catch_sr( char const* rawChars, std::size_t size ) noexcept -> Catch::StringRef {
    return Catch::StringRef( rawChars, size );
}

#endif // CATCH_STRINGREF_HPP_INCLUDED
