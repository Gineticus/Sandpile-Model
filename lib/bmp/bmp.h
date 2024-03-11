#pragma once

#include <cinttypes>
#include <iostream>
#include <fstream>
#include <cstring>

#include "lib/array/array.hpp"

struct BMP {
    const Array<Array<uint64_t>>& table;
    size_t number;

    BMP() = delete;
    BMP(const Array<Array<uint64_t>>& table, size_t number) : table(table), number(number) {}

    void Write(const char* path);
};
