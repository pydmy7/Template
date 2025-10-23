#include "scl/bitmask_enum.hpp"

#include <iostream>

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
    if (static_cast<bool>(p & Permission::Read)) {
        std::cout << "success" << std::endl;
        return 0;
    }
    std::cout << "failure" << std::endl;
    return 1;
}
