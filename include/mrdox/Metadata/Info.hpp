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

#ifndef MRDOX_API_METADATA_INFO_HPP
#define MRDOX_API_METADATA_INFO_HPP

#include <mrdox/Platform.hpp>
#include <mrdox/Metadata/Javadoc.hpp>
#include <mrdox/Metadata/Specifiers.hpp>
#include <mrdox/Metadata/Symbols.hpp>
#include <array>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

#include "Support/TypeTraits.hpp"
#include <concepts>

namespace clang {
namespace mrdox {

struct NamespaceInfo;
struct RecordInfo;
struct FunctionInfo;
struct EnumInfo;
struct TypedefInfo;
struct VariableInfo;
struct FieldInfo;
struct SpecializationInfo;

template<
    typename F,
    typename InfoTy,
    typename... Args>
concept invocable_for_all_info =
    (
        std::invocable<F, add_cvref_from_t<InfoTy, NamespaceInfo>, Args...> &&
        std::invocable<F, add_cvref_from_t<InfoTy, RecordInfo>, Args...> &&
        std::invocable<F, add_cvref_from_t<InfoTy, FunctionInfo>, Args...> &&
        std::invocable<F, add_cvref_from_t<InfoTy, EnumInfo>, Args...> &&
        std::invocable<F, add_cvref_from_t<InfoTy, TypedefInfo>, Args...> &&
        std::invocable<F, add_cvref_from_t<InfoTy, VariableInfo>, Args...> &&
        std::invocable<F, add_cvref_from_t<InfoTy, FieldInfo>, Args...> &&
        std::invocable<F, add_cvref_from_t<InfoTy, SpecializationInfo>, Args...>
    ) || std::invocable<F, InfoTy, Args...>;

/** Info variant discriminator
*/
enum class InfoKind
{
    Namespace = 0,
    Record,
    Function,
    Enum,
    Typedef,
    Variable,
    Field,
    Specialization
};

/** Common properties of all symbols
*/
struct MRDOX_VISIBLE
    Info
{
    /** The unique identifier for this symbol.
    */
    SymbolID id = SymbolID::zero;

    /** Kind of declaration.
    */
    InfoKind Kind;

    /** Declaration access.

        Class members use:
        @li `AccessKind::Public`,
        @li `AccessKind::Protected`, and
        @li `AccessKind::Private`.

        Namespace members use `AccessKind::None`.
    */
    AccessKind Access = AccessKind::None;

    /** The unqualified name.
    */
    std::string Name;

    /** In-order List of parent namespaces.
    */
    std::vector<SymbolID> Namespace;

    /** The extracted javadoc for this declaration.
    */
    std::unique_ptr<Javadoc> javadoc;

    //--------------------------------------------

    virtual ~Info() = default;
    Info(Info const &Other) = delete;
    Info(Info&& Other) = default;

    explicit
    Info(
        InfoKind kind,
        SymbolID ID = SymbolID::zero)
        : id(ID)
        , Kind(kind)
    {
    }

    //
    // Observers
    //

    MRDOX_DECL
    std::string
    extractName() const;

    /** Return a string representing the symbol type.

        For example, "namespace", "class", et. al.
    */
    MRDOX_DECL
    std::string_view
    symbolType() const noexcept;

    constexpr bool isNamespace()      const noexcept { return Kind == InfoKind::Namespace; }
    constexpr bool isRecord()         const noexcept { return Kind == InfoKind::Record; }
    constexpr bool isFunction()       const noexcept { return Kind == InfoKind::Function; }
    constexpr bool isEnum()           const noexcept { return Kind == InfoKind::Enum; }
    constexpr bool isTypedef()        const noexcept { return Kind == InfoKind::Typedef; }
    constexpr bool isVariable()       const noexcept { return Kind == InfoKind::Variable; }
    constexpr bool isField()          const noexcept { return Kind == InfoKind::Field; }
    constexpr bool isSpecialization() const noexcept { return Kind == InfoKind::Specialization; }

    template<
        typename Visitor,
        typename... Args>
    decltype(auto)
    visit(Visitor&& v, Args&&... args)
    {
        return Info::visit_impl(*this,
            std::forward<Visitor>(v),
            std::forward<Args>(args)...);
    }

    template<
        typename Visitor,
        typename... Args>
    decltype(auto)
    visit(Visitor&& v, Args&&... args) const
    {
        return Info::visit_impl(*this,
            std::forward<Visitor>(v),
            std::forward<Args>(args)...);
    }

private:
    template<
        typename InfoTy,
        typename Visitor,
        typename... Args>
    static
    void
    visit_impl(
        InfoTy& info,
        Visitor&& v,
        Args&&... args)
    {
        switch(info.Kind)
        {
        case InfoKind::Namespace:
            return invoke_r_if_valid<void>(
                std::forward<Visitor>(v),
                static_cast<add_cvref_from_t<InfoTy&, NamespaceInfo>>(info),
                std::forward<Args>(args)...);
        case InfoKind::Record:
            return invoke_r_if_valid<void>(
                std::forward<Visitor>(v),
                static_cast<add_cvref_from_t<InfoTy&, RecordInfo>>(info),
                std::forward<Args>(args)...);
        case InfoKind::Enum:
            return invoke_r_if_valid<void>(
                std::forward<Visitor>(v),
                static_cast<add_cvref_from_t<InfoTy&, EnumInfo>>(info),
                std::forward<Args>(args)...);
        case InfoKind::Function:
            return invoke_r_if_valid<void>(
                std::forward<Visitor>(v),
                static_cast<add_cvref_from_t<InfoTy&, FunctionInfo>>(info),
                std::forward<Args>(args)...);
        case InfoKind::Typedef:
            return invoke_r_if_valid<void>(
                std::forward<Visitor>(v),
                static_cast<add_cvref_from_t<InfoTy&, TypedefInfo>>(info),
                std::forward<Args>(args)...);
        case InfoKind::Variable:
            return invoke_r_if_valid<void>(
                std::forward<Visitor>(v),
                static_cast<add_cvref_from_t<InfoTy&, VariableInfo>>(info),
                std::forward<Args>(args)...);
        case InfoKind::Field:
            return invoke_r_if_valid<void>(
                std::forward<Visitor>(v),
                static_cast<add_cvref_from_t<InfoTy&, FieldInfo>>(info),
                std::forward<Args>(args)...);
        case InfoKind::Specialization:
            return invoke_r_if_valid<void>(
                std::forward<Visitor>(v),
                static_cast<add_cvref_from_t<InfoTy&, SpecializationInfo>>(info),
                std::forward<Args>(args)...);
        default:
            break;
        }
    }

    template<
        typename InfoTy,
        typename Visitor,
        typename... Args>
    requires invocable_for_all_info<
        Visitor, InfoTy&, Args...>
    static
    decltype(auto)
    visit_impl(
        InfoTy& info,
        Visitor&& v,
        Args&&... args)
    {
        switch(info.Kind)
        {
        case InfoKind::Namespace:
            return invoke_if_valid(
                std::forward<Visitor>(v),
                static_cast<add_cvref_from_t<InfoTy&, NamespaceInfo>>(info),
                std::forward<Args>(args)...);
        case InfoKind::Record:
            return invoke_if_valid(
                std::forward<Visitor>(v),
                static_cast<add_cvref_from_t<InfoTy&, RecordInfo>>(info),
                std::forward<Args>(args)...);
        case InfoKind::Enum:
            return invoke_if_valid(
                std::forward<Visitor>(v),
                static_cast<add_cvref_from_t<InfoTy&, EnumInfo>>(info),
                std::forward<Args>(args)...);
        case InfoKind::Function:
            return invoke_if_valid(
                std::forward<Visitor>(v),
                static_cast<add_cvref_from_t<InfoTy&, FunctionInfo>>(info),
                std::forward<Args>(args)...);
        case InfoKind::Typedef:
            return invoke_if_valid(
                std::forward<Visitor>(v),
                static_cast<add_cvref_from_t<InfoTy&, TypedefInfo>>(info),
                std::forward<Args>(args)...);
        case InfoKind::Variable:
            return invoke_if_valid(
                std::forward<Visitor>(v),
                static_cast<add_cvref_from_t<InfoTy&, VariableInfo>>(info),
                std::forward<Args>(args)...);
        case InfoKind::Field:
            return invoke_if_valid(
                std::forward<Visitor>(v),
                static_cast<add_cvref_from_t<InfoTy&, FieldInfo>>(info),
                std::forward<Args>(args)...);
        case InfoKind::Specialization:
            return invoke_if_valid(
                std::forward<Visitor>(v),
                static_cast<add_cvref_from_t<InfoTy&, SpecializationInfo>>(info),
                std::forward<Args>(args)...);
        default:
            MRDOX_UNREACHABLE();
        }
    }
};

//------------------------------------------------

/** Base class for providing variant discriminator functions.

    This offers functions that return a boolean at
    compile-time, indicating if the most-derived
    class is a certain type.
*/
template<InfoKind K>
struct IsInfo : Info
{
    /** The variant discriminator constant of the most-derived class.
    */
    static constexpr InfoKind kind_id = K;

    static constexpr bool isNamespace()      noexcept { return K == InfoKind::Namespace; }
    static constexpr bool isRecord()         noexcept { return K == InfoKind::Record; }
    static constexpr bool isFunction()       noexcept { return K == InfoKind::Function; }
    static constexpr bool isEnum()           noexcept { return K == InfoKind::Enum; }
    static constexpr bool isTypedef()        noexcept { return K == InfoKind::Typedef; }
    static constexpr bool isVariable()       noexcept { return K == InfoKind::Variable; }
    static constexpr bool isField()          noexcept { return K == InfoKind::Field; }
    static constexpr bool isSpecialization() noexcept { return K == InfoKind::Specialization; }

protected:
    constexpr IsInfo()
        : Info(K)
    {
    }

    constexpr explicit IsInfo(SymbolID ID)
        : Info(K, ID)
    {
    }
};

} // mrdox
} // clang

#endif
