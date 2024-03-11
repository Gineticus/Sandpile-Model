#include "bmp.h"

void BMP::Write(const char* path) {
    // filename - {path}/sanpile{number}.bmp
    char* filename = new char[std::strlen(path) + 20 + 12];
    std::strcpy(filename, path);
    std::strcat(filename, "/sandpile");
    char* number = new char[20];
    std::sprintf(number, "%zu", this->number);
    std::strcat(filename, number);
    std::strcat(filename, ".bmp");
    delete[] number;
    
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Can't open file");
    }

    // create header
    size_t height = table.Size();
    size_t width = table[0].Size();
    uint8_t padding =
        static_cast<uint8_t>((4 - (width / 2 + width % 2) % 4) % 4);
    uint64_t file_size =
        14 + 40 + 20 + (width / 2 + width % 2) * height + padding * height;
    uint32_t offset = 14 + 40 + 20;
    uint32_t header_size = 40;
    uint32_t zero = 0;
    file.write("BM", 2);
    file.write(reinterpret_cast<char*>(&file_size), 8);
    file.write(reinterpret_cast<char*>(&offset), 4);
    file.write(reinterpret_cast<char*>(&header_size), 4);
    file.write(reinterpret_cast<char*>(&width), 4);
    file.write(reinterpret_cast<char*>(&height), 4);

    uint16_t planes = 1;
    file.write(reinterpret_cast<char*>(&planes), 2);

    uint16_t bits_per_pixel = 4;
    file.write(reinterpret_cast<char*>(&bits_per_pixel), 2);

    file.write(reinterpret_cast<char*>(&zero), 4);

    file.write(reinterpret_cast<char*>(&zero), 4);

    file.write(reinterpret_cast<char*>(&zero), 4);
    file.write(reinterpret_cast<char*>(&zero), 4);

    uint32_t colors = 5;
    file.write(reinterpret_cast<char*>(&colors), 4);

    file.write(reinterpret_cast<char*>(&zero), 4);

    // color table
    //// white
    // file.write("\xff\xff\xff\x00", 4);
    //// green
    // file.write("\x00\xff\x00\x00", 4);
    //// yellow
    // file.write("\x00\xff\xff\x00", 4);
    //// purple
    // file.write("\xff\x00\xff\x00", 4);
    //// black
    // file.write("\x00\x00\x00\x00", 4);

    // white
    file.write("\xff\xff\xff\x00", 4);
    // green #27AE60
    file.write("\x60\xae\x27\x00", 4);
    // yellow #F1C40F
    file.write("\x0f\xc4\xf1\x00", 4);
    // purple #8E44AD
    file.write("\xad\x44\x8e\x00", 4);
    // black #232323
    file.write("\x23\x23\x23\x00", 4);

    // write pixels
    for (size_t i = 0; i < height; ++i) {
        for (size_t j = 0; j < width / 2; ++j) {
            uint8_t pixel = 0;
            pixel |= ((table[i][2 * j] > 3) ? 4 : table[i][2 * j]) << 4;
            pixel |= ((table[i][2 * j + 1] > 3) ? 4 : table[i][2 * j + 1]);
            file.write(reinterpret_cast<char*>(&pixel), 1);
        }
        if (width % 2 == 1) {
            uint8_t pixel = 0;
            pixel |= ((table[i][width - 1] > 3) ? 4 : table[i][width - 1]) << 4;
            file.write(reinterpret_cast<char*>(&pixel), 1);
        }
        for (size_t j = 0; j < padding; ++j) {
            file.write("\x00", 1);
        }
    }
}
