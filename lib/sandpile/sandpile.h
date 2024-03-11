#pragma once

#include <charconv>
#include <cinttypes>
#include <cstring>
#include <fstream>
#include <iostream>

#include "lib/array/array.hpp"

struct Cordinate {
    int16_t y;
    int16_t x;

    Cordinate() = default;
    Cordinate(int32_t y, int32_t x) : y(y), x(x) {}
};

class Sandpile {
   private:
    int16_t left = 0;
    int16_t right = 0;
    int16_t down = 0;
    int16_t up = 0;

   public:
    Array<Array<uint64_t>> table =
        Array<Array<uint64_t>>(1, Array<uint64_t>(1, 0));

    Array<Cordinate> cordinates;

    Sandpile() = default;
    Sandpile(const char* filename) { Make(filename); }

    bool Iteration(bool is_fast);

    bool Make(const char* filename);

    bool Expand(int16_t x, int16_t y);

    void Print();
};
