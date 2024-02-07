
//              Copyright Catch2 Authors
// Distributed under the Boost Software License, Version 1.0.
//   (See accompanying file LICENSE.txt or copy at
//        https://www.boost.org/LICENSE_1_0.txt)

// SPDX-License-Identifier: BSL-1.0
#include <catch2/internal/catch_reusable_string_stream.hpp>
#include <catch2/internal/catch_singletons.hpp>
#include <catch2/internal/catch_unique_ptr.hpp>
#include <catch2/internal/catch_lazy_expr.hpp>
#include <catch2/internal/catch_stringref.hpp>
#include <catch2/internal/catch_source_line_info.hpp>

#include <catch2/catch_version.hpp>
#include <catch2/catch_test_spec.hpp>

#include <cstdio>
#include <sstream>
#include <vector>
#include <cstddef>
#include <ostream>

namespace Catch {

    // This class encapsulates the idea of a pool of ostringstreams that can be reused.
    struct StringStreams {
        std::vector<Detail::unique_ptr<std::ostringstream>> m_streams;
        std::vector<std::size_t> m_unused;
        std::ostringstream m_referenceStream; // Used for copy state/ flags from

        auto add() -> std::size_t {
            if( m_unused.empty() ) {
                m_streams.push_back( Detail::make_unique<std::ostringstream>() );
                return m_streams.size()-1;
            }
            else {
                auto index = m_unused.back();
                m_unused.pop_back();
                return index;
            }
        }

        void release( std::size_t index ) {
            m_streams[index]->copyfmt( m_referenceStream ); // Restore initial flags and other state
            m_unused.push_back(index);
        }
    };

    struct ReusableStringStream::Impl
    {
        std::size_t m_index;
        std::ostream* m_oss;

        Impl()
        :   m_index( Singleton<StringStreams>::getMutable().add() ),
            m_oss( Singleton<StringStreams>::getMutable().m_streams[m_index].get() )
        {}

        ~Impl() {
            static_cast<std::ostringstream*>( m_oss )->str("");
            m_oss->clear();
            Singleton<StringStreams>::getMutable().release( m_index );
        }
    };

    ReusableStringStream::ReusableStringStream()
    :   m_impl(new Impl)
    {}

    ReusableStringStream::~ReusableStringStream() {
        delete m_impl;
    }

    std::string ReusableStringStream::str() const {
        return static_cast<std::ostringstream*>( m_impl->m_oss )->str();
    }

    void ReusableStringStream::str( std::string const& str ) {
        static_cast<std::ostringstream*>( m_impl->m_oss )->str( str );
    }

    auto ReusableStringStream::operator << ( int const* value ) -> ReusableStringStream&
    {
        (*m_impl->m_oss) << value;
        return *this;
    }

    auto ReusableStringStream::operator << ( char value ) -> ReusableStringStream&
    {
        (*m_impl->m_oss) << value;
        return *this;
    }

    auto ReusableStringStream::operator << ( char const* value ) -> ReusableStringStream&
    {
        (*m_impl->m_oss) << value;
        return *this;
    }

    auto ReusableStringStream::operator << ( std::string const& value ) -> ReusableStringStream&
    {
        (*m_impl->m_oss) << value;
        return *this;
    }

    auto ReusableStringStream::operator << ( LazyExpression const& value ) -> ReusableStringStream&
    {
        (*m_impl->m_oss) << value;
        return *this;
    }

    auto ReusableStringStream::operator << ( StringRef const& value ) -> ReusableStringStream&
    {
        (*m_impl->m_oss) << value;
        return *this;
    }

    auto ReusableStringStream::operator << ( StringRefBase const& value ) -> ReusableStringStream&
    {
        (*m_impl->m_oss) << StringRef(value);
        return *this;
    }

    auto ReusableStringStream::operator << ( SourceLineInfo const& value ) -> ReusableStringStream&
    {
        (*m_impl->m_oss) << value;
        return *this;
    }

    auto ReusableStringStream::operator << ( Version const& value ) -> ReusableStringStream&
    {
        (*m_impl->m_oss) << value;
        return *this;
    }

    auto ReusableStringStream::operator << ( TestSpec const& value ) -> ReusableStringStream&
    {
        (*m_impl->m_oss) << value;
        return *this;
    }

    auto ReusableStringStream::get() -> std::ostream& { return *m_impl->m_oss; }
}
