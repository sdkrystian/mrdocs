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

#ifndef MRDOX_API_CONFIG_HPP
#define MRDOX_API_CONFIG_HPP

#include <mrdox/Platform.hpp>
#include <mrdox/Support/Error.hpp>
#include <functional>
#include <memory>
#include <string>
#include <string_view>
#include <system_error>
#include <utility>
#include <vector>

namespace clang {
namespace mrdox {

class ThreadPool;

/** Configuration used to generate the Corpus and Docs

    This contains all the settings applied from
    the command line and the YML file (if any).
    A configuration is always connected to a
    particular directory from which absolute paths
    are calculated from relative paths.
*/
class MRDOX_DECL
    Config
{
protected:
    Config() noexcept;

public:
    struct ExtractOptions
    {
        enum class Policy
        {
            Always,
            Dependency,
            Never
        };

        /** Extraction policy for references to external declarations.

            This determines how declarations which are referenced by
            explicitly extracted declarations are extracted.

            Given a function parameter of type `std::string`, `std::string`
            would be extracted if this option is set to `Policy::Always`.
        */
        Policy referencedDeclarations = Policy::Dependency;

        /** Extraction policy for anonymous namespace.

            @li `Policy::Always`: anonymous namespaces and their
            members will always be extracted.

            @li `Policy::Dependency`: members of anonymous namespaces will only
            be extracted via dependency.

            @li `Policy::Never`: members of anonymous namespace will
            never be extracted, regardless of how they are referenced.
        */
        Policy anonymousNamespaces = Policy::Always;

        /** Extraction policy for inaccessible members.

            @li `Policy::Always`: all `private` and `protected` members
            will be extracted.

            @li `Policy::Dependency`: `private` and `protected` members will only
            be extracted via dependency.

            @li `Policy::Never`: `private` and `protected` will never be extracted.
        */
        Policy inaccessibleMembers = Policy::Always;

        Policy inaccessibleBases = Policy::Always;
    };

    struct Settings
    {

        /** Options controlling when declarations are extracted.
        */
        ExtractOptions extract;

        /** `true` if output should consist of multiple files.
        */
        bool multiPage = false;

        //--------------------------------------------

        /** Full path to the working directory

            The working directory is used to calculate
            full paths from relative paths.

            This string will always be native style
            and have a trailing directory separator.
        */
        std::string workingDir;

        /** Full path to the Addons directory.

            This string will always be native style
            and have a trailing directory separator.
        */
        std::string addonsDir;

        /** A string holding the complete configuration YAML.
        */
        std::string configYaml;

        /** A string holding extra configuration YAML.

            Any keys in this string which match keys used
            in @ref configYaml will effectively replace
            those entries in the configuration.

            A @ref Generator that wishes to implement
            format-specific options, should parse and
            apply `configYaml`, then parse and apply
            this string to the same settings.
        */
        std::string extraYaml;

        constexpr Settings const*
        operator->() const noexcept
        {
            return this;
        }
    };

    /** Destructor.
    */
    MRDOX_DECL
    virtual
    ~Config() noexcept = 0;

    /** Return a pool of threads for executing work.
    */
    MRDOX_DECL
    virtual
    ThreadPool&
    threadPool() const noexcept = 0;

    constexpr Settings const*
    operator->() const noexcept
    {
        return &settings();
    }

    virtual Settings const& settings() const noexcept = 0;
};

} // mrdox
} // clang

#endif
