#include <cstdint>
#include <istream>
#include <utility>
#include <vector>
#include <concepts>

namespace scl {

using i128 = __int128;
using u128 = unsigned __int128;

template<class T>
constexpr T power(T a, std::uint64_t b, T res = 1) {
    for (; b != 0; b /= 2, a *= a) {
        if (b & 1) {
            res *= a;
        }
    }
    return res;
}

template<std::uint32_t P>
constexpr std::uint32_t mulMod(std::uint32_t a, std::uint32_t b) {
    return static_cast<std::uint64_t>(a) * b % P;
}

template<std::uint64_t P>
constexpr std::uint64_t mulMod(std::uint64_t a, std::uint64_t b) {
    std::uint64_t res = a * b - static_cast<std::uint64_t>(1.L * a * b / P - 0.5L) * P;
    res %= P;
    return res;
}

constexpr std::int64_t safeMod(std::int64_t x, std::int64_t m) {
    x %= m;
    if (x < 0) {
        x += m;
    }
    return x;
}

constexpr std::pair<std::int64_t, std::int64_t> invGcd(std::int64_t a, std::int64_t b) {
    a = safeMod(a, b);
    if (a == 0) {
        return {b, 0};
    }

    std::int64_t s = b, t = a;
    std::int64_t m0 = 0, m1 = 1;

    while (t) {
        std::int64_t u = s / t;
        s -= t * u;
        m0 -= m1 * u;

        std::swap(s, t);
        std::swap(m0, m1);
    }

    if (m0 < 0) {
        m0 += b / s;
    }

    return {s, m0};
}

template<std::unsigned_integral U, U P>
struct ModIntBase {
public:
    constexpr ModIntBase() : x(0) {}
    template<std::unsigned_integral T>
    constexpr explicit ModIntBase(T x_) : x(x_ % mod()) {}
    template<std::signed_integral T>
    constexpr explicit ModIntBase(T x_) {
        using S = std::make_signed_t<U>;
        S v = x_ % S(mod());
        if (v < 0) {
            v += mod();
        }
        x = v;
    }

    constexpr static U mod() {
        return P;
    }

    constexpr U val() const {
        return x;
    }

    constexpr ModIntBase operator-() const {
        ModIntBase res;
        res.x = (x == 0 ? 0 : mod() - x);
        return res;
    }

    constexpr ModIntBase inv() const {
        return power(*this, mod() - 2, static_cast<ModIntBase>(1));
    }

    constexpr ModIntBase &operator*=(const ModIntBase &rhs) & {
        x = mulMod<mod()>(x, rhs.val());
        return *this;
    }
    constexpr ModIntBase &operator+=(const ModIntBase &rhs) & {
        x += rhs.val();
        if (x >= mod()) {
            x -= mod();
        }
        return *this;
    }
    constexpr ModIntBase &operator-=(const ModIntBase &rhs) & {
        x -= rhs.val();
        if (x >= mod()) {
            x += mod();
        }
        return *this;
    }
    constexpr ModIntBase &operator/=(const ModIntBase &rhs) & {
        return *this *= rhs.inv();
    }

    friend constexpr ModIntBase operator*(ModIntBase lhs, const ModIntBase &rhs) {
        lhs *= rhs;
        return lhs;
    }
    friend constexpr ModIntBase operator+(ModIntBase lhs, const ModIntBase &rhs) {
        lhs += rhs;
        return lhs;
    }
    friend constexpr ModIntBase operator-(ModIntBase lhs, const ModIntBase &rhs) {
        lhs -= rhs;
        return lhs;
    }
    friend constexpr ModIntBase operator/(ModIntBase lhs, const ModIntBase &rhs) {
        lhs /= rhs;
        return lhs;
    }

    friend constexpr std::istream &operator>>(std::istream &is, ModIntBase &a) {
        std::int64_t i;
        is >> i;
        a = i;
        return is;
    }
    friend constexpr std::ostream &operator<<(std::ostream &os, const ModIntBase &a) {
        return os << a.val();
    }

    friend constexpr bool operator==(const ModIntBase &lhs, const ModIntBase &rhs) {
        return lhs.val() == rhs.val();
    }
    friend constexpr std::strong_ordering operator<=>(const ModIntBase &lhs, const ModIntBase &rhs) {
        return lhs.val() <=> rhs.val();
    }

private:
    U x;
};

template<std::uint32_t P>
using ModInt = ModIntBase<std::uint32_t, P>;
template<std::uint64_t P>
using ModInt64 = ModIntBase<std::uint64_t, P>;

struct Barrett {
public:
    explicit Barrett(std::uint32_t m_) : m(m_), im(static_cast<std::uint64_t>(-1) / m_ + 1) {}

    constexpr std::uint32_t mod() const {
        return m;
    }

