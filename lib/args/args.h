#pragma once

#include <charconv>
#include <cinttypes>
#include <cstring>
#include <iostream>
#include <limits>

class Args {
   public:
    char* input = nullptr;
    char* output = nullptr;
    size_t max_iter = std::numeric_limits<size_t>::max();
    size_t freq = std::numeric_limits<size_t>::max();
    bool is_fast = false;

    void Get(int argc, char** argv);

    void Print() const;

    bool IsValid() const;
};
