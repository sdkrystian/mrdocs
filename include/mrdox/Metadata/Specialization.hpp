//
// Licensed under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
// Copyright (c) 2023 Krystian Stasiowski (sdkrystian@gmail.com)
//
// Official repository: https://github.com/cppalliance/mrdox
//

#ifndef MRDOX_API_METADATA_SPECIALIZATION_HPP
#define MRDOX_API_METADATA_SPECIALIZATION_HPP

#include <mrdox/Platform.hpp>
#include <mrdox/MetadataFwd.hpp>
#include <mrdox/Metadata/Info.hpp>
#include <mrdox/Metadata/Symbols.hpp>
#include <utility>
#include <vector>

namespace clang {
namespace mrdox {

/** Primary and specialized IDs of specialized members
*/
struct SpecializedMember
{
    /** ID of the member in the primary template */
    const Info* Primary;

    /** ID of the member specialization */
    const Info* Specialized;

    SpecializedMember() = default;

    SpecializedMember(
        const Info* primary,
        const Info* specialized)
        : Primary(primary)
        , Specialized(specialized)
    {
    }

    bool operator==(const SpecializedMember&) const = default;
};

/** Specialization info for members of implicit instantiations
*/
struct SpecializationInfo
    : IsInfo<InfoKind::Specialization>
{
    /** The template arguments the parent template is specialized for */
    std::vector<std::unique_ptr<TArg>> Args;

    /** ID of the template to which the arguments pertain */
    const Info* Primary = nullptr;

    /** The specialized members.

        A specialized member `C` may itself be a `SpecializationInfo`
        if any of its members `M` are explicitly specialized for an implicit
        instantiation of `C`.
    */
    std::vector<SpecializedMember> Members;

    explicit
    SpecializationInfo(
        SymbolID ID = SymbolID::zero)
        : IsInfo(ID)
    {
    }
};

} // mrdox
} // clang

#endif
