#pragma once

#include <cstdint>
#include <ostream>
#pragma pack(1)
struct test
{
    std::int32_t first;
    std::int32_t second;
    char test_string[10];
};

inline std::ostream &operator<<(std::ostream &os, const test &t)
{
    os << "first: " << t.first << " second: " << t.second << " test_string: " << t.test_string;
    return os;
}

#pragma pack()
