//
// This is a derivative work. originally part of the LLVM Project.
// Licensed under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
// Copyright (c) 2023 Vinnie Falco (vinnie.falco@gmail.com)
//
// Official repository: https://github.com/cppalliance/mrdox
//

#ifndef MRDOX_API_METADATA_NAMESPACE_HPP
#define MRDOX_API_METADATA_NAMESPACE_HPP

#include <mrdox/Platform.hpp>
#include <mrdox/Metadata/Info.hpp>
#include <mrdox/ADT/BitField.hpp>
#include <vector>

namespace clang {
namespace mrdox {

union NamespaceFlags
{
    BitFieldFullValue raw{.value=0u};

    BitFlag<0> isInline;
    BitFlag<1> isAnonymous;
};

/** Describes a namespace.
*/
struct NamespaceInfo
    : IsInfo<InfoKind::Namespace>
{
    std::vector<const Info*> Members;
    std::vector<const Info*> Specializations;

    NamespaceFlags specs;

    //--------------------------------------------

    explicit
    NamespaceInfo(
        SymbolID ID = SymbolID::zero)
        : IsInfo(ID)
    {
    }
};

} // mrdox
} // clang

#endif
