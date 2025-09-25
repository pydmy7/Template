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
