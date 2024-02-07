
//              Copyright Catch2 Authors
// Distributed under the Boost Software License, Version 1.0.
//   (See accompanying file LICENSE.txt or copy at
//        https://www.boost.org/LICENSE_1_0.txt)

// SPDX-License-Identifier: BSL-1.0
#ifndef CATCH_INTERFACES_ENUM_VALUES_REGISTRY_HPP_INCLUDED
#define CATCH_INTERFACES_ENUM_VALUES_REGISTRY_HPP_INCLUDED

#include <catch2/internal/catch_stringrefbase.hpp>

#include <vector> // TODO: remove

namespace Catch {

    namespace Detail {
        struct EnumInfo {
            StringRefBase m_name;
            std::vector<std::pair<int, StringRefBase>> m_values;

            ~EnumInfo();

            StringRefBase lookup( int value ) const;
        };
    } // namespace Detail

    class IMutableEnumValuesRegistry {
    public:
        virtual ~IMutableEnumValuesRegistry(); // = default;

        virtual Detail::EnumInfo const& registerEnum( StringRefBase enumName, StringRefBase allEnums, std::vector<int> const& values ) = 0;

        template<typename E>
        Detail::EnumInfo const& registerEnum( StringRefBase enumName, StringRefBase allEnums, std::initializer_list<E> values ) {
            static_assert(sizeof(int) >= sizeof(E), "Cannot serialize enum to int");
            std::vector<int> intValues;
            intValues.reserve( values.size() );
            for( auto enumValue : values )
                intValues.push_back( static_cast<int>( enumValue ) );
            return registerEnum( enumName, allEnums, intValues );
        }
    };

} // Catch

#endif // CATCH_INTERFACES_ENUM_VALUES_REGISTRY_HPP_INCLUDED
