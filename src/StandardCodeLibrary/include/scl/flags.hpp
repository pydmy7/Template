#pragma once

#include <type_traits>
#include <cassert>

namespace bitmask_enum {

// =====================================================================
//  启用某个 enum 支持位运算
// =====================================================================
template <typename E>
struct EnableBitMaskOps {
    static constexpr bool enabled = false;
};

namespace detail {

template <typename E>
using enable_if_enum_t =
    std::enable_if_t<EnableBitMaskOps<E>::enabled, E>;

template <typename E>
using underlying_t = std::underlying_type_t<E>;

} // namespace detail

// =====================================================================
//  基本位运算符
// =====================================================================

template <typename E>
constexpr detail::enable_if_enum_t<E>
operator|(E lhs, E rhs) noexcept {
    using U = detail::underlying_t<E>;
    return static_cast<E>(static_cast<U>(lhs) | static_cast<U>(rhs));
}

template <typename E>
constexpr detail::enable_if_enum_t<E>
operator&(E lhs, E rhs) noexcept {
    using U = detail::underlying_t<E>;
    return static_cast<E>(static_cast<U>(lhs) & static_cast<U>(rhs));
}

template <typename E>
constexpr detail::enable_if_enum_t<E>
operator^(E lhs, E rhs) noexcept {
    using U = detail::underlying_t<E>;
    return static_cast<E>(static_cast<U>(lhs) ^ static_cast<U>(rhs));
}

template <typename E>
constexpr detail::enable_if_enum_t<E>
operator~(E e) noexcept {
    using U = detail::underlying_t<E>;
    return static_cast<E>(~static_cast<U>(e));
}

// =====================================================================
//  复合赋值运算符
// =====================================================================

template <typename E>
constexpr std::enable_if_t<EnableBitMaskOps<E>::enabled, E&>
operator|=(E& lhs, E rhs) noexcept {
    return lhs = lhs | rhs;
}

template <typename E>
constexpr std::enable_if_t<EnableBitMaskOps<E>::enabled, E&>
operator&=(E& lhs, E rhs) noexcept {
    return lhs = lhs & rhs;
}

template <typename E>
constexpr std::enable_if_t<EnableBitMaskOps<E>::enabled, E&>
operator^=(E& lhs, E rhs) noexcept {
    return lhs = lhs ^ rhs;
}

// =====================================================================
//  位移运算符
// =====================================================================

template <typename E>
constexpr detail::enable_if_enum_t<E>
operator<<(E lhs, int shift) noexcept {
    using U = detail::underlying_t<E>;
    assert(shift >= 0 && shift < static_cast<int>(sizeof(U) * 8));
    return static_cast<E>(static_cast<U>(lhs) << shift);
}

template <typename E>
constexpr detail::enable_if_enum_t<E>
operator>>(E lhs, int shift) noexcept {
    using U = detail::underlying_t<E>;
    assert(shift >= 0 && shift < static_cast<int>(sizeof(U) * 8));
    return static_cast<E>(static_cast<U>(lhs) >> shift);
}

template <typename E>
constexpr std::enable_if_t<EnableBitMaskOps<E>::enabled, E&>
operator<<=(E& lhs, int shift) noexcept {
    lhs = lhs << shift;
    return lhs;
}

template <typename E>
constexpr std::enable_if_t<EnableBitMaskOps<E>::enabled, E&>
operator>>=(E& lhs, int shift) noexcept {
    lhs = lhs >> shift;
    return lhs;
}

// =====================================================================
//  编译期验证辅助函数（测试示例）
// =====================================================================

#if defined(__cpp_constexpr)
template <typename E>
constexpr bool self_test() noexcept {
    if constexpr (!EnableBitMaskOps<E>::enabled)
        return true;
    using U = detail::underlying_t<E>;
    constexpr E a = static_cast<E>(1 << 0);
    constexpr E b = static_cast<E>(1 << 1);
    constexpr E c = static_cast<E>(1 << 2);

    E test = a | b;
    test &= ~a;
    test |= c;
    test ^= b;
    test <<= 1;
    test >>= 1;
    // 转回底层整数验证
    return static_cast<U>(test) == static_cast<U>(c);
}
#endif

} // namespace bitmask_enum


// =====================================================================
// Example Usage
// =====================================================================
/*
#include "bitmask_enum.hpp"
using namespace bitmask_enum;

enum class Permission : uint8_t {
    None    = 0,
    Read    = 1 << 0,
    Write   = 1 << 1,
    Execute = 1 << 2
};

template <>
struct bitmask_enum::EnableBitMaskOps<Permission> {
    static constexpr bool enabled = true;
};

constexpr bool ok = bitmask_enum::self_test<Permission>();
static_assert(ok, "bitmask_enum self test failed.");

int main() {
    Permission p = Permission::Read | Permission::Write;
    p |= Permission::Execute;
    p <<= 1;
    p >>= 1;
    if (p & Permission::Read)
        return 0;
    return 1;
}
*/