    constexpr std::uint32_t mul(std::uint32_t a, std::uint32_t b) const {
        std::uint64_t z = a;
        z *= b;

        std::uint64_t x = static_cast<std::uint64_t>((static_cast<u128>(z) * im) >> 64);

        std::uint32_t v = static_cast<std::uint32_t>(z - x * m);
        if (m <= v) {
            v += m;
        }
        return v;
    }

private:
    std::uint32_t m;
    std::uint64_t im;
};

template<std::uint32_t Id>
struct DynModInt {
public:
    constexpr DynModInt() : x(0) {}
    template<std::unsigned_integral T>
    constexpr explicit DynModInt(T x_) : x(x_ % mod()) {}
    template<std::signed_integral T>
    constexpr explicit DynModInt(T x_) {
        int v = x_ % static_cast<int>(mod());
        if (v < 0) {
            v += mod();
        }
        x = v;
    }

    constexpr static void setMod(std::uint32_t m) {
        bt = Barrett{m};
    }

    static std::uint32_t mod() {
        return bt.mod();
    }

    constexpr std::uint32_t val() const {
        return x;
    }

    constexpr DynModInt operator-() const {
        DynModInt res;
        res.x = (x == 0 ? 0 : mod() - x);
        return res;
    }

    constexpr DynModInt inv() const {
        auto v = invGcd(x, mod());
        assert(v.first == 1);
        return v.second;
    }

    constexpr DynModInt &operator*=(const DynModInt &rhs) & {
        x = bt.mul(x, rhs.val());
        return *this;
    }
    constexpr DynModInt &operator+=(const DynModInt &rhs) & {
        x += rhs.val();
        if (x >= mod()) {
            x -= mod();
        }
        return *this;
    }
    constexpr DynModInt &operator-=(const DynModInt &rhs) & {
        x -= rhs.val();
        if (x >= mod()) {
            x += mod();
        }
        return *this;
    }
    constexpr DynModInt &operator/=(const DynModInt &rhs) & {
        return *this *= rhs.inv();
    }

    friend constexpr DynModInt operator*(DynModInt lhs, const DynModInt &rhs) {
        lhs *= rhs;
        return lhs;
    }
    friend constexpr DynModInt operator+(DynModInt lhs, const DynModInt &rhs) {
        lhs += rhs;
        return lhs;
    }
    friend constexpr DynModInt operator-(DynModInt lhs, const DynModInt &rhs) {
        lhs -= rhs;
        return lhs;
    }
    friend constexpr DynModInt operator/(DynModInt lhs, const DynModInt &rhs) {
        lhs /= rhs;
        return lhs;
    }

    friend constexpr std::istream &operator>>(std::istream &is, DynModInt &a) {
        std::int64_t i;
        is >> i;
        a = i;
        return is;
    }
    friend constexpr std::ostream &operator<<(std::ostream &os, const DynModInt &a) {
        return os << a.val();
    }

    friend constexpr bool operator==(const DynModInt &lhs, const DynModInt &rhs) {
        return lhs.val() == rhs.val();
    }
    friend constexpr std::strong_ordering operator<=>(const DynModInt &lhs, const DynModInt &rhs) {
        return lhs.val() <=> rhs.val();
    }

private:
    std::uint32_t x;
    static Barrett bt;
};

template<std::uint32_t Id>
Barrett DynModInt<Id>::bt = Barrett{998244353};

using Z = ModInt<998244353>;

struct Comb {
    int n_;
    std::vector<Z> fac_;
    std::vector<Z> invfac_;
    std::vector<Z> inv_;

    Comb() : n_{0}, fac_{1}, invfac_{1}, inv_{0} {}
    explicit Comb(int n) : Comb() {
        init(n);
    }

    void init(int m) {
        if (m <= n_) return;
        fac_.resize(m + 1);
        invfac_.resize(m + 1);
        inv_.resize(m + 1);

        for (int i = n_ + 1; i <= m; i++) {
            fac_[i] = fac_[i - 1] * Z{i};
        }
        invfac_[m] = fac_[m].inv();
        for (int i = m; i > n_; i--) {
            invfac_[i - 1] = invfac_[i] * Z{i};
            inv_[i] = invfac_[i] * fac_[i - 1];
        }
        n_ = m;
    }

    Z fac(int m) {
        if (m > n_) init(2 * m);
        return fac_[m];
    }
    Z invfac(int m) {
        if (m > n_) init(2 * m);
        return invfac_[m];
    }
    Z inv(int m) {
        if (m > n_) init(2 * m);
        return inv_[m];
    }
    Z binom(int n, int m) {
        if (n < m || m < 0) {
            return Z{0};
        }
        return fac(n) * invfac(m) * invfac(n - m);
    }
};

}  // namespace scl

int main() {}
