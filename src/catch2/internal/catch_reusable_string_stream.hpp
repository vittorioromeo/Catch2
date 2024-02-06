
//              Copyright Catch2 Authors
// Distributed under the Boost Software License, Version 1.0.
//   (See accompanying file LICENSE.txt or copy at
//        https://www.boost.org/LICENSE_1_0.txt)

// SPDX-License-Identifier: BSL-1.0
#ifndef CATCH_REUSABLE_STRING_STREAM_HPP_INCLUDED
#define CATCH_REUSABLE_STRING_STREAM_HPP_INCLUDED

#include <catch2/internal/catch_noncopyable.hpp>

#include <iosfwd>

#include <string>

namespace Catch {

    class LazyExpression;
    class StringRef;
    struct SourceLineInfo;
    struct Version;
    class TestSpec;

    class ReusableStringStream : Detail::NonCopyable {
        struct Impl;

        Impl* m_impl;
    public:
        ReusableStringStream();
        ~ReusableStringStream();

        //! Returns the serialized state
        std::string str() const;
        //! Sets internal state to `str`
        void str(std::string const& str);

#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic push
// Old versions of GCC do not understand -Wnonnull-compare
#pragma GCC diagnostic ignored "-Wpragmas"
// Streaming a function pointer triggers Waddress and Wnonnull-compare
// on GCC, because it implicitly converts it to bool and then decides
// that the check it uses (a? true : false) is tautological and cannot
// be null...
#pragma GCC diagnostic ignored "-Waddress"
#pragma GCC diagnostic ignored "-Wnonnull-compare"
#endif

    auto operator << ( int const* value ) -> ReusableStringStream&;
    auto operator << ( char value ) -> ReusableStringStream&;
    auto operator << ( char const* value ) -> ReusableStringStream&;
    auto operator << ( std::string const& value ) -> ReusableStringStream&;
    auto operator << ( LazyExpression const& value ) -> ReusableStringStream&;
    auto operator << ( StringRef const& value ) -> ReusableStringStream&;
    auto operator << ( SourceLineInfo const& value ) -> ReusableStringStream&;
    auto operator << ( Version const& value ) -> ReusableStringStream&;
    auto operator << ( TestSpec const& value ) -> ReusableStringStream&;

/*
    template<typename T>
    auto operator << ( T const& value ) -> ReusableStringStream& {
        *m_oss << value;
        return *this;
    }
*/

#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic pop
#endif
        auto get() -> std::ostream&;
    };
}

#endif // CATCH_REUSABLE_STRING_STREAM_HPP_INCLUDED
