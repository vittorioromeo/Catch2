
//              Copyright Catch2 Authors
// Distributed under the Boost Software License, Version 1.0.
//   (See accompanying file LICENSE.txt or copy at
//        https://www.boost.org/LICENSE_1_0.txt)

// SPDX-License-Identifier: BSL-1.0
#ifndef CATCH_INTERFACES_ENUM_VALUES_REGISTRY_HPP_INCLUDED
#define CATCH_INTERFACES_ENUM_VALUES_REGISTRY_HPP_INCLUDED

#include <catch2/internal/catch_stringrefbase.hpp>

#include <cstddef>

namespace Catch {

    namespace Detail {
        struct EnumInfo {
            struct StorageImpl;
            StringRefBase m_name;
            StorageImpl* m_storageImpl;
            // std::vector<std::pair<int, StringRefBase>> m_values;

            ~EnumInfo();

            EnumInfo() = default;
            EnumInfo(const EnumInfo&) = delete;
            EnumInfo(EnumInfo&&) = delete;

            StringRefBase lookup( int value ) const;
        };
    } // namespace Detail

    class IMutableEnumValuesRegistry {
    public:
        virtual ~IMutableEnumValuesRegistry(); // = default;

        virtual Detail::EnumInfo const& registerEnum( StringRefBase enumName, StringRefBase allEnums, const int* valuesPtr, std::size_t valuesCount ) = 0;

        template <typename E, std::size_t N>
        Detail::EnumInfo const& registerEnum( StringRefBase enumName, StringRefBase allEnums, const E(&values)[N] ) {
            static_assert(sizeof(int) >= sizeof(E), "Cannot serialize enum to int");
            int intValues[N];
            std::size_t i = 0;
            for( auto enumValue : values )
                intValues[i++] = static_cast<int>( enumValue );
            return registerEnum( enumName, allEnums, intValues );
        }
    };

} // Catch

#endif // CATCH_INTERFACES_ENUM_VALUES_REGISTRY_HPP_INCLUDED
