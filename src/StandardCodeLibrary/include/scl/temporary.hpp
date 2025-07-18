i64 ceilDiv(i64 n, i64 m) {
    if (n >= 0) {
        return (n + m - 1) / m;
    } else {
        return n / m;
    }
}

i64 floorDiv(i64 n, i64 m) {
    if (n >= 0) {
        return n / m;
    } else {
        return (n - m + 1) / m;
    }
}

std::int64_t sqrt(std::int64_t n) {
    std::int64_t s = std::sqrt(n);
    while (s * s > n) {
        --s;
    }
    while ((s + 1) * (s + 1) <= n) {
        ++s;
    }
    return s;
}

std::int64_t sqrt(std::int64_t n) {
    auto x = std::sqrt(2.0L * n);

    auto f = [](auto x) {
        return x * (x + 1) / 2;
    };

    // f(x - 1) + 1 <= n <= f(x)
    while (f(x) < n) {
        ++x;
    }
    while (f(x - 1) + 1 > n) {
        --x;
    }

    return x;
}
