
//              Copyright Catch2 Authors
// Distributed under the Boost Software License, Version 1.0.
//   (See accompanying file LICENSE.txt or copy at
//        https://www.boost.org/LICENSE_1_0.txt)

// SPDX-License-Identifier: BSL-1.0
#ifndef CATCH_STRINGREFBASE_HPP_INCLUDED
#define CATCH_STRINGREFBASE_HPP_INCLUDED

namespace Catch {

    class StringRefBase
    {
    public:
        using size_type = decltype(sizeof(int));
        using const_iterator = const char*;

        static constexpr size_type npos{ static_cast<size_type>( -1 ) };

        constexpr StringRefBase() noexcept = default;

        constexpr StringRefBase( char const* rawChars, size_type size ) noexcept
        :   m_start( rawChars ),
            m_size( size )
        {}

        template <StringRefBase::size_type N>
        constexpr StringRefBase(const char(&arr)[N]) noexcept : m_start(arr), m_size(N - 1)
        {
        }

        constexpr auto empty() const noexcept -> bool {
            return m_size == 0;
        }
        constexpr auto size() const noexcept -> size_type {
            return m_size;
        }

        constexpr const_iterator begin() const { return m_start; }
        constexpr const_iterator end() const { return m_start + m_size; }

        auto operator == ( StringRefBase other ) const noexcept -> bool;
        auto operator != (StringRefBase other) const noexcept -> bool;

        // Returns the current start pointer. May not be null-terminated.
        constexpr char const* data() const noexcept {
            return m_start;
        }


    protected:
        char const* m_start = "";
        size_type m_size = 0;
    };


} // namespace Catch

#endif // CATCH_STRINGREFBASE_HPP_INCLUDED
