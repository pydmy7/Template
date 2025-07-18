#pragma once

#include <type_traits>

namespace scl {

template<typename T>
requires std::is_enum_v<T>
class Flags {
public:
    using underlying_type = std::underlying_type_t<T>;

    constexpr Flags() noexcept = default;

    constexpr explicit Flags(T value) noexcept
        : value_{static_cast<underlying_type>(value)} {}

    constexpr explicit Flags(underlying_type value) noexcept
        : value_{value} {}

    constexpr Flags(Flags&&) noexcept = default;
    constexpr Flags(const Flags&) noexcept = default;

    constexpr Flags& operator=(const Flags&) noexcept = default;

    constexpr Flags& operator=(T value) noexcept {
        value_ = static_cast<underlying_type>(value);
        return *this;
    }

    constexpr explicit operator T() const noexcept {
        return static_cast<T>(value_);
    }

    constexpr explicit operator underlying_type() const noexcept {
        return value_;
    }

    constexpr Flags operator|(T o) const noexcept {
        return Flags{value_ | static_cast<underlying_type>(o)};
    }

    constexpr Flags operator&(T o) const noexcept {
        return Flags{value_ & static_cast<underlying_type>(o)};
    }

    constexpr Flags operator^(T o) const noexcept {
        return Flags{value_ ^ static_cast<underlying_type>(o)};
    }

    constexpr Flags operator~() const noexcept {
        return Flags{~value_};
    }

    constexpr Flags& operator|=(T o) noexcept {
        value_ |= static_cast<underlying_type>(o);
        return *this;
    }

    constexpr Flags& operator&=(T o) noexcept {
        value_ &= static_cast<underlying_type>(o);
        return *this;
    }

    constexpr Flags& operator^=(T o) noexcept {
        value_ ^= static_cast<underlying_type>(o);
        return *this;
    }

    constexpr bool contains(T flag) const noexcept {
        return (value_ & static_cast<underlying_type>(flag)) != 0;
    }

    constexpr void reset() noexcept {
        value_ = 0;
    }

    constexpr bool any() const noexcept {
        return value_ != 0;
    }

    constexpr bool operator==(const Flags& other) const noexcept = default;

    friend constexpr Flags operator|(T lhs, T rhs) noexcept {
        return Flags(lhs) | rhs;
    }

    friend constexpr Flags operator&(T lhs, T rhs) noexcept {
        return Flags(lhs) & rhs;
    }

    friend constexpr Flags operator^(T lhs, T rhs) noexcept {
        return Flags(lhs) ^ rhs;
    }

private:
    underlying_type value_{};
};

}  // namespace scl

template<typename E>
requires std::is_enum_v<E>
constexpr scl::Flags<E> operator|(E lhs, E rhs) noexcept {
    return scl::Flags<E>{lhs} | rhs;
}

template<typename E>
requires std::is_enum_v<E>
constexpr scl::Flags<E> operator&(E lhs, E rhs) noexcept {
    return scl::Flags<E>{lhs} & rhs;
}

template<typename E>
requires std::is_enum_v<E>
constexpr scl::Flags<E> operator^(E lhs, E rhs) noexcept {
    return scl::Flags<E>{lhs} ^ rhs;
}

template<typename E>
requires std::is_enum_v<E>
constexpr scl::Flags<E> operator~(E val) noexcept {
    return ~scl::Flags<E>{val};
}
