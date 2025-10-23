#pragma once

#include <type_traits>
#include <cassert>

namespace bitmask_enum {

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

template <typename E>
constexpr bool self_test() noexcept {
    if constexpr (!EnableBitMaskOps<E>::enabled) {
        return true;
    }
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
    return static_cast<U>(test) == static_cast<U>(c);
}

} // namespace bitmask_enum
