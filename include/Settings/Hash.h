//
// Created by adven on 2024/4/29.
//

#ifndef FUCK_PTSD_HASH_H
#define FUCK_PTSD_HASH_H

#include <iostream>

namespace Settings {
namespace Hash {
typedef std::size_t ht;

static constexpr ht prime = 0x100000001B3ull;
static constexpr ht basis = 0xCBF29CE484222325ull;

static auto HashConvert(const std::string& str) -> ht {
    ht ret = basis;

    for (std::size_t i = 0; i < str.size(); ++i) {
        ret ^= static_cast<std::size_t>(str[i]);
        ret *= prime;
    }

    return ret;
};

static constexpr auto hash_compile_time(const char* str, ht last_value = basis)
    -> ht {
    return *str ? hash_compile_time(str + 1, (*str ^ last_value) * prime)
                : last_value;
}

static constexpr unsigned long long operator"" _hash(const char* str, size_t) {
    return hash_compile_time(str);
}

}  // namespace Hash
}  // namespace Settings

#endif  // FUCK_PTSD_HASH_H
