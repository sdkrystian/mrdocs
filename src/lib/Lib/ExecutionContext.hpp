//
// This is a derivative work. originally part of the LLVM Project.
// Licensed under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
// Copyright (c) 2023 Vinnie Falco (vinnie.falco@gmail.com)
// Copyright (c) 2023 Krystian Stasiowski (sdkrystian@gmail.com)
//
// Official repository: https://github.com/cppalliance/mrdox
//

#ifndef MRDOX_LIB_TOOL_EXECUTIONCONTEXT_HPP
#define MRDOX_LIB_TOOL_EXECUTIONCONTEXT_HPP

#include "ConfigImpl.hpp"
#include "Diagnostics.hpp"
#include "Info.hpp"
#include <mrdox/Support/Error.hpp>
#include <llvm/ADT/SmallString.h>
#include <mutex>
#include <shared_mutex>
#include <unordered_map>
#include <vector>

namespace clang {
namespace mrdox {

/** A custom execution context for visitation.

    This execution context extends the clang base
    class by adding additional state beyond the
    ToolResults, shared by all AST visitor
    instances.
*/
class ExecutionContext
{
protected:
    const ConfigImpl& config_;

public:
    ExecutionContext(
        const ConfigImpl& config)
        : config_(config)
    {
    }

    virtual
    void
    report(
        UnresolvedInfoSet&& results,
        Diagnostics&& diags) = 0;

    virtual
    void
    reportEnd(report::Level level) = 0;

    virtual
    mrdox::Expected<InfoSet>
    results() = 0;
};

// ----------------------------------------------------------------

class InfoExecutionContext
    : public ExecutionContext
{
    std::shared_mutex mutex_;
    Diagnostics diags_;
    UnresolvedInfoSet results_;

public:
    using ExecutionContext::ExecutionContext;

    void
    report(
        UnresolvedInfoSet&& results,
        Diagnostics&& diags) override;

    void
    reportEnd(report::Level level) override;

    mrdox::Expected<InfoSet>
    results() override;
};

// ----------------------------------------------------------------

class BitcodeExecutionContext
    : public ExecutionContext
{
    std::mutex mutex_;
    Diagnostics diags_;

    std::unordered_map<SymbolID,
        std::vector<llvm::SmallString<0>>> bitcode_;

public:
    using ExecutionContext::ExecutionContext;

    void
    report(
        UnresolvedInfoSet&& results,
        Diagnostics&& diags) override;

    void
    reportEnd(report::Level level) override;

    mrdox::Expected<InfoSet>
    results() override;

    auto& getBitcode()
    {
        return bitcode_;
    }
};


} // mrdox
} // clang

#endif
