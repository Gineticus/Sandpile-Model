#include "sandpile.h"

bool Sandpile::Make(const char* filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Can't open file: " << filename << std::endl;
        return false;
    }

    char* line = new char[100];
    while (file.getline(line, 100)) {
        int16_t x, y;
        uint64_t value;
        auto result = std::from_chars(line, line + strlen(line), x);
        if (result.ec != std::errc()) {
            std::cerr << "Invalid x: " << line << std::endl;
        }
        result = std::from_chars(result.ptr + 1, line + strlen(line), y);
        if (result.ec != std::errc()) {
            std::cerr << "Invalid y: " << line << std::endl;
        }
        result = std::from_chars(result.ptr + 1, line + strlen(line), value);
        if (result.ec != std::errc()) {
            std::cerr << "Invalid value: " << line << std::endl;
        }
        Expand(y, x);
        table[y - down][x - left] = value;
        if (value > 3) {
            cordinates.PushBack(Cordinate(y, x));
        }
    }
    delete[] line;
    file.close();
    return true;
}

bool Sandpile::Expand(int16_t y, int16_t x) {
    bool flag = false;
    while (x < left) {
        for (int16_t i = 0; i < table.Size(); ++i) {
            table[i].PushFront(0);
        }
        --left;
        flag = true;
    }
    while (x > right) {
        for (int16_t i = 0; i < table.Size(); ++i) {
            table[i].PushBack(0);
        }
        ++right;
        flag = true;
    }
    while (y < down) {
        table.PushFront(Array<uint64_t>(table[0].Size(), 0));
        --down;
        flag = true;
    }
    while (y > up) {
        table.PushBack(Array<uint64_t>(table[0].Size(), 0));
        ++up;
        flag = true;
    }
    return flag;
}

void Sandpile::Print() {
    std::cout << "left: " << left << std::endl;
    std::cout << "right: " << right << std::endl;
    std::cout << "down: " << down << std::endl;
    std::cout << "up: " << up << std::endl;
    std::cout << "table:" << std::endl;
    for (int16_t i = 0; i < table.Size(); ++i) {
        for (int16_t j = 0; j < table[i].Size(); ++j) {
            std::cout << table[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

bool Sandpile::Iteration(bool is_fast) {
    if (is_fast) {
        bool flag = false;
        for (int16_t y = down; y <= up; ++y) {
            for (int16_t x = left; x <= right; ++x) {
                if (table[y - down][x - left] > 3) {
                    Expand(y - 1, x - 1);
                    Expand(y + 1, x + 1);

                    uint64_t value = table[y - down][x - left];

                    table[y - down][x - left] = value % 4;
                    table[y - down - 1][x - left] += value / 4;
                    table[y - down + 1][x - left] += value / 4;
                    table[y - down][x - left - 1] += value / 4;
                    table[y - down][x - left + 1] += value / 4;

                    flag = true;
                }
            }
        }
        return flag;
    }
    if (cordinates.Empty()) {
        return false;
    }
    size_t size = cordinates.Size();
    for (size_t k = 0; k < size; ++k) {
        int16_t y = cordinates.Front().y;
        int16_t x = cordinates.Front().x;
        cordinates.PopFront();
        Expand(y - 1, x - 1);
        Expand(y + 1, x + 1);

        table[y - down][x - left] -= 4;
        table[y - down - 1][x - left] += 1;
        table[y - down + 1][x - left] += 1;
        table[y - down][x - left - 1] += 1;
        table[y - down][x - left + 1] += 1;

        if (table[y - down][x - left] > 3) {
            cordinates.PushBack(Cordinate(y, x));
        }

        if (table[y - down - 1][x - left] == 4) {
            cordinates.PushBack(Cordinate(y - 1, x));
        }
        if (table[y - down + 1][x - left] == 4) {
            cordinates.PushBack(Cordinate(y + 1, x));
        }
        if (table[y - down][x - left - 1] == 4) {
            cordinates.PushBack(Cordinate(y, x - 1));
        }
        if (table[y - down][x - left + 1] == 4) {
            cordinates.PushBack(Cordinate(y, x + 1));
        }
    }

    return true;
}
