//
// Licensed under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
// Copyright (c) 2023 Krystian Stasiowski (sdkrystian@gmail.com)
//
// Official repository: https://github.com/cppalliance/mrdocs
//

#ifndef MRDOCS_LIB_TOOL_INFO_HPP
#define MRDOCS_LIB_TOOL_INFO_HPP

// #include "Symbols.hpp"
#include <mrdocs/Platform.hpp>
#include <mrdocs/Metadata/Info.hpp>
#include <memory>
#include <shared_mutex>
#include <unordered_set>

namespace clang {
namespace mrdocs {

class InfoContext;

using SymbolIDImpl = struct SymbolID::Impl
{
    using StorageType = std::array<std::uint8_t, 20>;

    const InfoContext& context_;
    StorageType data_;

    Impl(
        const InfoContext& context,
        const StorageType& data)
        : context_(context)
        , data_(data)
    {

    }
    SymbolID toSymbolID() const noexcept
    {
        return SymbolID(this);
    }

    struct Hash
    {
        using is_transparent = void;

        std::size_t operator()(const StorageType&) const;

        std::size_t operator()(const std::unique_ptr<Impl>&) const;
    };

    struct Equal
    {
        using is_transparent = void;

        bool operator()(
            const std::unique_ptr<Impl>& a,
            const std::unique_ptr<Impl>& b) const;

        bool operator()(
            const std::unique_ptr<Impl>& a,
            const StorageType& b) const;

        bool operator()(
            const StorageType& a,
            const std::unique_ptr<Impl>& b) const;
    };
};

struct InfoPtrHasher
{
    using is_transparent = void;

    std::size_t operator()(
        const std::unique_ptr<Info>& I) const;

    std::size_t operator()(
        const SymbolID& id) const;
};

struct InfoPtrEqual
{
    using is_transparent = void;

    bool operator()(
        const std::unique_ptr<Info>& a,
        const std::unique_ptr<Info>& b) const;

    bool operator()(
        const std::unique_ptr<Info>& a,
        const SymbolID& b) const;

    bool operator()(
        const SymbolID& a,
        const std::unique_ptr<Info>& b) const;
};

using InfoSet = std::unordered_set<
    std::unique_ptr<Info>, InfoPtrHasher, InfoPtrEqual>;


class InfoContext
{
    using RawID = SymbolIDImpl::StorageType;

    std::unordered_set<
        std::unique_ptr<SymbolIDImpl>,
        SymbolIDImpl::Hash,
        SymbolIDImpl::Equal> symbols_;
    std::shared_mutex symbols_mutex_;
    // const InfoContext& context_;
    SymbolIDImpl global_;

    InfoSet info_;
    std::shared_mutex info_mutex_;

public:
    // SymbolIDRegistry(const InfoContext& context);
    InfoContext();

    SymbolID globalNamespaceID();

    SymbolID getSymbolID(const RawID& id);

    InfoSet& info()
    {
        return info_;
    }

    const InfoSet& info() const
    {
        return info_;
    }

    #if 0
    class LocalInfoContext
    {
        InfoContext& upstream_;
        InfoSet local_info_;

    public:
        LocalInfoContext(InfoContext& upstream)
            : upstream_(upstream)
        {
        }

        Info* find(SymbolID id);

        template<typename InfoTy>
        std::pair<InfoTy&, bool>
        emplace(SymbolID id);
    };
    #endif
};

} // mrdocs
} // clang

#endif
